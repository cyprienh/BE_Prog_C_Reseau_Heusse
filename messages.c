#include "messages.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void construire_message(char *message, char motif, int lg, int number) {
  memset(message, 0, strlen(message));
  
  char num[5] = "-----";
  char* numc = (char*)malloc(5);    // String qui contiendra l'indice du message (ex: "  120")
  char* numf = (char*)malloc(5);    // String qui contiendra l'indice du message auquel on ajoute des tirets (ex: "--120")
  char* let = (char*)malloc(lg-5);  // String qui contiendra le reste du message (répétition caractères)

  for(int i=0; i<(lg-5); i++) {
    let[i] = motif;
  }

  sprintf(numc, "%d", number);  // On met le nombre dans numc
  int len = 5 - strlen(numc);   // On calcule le nombre de tirets à ajouter 
  sprintf(numf, "%*.*s%s", len, len, num, numc); // On construit l'indice du message avec les tirets 
  strncpy(message, numf, 5);    // On copie l'indice du message au début du message
  strcat(message, let);         // On concatène le reste du message
  message[lg] = 0;
}

void utilisation_message() {
  printf("UTILISATION: tsock -p [options] port\n");
  printf("             tsock -s [options] adresse port\n");
  printf("             tsock -b [options] port\n");
  printf("             tsock -e utilisateur [options] adresse port\n");
  printf("             tsock -r utilisateur [options] adresse port\n");
  printf("\n");
  printf("OPTIONS:\n");
  printf("    -l ## : Longueur de chaque message à envoyer en octets (défaut: 30)\n");
  printf("            Compatible avec -p, -s, -e\n");
  printf("    -n ## : Nombre de messages à envoyer/recevoir (défaut: 10)\n");
  printf("            Compatible avec -p, -s, -e\n");
  printf("    -u    : Utiliser le protocole UDP (défaut: TCP) \n");
  printf("            Compatible avec -p, -s\n");
}