
uv mongo
================
Async mongo library based on [libuv](https://github.com/joyent/libuv) and [clibs](https://github.com/clibs)

### Usage
```c
void
find_cb(uvmongo_t * m, bson * res) {
  bson_print(res);
}

int
main(int argc, char ** argv) {
  uvmongo_t * m = uvmongo_connect("localhost", 27017);
  uvmongo_db_t * mydb = uvmongo_db(m, "mydb");
  uvmongo_collection_t * accounts = uvmongo_collection(mydb, "blogs");
  
  bson selector[1];
  bson_init(selector);
  bson_append_string(selector, "user", "yorkie");
  bson_finish(selector);
  uvmongo_find_one(accounts, selector, NULL, find_cb);
  uv_run(uv_default_loop(), UV_RUN_DEFAULT);
  return 0;
}
```


### API
##### uvmongo_connect(char * hostname, int port);
create a connection to your mongoDB

##### uvmongo_close(uvmongo_t * m);
close this connection to your mongoDB

##### uvmongo_db(uvmongo_t * m, char * name);
select db in your mongoDB

##### uvmongo_collection(uvmongo_db_t * db, char * name);
select collection of a db

##### uvmongo_find(uvmongo_collection_t * coll, bson * selector, bson * fields, int skip, int limit, void * callback);
query documents

##### uvmongo_find_one(uvmongo_collection_t * coll, bson * selector, bson * fields, void * callback);
query one document

### Run tests
```sh
$ make test
```

### License
MIT