
#include <stdio.h>
#include <assert.h>
#include <uv.h>
#include "uvmongo.h"
#include "db.h"
#include "collection.h"

int
main(int argc, char ** argv) {
  uvmongo_t * m = uvmongo_new("localhost", 27017);
  // uvmongo_db_t * mydb = uvmongo_db(m, "wave-api");
  // uvmongo_collection_t * accounts = uvmongo_collection(mydb, "accounts");
  // printf("%s\n", mydb->name->data);
  // printf("%s\n", accounts->fullname->data);
  uv_run(uv_default_loop(), UV_RUN_DEFAULT);
  return 0;
}