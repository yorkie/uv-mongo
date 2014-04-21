
#pragma once

#include "uvmongo.h"
#include "db.h"

#define DEFAULT_CHUNK_SIZE 256*1024

typedef struct uvmongo_gridfs_s uvmongo_gridfs_t;
typedef struct uvmongo_gridfile_s uvmongo_gridfile_t;

typedef enum uvmongo_storage_type_e {
  GRIDFILE_DEFAULT = 0,
  GRIDFILE_NOMD5   = 1 << 0
} uvmongo_storage_type_t;

struct uvmongo_gridfs_s {
  uvmongo_db_t * db;
  buffer_t * files_ns;
  buffer_t * chunks_ns;
};

uvmongo_gridfs_t *
uvmongo_gridfs_new(uvmongo_db_t * db, char * name);

int
uvmongo_gridfs_free(uvmongo_gridfs_t * fs);

int
uvmongo_gridfs_find(uvmongo_gridfs_t * fs, bson * query, 
                                           uvmongo_document_cb callback, 
                                           void * privdata);

int
uvmongo_gridfs_save(uvmongo_gridfs_t * fs, const char * filename, 
                                           const char * buf, 
                                           const char * content_type, 
                                           const bson * metadata);
