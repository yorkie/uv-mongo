
#pragma once

#include "uvmongo.h"
#include "collection.h"

uvmongo_cursor_t *
uvmongo_cursor_new(uvmongo_collection_t * coll);

int
uvmongo_cursor_set_id(uvmongo_cursor_t * cursor, int64_t id);

int
uvmongo_cursor_set_query(uvmongo_cursor_t * cursor, bson * query, bson * fields);

int
uvmongo_cursor_set_skip(uvmongo_cursor_t * cursor, unsigned int skip);

int
uvmongo_cursor_set_limit(uvmongo_cursor_t * cursor, unsigned int limit);

int
uvmongo_cursor_free(uvmongo_cursor_t * cursor);
