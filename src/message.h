
#pragma once

#include <string.h>
#include "uvmongo.h"

/*
 * create a new message to sent in the future
 */
uvmongo_message_t *
uvmongo_message_new(size_t msglen , int req_id , int res_to , int opcode);

/*
 * free this message object
 */
int
uvmongo_message_free(uvmongo_message_t * msg);

/*
 * set callback when this message is response
 */
int
uvmongo_message_set_callback(uvmongo_message_t * msg, uvmongo_document_cb callback);

/*
 * append cstring to message
 */
char *
uvmongo_message_append(char * start, const void * data , size_t len);

/*
 * append 32 integer to message
 */
char *
uvmongo_message_append32(char * start , const void * data);

/*
 * append 64 integer to message
 */
char *
uvmongo_message_append64(char * start , const void * data);

/*
 * serialize OP_QUERY
 */
uvmongo_message_t *
uvmongo_message_serialize_query(uvmongo_cursor_t * cursor);

/*
 * serialize OP_GET_MORE
 */
uvmongo_message_t *
uvmongo_message_serialize_more(uvmongo_cursor_t * cursor);

/*
 * serialize OP_DELETE
 */
uvmongo_message_t *
uvmongo_message_serialize_delete(buffer_t * fullname, bson * selector);

/*
 * serialize OP_UPDATE
 */
uvmongo_message_t *
uvmongo_message_serialize_update(buffer_t * fullname, bson * selector, 
                                                      bson * setup, 
                                                      uvmongo_update_flags_t flag);

/*
 * serialize OP_INSERT, only supports single insert
 */
uvmongo_message_t *
uvmongo_message_serialize_insert(buffer_t * fullname, bson * obj);

/*
 * send message abstraction
 */
int
uvmongo_message_send(uvmongo_t * m, uvmongo_message_t * message);

/*
 * implemtation for database response OP_REPLY
 */
int
uvmongo_message_read(uvmongo_t * m, char * msg, size_t buflen);
