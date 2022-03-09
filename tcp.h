#ifndef _TCP_H_
#define _TCP_H_

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

  int creer_socket_tcp();

  int construire_puit_tcp(int sock, int port, struct sockaddr_in *adr_local);

  int construire_source_tcp(int sock, char* addr, int port, struct sockaddr_in *adr_distant);

  void recevoir_messages_tcp(int sock, int number, int length, int serveur);

  void envoyer_messages_tcp(int sock, int number, int length, int serveur);

  void fermer_socket_tcp(int sock);

#endif