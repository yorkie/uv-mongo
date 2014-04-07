
#include <assert.h>
#include <buffer/buffer.h>
#include "db.h"

uvmongo_db_t *
uvmongo_db_new(uvmongo_t * m, char * name) {
  uvmongo_db_t * db = (uvmongo_db_t *) malloc(sizeof(uvmongo_db_t));
  db->name = buffer_new_with_copy(name);
  db->colls = hash_new();
  db->mongo = m;
  return db;
}

int
uvmongo_db_free(uvmongo_db_t * db) {
  free(db);
  db = NULL;
  return UVMONGO_OK;
}

uvmongo_collection_t *
uvmongo_collection(uvmongo_db_t * db, char * name) {
  assert(name != NULL);
  uvmongo_collection_t * ret;
  ret = (uvmongo_collection_t *) hash_get(db->colls, name);
  if (!ret) {
    ret = uvmongo_collection_new(db, name);
    hash_set(db->colls, name, ret);
  }
  return ret;
}

int
uvmongo_db_auth(uvmongo_db_t * db, char * username, char * password) {
  return UVMONGO_OK;
}