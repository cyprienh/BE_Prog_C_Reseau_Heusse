#include "bal.h"

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
#include "liste.h"

void recevoir_messages_serveur_bal(int sock, t_liste* utilisateurs, t_infos infos) {
  int lg_recu = -1;

  for(int i=0; i<infos.nb_messages || lg_recu == 0; i++){
    char* msg = (char*)malloc(infos.lg_messages+1);
    t_message* msg_final = (t_message*)malloc(sizeof(t_message));
    // Réception des messages
    if ((lg_recu = read(sock, msg, infos.lg_messages)) < 0) {
      printf("ERREUR: Échec du read\n"); 
      exit(1);
    }
    if(lg_recu != 0) {
      t_liste* liste;
      t_listIterator iter = iterator(utilisateurs);
      int trouve = 0;
      while(hasNext(&iter) && trouve == 0) {
        t_utilisateur* utilisateur = (t_utilisateur*)next(&iter);
        if(utilisateur->dest == infos.destinataire) {
          liste = utilisateur->liste;
          trouve = 1;
        }
      }

      msg_final->message = msg;
      msg_final->lg = infos.lg_messages;

      if(trouve) {  // Si l'utilisateur a déjà reçu du courier
        printf("SERVEUR: Courier pour l'utilisateur: %d\n", infos.destinataire);
        addLast(liste, msg_final); // On ajoute le courier à sa liste
      } else {      // Si il n'a jamais reçu de courier
        printf("SERVEUR: Nouvel utilisateur: %d\n", infos.destinataire);
        t_liste* nouvelle_liste = init();
        t_utilisateur* nouvel_utilisateur = (t_utilisateur*)malloc(sizeof(t_utilisateur));
        nouvel_utilisateur->dest = infos.destinataire;
        nouvel_utilisateur->liste = nouvelle_liste;
        addLast(utilisateurs, nouvel_utilisateur);  // On l'ajoute à la liste des utilisateurs
        addLast(nouvelle_liste, msg_final);         // On ajoute le courier à sa nouvelle liste
      }
    }
  }
}

void envoyer_messages_serveur_bal(int sock, t_liste* utilisateurs, t_infos infos) {
  t_listIterator iter = iterator(utilisateurs);
  int trouve = 0;
  while(hasNext(&iter) && trouve == 0) {
    t_utilisateur* utilisateur = (t_utilisateur*)next(&iter);
    if(utilisateur->dest == infos.destinataire) {
      trouve = 1;
      int nb_messages = size(utilisateur->liste);
      for(int i=0; i<nb_messages; i++) {
        t_message* msg = (t_message*)popFirst(utilisateur->liste);
        if(write(sock, &(msg->lg), sizeof(int)) == -1) {
          printf("ERREUR: Échec de l'envoi du message\n");
          exit(1);
        }
        if(write(sock, msg->message, msg->lg) == -1) {
          printf("ERREUR: Échec de l'envoi du message\n");
          exit(1);
        }
        printf("SERVEUR: Reception n°%-5d (%d) [%s]\n", i+1, msg->lg, msg->message);
      }
      printf("INFO: Fin de l'envoi\n");
    }
  }
}

void detecter_type_client_bal(int sock, t_liste* utilisateurs) {
  int lg_recu = -1;
  t_infos infos;

  if ((lg_recu = read(sock, &infos, sizeof(t_infos))) <= 0) {
    printf("ERREUR: Échec du read\n"); 
    exit(1);
  }

  // Si le client veut émettre des messages, le serveur fait la réception 
  if(infos.emetteur) {
    recevoir_messages_serveur_bal(sock, utilisateurs, infos);
  // Sinon le client veut recevoir ses messages donc on les-lui envoie
  } else {
    printf("INFO: Demande de reception par l'utilisateur %d\n", infos.destinataire);
    envoyer_messages_serveur_bal(sock, utilisateurs, infos);
  }
}

void recevoir_messages_client_bal(int sock, t_infos infos) {
  int i = 0;
  int lg_recu = -1;

  // Envoi du PDU applicatif au serveur
  if(write(sock, &infos, sizeof(t_infos)) == -1) {
    printf("ERREUR: Échec de l'envoi du message\n");
    exit(1);
  }

  // Réception des messages
  while(lg_recu != 0) {
    i++;
    int length;
    // Réception de la taille du message qui va suivre
    if ((lg_recu = read(sock, &length, sizeof(int))) < 0) {
      printf("ERREUR: Échec du read\n"); 
      exit(1);
    }
    char* msg = (char*)malloc(length+1);
    // Réception du message en lui-même
    if ((lg_recu = read(sock, msg, length)) < 0) {
      printf("ERREUR: Échec du read\n"); 
      exit(1);
    }
    if(lg_recu != 0) {
      msg[length] = 0;
      printf("CLIENT: Reception n°%-5d (%d) [%s]\n", i, length, msg);
    }
  }
    
  if(i == 1) {
    printf("INFO: Pas de messages\n");
  }
}

void envoyer_messages_client_bal(int sock, t_infos infos) {
  char* msg = (char*)malloc(infos.lg_messages+1);
  char first[5] = "envoi";
  char motif = 'a';

  // Envoi du PDU applicatif au serveur
  if(write(sock, &infos, sizeof(t_infos)) == -1) {
    printf("ERREUR: Échec de l'envoi du message\n");
    exit(1);
  }

  // Envoi des messages au serveur
  for(int i=0; i<infos.nb_messages; i++) {
    construire_message(msg, motif++, infos.lg_messages, infos.emetteur);
    printf("CLIENT: Envoi n°%-5d (%d) [%s]\n", i+1, infos.lg_messages, msg);
    if(write(sock, msg, infos.lg_messages) == -1) {
      printf("ERREUR: Échec de l'envoi du message\n");
      exit(1);
    }
    if(motif == 'z'+1)
      motif = 'a';
  }
}