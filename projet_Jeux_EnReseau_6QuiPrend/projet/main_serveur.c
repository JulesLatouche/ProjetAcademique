#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>
#include "Gestionnaire_Jeux.h"
#include "reseau.h"

int main() {
    int sockfd;
    int clients[MAX_Joueur];
    srand(time(NULL));
    FILE *logf = fopen("game.log", "a");
    fprintf(logf, "SERVEUR_LANCE\n");
    fflush(logf);
    fclose(logf);


    printf("Choisissez le mode de jeu :\n");
    printf(" 1) Joueur vs Joueur\n");
    printf(" 2) Joueur vs Robot\n");
    char input[32];
    printf("Choisis un mode de jeux  : ");
    fgets(input, sizeof(input), stdin);
    int choix = atoi(input);

    if (choix == 2)
        mode_jeu = MODE_JVROBOT;
    else
        mode_jeu = MODE_JVJ;
    printf(" 111 [DEBUG] nb_joueurs = %d\n", nb_joueurs);

    if (mode_jeu == MODE_JVJ)
        nb_joueurs = 2;
    else
        nb_joueurs = 2;
    printf(" 222 [DEBUG] nb_joueurs = %d\n", nb_joueurs);

    sockfd = creer_socket_serveur(5050);

    if (mode_jeu == MODE_JVJ) {
        attendre_connexion_joueurs(sockfd, 2, clients);
    } else {
        attendre_connexion_joueurs(sockfd, 1, clients); // seulement l'humain
    }
    printf("333 [DEBUG] nb_joueurs = %d\n", nb_joueurs);

    // Initialisation des cartes, plateaux
    Initialisation_Carte();
    Initialisation_Plateaux();

    // Création des joueurs (humains + robot si mode_jeu == MODE_JVROBOT)
    Initalisation_Joueur(clients);

    Partie();

    // fermeture correcte des sockets
    for (int i =0 ; i< nb_joueurs;i++) {
        close(Lst_joueur[i].sock);
    }
    return 0;
}

