#ifndef GESTIONNAIRE_JEUX_H
#define GESTIONNAIRE_JEUX_H

#include "Structure.h"  // Pour les types Joueur, Carte, etc.


// CONSTANTES DU JEU
#define MAX_Joueur 2
#define Nb_Carte_Totale 104
#define TAILLE_BUF 256
#define NB_manche  2
#define PORT 5050
#define SCORE_LIMITE 66

extern int nb_joueurs;                  // nombre réel dans la partie
extern Joueur Lst_joueur[MAX_Joueur];   // capacité max
extern Carte carte[Nb_Carte_Totale];
extern Carte plateau[4][6];
extern ModeJeu mode_jeu;

// Initialisation
void Initialisation_Plateaux();
void Initialisation_Plateaux_vide();
void Initalisation_Joueur(int client []);
void Initialisation_Carte();
//  Affichag
void Afficher_Plateaux();
void Afficher_carte();
void Afficher_score();
void Afficher_carte();
void afficher_etat_cartes();
void afficher_etat_de_la_carte(Carte c);
// Gestion des cartes et du jeu
void trier_carte(Carte cartes[], int n);
void placer_carte(Joueur *joueur, Carte c);
void placer_carte_V2(Joueur *joueur, Carte c , Joueur Lst_AutreJoueurs[] , int nbjoueur);
int mise_a_jour_carte_joueur(Carte main_joueur[], int nb_cartes);
int calculer_boeufs(Carte *c);
int tirer_carte_non_utilisee();
int tirer_carte_alea();
int Score_Ligne(int ligne);


// Logique principale
void placer_carte(Joueur *joeur , Carte c);
void Manche_Jeux();               // Joue une manche complète (10 tours)
void Partie();             // Gère plusieurs manches et conditions de victoire


void convertir_carte_en_char(Joueur *j ,char *msg);
void convertir_plateau_en_char(char *plateau_msg);
void convertir_score_en_char(Joueur*j, char *msg);

// robot

int robot_choisir_carte(Joueur *j);
int robot_choisir_ligne();
int choisir_carte(Joueur *j);
int choisir_ligne(Joueur *j);


// version de lancement


#endif
