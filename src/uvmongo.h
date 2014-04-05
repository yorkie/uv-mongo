
#pragma once

#include <net/net.h>
#include <hash/hash.h>
#include "bson.h"

#define UVMONGO_OK      +0
#define UVMONGO_ERROR   -1

typedef struct uvmongo_s uvmongo_t;
typedef void (*uvmongo_connect_cb) (uvmongo_t*);
typedef void (*uvmongo_error_cb)   (uvmongo_t*, int);

struct uvmongo_s {
  char * db_name;
  hash_t * dbs;
  hash_t * colls;
  net_t * net;
  uvmongo_connect_cb connect_cb;
  uvmongo_error_cb error_cb;
  void * data;
};

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
typedef struct uvmongo_msg_header_s {
  int req_id;   /* request id */
  int res_to;   /* response to */
  int msglen;   /* message length */
  int opcode;   /* operator code */
} uvmongo_msg_header_t;

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

uvmongo_t *
uvmongo_new(char * hostname, int port);

int
uvmongo_free(uvmongo_t * m);

int
uvmongo_connect(uvmongo_t * m);

void
uvmongo_on_connected(net_t * net);

void
uvmongo_on_data(net_t * net, size_t read, char * buf);

void
uvmongo_on_error(net_t * net, err_t read, char * msg);

// uvmongo_db_t *
// uvmongo_select_db(uvmongo_t * m, char * name);

// uvmongo_collection_t *
// uvmongo_collection(uvmongo_t * m, char * name);

int
uvmongo_findAndModify(uvmongo_t * m);

int
uvmongo_update(uvmongo_t * m, bson * query, bson * setup);
