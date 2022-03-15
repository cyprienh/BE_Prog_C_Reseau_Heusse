#include "tcp.h"

#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "messages.h"

int creer_socket_tcp() {
  int sock ;
  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) { 
    printf("ERREUR: Échec de création du socket\n");
    exit(1); 
  }
}

int construire_puit_tcp(int sock, int port, struct sockaddr_in *adr_local) {
  int lg_adr_local = sizeof(*adr_local);

  memset((char *)adr_local, 0, sizeof(*adr_local));
  adr_local->sin_family = AF_INET;  // Domaine Internet
  adr_local->sin_port = port;       // N° de port
  adr_local->sin_addr.s_addr = INADDR_ANY;

  // Association @ socket <-> Représentation interne
  if (bind (sock, (struct sockaddr *)adr_local, lg_adr_local) == -1) { 
    printf("ERREUR: Échec du bind\n") ;
    exit(1);
  }
}

int attendre_connexion_tcp(int sock, struct sockaddr *adr_client, int *lg_adr_client) {
  int sock_bis;
  listen(sock, 5) ;

  if ((sock_bis = accept(sock, (struct sockaddr *)adr_client, lg_adr_client)) == -1) {
    printf("ERREUR: Échec du accept\n") ;
    exit(1) ;
  }

  return sock_bis;
}

int construire_source_tcp(int sock, char* addr, int port, struct sockaddr_in *adr_distant) {
  struct hostent *hp ;
  
  int lg_adr_distant = sizeof(*adr_distant) ;

  memset((char *)adr_distant, 0, sizeof(*adr_distant)) ;
  adr_distant->sin_family = AF_INET ;   // Domaine Internet
  adr_distant->sin_port = port ;        // N° de port

  if ((hp = gethostbyname(addr)) == NULL) { 
    printf("ERREUR: Échec de la connexion (adresse invalide).\n");
    exit(1); 
  }

  memcpy((char*)&(adr_distant->sin_addr.s_addr), hp->h_addr, hp->h_length ) ;

  if (connect(sock, (struct sockaddr *)adr_distant, lg_adr_distant) == -1) {
    printf("ERREUR: Échec de la connexion.\n") ;
    exit(1) ;
  }

  return lg_adr_distant;
}

void recevoir_messages_tcp(int sock, int number, int length) {
  int lg_recu = -1;
  char* msg = (char*)malloc(length+1);

  int i=0;

  while((number == -1 && lg_recu != 0) || (number != -1 && i < number)){
    i++;
    if ((lg_recu = read(sock, msg, length)) < 0) {
      printf("ERREUR: Échec de lecture des messages\n"); 
      exit(1);
    }
    if(lg_recu != 0) {
      msg[length] = 0;
      printf("PUIT: Reception n°%-5d (%d) [%s]\n", i, length, msg);
    }
  }

}

void envoyer_messages_tcp(int sock, int number, int length) {
  char* msg = (char*)malloc(length+1);
  char motif = 'a';

  int current = 0;

  for(int i=0; i<number; i++) {
    if(current == 99999)
      current = 0;
    current++;
    construire_message(msg, motif++, length, current);
    printf("SOURCE: Envoi n°%-5d (%d) [%s]\n", current, length, msg);
    if(write(sock, msg, length) == -1) {
      printf("ERREUR: Échec de l'envoi du message\n");
      exit(1);
    }
    if(motif == 'z'+1)
      motif = 'a';
  }
}

void fermer_socket_tcp(int sock) {
  if (close(sock) == -1) { 
    printf("ERREUR: Échec de destruction du socket\n");
    exit(1); 
  }
}