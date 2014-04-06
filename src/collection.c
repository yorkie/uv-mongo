
#include "collection.h"

uvmongo_collection_t *
uvmongo_collection_new(uvmongo_db_t * db, char * name) {
  uvmongo_collection_t * coll = (uvmongo_collection_t *) malloc(sizeof(uvmongo_collection_t));
  coll->name = buffer_new_with_copy(name);
  coll->fullname = buffer_new_with_copy(db->name->data);
  buffer_append(coll->fullname, ".");
  buffer_append(coll->fullname, name);
  coll->db = db;
  return coll;
}

int
uvmongo_collection_free(uvmongo_collection_t * coll) {
  free(coll);
  coll = NULL;
  return UVMONGO_OK;
}

int
uvmongo_find(uvmongo_collection_t * coll, bson * query, bson * fields, int skip) {
  return UVMONGO_OK;
}

int
uvmongo_findOne(uvmongo_collection_t * coll, bson * query, bson * fields) {
  return UVMONGO_OK;
}
