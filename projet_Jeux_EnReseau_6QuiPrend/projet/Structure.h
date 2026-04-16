//
// Created by basti on 03/11/2025.
//

#ifndef PROJET_STRUCTURE_H
#define PROJET_STRUCTURE_H

/* Structure.h : définitions des types utilisés par le projet */

#include <stddef.h>

#define Carte_par_Joueur 10

typedef enum {
    ETAT_PIOCHE,    /* pas encore utilisée */
    ETAT_MAIN,      /* distribuée à un joueur */
    ETAT_PLATEAU,   /* sur le plateau */
    ETAT_JOUEE      /* jouée pendant un tour (ou défaussée) */
} EtatCarte;

typedef struct {
    int numero;   /* de 1 à 104 */
    int boeufs;   /* nombre de têtes revient au score que le joueur va cumuler durant la partie*/
    EtatCarte etat;
    int joueur_id; /* id du joueur propriétaire (ou -1 si aucun) */
} Carte;

typedef struct {
    Carte cartes[Carte_par_Joueur];
    int id;
    int score;
    int nb_cartes;
    int sock; /* socket associé (si joueur réseau), -1 sinon */
    int est_robot; // si 0 = humain si 1 = robot
} Joueur;

typedef enum {
    MODE_JVJ = 1,
    MODE_JVROBOT= 2
} ModeJeu;


#endif /* PROJET_STRUCTURE_H */
