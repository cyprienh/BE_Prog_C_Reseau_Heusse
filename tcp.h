#ifndef _TCP_H_
#define _TCP_H_

  #include <netinet/in.h>

  /**
   * @brief Création d'un socket en TCP
   * 
   * @return int - Identifiant du socket créé
   */
  int creer_socket_tcp();

  /**
   * @brief Construction du puit en TCP
   * 
   * @param sock Identifiant du socket du puit
   * @param port Port utilisé pour communiquer
   * @param adr_local Pointeur vers l'adresse locale du socket
   * @return int - Taille de l'adresse locale
   */
  int construire_puit_tcp(int sock, int port, struct sockaddr_in *adr_local);

  /**
   * @brief Attente de la connexion d'un client par le puit en TCP
   * 
   * @param sock Identifiant du socket du puit
   * @param adr_client Pointeur vers l'adresse de la source
   * @param lg_adr_client Longueur de l'adresse de la source
   * @return int - Identifiant du nouveau socket pour communiquer avec la source
   */
  int attendre_connexion_tcp(int sock, struct sockaddr *adr_client, int *lg_adr_client);

  /**
   * @brief Construction de la source en TCP
   * 
   * @param sock Identifiant du socket de la source
   * @param addr Adresse du puit auquel envoyer les messages
   * @param port Port utilisé pour communiquer
   * @param adr_distant Pointeur vers l'adresse du puit distant
   * @return int - Taille de l'adresse distante
   */
  int construire_source_tcp(int sock, char* addr, int port, struct sockaddr_in *adr_distant);

  /**
   * @brief Réception des messages en TCP par le puit
   * 
   * @param sock Identifiant du socket du puit
   * @param number Nombre de messages à recevoir
   * @param length Longueur de chaque message à recevoir
   */
  void recevoir_messages_tcp(int sock, int number, int length);

  /**
   * @brief Envoi des messages en TCP par la source
   * 
   * @param sock Identifiant du socket de la source
   * @param number Nombre de messages à recevoir
   * @param length Longueur de chaque message à recevoir
   */
  void envoyer_messages_tcp(int sock, int number, int length);

  /**
   * @brief Fermeture d'un socket en TCP
   * 
   * @param sock Identifiant du socket à fermer
   */
  void fermer_socket_tcp(int sock);

#endif