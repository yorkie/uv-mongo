
#include <stdio.h>
#include <assert.h>
#include <uv.h>
#include "uvmongo.h"

int
main(int argc, char ** argv) {
  uvmongo_t * m = uvmongo_new("localhost", 27017);
  uvmongo_connect(m);
  uv_run(uv_default_loop(), UV_RUN_DEFAULT);
  return 0;
}