
#include <stdio.h>
#include <assert.h>
#include <uv.h>
#include "uvmongo.h"
#include "db.h"
#include "collection.h"

void
find_cb(uvmongo_t * m, bson * res) {
  bson_print(res);
}

int
main(int argc, char ** argv) {
  uvmongo_t * m = uvmongo_connect("localhost", 27017);
  uvmongo_db_t * mydb = uvmongo_db(m, "wave-api");
  uvmongo_collection_t * test = uvmongo_collection(mydb, "test");
  
  bson obj[1];
  bson_init(obj);
  bson_append_string(obj, "key", "hello world");
  bson_append_string(obj, "val", "dashboard\n");
  bson_finish(obj);
  uvmongo_insert(test, obj);

  uv_run(uv_default_loop(), UV_RUN_DEFAULT);
  return 0;
}