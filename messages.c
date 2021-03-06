#include "messages.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void construire_message(char *message, char motif, int lg, int number) {
  memset(message, 0, strlen(message));

  char num[5] = "-----";
  char* numf = (char*)malloc(5);
  char* numc = (char*)malloc(5);
  char* let = (char*)malloc(lg-5);

  for(int i=0; i<(lg-5); i++) {
    let[i] = motif;
  }

  sprintf(numc, "%d", number);
  int len = 5 - strlen(numc);
  sprintf(numf, "%*.*s%s", len, len, num, numc);
  strncpy(message, numf, 5);
  strcat(message, let);
  message[lg] = 0;
}

void utilisation_message() {
  printf("UTILISATION: tsock [options] -c port\n");
  printf("UTILISATION: tsock [options] -s port\n");
  printf("\n");
  printf("OPTIONS:\n");
  printf("    -u    : Utiliser le protocole UDP (défaut: TCP) \n");
  printf("            Compatible avec -c, -s\n");
  printf("    -e    : En TCP: Le client/serveur est aussi l'émetteur \n");
  printf("            Compatible avec -c, -s\n");
  printf("    -r    : En TCP: Le client/serveur est aussi le récepteur \n");
  printf("            Compatible avec -c, -s\n");
  printf("    -l ## : Longueur de chaque message à envoyer en octets (défaut: 30)\n");
  printf("            Compatible avec -c, -s, -e, -r, -u\n");
  printf("    -n ## : Nombre de messages à envoyer/recevoir (défaut: 10)\n");
  printf("            Compatible avec -c, -s, -e, -r, -u\n");
}