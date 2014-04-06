
#include "collection.h"
#include "message.h"
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
uvmongo__find(uvmongo_collection_t * coll, bson * query, bson * fields, int skip, int limit, uvmongo_document_cb callback) {
  uvmongo_cursor_t * cursor = uvmongo_cursor_new(coll);
  uvmongo_cursor_set_query(cursor, query, fields);
  uvmongo_cursor_set_skip(cursor, skip);
  uvmongo_cursor_set_limit(cursor, limit);
  
  uvmongo_message_t * msg = uvmongo_message_serialize_query(cursor);
  uvmongo_message_set_callback(msg, callback);
  uvmongo_message_send(coll->db->mongo, msg);
  return UVMONGO_OK;
}

int
uvmongo_find(uvmongo_collection_t * coll, bson * query, bson * fields, int skip, int limit, uvmongo_document_cb callback) {
  uvmongo_t * m = coll->db->mongo;
  uvmongo_cursor_t * cursor = uvmongo_cursor_new(coll);
  uvmongo_cursor_set_query(cursor, query, fields);
  uvmongo_cursor_set_skip(cursor, skip);
  uvmongo_cursor_set_limit(cursor, limit);
  
  uvmongo_message_t * msg = uvmongo_message_serialize_query(cursor);
  uvmongo_message_set_callback(msg, callback);

  if (m->connected != UVMONGO_OK) {
    list_rpush(m->ready_queue, list_node_new(msg));
  } else {
    uvmongo_message_send(m, msg);
  }

  
  return UVMONGO_OK;
}

int
uvmongo_find_one(uvmongo_collection_t * coll, bson * query, bson * fields, uvmongo_document_cb callback) {
  return uvmongo_find(coll, query, fields, 0, 1, callback);
}
