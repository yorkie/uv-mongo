
#pragma once

#include "mongo.h"
#include "collection.h"

typedef struct mongo_db_s {
  mongo_t * mongo;
  char * name;
} mongo_db_t;

mongo_db_t *
mongo_db_new(mongo_t * mongo, char * name);

int
mongo_db_free(mongo_db_t * db);

mongo_collection_t *
mongo_db_get_collection(mongo_db_t * db, char * name);

int
mongo_db_auth(mongo_db_t * db, char * username, char * password);