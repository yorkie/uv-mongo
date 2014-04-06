
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

  /* Just for test */
  // bson query[1];
  // bson fields[1];
  // char * data;
  // char * ns = "wave-api.accounts";
  // int option = TailableCursor;
  // int skip = 0;
  // int limit = 0;

  // bson_init(query);
  // bson_finish(query);
  // bson_init(fields);
  // bson_finish(fields);

  // size_t msglen = 16 + 4 + (strlen(ns)+1) + 4 + 4 + bson_size(query)+bson_size(fields);
  // uvmongo_message_t * msg = uvmongo_message_new(msglen, 0, 0, OP_QUERY);
  // data = &msg->data;
  // data = uvmongo_message_append32(data, &option);
  // data = uvmongo_message_append(data, ns, strlen(ns)+1);
  // data = uvmongo_message_append32(data, &skip);
  // data = uvmongo_message_append32(data, &limit);
  // data = uvmongo_message_append(data, query->data, bson_size(query));
  // data = uvmongo_message_append(data, fields->data, bson_size(fields));

  // assert(data == ((char*)msg) + msg->header.msglen);
  // uvmongo_message_send(m, msg);
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
