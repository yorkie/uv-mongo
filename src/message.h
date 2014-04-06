
#pragma once

#include <string.h>
#include "uvmongo.h"

uvmongo_message_t *
uvmongo_message_new(size_t msglen , int req_id , int res_to , int opcode);

int
uvmongo_message_set_callback(uvmongo_message_t * msg, uvmongo_document_cb callback);

char *
uvmongo_message_append(char * start, const void * data , size_t len);

char *
uvmongo_message_append32(char * start , const void * data);

char *
uvmongo_message_append64(char * start , const void * data);

uvmongo_message_t *
uvmongo_message_serialize_query(uvmongo_cursor_t * cursor);

uvmongo_message_t *
uvmongo_message_serialize_delete(bson * query);

uvmongo_message_t *
uvmongo_message_serialize_update(bson * query, bson * setup);

uvmongo_message_t *
uvmongo_message_serialize_insert(bson * obj);

int
uvmongo_message_send(uvmongo_t * m, uvmongo_message_t * message);

int
uvmongo_message_read(uvmongo_t * m, char * msg, size_t buflen);
