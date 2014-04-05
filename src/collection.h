
#pragma once

#include "mongo.h"
#include "db.h"
#include "cursor.h"

typedef mongo_collection_s {
  mongo_db_t * db;
  char * name;
} mongo_collection_t;

mongo_collection_t *
mongo_collection_new(mongo_db_t * db, char * name);

int
mongo_collection_free(mongo_collection_t * coll);

mongo_cursor_t *
mongo_collection_find(mongo_collection_t * coll, bson * query, bson * fields);