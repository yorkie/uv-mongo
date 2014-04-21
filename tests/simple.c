
#include <stdio.h>
#include <assert.h>
#include <uv.h>
#include "uvmongo.h"
#include "db.h"
#include "collection.h"
#include "gridfs.h"
#include "bson.h"

void
find_cb(uvmongo_t * m, bson * res, void * privdata) {
  char * transfer = (char *) privdata;
  fprintf(stdout, "%s\n", transfer);
  bson_print(res);
}

int
main(int argc, char ** argv) {
  uvmongo_t * m = uvmongo_connect("localhost", 27017);
  uvmongo_gridfs_t * fs = uvmongo_gridfs_new(uvmongo_db(m, "coco"), "data");
  char transfer[] = "Hello World";

  bson query[1];
  bson_init(query);
  bson_finish(query);
  uvmongo_gridfs_find(fs, query, find_cb, &transfer);
  bson_destroy(query);

  // uvmongo_gridfs_save(fs, "coco-file", "hahah\n", "text/plain", NULL);
  uv_run(uv_default_loop(), UV_RUN_DEFAULT);
  return 0;
}