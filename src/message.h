
#pragma once

#include "uvmongo.h"

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
  Multi  = 1
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
  TailableCursor  = 1,
  SlaveOk         = 2,
  OplogReplay     = 3,
  NoCursorTimeout = 4,
  AwaitData       = 5,
  Exhaust         = 6,
  Partial         = 7
} uvmongo_query_flags_t;


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

uvmongo_message_t *
uvmongo_message_new(size_t msglen , int req_id , int res_to , int opcode);

char *
uvmongo_message_append(char * start, const void * data , size_t len);

char *
uvmongo_message_append32(char * start , const void * data);

char *
uvmongo_message_append64(char * start , const void * data);

int
uvmongo_message_send(uvmongo_t * m, uvmongo_message_t * message);

int
uvmongo_message_read(uvmongo_t * m, char * msg);

