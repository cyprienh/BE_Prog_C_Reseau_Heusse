#ifndef _UDP_H_
#define _UDP_H_

  #include <netinet/in.h>

  /**
   * @brief Création d'un socket en UDP
   * 
   * @return int - Identifiant du socket créé
   */
  int creer_socket_udp();

  /**
   * @brief Construction du puit en UDP
   * 
   * @param sock Identifiant du socket du puit
   * @param port Port utilisé pour communiquer
   * @param adr_local Pointeur vers l'adresse locale du socket
   * @return int - Taille de l'adresse locale
   */
  int construire_puit_udp(int sock, int port, struct sockaddr_in *adr_local);

  /**
   * @brief Construction de la source en UDP
   * 
   * @param sock Identifiant du socket de la source
   * @param addr Adresse du puit auquel envoyer les messages
   * @param port Port utilisé pour communiquer
   * @param adr_distant Pointeur vers l'adresse du puit distant
   * @return int - Taille de l'adresse distante
   */
  int construire_source_udp(int sock, char* addr, int port, struct sockaddr_in *adr_distant);

  /**
   * @brief Reception des messages en UDP par le puit
   * 
   * @param sock Identifiant du socket du puit
   * @param padr_em Pointeur vers l'adresse de la source
   * @param number Nombre de messages à recevoir
   * @param lg_msg Longueur de chaque message à recevoir
   * @return int - Taille de l'adresse de la source
   */
  int recevoir_messages_udp(int sock, struct sockaddr *padr_em, int number, int length);

  /**
   * @brief Envoi des messages en UDP par la source
   * 
   * @param sock Identifiant du socket de la source
   * @param adr_distant Pointeur vers l'adresse du puit
   * @param number Nombre de messages à recevoir
   * @param length Longueur de chaque message à recevoir
   */
  void envoyer_messages_udp(int sock, struct sockaddr_in *adr_distant, int number, int length);

  /**
   * @brief Fermeture d'un socket en UDP
   * 
   * @param sock Identifiant du socket à fermer
   */
  void fermer_socket_udp(int sock);

#endif