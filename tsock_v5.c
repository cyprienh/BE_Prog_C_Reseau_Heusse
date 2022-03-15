// Librairie standard
#include <stdlib.h>
// getopt
#include <unistd.h>
// Déclaration des types de base
#include <sys/types.h>
// Constantes relatives aux domaines, types et protocoles
#include <sys/socket.h>
// Constantes et structures propres au domaine UNIX
#include <sys/un.h>
// Constantes et structures propres au domaine INTERNET
#include <netinet/in.h>
// Structures retournées par les fonctions de gestion de la base de données du réseau
#include <netdb.h>
// Communication avec les entrées/sorties
#include <stdio.h>
// Gestion des erreurs
#include <errno.h>
// Procédures liées à la création et utilisation de string en C
#include <string.h>

#include "udp.h"
#include "tcp.h"
#include "bal.h"
#include "liste.h"
#include "messages.h"

/*
==== PROTOTYPES DES PROCÉDURES PRINCIPALES ====
*/
/**
 * @brief Construction du socket et utilisation du puits en UDP
 * 
 * @param port Le port utilisé pour communiquer
 * @param nb_msg Le nombre de messages à recevoir
 * @param lg_msg La longueur de chaque message à recevoir
 */
void puit_udp(int port, int nb_msg, int lg_msg);
/**
 * @brief Construction du socket et utilisation de la source en UDP
 * 
 * @param addr L'adresse du puit auquel envoyer les messages
 * @param port Le port utilisé pour communiquer
 * @param nb_msg Le nombre de messages à recevoir
 * @param lg_msg La longueur de chaque message à recevoir
 */
void source_udp(char* addr, int port, int nb_msg, int lg_msg);

/**
 * @brief Construction du socket et utilisation du puits en TCP
 * 
 * @param port Le port utilisé pour communiquer
 * @param nb_msg Le nombre de messages à recevoir
 * @param lg_msg La longueur de chaque message à recevoir
 */
void puit_tcp(int port, int nb_msg, int lg_msg);
/**
 * @brief Construction du socket et utilisation de la source en TCP
 * 
 * @param addr L'adresse du puit auquel envoyer les messages
 * @param port Le port utilisé pour communiquer
 * @param nb_msg Le nombre de messages à recevoir
 * @param lg_msg La longueur de chaque message à recevoir
 */
void source_tcp(char* addr, int port, int nb_msg, int lg_msg);

/**
 * @brief Construction du socket et gestion des connexions au serveur BAL
 * 
 * @param port Le port utilisé pour communiquer
 */
void serveur_bal(int port);
/**
 * @brief Construction du socket et émission des messages par le client BAL
 * 
 * @param addr L'adresse du puit auquel envoyer les messages
 * @param port Le port utilisé pour communiquer
 * @param infos Structure contenant les informations à envoyer au serveur BAL
 */
void emetteur_bal(char* addr, int port, t_infos infos);
/**
 * @brief Construction du socket et réception des messages par le client BAL
 * 
 * @param addr L'adresse du puit auquel envoyer les messages
 * @param port Le port utilisé pour communiquer
 * @param infos Structure contenant les informations à envoyer au serveur BAL
 */
void recepteur_bal(char* addr, int port, t_infos infos);

/*
==== MAIN DE LA COMMANDE TSOCK ====
*/
void main (int argc, char **argv) {
	int c;
	extern char *optarg;
	extern int optind;
	int nb_message = -1;  // Nombre de messages à envoyer/recevoir, -1 indique infini pour le puit
  int lg_message = -1;  // Longeur des messages à envoyer/recevoir
	int source = -1 ;     // Type du socket: 0=puits, 1=source

  int type = 0;         // Protocole à utiliser: 0=tcp, 1=udp

  int port = atoi(argv[argc-1]);  // Port à utiliser pour communiquer
  char* addr;                     // Pointeur vers la chaine de caractères de l'adresse internet

  int bal = 0;        // Indique s'il s'agit d'un serveur BAL: 0=faux, 1=vrai
  int emetteur = -1;  // Identifiant de l'utilisateur à qui envoyer un message, -1 si aucun
  int recepteur = -1; // Identifiant de l'utilisateur qui veut recevoir ses messages, -1 si aucun

  if(argc <= 1) {
    utilisation_message();
    exit(1);
  }

	while ((c = getopt(argc, argv, "vbpn:sl:ue:r:")) != -1) {
		switch (c) {
		case 'p':
			if (source == 1) {
				utilisation_message();
				exit(1);
			}
      if (bal == 1) {
        printf("ERREUR: L'option -b n'est pas compatible avec les options -s & -p\n");
        exit(1);
      }
			source = 0;
			break;

		case 's':
			if (source == 0) {
				utilisation_message();
				exit(1) ;
			}
      if (bal == 1) {
        printf("ERREUR: L'option -b n'est pas compatible avec les options -s & -p\n");
        exit(1);
      }
			source = 1;
			break;

    case 'u':
			type = 1;
			break;

    case 'b':
      if (source != -1) {
        printf("ERREUR: Les options -s & -p ne sont pas compatibles avec l'option -b\n");
        exit(1);
      }
			bal = 1;
			break;

		case 'n':
			nb_message = atoi(optarg);
			break;

    case 'e':
			emetteur = atoi(optarg);
      if(emetteur < 0) {
        printf("ERREUR: L'identifiant d'un utilisateur doit être positif\n");
        exit(1);
      }
			break;

    case 'r':
			recepteur = atoi(optarg);
      if(recepteur < 0) {
        printf("ERREUR: L'identifiant d'un utilisateur doit être positif\n");
        exit(1);
      }
			break;

    case 'l':
			lg_message = atoi(optarg);
      if(lg_message <= 5) {
        printf("ERREUR: La longueur d'un message doit être strictement supérieure à 5\n");
        exit(1);
      }
			break;

    case 'v':
			printf("VERSION: 5.0\n");
      exit(0);
			break;

		default:
			utilisation_message();
			break;
		}
	}

  if(lg_message == -1) {
    lg_message = 30;
  }

  if(!(port > 0 && port <= 65535)) {
    printf("ERREUR: La valeur du port doit être comprise entre 1 et 65535\n");
    exit(1);
  }

  if(bal) {
    if(type == 1) {
      printf("ERREUR: Le protocole BAL ne fonctionne qu'en TCP\n");
      exit(1);
    }
    if(nb_message != -1) {
      printf("AVERTISSEMENT: L'option -b outrepasse l'option -n\n");
    }
    if(emetteur != -1 || recepteur != -1) {
      printf("AVERTISSEMENT: L'option -b outrepasse les options -r & -e\n");
    }

    printf("SERVEUR: port=%d, TP=tcp\n", port);
    serveur_bal(port);

  } else {
    if(emetteur >= 0) {
      printf("INFO: Émission de %d messages de taille %d pour l'utilisateur %d\n", nb_message, lg_message, emetteur);
      addr = argv[argc-2];
      t_infos infos;
      infos.emetteur = 1;
      infos.lg_messages = lg_message;
      infos.nb_messages = nb_message;
      infos.destinataire = emetteur;
      emetteur_bal(addr, port, infos);
    } else if(recepteur >= 0) {
      printf("INFO: Réception des messages pour l'utilisateur %d\n", recepteur);
      addr = argv[argc-2];
      t_infos infos;
      infos.emetteur = 0;
      infos.lg_messages = 0;
      infos.nb_messages = 0;
      infos.destinataire = recepteur;
      recepteur_bal(addr, port, infos);
    } else {
      char text_type[4]; 
      if(type == 1)
        strcpy(text_type, "udp");
      else
        strcpy(text_type, "tcp");

      if (source == -1) {
        utilisation_message();
        exit(1) ;
      } else if(source == 1) { //source
        addr = argv[argc-2];
        if(nb_message == -1) {
          nb_message = 10;
        }
        printf("SOURCE: lg_mesg_emis=%d, port=%d, nb_envois=%d, TP=%s, dest=%s\n", lg_message, port, nb_message, text_type, addr);
      } else if(source == 0) { //puit
        if(nb_message == -1)
          printf("PUIT: lg_mesg_lu=%d, port=%d, nb_receptions=infini, TP=%s\n", lg_message, port, text_type);
        else
          printf("PUIT: lg_mesg_lu=%d, port=%d, nb_receptions=%d, TP=%s\n", lg_message, port, nb_message, text_type);
      }

      if(type == 1) {
        if(source) {
          source_udp(addr, port, nb_message, lg_message);
        } else {
          puit_udp(port, nb_message, lg_message);
        }
      } else if(type == 0) {
        if(source) {
          source_tcp(addr, port, nb_message, lg_message);
        } else {
          puit_tcp(port, nb_message, lg_message);
        }
      }
    }
  }

}

/*
==== PROCÉDURES ====
*/
void puit_udp(int port, int nb_msg, int lg_msg) {
  struct sockaddr_in adr_local;
  struct sockaddr *padr_em;

  int sock = creer_socket_udp();

  int lg_adr_local = construire_puit_udp(sock, port, &adr_local);

  int lg_padr_em = recevoir_messages_udp(sock, padr_em, nb_msg, lg_msg);

  fermer_socket_udp(sock);
}

void source_udp(char* addr, int port, int nb_msg, int lg_msg) {
  struct sockaddr_in adr_distant;

  int sock = creer_socket_udp();

  int lg_adr_distant = construire_source_udp(sock, addr, port, &adr_distant);

  envoyer_messages_udp(sock, &adr_distant, nb_msg, lg_msg);

  fermer_socket_udp(sock);
}

void puit_tcp(int port, int nb_msg, int lg_msg) {
  struct sockaddr_in adr_local;
  struct sockaddr adr_client;

  int lg_adr_client;

  int sock = creer_socket_tcp();

  int lg_adr_local = construire_puit_tcp(sock, port, &adr_local);

  int sock2 = attendre_connexion_tcp(sock, &adr_client, &lg_adr_client);

  recevoir_messages_tcp(sock2, nb_msg, lg_msg);

  fermer_socket_tcp(sock);
  fermer_socket_tcp(sock2);
}

void source_tcp(char* addr, int port, int nb_msg, int lg_msg) {
  struct sockaddr_in adr_distant;

  int sock = creer_socket_tcp();

  int lg_adr_distant = construire_source_tcp(sock, addr, port, &adr_distant);

  envoyer_messages_tcp(sock, nb_msg, lg_msg);

  fermer_socket_tcp(sock);
}

void recepteur_bal(char* addr, int port, t_infos infos) {
  struct sockaddr_in adr_distant;

  int sock = creer_socket_tcp();

  int lg_adr_distant = construire_source_tcp(sock, addr, port, &adr_distant);

  recevoir_messages_client_bal(sock, infos);

  printf("INFO: Fermeture de la connexion avec le serveur\n");
  fermer_socket_tcp(sock);
}

void emetteur_bal(char* addr, int port, t_infos infos) {
  struct sockaddr_in adr_distant;

  int sock = creer_socket_tcp();

  int lg_adr_distant = construire_source_tcp(sock, addr, port, &adr_distant);

  envoyer_messages_client_bal(sock, infos);

  printf("INFO: Fermeture de la connexion avec le serveur\n");
  fermer_socket_tcp(sock);
}

void serveur_bal(int port) {
  struct sockaddr_in adr_local;
  struct sockaddr adr_client;

  int lg_adr_client;

  t_liste* utilisateurs = init();

  int sock = creer_socket_tcp();

  int lg_adr_local = construire_puit_tcp(sock, port, &adr_local);

  while(1) {
    int sock2 = attendre_connexion_tcp(sock, &adr_client, &lg_adr_client);

    detecter_type_client_bal(sock2, utilisateurs);

    fermer_socket_tcp(sock2);
    printf("INFO: Fermeture de la connexion avec le client\n");
  };

  fermer_socket_tcp(sock);
}