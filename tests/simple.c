
#include <stdio.h>
#include <assert.h>
#include <uv.h>
#include "uvmongo.h"
#include "db.h"
#include "collection.h"
#include "gridfs.h"
#include "bson.h"

static void
on_data(uvmongo_t * m, bson * res, void * privdata) {
  bson_print(res);
}

static void
on_drain(uvmongo_t * m, void * privdata) {
  fprintf(stdout, "end\n");
}

int
main(int argc, char ** argv) {
  uvmongo_t * m = uvmongo_connect("localhost", 27017);
  uvmongo_gridfs_t * fs = uvmongo_gridfs_new(uvmongo_db(m, "coco"), "data");
  char transfer[] = "Hello World";
  uvmongo_gridfs_find(fs, "coco-file", on_data, &transfer);

  // uvmongo_gridfs_save(fs, "coco-file", "hahah\n", "text/plain", NULL);
  uv_run(uv_default_loop(), UV_RUN_DEFAULT);
  return 0;
}