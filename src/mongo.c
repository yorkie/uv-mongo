
#include "mongo.h"

mongo_t *
mongo_new(char * hostname, int port) {
  mongo_t * m = (mongo_t *) malloc(sizeof(mongo_t));
  m->net = net_new(hostname, port);
  m->db_name = "test";
  m->dbs = list_new();
  m->colls = list_new();
  return m;
}

int
mongo_free(mongo_t * m) {
  net_free(m);
}

int
mongo_connect(mongo_t * m) {
  assert(m->net != NULL);
  return net_connect(m->net);
}

int
mongo_set_connect_cb(mongo_t * m, mongo_connect_cb cb) {
  m->connect_cb = cb;
  return MONGO_OK;
}

mongo_db_t *
mongo_db(mongo_t * m, char * name) {
  mongo_db_t * ret;
  if (!name) {
    name = m->db_name;
  }
  if (!hash_has(m->dbs, name)) {
    ret = (mongo_db_t *) hash_get(m->dbs, name);
  } else {
    ret = mongo_db_new(m, name);
    hash_set(m->dbs, name, ret);
  }
  return ret;
}

mongo_collection_t *
mongo_collection(mongo_t * m, char * name) {
  mongo_collection_t * ret;
  assert(name != NULL);
  if (hash_has(m->colls, name)) {
    ret = (mongo_collection_t *) hash_get(m->colls, name);
  } else {
    ret = mongo_collection_new(m->default_db, name);
    hash_set(m->colls, name, ret)
  }
  return ret;
}

mongo_cursor_t *
mongo_find(mongo_t * m, bson * query, bson * field) {

}

int
mongo_findOne(mongo_t * m, bson * query, bson * field) {

}

int
mongo_findAndModify(mongo_t * m) {

}

int
mongo_update(mongo_t * m, bson * query, bson * setup) {

}


