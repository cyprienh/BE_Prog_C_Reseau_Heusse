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
#include "tcp.h"
#include "messages.h"

int serveur_udp(int port, int nb_msg, int lg_msg, int emetteur);
int client_udp(char* addr, int port, int nb_msg, int lg_msg, int emetteur);

int serveur_tcp(int port, int nb_msg, int lg_msg, int emetteur);
int client_tcp(char* addr, int port, int nb_msg, int lg_msg, int emetteur);

void main (int argc, char **argv) {
	int c;
	extern char *optarg;
	extern int optind;
	int nb_message = -1; /* Nb de messages à envoyer ou à recevoir, par défaut : 10 en émission, infini en réception */
  int lg_message = -1; /* Longueur des messages à envoyer ou recevoir, par défaut 30 */
	int source = -1; /* 0=puits, 1=source */

  int type = 0; /* 0=tcp, 1=udp */

  int client = -1;    /* 0=serveur, 1=client */
  int emetteur = -1;  /* 0=recepteur, 1=emetteur */

  int port = atoi(argv[argc-1]); /* Port du puit */
  char* addr; /* Adresse du puit si source */

  int args_desire = 1;  /* Nombre d'argument qu'on s'attend à recevoir */

	while ((c = getopt(argc, argv, "csern:l:u")) != -1) {
		switch (c) {
      case 'c':
        if (client == 0) {
          utilisation_message();
          exit(1);
        }
        client = 1;
        args_desire += 3;
        break;

      case 's':
        if (client == 1) {
          utilisation_message();
          exit(1);
        }
        client = 0;
        args_desire += 2;
        break;

      case 'e':
        if (emetteur == 0) {
          utilisation_message();
          exit(1);
        }
        emetteur = 1;
        args_desire += 1;
        break;

      case 'r':
        if (emetteur == 1) {
          utilisation_message();
          exit(1);
        }
        emetteur = 0;
        args_desire += 1;
        break;

      case 'u':
        type = 1;
        args_desire += 1;
        break;

      case 'n':
        nb_message = atoi(optarg);
        args_desire += 2;
        break;

      case 'l':
        lg_message = atoi(optarg);
        if(lg_message <= 5) {
          printf("ERREUR: la longueur d'un message doit être strictement supérieure à 5\n");
          exit(1);
        }
        args_desire += 2;
        break;

      default:
        utilisation_message();
        break;
		}
	}

  if(argc == args_desire) {
    char text_type[4]; 
    if(type == 1)
      strcpy(text_type, "udp");
    else
      strcpy(text_type, "tcp");

    if(lg_message == -1) {
      lg_message = 30;
    }

    if(emetteur == -1) {
      emetteur = client;
    }

    if(client == -1) {
      utilisation_message();
      exit(1);
    }

    if(type == 1 && client != emetteur) {
      printf("ERREUR: En UDP, le client doit être l'émetteur.\n");
      exit(1);
    }
    
    if(client == 1) { //source - client
      addr = argv[argc-2];
      if(emetteur == 1) {
        if(nb_message == -1) {
          nb_message = 10;
        }
        printf("CLIENT: lg_mesg_emis=%d, port=%d, nb_envois=%d, TP=%s, dest=%s\n", lg_message, port, nb_message, text_type, addr);
      } else if(emetteur == 0) {
        if(nb_message == -1)
          printf("CLIENT: lg_mesg_lu=%d, port=%d, nb_receptions=infini, TP=%s\n", lg_message, port, text_type, addr);
        else
          printf("CLIENT: lg_mesg_lu=%d, port=%d, nb_receptions=%d, TP=%s\n", lg_message, port, nb_message, text_type, addr);
      }
    } else if(client == 0) { //puit - server
      if(emetteur == 1) {
        if(nb_message == -1) {
          nb_message = 10;
        }
        printf("SERVEUR: lg_mesg_emis=%d, port=%d, nb_envois=%d, TP=%s\n", lg_message, port, nb_message, text_type);
      } else if(emetteur == 0) {
        if(nb_message == -1)
          printf("SERVEUR: lg_mesg_lu=%d, port=%d, nb_receptions=infini, TP=%s\n", lg_message, port, text_type);
        else
          printf("SERVEUR: lg_mesg_lu=%d, port=%d, nb_receptions=%d, TP=%s\n", lg_message, port, nb_message, text_type);
      }
    }

    if(type == 1) {
      if(client) {
        client_udp(addr, port, nb_message, lg_message, emetteur);
      } else {
        serveur_udp(port, nb_message, lg_message, emetteur);
      }
    } else if(type == 0) {
      if(client) {
        client_tcp(addr, port, nb_message, lg_message, emetteur);
      } else {
        serveur_tcp(port, nb_message, lg_message, emetteur);
      }
    }
  } else {
    printf("ERREUR: Mauvais nombre d'arguments dans l'appel de la fonction (%d/%d)\n", argc, args_desire);
    exit(1);
  }

}

int serveur_udp(int port, int nb_msg, int lg_msg, int emetteur) {
  struct sockaddr_in adr_local;
  struct sockaddr *padr_em;

  int sock = creer_socket_udp();

  int lg_adr_local = construire_puit_udp(sock, port, &adr_local);

  recevoir_messages_udp(sock, padr_em, nb_msg, lg_msg);

  fermer_socket_udp(sock);
}

int client_udp(char* addr, int port, int nb_msg, int lg_msg, int emetteur) {
  struct sockaddr_in adr_distant;

  int sock = creer_socket_udp();

  int lg_adr_distant = construire_source_udp(sock, addr, port, &adr_distant);

  envoyer_messages_udp(sock, &adr_distant, nb_msg, lg_msg);

  fermer_socket_udp(sock);
}

int serveur_tcp(int port, int nb_msg, int lg_msg, int emetteur) {
  struct sockaddr_in adr_local;
  struct sockaddr adr_client;

  int sock2;
  int lg_adr_client;

  int sock = creer_socket_tcp();

  int lg_adr_local = construire_puit_tcp(sock, port, &adr_local);

  listen(sock, 5);

  while (1) {

    if ((sock2 = accept( sock, (struct sockaddr *)&adr_client, &lg_adr_client)) == -1) {
      printf("ERREUR: Échec du accept.\n"); 
      exit(1);
    }

    switch (fork() ) {
      case - 1 : /* il y a une erreur */
        printf("ERREUR: fork.\n");
        exit(1);

      case 0 : /* on est dans le proc. fils */
        close(sock); /* fermeture socket du proc. père */
        if(emetteur) {
          envoyer_messages_tcp(sock2, nb_msg, lg_msg, 1);
        } else {
          recevoir_messages_tcp(sock2, nb_msg, lg_msg, 1);
        }
        //printf("SERVEUR: Fin de la réception.\n");
        exit(0);
        
      default : /* on est dans le proc. père */
        close(sock2); /* fermeture socket du proc. fils */
    }

  }

  fermer_socket_tcp(sock);
}

int client_tcp(char* addr, int port, int nb_msg, int lg_msg, int emetteur) {
  struct sockaddr_in adr_distant;

  int sock = creer_socket_tcp();

  int lg_adr_distant = construire_source_tcp(sock, addr, port, &adr_distant);

  if(emetteur) {
    envoyer_messages_tcp(sock, nb_msg, lg_msg, 0);
  } else {
    recevoir_messages_tcp(sock, nb_msg, lg_msg, 0);
  }

  fermer_socket_tcp(sock);
}