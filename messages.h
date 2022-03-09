#ifndef _MESSAGES_H_
#define _MESSAGES_H_

  typedef struct {
    int emetteur;     // 1=emetteur, 0=recepteur
    int nb_messages;
    int lg_messages;
    int destinataire; // identifiant du destinataire
  } t_infos;

  /**
   * @brief Construction des messages à envoyer par les sources
   * 
   * @param message Pointeur vers le message à créer
   * @param motif Le caractère à répéter
   * @param lg La taille du message à créer
   * @param number L'indice du message à créer
   */
  void construire_message(char *message, char motif, int lg, int number);

  /**
   * @brief Affichage des consignes d'utilisation de la commande
   * 
   */
  void utilisation_message();

#endif