
#include <assert.h>
#include <list/list.h>
#include "bson.h"
#include "uvmongo.h"
#include "message.h"
#include "db.h"
#include "collection.h"

uvmongo_t *
uvmongo_connect(char * hostname, int port) {
  uvmongo_t * m = (uvmongo_t *) malloc(sizeof(uvmongo_t));
  m->net = net_new(hostname, port);
  m->dbs = hash_new();
  m->msgs = hash_new();
  m->ready_queue = list_new();
  m->ismaster = -1;
  m->connected = -1;

  /*
   * init net
   */
  m->net->conn_cb = uvmongo_on_connected;
  m->net->read_cb = uvmongo_on_data;
  m->net->error_cb = uvmongo_on_error;
  m->net->data = m;
  net_connect(m->net);
  return m;
}

int
uvmongo_close(uvmongo_t * m) {
  net_free(m->net);
  free(m);
  m = NULL;
  return UVMONGO_OK;
}

int
uvmongo_run_command(uvmongo_t * m, bson * cmd, uvmongo_document_cb callback) {
  uvmongo_collection_t * cmds = uvmongo_collection(uvmongo_db(m, "admin"), "$cmd");
  uvmongo__find(cmds, cmd, NULL, 0, 1, callback, NULL);
  return UVMONGO_OK;
}

int
uvmongo_checkmaster(uvmongo_t * m) {
  bson * cmd = bson_alloc();
  bson_init(cmd);
  bson_append_int(cmd, "ismaster", 1);
  bson_finish(cmd);
  uvmongo_run_command(m, cmd, uvmongo_checkmaster_cb);
  bson_free(cmd);
  return UVMONGO_OK;
}

void
uvmongo_checkmaster_cb(uvmongo_t * m, bson * res, void * privdata) {
  bson_iterator it;
  char *key;
  bson_iterator_from_buffer(&it, res->data);
  while (bson_iterator_next(&it)) {
    key = (char *) bson_iterator_key(&it);
    if (strcmp("ismaster", key) == 0) {
      m->ismaster = bson_iterator_bool(&it);
      m->connected = UVMONGO_OK;
      break;
    }
  }

  printf("successfully connect to primary : %d\n", m->ismaster);
  list_node_t * task;
  do {
    task = list_lpop(m->ready_queue);
    if (task != NULL) {
      uvmongo_message_t * msg = (uvmongo_message_t *)(task->val);
      uvmongo_message_send(m, msg);
    }
  } while (task);
}

void
uvmongo_on_connected(net_t * net) {
  uvmongo_t * m = (uvmongo_t *) net->data;
  uvmongo_checkmaster(m);
}

void
uvmongo_on_data(net_t * net, size_t read, char * buf) {
  printf("received data(%zu)\n", read);
  uvmongo_t * m = (uvmongo_t *) net->data;
  uvmongo_message_read(m, buf, read);
  net_resume(net);
}

void
uvmongo_on_error(net_t * net, err_t read, char * msg) {
  printf("uvmongo> error: %s\n", msg);
}

int
uvmongo_set_conn_cb(uvmongo_t * m, uvmongo_connect_cb cb) {
  m->connect_cb = cb;
  return UVMONGO_OK;
}

uvmongo_db_t *
uvmongo_db(uvmongo_t * m, char * name) {
  assert(name != NULL);
  uvmongo_db_t * ret;
  ret = hash_get(m->dbs, name);
  if (!ret) {
    ret = uvmongo_db_new(m, name);
    hash_set(m->dbs, name, ret);
  }
  return ret;
}
