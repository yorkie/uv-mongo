
#pragma once

#include "uvmongo.h"
#include "collection.h"

typedef struct uvmongo_cursor_s
{
  uvmongo_collection_t * coll;
} uvmongo_cursor_t;

uvmongo_cursor_t *
uvmongo_cursor_new();

int
uvmongo_cursor_free(uvmongo_cursor_t * cursor);
