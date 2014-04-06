
uv mongo
================

open sourced async mongo library based on libuv and clibs library

### Usage
```c
void
find_cb(uvmongo_t * m, bson * res) {
  bson_print(res);
}

int
main(int argc, char ** argv) {
  uvmongo_t * m = uvmongo_new("localhost", 27017);
  uvmongo_db_t * mydb = uvmongo_db(m, "wave-api");
  uvmongo_collection_t * accounts = uvmongo_collection(mydb, "accounts");
  bson query[1], fields[1];
  bson_init(query);
  bson_finish(query);
  bson_init(fields);
  bson_finish(fields);
  uvmongo_find_one(accounts, query, fields, find_cb);
  
  uv_run(uv_default_loop(), UV_RUN_DEFAULT);
  return 0;
}
```


### API
##### uvmongo_new(char * hostname, int port);
create a connection to your mongoDB

##### uvmongo_db(uvmongo_t * m, char * name);
select db in your mongoDB

##### uvmongo_collection(uvmongo_db_t * db, char * name);
select collection of a db

##### uvmongo_find(uvmongo_collection_t * coll, bson * query, bson * field, int skip, int limit, void * callback);
query documents

##### uvmongo_find_one(uvmongo_collection_t * coll, bson * query, bson * field, void * callback);
query one document


### Run tests
```sh
$ make test
```

### License
MIT