
#include <stdio.h>
#include <assert.h>
#include <uv.h>
#include "uvmongo.h"
#include "db.h"
#include "collection.h"

void
find_cb(uvmongo_t * m, bson * res) {
  //bson_print(res);
}

int
main(int argc, char ** argv) {
  uvmongo_t * m = uvmongo_new("localhost", 27017);
  uvmongo_db_t * mydb = uvmongo_db(m, "wave-api");
  uvmongo_collection_t * accounts = uvmongo_collection(mydb, "accounts");
  bson query[1];
  bson_init(query);
  bson_finish(query);
  uvmongo_find(accounts, query, NULL, 0, 20, find_cb);
  
  uv_run(uv_default_loop(), UV_RUN_DEFAULT);
  return 0;
}