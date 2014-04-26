
#include <assert.h>
#include "gridfs.h"
#include "collection.h"

static bson *
chunk_new(bson_oid_t id, int num, char * src, size_t src_size) {
  bson * ret = bson_alloc();
  bson_init_size(ret, (int)src_size + 128);
  bson_append_oid(ret, "files_id", &id);
  bson_append_int(ret, "n", num);
  bson_append_binary(ret, "data", BSON_BIN_BINARY, src, (int)src_size);
  bson_finish(ret);
  return ret;
}

static void
uvmongo_chunks_data_cb(uvmongo_t * m, bson * chunk, void * privdata) {
  bson_print(chunk);
}

static void
uvmongo_chunks_drain_cb(uvmongo_t * m, void * privdata) {

}

static void
uvmongo_files_cb(uvmongo_t * m, bson * res, void * privdata) {
  uvmongo_gridfs_packet_t * packet;
  uvmongo_collection_t * chunks;
  bson query[1];
  bson_iterator it[1];
  bson_oid_t * fileId = NULL;

  packet = (uvmongo_gridfs_packet_t *) privdata;
  chunks = uvmongo_collection(packet->fs->db, packet->fs->chunks_ns->data);

  bson_iterator_init(it, res);
  while (bson_iterator_next(it) != BSON_EOO) {
    const char * key = bson_iterator_key(it);
    if (strcmp(key, "_id") == 0) {
      fileId = bson_iterator_oid(it);
      break;
    }
  }

  assert(fileId != NULL);

  bson_init(query);
  bson_append_oid(query, "files_id", fileId);
  bson_finish(query);
  uvmongo_find(chunks, query, NULL, 0, 0, uvmongo_chunks_data_cb, 
                                          uvmongo_chunks_drain_cb, 
                                          privdata);
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
uvmongo_gridfs_find(uvmongo_gridfs_t * fs, char * name,
                                           uvmongo_document_cb callback,
                                           void * privdata) {

  bson query[1];
  uvmongo_collection_t * files;
  uvmongo_gridfs_packet_t * packet;
  files = uvmongo_collection(fs->db, fs->files_ns->data);
  packet = (uvmongo_gridfs_packet_t *) malloc(sizeof(uvmongo_gridfs_packet_t));
  packet->fs = fs;
  packet->data = buffer_new();
  packet->privdata = privdata;

  bson_init(query);
  if (name != NULL) {
    bson_append_string(query, "filename", name);
  }
  bson_finish(query);
  uvmongo_find_one(files, query, NULL, uvmongo_files_cb, packet);
  bson_destroy(query);
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