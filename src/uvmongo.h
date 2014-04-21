
#pragma once

#define UVMONGO_OK      +0
#define UVMONGO_ERROR   -1

#include <net/net.h>
#include <list/list.h>
#include <hash/hash.h>
#include <buffer/buffer.h>
#include "bson.h"

typedef struct uvmongo_s uvmongo_t;
typedef void (*uvmongo_connect_cb)  (uvmongo_t*);
typedef void (*uvmongo_document_cb) (uvmongo_t*, bson*, void*);
typedef void (*uvmongo_error_cb)    (uvmongo_t*, int);

/*
 * OPCODE TABLE
 */
typedef enum uvmongo_opcode_e {
  /* reply to a client request. responseTo is set */
  OP_REPLY        = 1,
  /* generic msg command followed by a string */
  OP_MSG          = 1000,
  /* update document */
  OP_UPDATE       = 2001,
  /* insert new document */
  OP_INSERT       = 2002,
  /* formerly used for OP_GET_BY_OID */
  RESERVED        = 2003,
  /* query a collection */
  OP_QUERY        = 2004,
  /* get more data from a query. See Cursors */
  OP_GET_MORE     = 2005,
  /* delete documents */
  OP_DELETE       = 2006,
  /* tell database client is done with a cursor */
  OP_KILL_CURSORS = 2007
} uvmongo_opcode_t;

/*
 * UPDATE FLAGS TABLE
 */
typedef enum uvmongo_update_flags_e {
  Upsert = 0,
  Multi  = 1 << 0
} uvmongo_update_flags_t;

/*
 * INSERT FLAGS TABLE
 */
typedef enum uvmongo_insert_flags_e {
  ContinueOnError = 0
} uvmongo_insert_flags_t;

/*
 * DELETE FLAGS TABLE
 */
typedef enum uvmongo_del_flags_e {
  SingleRemove = 0
} uvmongo_del_flags_t;

/*
 * QUERY FLAGS TABLE
 * visit: http://docs.mongodb.org/meta-driver/latest/legacy/mongodb-wire-protocol/#op-query
 */
typedef enum uvmongo_query_flags_e {
  Reserved        = 0,
  TailableCursor  = 1 << 0,
  SlaveOk         = 1 << 1,
  OplogReplay     = 1 << 2,
  NoCursorTimeout = 1 << 3,
  AwaitData       = 1 << 4,
  Exhaust         = 1 << 5,
  Partial         = 1 << 6
} uvmongo_query_flags_t;

struct uvmongo_s {
  int ismaster;
  int connected;
  hash_t * dbs;
  hash_t * colls;
  hash_t * msgs;
  list_t * ready_queue;
  net_t * net;
  uvmongo_connect_cb connect_cb;
  uvmongo_error_cb error_cb;
  void * data;
};

typedef struct uvmongo_db_s {
  buffer_t * name;
  hash_t * colls;
  uvmongo_t * mongo;
} uvmongo_db_t;

typedef struct uvmongo_collection_s {
  buffer_t * name;
  buffer_t * fullname;
  uvmongo_db_t * db;
} uvmongo_collection_t;

typedef struct uvmongo_cursor_s {
  int64_t id;
  int skip;
  int limit;
  bson * query;
  bson * fields;
  uvmongo_collection_t * coll;
  uvmongo_query_flags_t flag;
} uvmongo_cursor_t;

#pragma pack(1)
/*
 * req_id: This is a client or database-generated identifier that uniquely identifies this message. 
 *         For the case of client-generated messages (e.g. OP_QUERY and OP_GET_MORE), it will be returned 
 *         in the responseTo field of the OP_REPLY message. Along with the reponseTo field in responses, 
 *         clients can use this to associate query responses with the originating query.
 * res_to: In the case of a message from the database, this will be the requestID taken from the OP_QUERY 
 *         or OP_GET_MORE messages from the client. Along with the requestID field in queries, clients can 
 *         use this to associate query responses with the originating query.
 * msglen: This is the total size of the message in bytes. This total includes the 4 bytes that holds the 
 *         message length.
 * opcode: Type of message.
 *
 * For more infomation, you should visit: 
 *       http://docs.mongodb.org/meta-driver/latest/legacy/mongodb-wire-protocol/#standard-message-header
 *
 */
typedef struct uvmongo_header_s {
  int msglen;   /* message length */
  int req_id;   /* request id */
  int res_to;   /* response to */
  int opcode;   /* operator code */
} uvmongo_header_t;

/*
 * message structure
 */
typedef struct uvmongo_message_s {
  void * privdata;
  uvmongo_document_cb callback;
  uvmongo_cursor_t * cursor;
  uvmongo_header_t header;
  char data;
} uvmongo_message_t;

/*
 * replay response
 */
typedef struct uvmongo_reply_fields_s {
  int flag;
  int64_t cursorID;
  int start;
  int num;
} uvmongo_reply_fields_t;

/*
 * reply struct
 */
typedef struct uvmongo_reply_s {
  uvmongo_header_t header;
  uvmongo_reply_fields_t fields;
  char * objs;
} uvmongo_reply_t;
#pragma pack()

uvmongo_t *
uvmongo_connect(char * hostname, int port);

int
uvmongo_close(uvmongo_t * m);

int
uvmongo_run_command(uvmongo_t * m, bson * cmd, uvmongo_document_cb callback);

int
uvmongo_checkmaster(uvmongo_t * m);

void
uvmongo_checkmaster_cb(uvmongo_t * m, bson * res, void * privdata);

void
uvmongo_on_connected(net_t * net);

void
uvmongo_on_data(net_t * net, size_t read, char * buf);

void
uvmongo_on_error(net_t * net, err_t read, char * msg);

uvmongo_db_t *
uvmongo_db(uvmongo_t * m, char * name);
