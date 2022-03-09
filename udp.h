#ifndef _UDP_H_
#define _UDP_H_

  /* librairie standard ... */
  #include <stdlib.h>
  /* pour getopt */
  #include <unistd.h>
  /* déclaration des types de base */
  #include <sys/types.h>
  /* constantes relatives aux domaines, types et protocoles */
  #include <sys/socket.h>
  /* constantes et structures propres au domaine UNIX */
  #include <sys/un.h>
  /* constantes et structures propres au domaine INTERNET */
  #include <netinet/in.h>
  /* structures retournées par les fonctions de gestion de la base de
  données du réseau */
  #include <netdb.h>
  /* pour les entrées/sorties */
  #include <stdio.h>
  /* pour la gestion des erreurs */
  #include <errno.h>

  #include <string.h>
  #include <arpa/inet.h>

  #include "udp.h"

  int creer_socket_udp();

  int construire_puit_udp(int sock, int port, struct sockaddr_in *adr_local);

  int construire_source_udp(int sock, char* addr, int port, struct sockaddr_in *adr_distant);

  void recevoir_messages_udp(int sock, struct sockaddr *padr_em, int number, int length);

  void envoyer_messages_udp(int sock, struct sockaddr_in *adr_distant, int number, int length);

  void fermer_socket_udp(int sock);

#endif