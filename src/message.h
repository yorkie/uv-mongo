
#pragma once

#include "uvmongo.h"

uvmongo_message_t *
uvmongo_message_new(size_t msglen , int req_id , int res_to , int opcode);

char *
uvmongo_message_append(char * start, const void * data , size_t len);

char *
uvmongo_message_append32(char * start , const void * data);

char *
uvmongo_message_append64(char * start , const void * data);

int
uvmongo_message_serialize_query(uvmongo_t * m, uvmongo_cursor_t * cursor);

int
uvmongo_message_serialize_delete(uvmongo_t * m, bson * query);

int
uvmongo_message_serialize_update(uvmongo_t * m, bson * query, bson * setup);

int
uvmongo_message_serialize_insert(uvmongo_t * m, bson * obj);

int
uvmongo_message_send(uvmongo_t * m, uvmongo_message_t * message);

int
uvmongo_message_read(uvmongo_t * m, char * msg, uvmongo_doc_cb doc_cb);

