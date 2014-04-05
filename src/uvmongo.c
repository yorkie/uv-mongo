
#include <assert.h>
#include <uvmongo.h>

uvmongo_t *
uvmongo_new(char * hostname, int port) {
  uvmongo_t * m = (uvmongo_t *) malloc(sizeof(uvmongo_t));
  m->net = net_new(hostname, port);
  m->db_name = "test";
  m->dbs = hash_new();

  /*
   * init net
   */
  m->net->conn_cb = uvmongo_on_connected;
  m->net->read_cb = uvmongo_on_data;
  m->net->error_cb = uvmongo_on_error;
  m->net->data = m;
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
}

void
uvmongo_on_data(net_t * net, size_t read, char * buf) {
  printf("uvmongo> received data(%zu): %s\n", read, buf);
}

void
uvmongo_on_error(net_t * net, err_t read, char * msg) {

}

int
uvmongo_set_conn_cb(uvmongo_t * m, uvmongo_connect_cb cb) {
  m->connect_cb = cb;
  return UVMONGO_OK;
}

// uvmongo_db_t *
// uvmongo_db(uvmongo_t * m, char * name) {
//   uvmongo_db_t * ret;
//   if (!name) {
//     name = m->db_name;
//   }
//   if (!hash_has(m->dbs, name)) {
//     ret = (uvmongo_db_t *) hash_get(m->dbs, name);
//   } else {
//     ret = uvmongo_db_new(m, name);
//     hash_set(m->dbs, name, ret);
//   }
//   return ret;
// }

// uvmongo_collection_t *
// uvmongo_collection(uvmongo_t * m, char * name) {
//   uvmongo_db_t * db = uvmongo_db(m, "test");
//   return uvmongo_db_get_collection(db, name);
// }

// uvmongo_cursor_t *
// uvmongo_find(uvmongo_t * m, bson * query, bson * field) {

// }

int
uvmongo_findOne(uvmongo_t * m, bson * query, bson * field) {
  return UVMONGO_OK;
}

int
uvmongo_findAndModify(uvmongo_t * m) {
  return UVMONGO_OK;
}

int
uvmongo_update(uvmongo_t * m, bson * query, bson * setup) {
  return UVMONGO_OK;
}


