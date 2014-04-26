
#pragma once

#include "uvmongo.h"
#include "db.h"

/*
 * create a new collection object
 */
uvmongo_collection_t *
uvmongo_collection_new(uvmongo_db_t * db, char * name);

/*
 * free a collection
 */
int
uvmongo_collection_free(uvmongo_collection_t * coll);

/*
 * internal usage for find command
 */
int
uvmongo__find(uvmongo_collection_t * coll, bson * query,
                                           bson * fields,
                                           bson * special,
                                           int skip,
                                           int limit,
                                           uvmongo_document_cb on_data,
                                           uvmongo_response_cb on_drain,
                                           void * privdata);

/*
 * find documents in a collection selected
 */
int
uvmongo_find(uvmongo_collection_t * coll, bson * query,
                                          bson * fields,
                                          bson * special,
                                          int skip,
                                          int limit,
                                          uvmongo_document_cb on_data,
                                          uvmongo_response_cb on_drain,
                                          void * privdata);

/*
 * find one document in a collection selected
 */
int
uvmongo_find_one(uvmongo_collection_t * coll, bson * query,
                                              bson * fields,
                                              bson * special,
                                              uvmongo_document_cb callback,
                                              void * privdata);

/*
 * inserts document
 */
int
uvmongo_insert(uvmongo_collection_t * coll, bson * doc);

/*
 * update documents
 */
int
uvmongo_update(uvmongo_collection_t * coll, bson * selector, bson * setup);

/*
 * delete documents
 */
int
uvmongo_remove(uvmongo_collection_t * coll, bson * selector);

