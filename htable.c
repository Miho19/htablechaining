#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "htable.h"

#define NSIZE 14

static struct node *_htablesearch(table t, char *key);


void *emalloc(size_t s, char *message);

struct data {
    char buffer[NSIZE];
};

struct node {
    char key[NSIZE];
    void *ptr;
    struct node *next;
};

struct htable {
    uint capacity;
    uint nkeys;
    struct node **nodes;
    void (*pf)(uint, void *);
};


uint hash(struct htable *h, char *key) {
    uint r;
    r = 0;
    
    while(*key != '\0'){
        
        r = ((*key + 31) * *key) % h->capacity;
        key++;
    }
    
    return r;
}



uint htableinsert(table p, char *key, void *ptr){
    uint index;
    uint collisions;
    struct node *n;
    struct node *temp;
    
    struct htable *h;
    h = (struct htable *)p;
    n = 0;
    
    if(!key || !ptr || !h) return 0;
    index = hash(h, key);
    
    n = emalloc(sizeof(*n), "htableinsert node");
    n->ptr = ptr;
    n->next = 0;
    strncpy(n->key, key, strlen(key) + 1);
    
    if(h->nodes[index] == 0) {
        h->nodes[index] = n;
        h->nkeys++;
        return 1;
    }
        
    temp = h->nodes[index];
    
    n->next = temp;
    h->nodes[index] = n;
    h->nkeys++;
    
    return 1;
}

void *emalloc(size_t s, char *message) {
    void *r;
    r = 0;
    r = malloc(s);
    
    if(!r) {
        fprintf(stderr, "%s\n",message);
        exit(1);
    }
    
    return r;
}

void htabledelete(table t, char *key) {
    struct node *n;
    struct node *prev;
    struct htable *h;
    
    uint index;
    
    h = (struct htable *)t;
    
    n = 0;
    n = _htablesearch(t, key);
    if(!n) return;
    index = hash(h, key);
    
    prev = 0;
   
    if(!(n == h->nodes[index])) {
        for(prev = h->nodes[index]; prev->next != n && prev != 0; prev = prev->next){
            
        }
        prev->next = n->next;
    } else {
        
        h->nodes[index] = n->next;
    }
    
    n->ptr = 0;
    n->next = 0;
    strncpy(n->key, "", strlen("") + 1);
    free(n);
}

static struct node *_htablesearch(table t, char *key) {
    uint index;
    struct node *n;
    struct htable *h;
    
    h = (struct htable*)t;
    
    if(!h || !key) return 0;
    
    index = hash(h, key);
    
    if(h->nodes[index] == 0) return 0;
    n = h->nodes[index];
    
    for(; n != 0; n = n->next) {
        if(strncmp(n->key, key, strlen(key) + 1) == 0)
            return n;
    }
    
    return 0;
}

void* htablesearch(table t, char *key){
   struct node *n;
   n = 0;
   n = _htablesearch(t, key);
   
  if(!n) return 0;
  return n->ptr;
  
}

void* htablecreate(uint capacity) {
    struct htable *h;
    uint i;
    
    h = 0;
    h = emalloc(sizeof(*h), "htable create h"); 
    h->capacity = capacity;
    h->nkeys = 0;
    h->nodes = 0;
    h->nodes = emalloc(sizeof(h->nodes[0]) * h->capacity,"htable create h->nodes");
    
    for(i = 0; i < h->capacity; i++)
        h->nodes[i] = 0;
    
    return (void *)h;
    
}

void htableprint(table t){
    uint i;
    struct htable *h;
    struct node *n;
    
    h = (struct htable *)t;
    
    if(h->nkeys == 0) return;
    
    printf("START\n");
    
    for(i=0;i<h->capacity;i++) {
        n = h->nodes[i];
        printf("\t%d\t", i);
        if(n == 0)
            printf("---");
        for(; n != 0; n = n->next){
            printf("%s%s", n->key, n->next == 0 ? "" : "-->");
        }
        printf("\n");
    }
        
    
    printf("END\n");
}

void htabledestroy(table p){
    uint i;
    struct node *temp;
    struct node *next;
    
    struct htable *h;
    h = (struct htable *)p;
    
    
    for(i = 0; i < h->capacity;i++){
        if(h->nodes[i]) {
            for(temp = h->nodes[i]; temp != 0; temp = next) {
                next = temp->next;
                free(temp);
            }
        }
    }
    
    free(h->nodes);
    free(h);
}
