
#pragma once

#include <hash/hash.h>
#include <uvmongo.h>
// #include "collection.h"

typedef struct uvmongo_db_s {
  char * name;
  hash_t * colls;
  uvmongo_t * mongo;
} uvmongo_db_t;

uvmongo_db_t *
uvmongo_db_new(uvmongo_t * mongo, char * name);

int
uvmongo_db_free(uvmongo_db_t * db);

// uvmongo_collection_t *
// uvmongo_db_get_collection(uvmongo_db_t * db, char * name);

// int
// uvmongo_db_auth(uvmongo_db_t * db, char * username, char * password);