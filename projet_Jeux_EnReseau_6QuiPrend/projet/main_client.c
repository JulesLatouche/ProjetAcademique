//
// Created by basti on 03/11/2025.
//

//
// Created by basti on 03/11/2025.
//

#include <ctype.h>
#include "reseau.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Gestionnaire_Jeux.h"



void traiter_message_serveur(int sockfd, char *buffer) {
    //  l'affichage de plateau ce fait a chaque début de manche
    if (strncmp(buffer, "DEMANDER_CARTE", 14) == 0) {
        char input[32];
        printf("Choisis une carte : ");
        fgets(input, sizeof(input), stdin);
        int choix = atoi(input);
        sprintf(buffer, "CARTE %d", choix);
        envoyer_message(sockfd, buffer,strlen(buffer));
    }
    else if (strncmp(buffer, "DEMANDER_LIGNE", 14) == 0) {
        char input[32];
        printf("Choisis la ligne à ramasser (0-3) : ");
        fgets(input, sizeof(input), stdin);
        int choix = atoi(input);
        sprintf(buffer, "LIGNE %d", choix);
        envoyer_message(sockfd, buffer,strlen(buffer));
    }
    else if (strncmp(buffer, "FIN", 10) == 0) {
        printf("La partie est terminée !\n");
        exit(0);
    }
}

int main() {
    int sockfd = connexion_serveur("127.0.0.1",5050);
    char buffer[TAILLE_BUF];
    while (1) {
        memset(buffer, 0, sizeof(buffer)); // nettoie le buffer
        int n = recevoir_message(sockfd, buffer, TAILLE_BUF);
        if (n <= 0) {
            printf("[DEBUG SORTIE DU PROG PROBLEME DE MSG RECU]");
            break;
        }// déconnexion
        if ( n > 0) {
            buffer[n] = '\0';
            // pour la gestion joueur et robot car les msg vont plus vite que en TCP
            char *ligne = strtok(buffer, "\n");
            while (ligne != NULL) {
                //printf("[DEBUG CLIENT] Message traité: '%s'\n", buffer);
                traiter_message_serveur(sockfd, ligne);
                ligne = strtok(NULL, "\n");
            }

        }
    }




    close(sockfd);
    return 0;
}
