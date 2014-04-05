
#pragma once

#include "uvmongo.h"
#include "db.h"
// #include "cursor.h"

typedef struct uvmongo_collection_s {
  char * name;
  uvmongo_db_t * db;
} uvmongo_collection_t;

uvmongo_collection_t *
uvmongo_collection_new(uvmongo_db_t * db, char * name);

int
uvmongo_collection_free(uvmongo_collection_t * coll);

// uv_cursor_t *
// uv_collection_find(mongo_collection_t * coll, bson * query, bson * fields);