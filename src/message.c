#include <math.h>
#include <assert.h>
#include <net/net.h>
#include "bson.h"
#include "message.h"
#include "cursor.h"
 
#define Length(num) num == 0 ? 1 : (int)log10((int)num)+2;
 
static const int ZERO = 0;
static const int ONE = 1;
 
uvmongo_message_t *
uvmongo_message_new(size_t msglen, int req_id, int res_to, int opcode) {
  uvmongo_message_t * message;
  if (msglen > INT32_MAX) {
    return NULL;
  }

  // pointers size in uvmongo_message_t
  size_t pt_size = sizeof(uvmongo_cursor_t *) + sizeof(uvmongo_document_cb);
  
  message = (uvmongo_message_t *) bson_malloc(msglen + pt_size);
  if (!req_id) {
    req_id = rand();
  }
  message->header.msglen = (int) msglen;
  message->header.req_id = req_id;
  message->header.res_to = res_to;
  message->header.opcode = opcode;
  message->cursor = NULL;
  return message;
}
 
int
uvmongo_message_free(uvmongo_message_t * msg) {
  free(msg);
  msg = NULL;
  return UVMONGO_OK;
}
 
int
uvmongo_message_set_callback(uvmongo_message_t * msg, uvmongo_document_cb callback) {
  msg->callback = callback;
  return UVMONGO_OK;
}
 
char *
uvmongo_message_append(char * start, const void * data , size_t len) {
  memcpy(start, data, len);
  return start + len;
}
 
char *
uvmongo_message_append32(char * start , const void * data) {
  bson_little_endian32(start, data);
  return start + 4;
}
 
char *
uvmongo_message_append64(char * start , const void * data) {
  bson_little_endian64(start, data);
  return start + 8;
}
 
uvmongo_message_t *
uvmongo_message_serialize_query(uvmongo_cursor_t * cursor) {
  char * data;
  char * ns = buffer_string(cursor->coll->fullname);
  size_t nslen = strlen(ns) + 1;
  size_t bslen = bson_size(cursor->query) + bson_size(cursor->fields);
  size_t msglen = 16 + 4 + nslen + 4 + 4 + bslen;
  uvmongo_message_t * msg = uvmongo_message_new(msglen, 0, 0, OP_QUERY);
  msg->cursor = cursor;
 
  data = &msg->data;
  data = uvmongo_message_append32(data, &cursor->flag);
  data = uvmongo_message_append(data, ns, nslen);
  data = uvmongo_message_append32(data, &cursor->skip);
  data = uvmongo_message_append32(data, &cursor->limit);
  data = uvmongo_message_append(data, cursor->query->data, bson_size(cursor->query));
  if (cursor->fields) {
    data = uvmongo_message_append(data, cursor->fields->data, bson_size(cursor->fields));
  }
 
  assert(data == ((char*)msg) + sizeof(uvmongo_cursor_t*)
                              + sizeof(uvmongo_document_cb) 
                              + msg->header.msglen);
  return msg;
}
 
uvmongo_message_t *
uvmongo_message_serialize_more(uvmongo_cursor_t * cursor) {
  char * data;
  char * ns = buffer_string(cursor->coll->fullname);
  size_t nslen = strlen(ns) + 1;
  size_t msglen = 16 + 4 + nslen + 4 + 8;
  uvmongo_message_t * msg = uvmongo_message_new(msglen, 0, 0, OP_GET_MORE);
 
  data = &msg->data;
  data = uvmongo_message_append32(data, &ZERO);
  data = uvmongo_message_append(data, ns, nslen);
  data = uvmongo_message_append32(data, &cursor->limit);
  data = uvmongo_message_append64(data, &cursor->id);
  return msg;  
}
 
uvmongo_message_t *
uvmongo_message_serialize_delete(buffer_t * fullname, bson * selector) {
  char * data;
  char * ns = buffer_string(fullname);
  size_t nslen = strlen(ns) + 1;
  size_t bslen = bson_size(selector);
  size_t msglen = 16 + 4 + nslen + 4 + bslen;
  uvmongo_message_t * msg = uvmongo_message_new(msglen, 0, 0, OP_DELETE);
 
  data = &msg->data;
  data = uvmongo_message_append32(data, &ZERO);
  data = uvmongo_message_append(data, ns, nslen);
  data = uvmongo_message_append32(data, &ONE);
  data = uvmongo_message_append(data, selector->data, bson_size(selector));
  return msg;
}
 
uvmongo_message_t *
uvmongo_message_serialize_update(buffer_t * fullname, bson * selector, 
                                                      bson * setup, 
                                                      uvmongo_update_flags_t flag) {
  char * data;
  char * ns = buffer_string(fullname);
  size_t nslen = strlen(ns) + 1;
  size_t bslen = bson_size(selector) + bson_size(setup);
  size_t msglen = 16 + 4 + nslen + 4 + bslen;
  uvmongo_message_t * msg = uvmongo_message_new(msglen, 0, 0, OP_UPDATE);
 
  data = &msg->data;
  data = uvmongo_message_append32(data, &ZERO);
  data = uvmongo_message_append(data, ns, nslen);
  data = uvmongo_message_append32(data, &flag);
  data = uvmongo_message_append(data, selector->data, bson_size(selector));
  data = uvmongo_message_append(data, setup->data, bson_size(setup));
  return msg;
}
 
uvmongo_message_t *
uvmongo_message_serialize_insert(buffer_t * fullname, bson * obj) {
  /*
   * FIXME(Yorkie): support batch insert
   */
  char * data;
  char * ns = buffer_string(fullname);
  size_t nslen = strlen(ns) + 1;
  size_t bslen = bson_size(obj);
  size_t msglen = 16 + 4 + nslen + bslen;
  uvmongo_message_t * msg = uvmongo_message_new(msglen, 0, 0, OP_INSERT);
 
  data = &msg->data;
  data = uvmongo_message_append32(data, &ZERO);
  data = uvmongo_message_append(data, ns, nslen);
  data = uvmongo_message_append(data, obj->data, bslen);
  data = &msg->data;
  return msg;
}
 
uvmongo_message_t *
uvmongo_message_serialize_killCursors(uvmongo_t * m) {
  /*
   * TODO(Yorkie): Not Implementated
   */
  return NULL;
};
 
int
uvmongo_message_send(uvmongo_t * m, uvmongo_message_t * message) {
  uvmongo_header_t header;
  int r, i = 0;
  int header_len;
 
  char * data = &message->data;
  bson_little_endian32(&header.msglen, &message->header.msglen);
  bson_little_endian32(&header.req_id, &message->header.req_id);
  bson_little_endian32(&header.res_to, &message->header.res_to);
  bson_little_endian32(&header.opcode, &message->header.opcode);
 
  char buf[message->header.msglen];
  header_len = sizeof(uvmongo_header_t);
  for (; i<header_len; i++) {
    buf[i] = ((char*)&header)[i];
  }
  for (; i<message->header.msglen; i++) {
    buf[i] = (&message->data)[i-header_len];
  }
 
  r = net_write2(m->net, buf, message->header.msglen);
  if (r != NET_OK) {
    bson_free(message);
    return r;
  }
 
  int msg_name_len = Length(header.req_id);
  char * msg_name = malloc(msg_name_len);
  snprintf(msg_name, msg_name_len, "%d", header.req_id);
  hash_set(m->msgs, msg_name, message);
  return UVMONGO_OK;
}
 
int
uvmongo_message_read(uvmongo_t * m, char * msg, size_t buflen) {
  uvmongo_header_t * header;
  uvmongo_message_t * message;
  uvmongo_reply_fields_t * fields;
  uvmongo_reply_t * reply;
  unsigned int len, next;
 
  header = (uvmongo_header_t *) msg;
  fields = (uvmongo_reply_fields_t *)(msg+16);
  bson_little_endian32(&len, &header->msglen);
 
  reply = (uvmongo_reply_t *) bson_malloc(sizeof(uvmongo_reply_t) - sizeof(char) + len - 16 - 20);
  reply->header.msglen = len;
  bson_little_endian32(&reply->header.req_id, &header->req_id);
  bson_little_endian32(&reply->header.res_to, &header->res_to);
  bson_little_endian32(&reply->header.opcode, &header->opcode);
  bson_little_endian32(&reply->fields.flag, &fields->flag);
  bson_little_endian64(&reply->fields.cursorID, &fields->cursorID);
  bson_little_endian32(&reply->fields.start, &fields->start);
  bson_little_endian32(&reply->fields.num, &fields->num);
 
  reply->objs = msg+16+20;
  next = 0;
  
  int msg_name_len = Length(reply->header.res_to);
  char msg_name[msg_name_len];
  snprintf(msg_name, sizeof(msg_name), "%d", reply->header.res_to);
  message = (uvmongo_message_t *)(hash_get(m->msgs, msg_name));
  if (message == NULL) {
    fprintf(stderr, "could not found request id: %s", msg_name);
    return UVMONGO_ERROR;
  }
 
  do {
    bson res[1];
    bson_init_finished_data(res, reply->objs + next, 1);
    if (!res) {
      break;
    }
    message->callback(m, res);
    next += bson_size(res);
    if (next >= reply->header.msglen - 36) {
      break;
    }
  } while (next);
 
  /*
   * Handle Get more
   */
  // printf("test:%d, %lld\n", reply->fields.num, reply->fields.cursorID);
  // printf("limit:%d\n", message->cursor->limit);
  if (reply->fields.cursorID == 0 || reply->fields.num < message->cursor->limit) {
    uvmongo_cursor_free(message->cursor);
  } else {
    uvmongo_cursor_t * cursor = uvmongo_cursor_new(message->cursor->coll);
    uvmongo_cursor_set_id(cursor, reply->fields.cursorID);
    uvmongo_cursor_set_limit(cursor, message->cursor->limit);
    uvmongo_message_t * new_msg = uvmongo_message_serialize_more(cursor);
    new_msg->cursor = cursor;
    uvmongo_message_set_callback(new_msg, message->callback);
    uvmongo_message_send(m, new_msg);
  }
 
  /* 
   * check the buffer size is bigger than reply length,
   * that means multiple replies has been received.
   */
  if (buflen > reply->header.msglen) {
    uvmongo_message_read(m, msg + reply->header.msglen, buflen - reply->header.msglen);
  }
  return UVMONGO_OK;
}