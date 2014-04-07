
#pragma once

#include <hash/hash.h>
#include "collection.h"
#include "uvmongo.h"

/*
 * create a new database
 */
uvmongo_db_t *
uvmongo_db_new(uvmongo_t * mongo, char * name);

/*
 * free database object
 */
int
uvmongo_db_free(uvmongo_db_t * db);

/*
 * select collection
 */
uvmongo_collection_t *
uvmongo_collection(uvmongo_db_t * db, char * name);

/*
 * authenticate
 */
int
uvmongo_db_auth(uvmongo_db_t * db, char * username, char * password);