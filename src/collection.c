
#include "collection.h"
#include "cursor.h"

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
uvmongo_find(uvmongo_collection_t * coll, bson * query, bson * fields, int skip, int limit) {
  uvmongo_cursor_t * cursor = uvmongo_cursor_new(coll);
  uvmongo_cursor_set_query(cursor, query, fields);
  uvmongo_cursor_set_skip(cursor, skip);
  uvmongo_cursor_set_limit(cursor, limit);
  uvmongo_message_serialize_query(coll->db->mongo, cursor);
  return UVMONGO_OK;
}

int
uvmongo_find_one(uvmongo_collection_t * coll, bson * query, bson * fields) {
  return uvmongo_find(coll, query, fields, 0, 1);
}
