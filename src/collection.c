
#include "collection.h"

uvmongo_collection_t *
uvmongo_collection_new(uvmongo_db_t * db, char * name) {
  uvmongo_collection_t * coll = (uvmongo_collection_t *) malloc(sizeof(uvmongo_collection_t));
  coll->name = name;
  coll->db = db;
  return coll;
}

int
uvmongo_collection_free(uvmongo_collection_t * coll) {
  free(coll);
  coll = NULL;
  return UVMONGO_OK;
}

// uvmongo_cursor_t *
// uvmongo_collection_find(uvmongo_collection_t * coll, bson * query, bson * fields) {

// }