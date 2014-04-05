
#include "db.h"

uvmongo_db_t *
uvmongo_db_new(uvmongo_t * m, char * name) {
  uvmongo_db_t * db = (uvmongo_db_t *) malloc(sizeof(uvmongo_db_t));
  db->name = name;
  db->mongo = m;
  db->colls = hash_new();
  return db;
}

int
uvmongo_db_free(uvmongo_db_t * db) {
  free(db);
  db = NULL;
  return UVMONGO_OK;
}

// uvmongo_collection_t *
// uvmongo_db_get_collection(uvmongo_db_t * db, char * name) {
//   uvmongo_collection_t * ret;
//   assert(name != NULL);
//   if (hash_has(db->colls, name)) {
//     ret = (uvmongo_collection_t *) hash_get(db->colls, name);
//   } else {
//     ret = uvmongo_collection_new(db->default_db, name);
//     hash_set(db->colls, name, ret);
//   }
//   return ret;
// }

// int
// uvmongo_db_auth(uvmongo_db_t * db, char * username, char * password) {

// }