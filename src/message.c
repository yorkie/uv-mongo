
#include <assert.h>
#include <net/net.h>
#include "bson.h"
#include "message.h"

uvmongo_message_t *
uvmongo_message_new(size_t msglen , int req_id , int res_to , int opcode) {
  uvmongo_message_t * message;
  if (msglen > INT32_MAX) {
    return NULL;
  }
  message = (uvmongo_message_t *) bson_malloc(msglen);
  if (!req_id) {
    req_id = rand();
  }
  message->header.msglen = (int) msglen;
  message->header.req_id = req_id;
  message->header.res_to = res_to;
  message->header.opcode = opcode;
  return message;
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

int
uvmongo_message_send(uvmongo_t * m, uvmongo_message_t * message) {
  uvmongo_header_t header;
  int r, i = 0;
  int header_len;

  bson_little_endian32(&header.msglen, &message->header.msglen);
  bson_little_endian32(&header.req_id, &message->header.req_id);
  bson_little_endian32(&header.res_to, &message->header.res_to);
  bson_little_endian32(&header.opcode, &message->header.opcode);

  char buf[message->header.msglen];
  header_len = sizeof(header);
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

  bson_free(message);
  return UVMONGO_OK;
}

int
uvmongo_message_read(uvmongo_t * m, char * msg) {
  uvmongo_header_t * header;
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

  do {
    bson res[1];
    bson_init_finished_data(res, reply->objs + next, 1);

    if (!res) break;
    next += bson_size(res);
    bson_print(res);
    printf("%d\n", next);

    if (next >= reply->header.msglen - 36) {
      break;
    }
  } while (next);

  printf("reqquest id: %d\n", reply->header.req_id);
  printf("response to: %d\n", reply->header.res_to);
  printf("opcode: %d\n", reply->header.opcode);
  printf("flag: %d\n", reply->fields.flag);
  printf("cursorID: %lld\n", reply->fields.cursorID);
  printf("start: %d\n", reply->fields.start);
  printf("number: %d\n", reply->fields.num);
  return 1;
}
