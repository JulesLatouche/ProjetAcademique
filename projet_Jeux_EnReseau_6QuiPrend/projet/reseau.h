#ifndef RESEAU_H
#define RESEAU_H

/* Reseau.h : prototypes pour la couche réseau */

#include "Structure.h"
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stddef.h>
#include <sys/types.h> /* pour ssize_t */

#define PORT_DEFAULT 5050
#define TAILLE_BUF 256

/* === Fonctions Serveur === */
int creer_socket_serveur(uint16_t port);
int attendre_connexion_joueurs(int sockfd, int nb_joueurs, int clients[]);

/* === Fonctions Client === */
int connexion_serveur(const char *ip_serveur, uint16_t port);

/* === Communication (utilitaires) === */
/* renvoie le nombre d'octets envoyés ou -1 en cas d'erreur */
ssize_t envoyer_message(int sockfd, const char *message, size_t taille);
ssize_t recevoir_message(int sockfd, char *buffer, size_t taille);

/* wrappers pratiques */
int envoyer_printf(int sock, const char *buffer);

/* broadcast helpers — fonctionnelle si tu gardes liste de Joueur avec champ sock */
void envoyer_a_tous_sauf(const Joueur liste[], int nb, int id_exclu, const char *msg);
void envoyer_messages_a_tous(const Joueur liste[], int nb, const char *msg);

/* fonctions de plus haut niveau pour le jeu (si exposées) */
int demander_carte_res(int sock, Joueur *j);
int demander_ligne_res(int sock, Joueur *j);

/* optionnel : broadcast global */
void broadcast(const char *msg, const Joueur liste[], int nb);

#endif /* RESEAU_H */
