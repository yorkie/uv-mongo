
#include "gridfs.h"
#include "collection.h"

static bson *
chunk_new(bson_oid_t id, int num, char * src, size_t src_size) {
  fprintf(stdout, "buf:%s\n", src);
  bson * ret = bson_alloc();
  bson_init_size(ret, (int)src_size + 128);
  bson_append_oid(ret, "files_id", &id);
  bson_append_int(ret, "n", num);
  bson_append_binary(ret, "data", BSON_BIN_BINARY, src, (int)src_size);
  bson_finish(ret);
  return ret;
}

static void
files_cb(uvmongo_t * m, bson * res) {
  bson_print(res);
}

uvmongo_gridfs_t *
uvmongo_gridfs_new(uvmongo_db_t * db, char * name) {
  uvmongo_gridfs_t * fs = (uvmongo_gridfs_t *) malloc(sizeof(uvmongo_gridfs_t));
  fs->db = db;
  fs->files_ns = buffer_new_with_copy(name);
  fs->chunks_ns = buffer_new_with_copy(name);
  buffer_append(fs->files_ns, ".files");
  buffer_append(fs->chunks_ns, ".chunks");
  return fs;
}

int
uvmongo_gridfs_free(uvmongo_gridfs_t * fs) {
  buffer_free(fs->files_ns);
  buffer_free(fs->chunks_ns);
  return UVMONGO_OK;
}

int
uvmongo_gridfs_find(uvmongo_gridfs_t * fs, bson * query) {
  uvmongo_find_one(uvmongo_collection(fs->db, fs->files_ns->data), query, NULL, files_cb);
  return UVMONGO_OK;
}

int
uvmongo_gridfs_save(uvmongo_gridfs_t * fs, const char * filename, 
                                           const char * buf, 
                                           const char * content_type, 
                                           const bson * metadata) {
  bson_oid_t id;
  bson file[1];
  uint64_t d;
  int pos = 0;
  int n = 0;
  int s = strlen(buf);

  /*
   * generate files data
   */
  bson_oid_gen(&id);
  bson_init(file);
  bson_append_oid(file, "_id", &id);
  bson_append_string(file, "filename", filename);
  bson_append_string(file, "contentType", content_type);
  d = (bson_date_t)1000 * time(NULL);
  bson_append_date(file, "uploadDate", d);
  bson_append_int(file, "chunkSize", strlen(buf));
  bson_append_int(file, "flags", 1);
  bson_append_string(file, "md5", "");
  if (metadata != NULL) {
    bson_append_bson(file, "metadata", metadata);
  }
  bson_finish(file);
  bson_print(file);

  uvmongo_insert(uvmongo_collection(fs->db, fs->files_ns->data), file);
  bson_destroy(file);

  do {
    bson * chunk = chunk_new(id, n++, (char*)buf+pos, strlen(buf));
    pos += DEFAULT_CHUNK_SIZE;
    uvmongo_insert(uvmongo_collection(fs->db, fs->chunks_ns->data), chunk);
    bson_destroy(chunk);
  } while (pos < s);
  return UVMONGO_OK;
}