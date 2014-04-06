
#pragma once

#include <hash/hash.h>
#include "collection.h"
#include "uvmongo.h"

uvmongo_db_t *
uvmongo_db_new(uvmongo_t * mongo, char * name);

int
uvmongo_db_free(uvmongo_db_t * db);

uvmongo_collection_t *
uvmongo_collection(uvmongo_db_t * db, char * name);

int
uvmongo_run_command(uvmongo_db_t * db, bson * cmd);

// int
// uvmongo_db_auth(uvmongo_db_t * db, char * username, char * password);