
#pragma once

#include <net/net.h>
#include <hash/hash.h>
#include "bson.h"

#define UVMONGO_OK      +0
#define UVMONGO_ERROR   -1

typedef struct uvmongo_s uvmongo_t;
typedef void (*uvmongo_connect_cb) (uvmongo_t*);
typedef void (*uvmongo_error_cb)   (uvmongo_t*, int);

struct uvmongo_s {
  char * db_name;
  hash_t * dbs;
  hash_t * colls;
  net_t * net;
  uvmongo_connect_cb connect_cb;
  uvmongo_error_cb error_cb;
  void * data;
};

uvmongo_t *
uvmongo_new(char * hostname, int port);

int
uvmongo_free(uvmongo_t * m);

int
uvmongo_connect(uvmongo_t * m);

void
uvmongo_on_connected(net_t * net);

void
uvmongo_on_data(net_t * net, size_t read, char * buf);

void
uvmongo_on_error(net_t * net, err_t read, char * msg);

// uvmongo_db_t *
// uvmongo_select_db(uvmongo_t * m, char * name);

// uvmongo_collection_t *
// uvmongo_collection(uvmongo_t * m, char * name);

int
uvmongo_findAndModify(uvmongo_t * m);

int
uvmongo_update(uvmongo_t * m, bson * query, bson * setup);
