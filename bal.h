#ifndef _BAL_H_
#define _BAL_H_

  #include "liste.h"
  #include "messages.h"

  typedef struct {
    char* message;
    int lg;
  } t_message;

  typedef struct {
    int dest;
    t_liste* liste;
  } t_utilisateur;

  /**
   * @brief Reception des message d'un émetteur par le serveur BAL
   * 
   * @param sock Identifiant du socket du serveur
   * @param utilisateurs Liste des boîtes aux lettres des utilisateurs
   * @param infos PDU applicatif 
   */
  void recevoir_messages_serveur_bal(int sock, t_liste* liste, t_infos infos);

  /**
   * @brief Envoi des messages aux récepteur par le serveur BAL
   * 
   * @param sock Identifiant du socket du serveur
   * @param utilisateurs Liste des boîtes aux lettres des utilisateurs
   * @param infos PDU applicatif 
   */
  void envoyer_messages_serveur_bal(int sock, t_liste* liste, t_infos infos);

  /**
   * @brief Réception du PDU applicatif par le serveur BAL et action en fonction
   * 
   * @param sock Identifiant du socket du serveur
   * @param utilisateurs Liste des boîtes aux lettres des utilisateurs
   */
  void detecter_type_client_bal(int sock, t_liste* liste);

  /**
   * @brief Envoi du PDU applicatif par le client et reception des messages de l'utilisateur
   * 
   * @param sock Identifiant du socket du client
   * @param infos PDU Applicatif
   */
  void recevoir_messages_client_bal(int sock, t_infos infos);

  /**
   * @brief Envoi du PDU applicatif puis des messages au serveur
   * 
   * @param sock Identifiant du socket du client
   * @param infos PDU Applicatif
   */
  void envoyer_messages_client_bal(int sock, t_infos infos);

#endif