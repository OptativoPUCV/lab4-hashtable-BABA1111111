#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"


typedef struct HashMap HashMap;
int enlarge_called=0;

struct HashMap {
    Pair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair * createPair( char * key,  void * value) {
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash( char * key, long capacity) {
    unsigned long hash = 0;
     char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}

int is_equal(void* key1, void* key2){
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}


void insertMap(HashMap * map, char * key, void * value) {
  Pair* nuevoPar;
  nuevoPar = (Pair*) malloc (sizeof(Pair));

  nuevoPar->key = key;
  nuevoPar->value = value;


  long valorHash = hash(key, map->capacity);
  while(map->buckets[valorHash] != NULL){
    if (valorHash == map->capacity - 1){
      valorHash = 0;
    }
    valorHash++;
  }

  map->buckets[valorHash] = nuevoPar;
  map->current = valorHash;
  map->size += 1;
  
}

void enlarge(HashMap * map) {
  enlarge_called = 1; //no borrar (testing purposes)

  Pair** parAux = map->buckets;
  long capacidadAux = map->capacity;
  
  map->capacity *= 2;

  map->buckets = (Pair**) malloc(map->capacity * sizeof(Pair*));
    
  map->size = 0;
  
  for (long i = 0; i < capacidadAux; i++){
    if (parAux[i] != NULL){
      insertMap(map, parAux[i]->key, parAux[i]->value);
    }
  }

  free(parAux);
}


HashMap * createMap(long capacity) {
  HashMap* mapa = (HashMap*) malloc(sizeof(HashMap));
  mapa->buckets = (Pair**) malloc(sizeof(Pair*) * capacity);

  mapa->size = 0;
  mapa->capacity = capacity;
  mapa->current = -1;

  for (int i = 0; i < capacity; i++){
    mapa->buckets[i] = NULL;
  }
  return mapa;
}


void eraseMap(HashMap * map,  char * key) {    
  long valorHash = hash(key, map->capacity);

  if (strcmp(map->buckets[valorHash]->key,key)==0){
    map->current = -1;
    map->buckets[valorHash]->key = NULL;
  } else {
  while(map->buckets[valorHash] != NULL){
    if (strcmp(map->buckets[valorHash]->key,key)==0){
      map->current = -1;
      map->buckets[valorHash]->key = NULL;
    }
    if (valorHash == map->capacity - 1){
      valorHash = 0;
    }
    valorHash++;
  }
  }
  map->current = -1;
  map->size--;
}
// struct HashMap {
//     Pair ** buckets;
//     long size; //cantidad de datos/pairs en la tabla
//     long capacity; //capacidad de la tabla
//     long current; //indice del ultimo dato accedido
// };

Pair * searchMap(HashMap * map,  char * key) {   
  
  long valorHash = hash(key, map->capacity);
  if (strcmp(map->buckets[valorHash]->key,key)==0){
      map->current = valorHash;
      return map->buckets[valorHash];
  } else {
    while(map->buckets[valorHash] != NULL){
      if (strcmp(map->buckets[valorHash]->key,key)==0){
        map->current = valorHash;
        return map->buckets[valorHash];
      }
      if (valorHash == map->capacity - 1){
        valorHash = 0;
      }
      valorHash++;
    }
  }
  map->current = valorHash;
  return NULL;
}

Pair * firstMap(HashMap * map) {
  
  if (map->size != 0){
    for (long i = 0; i < map->capacity; i++){
      if (map->buckets[i] != NULL && map->buckets[i]->key != NULL){
        map->current = i;
        return map->buckets[i];
      }
    }
  }

  return NULL;
}

Pair * nextMap(HashMap * map) {
  
  if (map->size != 0){
    for (long i = map->current + 1; i < map->capacity; i++){
      if (map->buckets[i] != NULL && map->buckets[i]->key != NULL){
        map->current = i;
        return map->buckets[i];
      }
    }
  }
    return NULL;
}
