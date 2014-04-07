
#include "cursor.h"

uvmongo_cursor_t *
uvmongo_cursor_new(uvmongo_collection_t * coll) {
  uvmongo_cursor_t * cursor = (uvmongo_cursor_t *) malloc(sizeof(uvmongo_cursor_t));
  cursor->coll = coll;
  cursor->flag = 0;
  cursor->skip = 0;
  cursor->limit = 0;
  return cursor;
}

int
uvmongo_cursor_set_id(uvmongo_cursor_t * cursor, int64_t id) {
  cursor->id = id;
  return UVMONGO_OK;
}

int
uvmongo_cursor_set_query(uvmongo_cursor_t * cursor, bson * query, bson * fields) {
  cursor->query = query;
  cursor->fields = fields;
  return UVMONGO_OK;
}

int
uvmongo_cursor_set_skip(uvmongo_cursor_t * cursor, unsigned int skip) {
  cursor->skip = skip;
  return UVMONGO_OK;
}

int
uvmongo_cursor_set_limit(uvmongo_cursor_t * cursor, unsigned int limit) {
  cursor->limit = limit;
  return UVMONGO_OK;
}

int
uvmongo_cursor_free(uvmongo_cursor_t * cursor) {
  /*
   * TODO(Yorkie): kill cursor firstly
   */
  free(cursor);
  cursor = NULL;
  return UVMONGO_OK;
}
