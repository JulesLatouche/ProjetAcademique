#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>


#include "Gestionnaire_Jeux.h"
#include "Structure.h"
#include "reseau.h"

int nb_cartes_par_rangee[4] = {0};
int nb_cartes_par_col[6] = {0};
Carte plateau[4][6]; // 4 rangées, max 6 cartes
Carte carte[Nb_Carte_Totale];
int nb_joueurs = 0;


Joueur Lst_joueur[MAX_Joueur];


ModeJeu mode_jeu = MODE_JVJ; // valeur par défaut



// Calcule des bœufs selon les règles
int calculer_boeufs(Carte *c) {
    int v = c->numero;

    if (v == 55)
        c->boeufs = 7;
    else if (v % 11 == 0)
        c->boeufs = 5;
    else if (v % 10 == 0)
        c->boeufs = 3;
    else if (v % 5 == 0)
        c->boeufs = 2;
    else
        c->boeufs = 1;

    //printf("Attribution : carte %d → %d boeufs\n", c->valeur, c->boeufs);
    return c->boeufs;
    }



void Initialisation_Plateaux_vide()
{
    for (int i = 0; i < 4; i++)
    {
        nb_cartes_par_rangee[i] = 0;
        for (int j = 0; j < 6; j++)
        {
            plateau[i][j].numero = 0;
            plateau[i][j].boeufs = 0;
        }
    }
}
void Afficher_Plateaux()
{
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0;j < 6; j++)
        {
            printf("%d (%d B) ",plateau[i][j].numero,plateau[i][j].boeufs);
        }
        printf(" \n");
    }

}

void Afficher_score()
{
    for (int i = 0; i < nb_joueurs; i++)
    {
        printf("le score du joueur %d est de : %d \n",Lst_joueur[i].id,Lst_joueur[i].score);
    }
}


// Tire une carte non encore utilisée
int tirer_carte_non_utilisee() {
    int indice;
    do {
        indice = rand() % Nb_Carte_Totale+1;
    } while (carte[indice].etat != ETAT_PIOCHE);
    carte[indice].etat = ETAT_MAIN;
    return indice;
}

/**
 *
 * @param nouvel_etat Le nouvelle état de notre carte que on met en param pour la rendre donc plus utilisable
 * @return renvoie l'indice de la carte tirer
 * */
int tirer_carte_alea(EtatCarte nouvel_etat) {
    int indice;
    do {
        indice = rand() % Nb_Carte_Totale;
        //printf("L'indice de la carte tirer en alea est : %d\n",indice);
    } while (carte[indice].etat != ETAT_PIOCHE);
    carte[indice].etat = nouvel_etat;
    return indice;
}



/**
 *
 * @param client on met donc ici la liste de nos client (2 le plus souvent)
 * on renvoie rien car il suffit juste de initialiser leurs main / id et on leurs associe un sock (le score 0)
 *
 */
void Initalisation_Joueur(int client[]) {

    if (mode_jeu == MODE_JVJ) {
        for (int i = 0; i < nb_joueurs; i++)
        {
            Lst_joueur[i].id = i + 1; // on met l'id par rapport au client +1 pour avoir client 1 2 3 ect ...
            Lst_joueur[i].score = 0;
            Lst_joueur[i].nb_cartes = Carte_par_Joueur;// param global (10)
            Lst_joueur[i].sock=client[i]; // donc par client  = sock associer
            for (int j = 0; j < Carte_par_Joueur; j++) { // donc en fonction du nb de carte par client on tire une carte (ici c'est un param global donc ça change pas)
                int idx = tirer_carte_alea(ETAT_MAIN);
                Lst_joueur[i].cartes[j] = carte[idx]; // on met la carte dans la main du joueur en question
                Lst_joueur[i].cartes[j].etat = ETAT_MAIN; // on lui change son état pour quelle plus réutilisable apres
            }
        }
    }
    else {
        printf("[DEBUG] mode de jeux Robot VS J \n");
        // on met les cartes
        // J VS ROBOT
        // Joueur humain
        Lst_joueur[0].id = 1;
        Lst_joueur[0].est_robot = 0;
        Lst_joueur[0].score = 0;
        Lst_joueur[0].nb_cartes = Carte_par_Joueur;
        Lst_joueur[0].sock = client[0];

        for (int j = 0; j < Carte_par_Joueur; j++) {
            int idx = tirer_carte_alea(ETAT_MAIN);
            Lst_joueur[0].cartes[j] = carte[idx];
            Lst_joueur[0].cartes[j].etat = ETAT_MAIN;
        }

        // Joueur robot
        Lst_joueur[1].id = 2;
        Lst_joueur[1].est_robot = 1;
        Lst_joueur[1].score = 0;
        Lst_joueur[1].nb_cartes = Carte_par_Joueur;
        Lst_joueur[1].sock = -1;

        for (int j = 0; j < Carte_par_Joueur; j++) {
            int idx = tirer_carte_alea(ETAT_MAIN);
            Lst_joueur[1].cartes[j] = carte[idx];
            Lst_joueur[1].cartes[j].etat = ETAT_MAIN;
        }
    }
}

/**
 * tout les carte de base son a un état de pioche le
 * changement d'etat est fait dans l'initialisation des joueurs
 */
void Initialisation_Carte() {
    for (int i = 0; i < Nb_Carte_Totale; i++) {
        carte[i].numero = i + 1;
        calculer_boeufs(&carte[i]);
        carte[i].etat = ETAT_PIOCHE;
    }
}
void distribuer_cartes()
{
    for (int i = 0; i < nb_joueurs; i++) {
        Lst_joueur[i].nb_cartes = Carte_par_Joueur;
        for (int j = 0; j < Carte_par_Joueur; j++) {
            int idx = tirer_carte_alea(ETAT_MAIN);
            Lst_joueur[i].cartes[j] = carte[idx];
        }
    }
}


/**
 * ICI on utilise pour quand le joueur doit ramasser une ligne et donc on ajoutera ce score au joueur en question
 * @param ligne La ligne que on veut calculer
 * @return renvoie donc le score total de la ligne en terme de tete de boeufs
 */
int Score_Ligne(int ligne){
    int score = 0;
    for (int i = 0; i < nb_cartes_par_rangee[ligne]; i++) // on peut pas mettre 6 car nos ligne peuvent etre vide
    {
        score += plateau[ligne][i].boeufs;
    }
    return score;
}

/**
 * on affiche l'ensemble des carte des joueurs en prenant en compte le nb de carte qu'il ont jouer
 */
void Afficher_carte()
{
    for (int i = 0; i < nb_joueurs; i++)
    {
        int cpt =0;
        for (int k = 0; k < Carte_par_Joueur; k++)
        {
            if (Lst_joueur[i].cartes[k].numero != 0) // donc si la valeur de la carte est diff de 0 on ajout au cpt le nb
                // que il doit afficher
            {
                cpt ++;
            }
        }
        printf("les carte du joueur %d\n ",Lst_joueur[i].id);

        for (int j = 0; j < cpt; j++)
        {
            if (Lst_joueur[i].cartes[j].etat == ETAT_MAIN) // es que c'est pertinant ?
                // ici on pourrai au lieu de faire un cpt juste checker l'état de la main a la prmière boubcle ?
                    // TODO refaire ici cette méthode en utilisant létat des carte et non un cpt
            {
                printf("cartes : %d \n",Lst_joueur[i].cartes[j].numero);
            }

        }
    }
}

/**
 *
 * @param cartes l'ensemble des cartes que l'on veut trier ici on trie en fonction des cartes jouer
 * @param n ici c'est le nb de joueur
 * Elle permet de ranger dans l'ordre les cartes
 */
void trier_carte(Carte cartes[], int n) {
    for (int i = 0; i < n - 1; i++) {
        int min = i;
        for (int j = i + 1; j < n; j++) {
            if (cartes[j].numero < cartes[min].numero) {
                min = j;
            }
        }
        if (min != i) {
            Carte tmp = cartes[i];
            cartes[i] = cartes[min];
            cartes[min] = tmp;
        }
    }
}



/* Utilise le plateau global : Carte plateau[4][6]; */
void Initialisation_Plateaux() {
    //printf("DEBUG: init plateau appelée\n");
    Initialisation_Plateaux_vide(); // remet nb_cartes_par_rangee[] à 0 et valeurs à 0
    for (int i = 0; i < 4; i++) {
        int indice = 0;
        indice = tirer_carte_alea(ETAT_PLATEAU);

        if (indice < 0 || indice >= Nb_Carte_Totale) {
            fprintf(stderr, "Erreur : indice carte invalide %d\n", indice);
            continue;
        }
        // placer la carte sur la rangée i, position 0
        plateau[i][0] = carte[indice];
        carte[indice].etat = ETAT_PLATEAU;   // marque la carte comme sur le plateau
        nb_cartes_par_rangee[i] = 1;
    }
    Afficher_Plateaux();

}

// affichage simple pour le debug
void afficher_etat_de_la_carte(Carte c) {
    printf("Carte %3d : etat = %d\n", c.numero, c.etat);
}

// pareil
void afficher_etat_cartes() {
    for (int i = 0; i < Nb_Carte_Totale; i++) {
        printf("[i=%3d] Carte valeur=%3d : etat = %d\n",
               i,                 // indice dans le tableau
               carte[i].numero,   // numéro logique (1-104)
               carte[i].etat);
    }
}


/**
 *
 * @param main_joueur la main du joueur en param on a une boucle dans la mise a jour avant pour faire l'esemble des joueurs
 * @param nb_cartes le nb de carte du joueur
 * @return renvoie la nouvelle taille de la main du joueur
 */
int mise_a_jour_carte_joueur(Carte main_joueur[], int nb_cartes) {
    int nouvelle_taille = 0;

    for (int i = 0; i < nb_cartes; i++) {
        if (main_joueur[i].etat == ETAT_MAIN) {
            // carte encore dans la main
            main_joueur[nouvelle_taille++] = main_joueur[i];
        }
    }

    // vider le reste proprement
    for (int i = nouvelle_taille; i < Carte_par_Joueur; i++) {
        main_joueur[i].numero = -1;
        main_joueur[i].boeufs = 0;
        main_joueur[i].etat = ETAT_JOUEE;
    }

    return nouvelle_taille;
}


/**
 *
 * @param j Le joueur en param que on veut envoyer les cartes
 * @param msg et donc le msg que on construit avec l'ensemble des carte du joueur (qu'il possède)
 */
void convertir_carte_en_char(Joueur *j, char *msg) {
    msg[0] = '\0';
    strcat(msg, "MAIN\n");

    char temp[32];
    for (int i = 0; i < j->nb_cartes; i++) {
        sprintf(temp, "%d(%d) ", j->cartes[i].numero, j->cartes[i].boeufs);
        strcat(msg, temp);
    }
    strcat(msg, "\n");
}

/**
 *
 * @param msg on construit un msg avec l'ensemble des cartes sur le plateau
 */
void convertir_plateau_en_char(char *msg) {
    msg[0] = '\0';

    strcat(msg, "PLATEAU\n");  // PAS d'espace, PAS de saut de ligne devant

    char temp[32];
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 6; j++) {
            sprintf(temp, "%3d ", plateau[i][j].numero);
            strcat(msg, temp);
        }
        strcat(msg, "\n");
    }
}

/**
 *
 * @param LstJoueurs La liste de nos joueurs
 * @param msg le message une fois conertie (donc le score)
 */
void convertir_score_en_char(Joueur LstJoueurs [], char *msg)
{
    msg[0] = '\0';
    strcat(msg, "SCORE\n");
    char temp[32];
    for (int i =0; i < nb_joueurs; i++)
    {
        sprintf(temp, "%d", LstJoueurs[i].score); //  le score est bon
        strcat(msg, temp);
        strcat(msg, "\n");
    }
}
// robot méthodes
/**
 *
 * @param j
 * @return
 */
int robot_choisir_carte(Joueur *j)
{
    int meilleur_indice = -1;
    int meilleure_proximite = 999;

    // pour chaque carte en main
    printf("[DEBUG] ROBOT choisi ça carte : ");
    for (int i = 0; i < j->nb_cartes; i++)
    {
        if (j->cartes[i].etat != ETAT_MAIN)
            continue;

        int val = j->cartes[i].numero;
        int meilleure_ligne = -1;
        int proximite = 999;

        // essaie de placer la carte sur les 4 rangées
        for (int l = 0; l < 4; l++)
        {
            int nb = nb_cartes_par_rangee[l];
            if (nb == 0) continue;

            int last = plateau[l][nb-1].numero;

            if (val > last)
            {
                int diff = val - last;
                if (diff < proximite)
                {
                    proximite = diff;
                    meilleure_ligne = l;
                }
            }
        }

        // on garde la carte qui a la meilleure proximité
        if (meilleure_ligne != -1 && proximite < meilleure_proximite)
        {
            meilleure_proximite = proximite;
            meilleur_indice = i;
        }
    }

    // cas où aucune carte ne peut être posée
    if (meilleur_indice == -1)
    {
        // joue la plus petite carte
        int min_val = 999;
        for (int i = 0; i < j->nb_cartes; i++)
        {
            if (j->cartes[i].etat == ETAT_MAIN && j->cartes[i].numero < min_val)
            {
                min_val = j->cartes[i].numero;
                meilleur_indice = i;
            }
        }
    }
    printf("%d\n",j->cartes[meilleur_indice].numero);
    return meilleur_indice;
}

int robot_choisir_ligne()
{
    int meilleure_ligne = 0;
    int min_boeufs = 999;

    for (int l = 0; l < 4; l++)
    {
        int total = 0;
        for (int i = 0; i < nb_cartes_par_rangee[l]; i++)
            total += plateau[l][i].boeufs;

        if (total < min_boeufs)
        {
            min_boeufs = total;
            meilleure_ligne = l;
        }
    }

    return meilleure_ligne;
}

int choisir_carte(Joueur *j)
{
    if (j->est_robot)
    {
        return robot_choisir_carte(j);   // IA
    }
        return demander_carte_res(j->sock, j); // sinon humain

}
int choisir_ligne(Joueur *j)
{
    if (j->est_robot)
    {
        return robot_choisir_ligne();
    }
        return demander_ligne_res(j->sock, j);
}



/**
 *
 * @param joueur le joueur actuel dans notre phase de gameplay
 * @param c la carte en question qui est jouer
 * @param Lst_AutreJoueurs  l'ensemble de nos joueurs
 * @param nbjoueur de même le nb de nos joueurs
 */
void placer_carte_V2(Joueur *joueur, Carte c , Joueur Lst_AutreJoueurs[] , int nbjoueur) // on part du principe que ya 2 joueurs pour l'instant
{
    int ligne_cible = -1;
    int meilleure_valeur = -1;
    // ligne cible recherche
    for (int l = 0; l < 4; l++) {
        int nb = nb_cartes_par_rangee[l];

        if (nb > 0) {
            int last = plateau[l][nb - 1].numero;

            if (last < c.numero && last > meilleure_valeur) {
                meilleure_valeur = last;
                ligne_cible = l;
            }
        }
    }

    // cas de la selection de la ligne si la carte a un niveau inf au autres cartes
    if (ligne_cible == -1) {

        // Informer tous les autres joueurs
        envoyer_a_tous_sauf(Lst_AutreJoueurs, nbjoueur, joueur->id, "INFO Un joueur doit choisir une ligne\n");
        envoyer_a_tous_sauf(Lst_AutreJoueurs, nbjoueur, joueur->id, "PLATEAU");


        // Attente de sa réponse EXCLUSIVEMENT pour lui
        if (!joueur->est_robot) {
            envoyer_message(joueur->sock, "DEMANDER_LIGNE", 0);
        }

        int ligne = choisir_ligne(joueur);


        // Calcul score
        int total_boeufs = 0;
        for (int k = 0; k < nb_cartes_par_rangee[ligne]; k++) {
            total_boeufs += plateau[ligne][k].boeufs;
        }
        joueur->score += total_boeufs;

        // Mise à jour plateau
        // Vider toute la ligne
        for (int k = 0; k < 6; k++) {
            plateau[ligne][k] = (Carte){0, 0, ETAT_JOUEE, -1};
        }
        plateau[ligne][0] = c;
        nb_cartes_par_rangee[ligne] = 1;
        printf("la ligne es %d",ligne );

        // Informer tout le monde du résultat
        char info[TAILLE_BUF];
        sprintf(info, "INFO Joueur %d a ramassé la ligne %d\n", joueur->id, ligne);
        envoyer_messages_a_tous(Lst_AutreJoueurs, nbjoueur, info);
        //envoyer_messages_a_tous(Lst_AutreJoueurs, nbjoueur, "PLATEAU");


        return;
    }
    if (nb_cartes_par_rangee[ligne_cible] == 5) {

        // Informer les autres
        char info[TAILLE_BUF];
        sprintf(info, "INFO Joueur %d ramasse la ligne %d (ligne complète)\n", joueur->id, ligne_cible+1);
        envoyer_messages_a_tous(Lst_AutreJoueurs,nbjoueur,info);

        // Calcul score
        int total = 0;
        for (int i = 0; i < nb_cartes_par_rangee[ligne_cible]; i++)
        {
            total += plateau[ligne_cible][i].boeufs;
        }
        joueur->score += total;

        // Réinitialisation
        for (int k = 0; k < 6; k++) {
            plateau[ligne_cible][k] = (Carte){0, 0, ETAT_JOUEE, -1};
        }
        plateau[ligne_cible][0] = c;
        nb_cartes_par_rangee[ligne_cible] = 1;

        envoyer_messages_a_tous(Lst_AutreJoueurs,nbjoueur,"PLATEAU");
        return;
    }


    int pos = nb_cartes_par_rangee[ligne_cible];
    plateau[ligne_cible][pos] = c;
    nb_cartes_par_rangee[ligne_cible]++;

    char info[128];
    sprintf(info, "INFO Joueur %d place %d sur la ligne %d\n", joueur->id, c.numero, ligne_cible);
    envoyer_messages_a_tous(Lst_AutreJoueurs,nbjoueur,info);

    //envoyer_messages_a_tous(Lst_AutreJoueurs,nbjoueur,"PLATEAU");
}

// le calcule du temps en miliseconde
long ms(void) {
    struct timespec tv;
    clock_gettime(CLOCK_MONOTONIC, &tv);
    return tv.tv_sec * 1000 + tv.tv_nsec / 1000000;
}
void Manche_Jeux() {

    // stats du temps
    char Buff_message[TAILLE_BUF];
    char Buff_plateau[TAILLE_BUF];
    char Buff_cartes[TAILLE_BUF];
    Carte cartes_jouees[MAX_Joueur];
    char Buff_tours[TAILLE_BUF];
    char Buff_score[TAILLE_BUF];
    FILE *logf = fopen("game.log", "a");    if (!logf) {
        perror("fopen game.log");
        exit(1);
    }
    printf("[DEBUG] game.log ouvert avec succès\n");
    fprintf(logf, "DEBUG_TEST\n");
    fflush(logf);

    fprintf(logf, "PARTIE_START id=%d\n", 1);
    sprintf(Buff_message,"INFO Debut d'une nouvelle partie ! \n");
    envoyer_messages_a_tous(Lst_joueur,nb_joueurs,Buff_message);
    fflush(logf);

    for (int i = 0; i < nb_joueurs; i++) {
        fprintf(logf, "JOUEUR id=%d type=%s\n",Lst_joueur[i].id,Lst_joueur[i].est_robot ? "robot" : "humain");
    }
    fflush(logf);

    printf("\n Début de la manche \n");
    for (int tour = 0; tour < 9; tour++) {
        //printf("on vide les deux buffers de carte et de plateau");
        fprintf(logf, "TOUR num=%d\n", tour + 1);
        fflush(logf);

        memset(Buff_plateau, 0, sizeof(Buff_plateau));
        memset(Buff_cartes, 0, sizeof(Buff_cartes));
        memset(Buff_tours, 0, sizeof(Buff_tours));
        memset(Buff_message, 0, sizeof(Buff_message));

        printf("\nTOUR %d \n", tour + 1);
        convertir_plateau_en_char(Buff_plateau);
        envoyer_messages_a_tous(Lst_joueur,nb_joueurs,Buff_plateau);
        int choix_joueur[MAX_Joueur];



        // boucle principale
        for (int i = 0; i < nb_joueurs; i++) {
            convertir_carte_en_char(&Lst_joueur[i], Buff_cartes);

            //verification des types de joueurs
            if (Lst_joueur[i].sock >= 0) {
                envoyer_message(Lst_joueur[i].sock, Buff_cartes, 0);
                printf("envoie du msg demander carte au sock  %d\n",Lst_joueur[i].sock);
                envoyer_message(Lst_joueur[i].sock, "DEMANDER_CARTE", 0);
            }
            fprintf(logf,"DEMANDER_CARTE joueur=%d time=%ld\n",Lst_joueur[i].id, ms());// activation du temps
            fflush(logf);
            choix_joueur[i] = choisir_carte(&Lst_joueur[i]);
            fprintf(logf,"REPONSE_CARTE joueur=%d time=%ld\n",Lst_joueur[i].id, ms()); // pour le calcul du delta
            fflush(logf);


            if (choix_joueur[i] < 0 || choix_joueur[i] >= Lst_joueur[i].nb_cartes) {
                printf("Indice hors borne (joueur %d), on prend 0\n", Lst_joueur[i].id);
                choix_joueur[i] = 0;
            }
            // Enregistre la carte jouée
            cartes_jouees[i] = Lst_joueur[i].cartes[choix_joueur[i]];
            cartes_jouees[i].joueur_id = Lst_joueur[i].id;
            Lst_joueur[i].cartes[choix_joueur[i]].etat = ETAT_JOUEE;

            fprintf(logf,"JOUE joueur=%d carte=%d boeufs=%d\n",Lst_joueur[i].id,cartes_jouees[i].numero,cartes_jouees[i].boeufs);
            fflush(logf);

            printf("Joueur %d joue la carte %d (%d boeufs)\n",Lst_joueur[i].id,cartes_jouees[i].numero,cartes_jouees[i].boeufs);

        }

        // Tri des cartes jouées
        trier_carte(cartes_jouees, nb_joueurs);

        // Placement dans l'ordre
        for (int i = 0; i < nb_joueurs; i++) {
            for (int j = 0; j < nb_joueurs; j++) {
                if (Lst_joueur[j].id == cartes_jouees[i].joueur_id) {
                    placer_carte_V2(&Lst_joueur[j], cartes_jouees[i],Lst_joueur,nb_joueurs  );
                    break;
                }
            }
        }

        //Mise à jour du nombre de cartes
        for (int i = 0; i < nb_joueurs; i++) {
            Lst_joueur[i].nb_cartes = mise_a_jour_carte_joueur(Lst_joueur[i].cartes, Lst_joueur[i].nb_cartes);
        }
        // on envoie le score a chaque fin de manche
        convertir_score_en_char(Lst_joueur, Buff_score);
        envoyer_messages_a_tous(Lst_joueur,nb_joueurs,Buff_score);
        for (int i = 0; i < nb_joueurs; i++) {
            fprintf(logf,"SCORE joueur=%d total=%d\n",Lst_joueur[i].id,Lst_joueur[i].score);
        }
        fflush(logf);

    }

}



void Partie()
{
    FILE *logf = fopen("game.log", "a");
    if (!logf) {
        perror("fopen game.log");
        exit(1);
    }
    int fin = 0;
    int manche = 1;
    while (!fin && manche <= NB_manche)
    {
        Manche_Jeux();
        for (int i = 0; i < nb_joueurs; i++) {
            if (Lst_joueur[i].score >= 66 ) {
                fin ++;// verif du score des joueurs et donc la fin du jeu
            }
        }
        printf("Manche Global=%d\n",manche);
        Initialisation_Plateaux();
        Initialisation_Carte();
        distribuer_cartes();
        manche++;
    }

    // fin de partie
    printf("\n FIN DE LA PARTIE \n");
    envoyer_messages_a_tous(Lst_joueur,nb_joueurs,"FIN");// on notifie la fin de la game a tout les joueurs

    int meilleur = 999;
    int gagnant = -1;
    for (int i = 0; i < nb_joueurs; i++) {
        if (Lst_joueur[i].score < meilleur) {
            meilleur = Lst_joueur[i].score;
            gagnant = Lst_joueur[i].id;
        }
    }

    // en fin de game
    fprintf(logf, "PARTIE_END gagnant=%d avec le score de %d\n", gagnant,meilleur);
    fflush(logf);
    fclose(logf);

    printf("Le gagnant est le Joueur %d avec %d têtes de boeufs !\n", gagnant, meilleur);
}

