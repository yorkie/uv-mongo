
#pragma once

#include "net/net.h"
#include "net/tls.h"
#include "hash/hash.h"
#include "bson.h"
#include "db.h"
#include "collection.h"
#include "cursor.h"

#define MONGO_OK      +0
#define MONGO_ERROR   -1

typedef struct mongo_s {
  char * db_name;
  hash_t * dbs;
  hash_t * colls;
  net_t * net;
  void * data;
} mongo_t;

typedef mongo_connect_cb (void*)(mongo_t*);
typedef mongo_cursor_cb (void*)(mongo_cursor_t*);
typedef mongo_error_cb (void*)(mongo_t*, int code);

mongo_t *
mongo_new(char * hostname, int port);

int
mongo_free(mongo_t * mongo);

int
mongo_connect(mongo_t * mongo);

mongo_db_t *
mongo_select_db(mongo_t * mongo, char * name);

mongo_collection_t *
mongo_collection(mongo_t * m, char * name);

int
mongo_findAndModify(mongo_t * mongo);

int
mongo_update(mongo_t * mongo, bson * query, bson * setup);


