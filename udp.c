#include "udp.h"

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

int creer_socket_udp() {
  int sock;
  if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) { 
    printf("ERREUR: Échec de création du socket\n") ;
    exit(1) ; 
  }
  return sock;
}

int construire_puit_udp(int sock, int port, struct sockaddr_in *adr_local) {
  int lg_adr_local = sizeof(*adr_local) ;

  memset((char *)adr_local, 0, sizeof(*adr_local));
  adr_local->sin_family = AF_INET;  // Domaine Internet
  adr_local->sin_port = port;       // N° de port
  adr_local->sin_addr.s_addr = INADDR_ANY;

  // Association @ socket <-> Représentation interne
  if (bind (sock, (struct sockaddr *)adr_local, lg_adr_local) == -1) { 
    printf("ERREUR: Échec du bind\n") ;
    exit(1);
  }

  return lg_adr_local;
}

int construire_source_udp(int sock, char* addr, int port, struct sockaddr_in *adr_distant) {
  struct hostent *hp ;

  memset((char *)adr_distant, 0, sizeof(*adr_distant)) ;
  adr_distant->sin_family = AF_INET ; // Domaine Internet
  adr_distant->sin_port = port ;      // N° de port

  if ((hp = gethostbyname(addr)) == NULL) { 
    printf("ERREUR: gethostbyname\n");
    exit(1); 
  }

  memcpy((char*)&(adr_distant->sin_addr.s_addr), hp->h_addr, hp->h_length ) ;

  return sizeof(adr_distant);
}

int recevoir_messages_udp(int sock, struct sockaddr *padr_em, int number, int length) {
  char* pmes = (char*)malloc(length);
  int plg_adr_em = sizeof(padr_em);  

  int i=0;

  while(number == -1 || (number != -1 && i < number)){
    i++;
    recvfrom(sock, pmes, length, 0, padr_em, &plg_adr_em);
    printf("PUIT: Reception n°%-5d (%d) [%s]\n", i, length, pmes);
  }

  return plg_adr_em;
}

void envoyer_messages_udp(int sock, struct sockaddr_in *adr_distant, int number, int length) {
  int lg_adr_distant = sizeof(*adr_distant);

  char* msg = (char*)malloc(length);
  char motif = 'a';

  int current = 0;

  for(int i=0; i<number; i++) {
     if(current == 99999)
      current = 0;
    current++;
    construire_message(msg, motif++, length, current);
    printf("SOURCE: Envoi n°%-5d (%d) [%s]\n", current, length, msg);
    int lg_emis = sendto(sock, msg, length, 0, (struct sockaddr *)adr_distant, lg_adr_distant);
    if(motif == 'z'+1)
      motif = 'a';
  }
}

void fermer_socket_udp(int sock) {
  if (close(sock) == -1) { 
    printf("ERREUR: Échec de destruction du socket\n");
    exit(1); 
  }
}