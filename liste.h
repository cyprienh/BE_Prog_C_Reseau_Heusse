#ifndef _LISTE_H
#define _LISTE_H

  // Noeud de la liste pouvant contenir n'importe quel type d'élément
  typedef struct t_noeud{
    void* element;
    struct t_noeud* suivant;
    struct t_noeud* precedant;
  }t_noeud;

  // Définition du type t_liste
  typedef struct{
    t_noeud* premier;
    t_noeud* dernier;
    int nb_elements;
  }t_liste;

  typedef struct{ 
    t_noeud* courant;
  }t_listIterator;

  /**
   * @brief Initialise une liste
   * 
   * @return t_liste* - Liste initialisée
   */
  t_liste* init();

  /**
   * @brief Efface la mémoire occupée par les noeuds et vide la liste
   * 
   * @param liste Liste à vider
   * @param clear_element Booléen indiquant si on veut supprimer les noeuds de la liste ou non
   */
  void clear(t_liste* liste, int clear_element);

  /**
   * @brief Retourne la taille de la liste
   * 
   * @param liste Liste dont on veut savoir la taille 
   * @return int - Taille de la liste
   */
  int size(const t_liste* liste);

  /**
   * @brief Ajoute un élément au début de la liste
   * 
   * @param liste Liste à laquelle on veut ajouter un élément
   * @param element Élément à ajouter
   */
  void addFirst(t_liste* liste, void* element);

  /**
   * @brief Ajoute un élément à la fin de la liste
   * 
   * @param liste Liste à laquelle on veut ajouter un élément
   * @param element Élément à ajouter
   */
  void addLast(t_liste* liste, void* element);

  /**
   * @brief Retourne le premier élément de la liste
   * 
   * @param liste Liste dont on veut récupérer le premier élément
   * @return void* - Premier élément de la liste
   */
  void* getFirst(const t_liste* liste);

  /**
   * @brief Retourne le dernier élément de la liste
   * 
   * @param liste Liste dont on veut récupérer le dernier élément
   * @return void* - Dernier élément de la liste
   */
  void* getLast(const t_liste* liste);

  // 
  /**
   * @brief Retourne l'élément à n'importe quelle position
   * 
   * @param liste Liste dont on veut récupérer un élément
   * @param pos Position de l'élément que l'on veut récupérer
   * @return void* - Élément à la position pos de la liste
   */
  void* get(const t_liste* liste,int pos);

  /**
   * @brief Ajoute un élément à la position indiquée
   * 
   * @param liste Liste à laquelle on veut ajouter un élément
   * @param element Élément à ajouter
   * @param pos Position à laquelle on veut ajouter un élément
   */
  void add(t_liste* liste, void* element, int pos);

  /**
   * @brief Retire le premier élément de la liste et le retourne
   * 
   * @param liste Liste dont on veut récupérer le premier élément
   * @return void* - Ancien premier élément de la liste
   */
  void* popFirst(t_liste* liste);

  /**
   * @brief Retire le dernier élément de la liste et le retourne
   * 
   * @param liste Liste dont on veut récupérer le dernier élément
   * @return void* - Ancien dernier élément de la liste
   */
  void* popLast(t_liste* liste);

  // Retire l'élément à la position pos et le retourne
  /**
   * @brief Retire l'élément à n'importe quelle position et le retourne
   * 
   * @param liste Liste dont on veut récupérer un élément
   * @param pos Position de l'élément à récupérer
   * @return void* - Élément anciennement à la position pos de la liste
   */
  void* pop(t_liste* liste, int pos);

  /**
   * @brief Retourne un itérateur pour parcourir les éléments de la liste efficacement
   * 
   * @param liste Liste pour laquelle on veut créer un itérateur 
   * @return t_listIterator - Itérateur pour la liste
   */
  t_listIterator iterator(t_liste* liste);
 
  /**
   * @brief Indique s’il reste des éléments à parcourir
   * 
   * @param iter Itérateur
   * @return int - Booléen indiquant s'il reste des éléments à parcourir
   */
  int hasNext(const t_listIterator* iter);

  /**
   * @brief Retourne l’ élément courant et avance l’itérateur vers le prochain élément
   * 
   * @param iter Itérateur
   * @return void* - Élément courant
   */
  void* next(t_listIterator* iter);

#endif
