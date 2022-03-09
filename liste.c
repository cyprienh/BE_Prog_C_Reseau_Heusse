#include "liste.h"

#include <stdlib.h>
#include <stdbool.h>

t_liste* init() {
  t_liste* liste = (t_liste*)malloc(sizeof(t_liste));
  liste->premier = NULL;
  liste->dernier = NULL;
  liste->nb_elements = 0;
  return liste;
}

void clear(t_liste* liste, int clear_element) {
  t_noeud* courant = liste->premier;
  t_noeud* suivant;
  if (size(liste) == 0) return;
  while (courant != NULL){
    suivant = courant->suivant;
    if(clear_element) free(courant->element);
    free(courant);
    courant = suivant;
  }
  liste->premier = NULL;
  liste->dernier = NULL;
  liste->nb_elements = 0;
}

int size(const t_liste* liste) {
  return liste->nb_elements;
}

void addFirst(t_liste* liste, void* element) {
  t_noeud* n = malloc(sizeof(t_noeud));
  n->element = element;
  n->suivant = liste->premier;
  n->precedant = NULL;
  liste->premier = n;
  if(size(liste) == 0) liste->dernier = n;
  liste->nb_elements++;
}

void addLast(t_liste* liste, void* element) {
  t_noeud* n = malloc(sizeof(t_noeud));
  n->element = element;
  n->suivant = NULL; 
  n->precedant = liste->dernier;
  if(size(liste) == 0) {
    liste->premier = n;
    liste->dernier = n;
  } else {
    liste->dernier->suivant = n;
    liste->dernier = n;
  }
  liste->nb_elements++;
}

void* getFirst(const t_liste* liste) {
  if(size(liste) == 0) return NULL;
  return liste->premier->element;
}

void* getLast(const t_liste* liste) {
  if(size(liste) == 0) return NULL;
  return liste->dernier->element;
}

void* get(const t_liste* liste, int pos) {
  if(pos < size(liste)) {
    int i = 0;
    t_noeud* n = liste->premier;
    while(i < pos) { 
      n = n->suivant;
      i++;
    }
    return n->element;
  }
  return NULL;
}

void add(t_liste* liste, void* element, int pos) {
  if(pos <= size(liste)) {
    int i = 0;
    t_noeud* n = liste->premier;
    while(i < pos-1) { 
      n = n->suivant;
      i++;
    }
    t_noeud* new = malloc(sizeof(t_noeud));
    new->element = element;
    if(pos != 0) {
      new->suivant = n->suivant;
      n->suivant = new;
    } else {
      new->suivant = liste->premier;
      liste->premier = new;
    }
    liste->nb_elements++;
  }
}

void* popFirst(t_liste* liste){
  t_noeud* n = liste->premier;
  if(size(liste) == 0) return NULL;
  void* e = n->element;
  liste->premier = liste->premier->suivant;
  if(liste->premier != NULL)
    liste->premier->precedant = NULL;
  free(n);
  liste->nb_elements--;
  return e;
}

void* popLast(t_liste* liste){
  t_noeud* n = liste->dernier;
  if(size(liste) == 0) return NULL;
  void* e = n->element;
  liste->dernier = liste->dernier->precedant;
  liste->dernier->suivant = NULL;
  free(n);
  liste->nb_elements--;
  return e;
}

void* pop(t_liste* liste, int pos){
  if(pos < size(liste)) {
    t_noeud* n = liste->premier;
    t_noeud* p;
    int i = 0;
    while(i < pos) {
      p = n;
      n = n->suivant;
      i++;
    }
    p->suivant = n->suivant;
    void* e = n->element;
    free(n);
    liste->nb_elements--;
    return e;
  }
  return NULL;
}

t_listIterator iterator(t_liste* liste) {
  t_listIterator iter;
  iter.courant = liste->premier;
  return iter;
}

int hasNext(const t_listIterator* iter) {
  return iter->courant != NULL;
}

void* next(t_listIterator* iter) {
  if(hasNext(iter)) {
    void* element = iter->courant->element;
    iter->courant = iter->courant->suivant;
    return element;
  }
  return NULL;
}



