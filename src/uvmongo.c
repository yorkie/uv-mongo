
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
uvmongo_on_connected(net_t * net) {
  printf("uvmongo> connected to %s:%d\n", net->hostname, net->port);
  uvmongo_t * m = (uvmongo_t *) net->data;
  uvmongo_collection_t * cmds = uvmongo_collection(uvmongo_db(m, "admin"), "$cmd");


  /* Just for test */
  bson query[1];
  bson fields[1];

  bson_init(query);
  bson_append_int(query, "ismaster", 1);
  bson_finish(query);
  bson_init(fields);
  bson_finish(fields);
  uvmongo_find_one(cmds, query, fields);
}

void
uvmongo_read_document(uvmongo_t * m, bson * doc) {
  bson_print(doc);
  printf("========\n");
}

void
uvmongo_on_data(net_t * net, size_t read, char * buf) {
  printf("uvmongo> received data(%zu)\n", read);
  uvmongo_t * m = (uvmongo_t *) net->data;
  uvmongo_message_read(m, buf, uvmongo_read_document);
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
