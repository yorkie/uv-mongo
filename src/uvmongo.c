
#include <assert.h>
#include "bson.h"
#include "uvmongo.h"
#include "message.h"
#include "db.h"

uvmongo_t *
uvmongo_new(char * hostname, int port) {
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
  uvmongo_connect(m);
  return m;
}

int
uvmongo_free(uvmongo_t * m) {
  net_free(m->net);
  free(m);
  m = NULL;
  return UVMONGO_OK;
}

int
uvmongo_connect(uvmongo_t * m) {
  assert(m->net != NULL);
  return net_connect(m->net);
}

void
uvmongo_checkmaster_cb(uvmongo_t * m, bson * res) {
  bson_iterator it;
  char *key;
  bson_iterator_from_buffer(&it, res->data);
  while (bson_iterator_next(&it)) {
    key = bson_iterator_key(&it);
    if (strcmp("ismaster", key) == 0) {
      m->ismaster = bson_iterator_bool(&it);
      m->connected = UVMONGO_OK;
      break;
    }
  }

  printf("successfully connect to primary : %d\n", m->ismaster);
}

int
uvmongo_checkmaster(uvmongo_t * m) {
  uvmongo_collection_t * cmds = uvmongo_collection(uvmongo_db(m, "admin"), "$cmd");
  bson * query = bson_alloc();
  bson_init(query);
  bson_append_int(query, "ismaster", 1);
  bson_finish(query);
  uvmongo__find(cmds, query, NULL, 0, 1, uvmongo_checkmaster_cb);
  bson_free(query);
}

void
uvmongo_on_connected(net_t * net) {
  uvmongo_t * m = (uvmongo_t *) net->data;
  uvmongo_checkmaster(m);
  uvmongo_checkmaster(m);
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
