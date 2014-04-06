
#pragma once

#include "uvmongo.h"
#include "db.h"
// #include "cursor.h"

uvmongo_collection_t *
uvmongo_collection_new(uvmongo_db_t * db, char * name);

int
uvmongo_collection_free(uvmongo_collection_t * coll);

int
uvmongo_find(uvmongo_collection_t * coll, bson * query, bson * fields, int skip);

int
uvmongo_findOne(uvmongo_collection_t * coll, bson * query, bson * fields);
