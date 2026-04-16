#include "reseau.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "Structure.h"
#define BACKLOG 10 // file d'attente tcp


/**
 *
 * @param PORT Le port qu'ont choisi de connexion
 * @return renvoie le sock du joueur
 */
int creer_socket_serveur(uint16_t PORT) {
    int sockfd;
    struct sockaddr_in serv_addr; // slide 35 CM4
    int opt = 1;

    sockfd = socket(AF_INET/*pour IPV4 ou  IPV6*/, SOCK_STREAM/*TCP*/, 0);
    if (sockfd < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    /* réutilisation du port */
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
        perror("setsockopt");
        close(sockfd);
        exit(EXIT_FAILURE);
    }


    // on vide les buffers des adress ect pour le bind
    memset(&serv_addr, 0, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET; // en fonction de l'adress IP
    serv_addr.sin_addr.s_addr = INADDR_ANY; // ip 32 bit ?
    serv_addr.sin_port = htons(PORT); // num du port

    // association d'un port et  une adresse IP au sock
    if (bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("bind");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    //Cette socket va servir à accepter des connexions entrantes TCP
    // grace au backlog on a une file d'attend de sock pouvant ce connecter a voir maintenant cmb au max on veut
    // 10 val par défault meme si en soit dans ce code on en accpet que 2 au max
    if (listen(sockfd, BACKLOG) < 0) {
        perror("listen");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Serveur en attente de joueurs…\n");
    return sockfd;
}

/* Attend nb_joueurs connexions et les place dans clients[] (taille >= nb_joueurs).
 * Retourne 0 si OK, -1 en cas d'erreur.
 */
/**
 *
 * @param sockfd le socket qui est sur ecoute normalement que on vas associer a un client
 * @param nb_joueurs le nb de joueur en fonction du mode de jeux
 * @param clients l'ensemble de nos joueurs qu'il soit robot ou humain
 * @return renvoie rien
 */
int attendre_connexion_joueurs(int sockfd, int nb_joueurs, int clients[]) {
    if (sockfd < 0 || nb_joueurs <= 0 || clients == NULL) return -1;

    struct sockaddr_in cli_addr; // la même strcut que pour serv addr
    socklen_t clilen = sizeof(cli_addr); // taille de notre sock

    for (int i = 0; i < nb_joueurs; ++i) {
        int client_fd = accept(sockfd, (struct sockaddr*)&cli_addr, &clilen);
        /*on accepte  la connexion d'un client (bloquante)
         * En global elle remplie les champs nécessaire pour que le client dialogue
         * avec le sock (structure)
         */
        if (client_fd < 0) {
            perror("Erreur accept");
            return -1;
        }
        clients[i] = client_fd;
        printf("Joueur %d connecté depuis %s:%d (fd=%d)\n",
               i + 1,
               inet_ntoa(cli_addr.sin_addr),
               ntohs(cli_addr.sin_port),
               client_fd);


    }
    return 0;
}

/* Connexion client : retourne sockfd >=0 ou -1 en cas d'erreur */
/**
 *
 * @param ip_serveur adresse ip  sur la quelle on ce connect
 * @param port pareil pour le port
 * @return
 */
int connexion_serveur(const char *ip_serveur, uint16_t port) {
    if (ip_serveur == NULL) return -1;

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Erreur socket");
        return -1;
    }

    struct sockaddr_in serv_addr; // parreil pour les structure de que on utilise précedament
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);


    // on met l'addresse ip en format binaire
    // obliger de le faire avant connect dcp
    if (inet_pton(AF_INET, ip_serveur, &serv_addr.sin_addr) != 1) {
        perror("Adresse IP non valide");
        close(sockfd);
        return -1;
    }

    // connexion TCP en utilisant le port et l'ip  avec le serveur de la part du client
    if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Erreur connexion serveur");
        close(sockfd);
        return -1;
    }

    printf("Connecté au serveur %s:%u (fd=%d)\n", ip_serveur, port, sockfd);
    return sockfd;
}


/* Envoie un buffer de taille 'taille'. Si taille == 0, utilise strlen(message).
 * Retourne le nombre d'octets envoyés ou -1 en cas d'erreur. */
/**
 *
 * @param sockfd le socket du client
 * @param message le message sous forme de buffer
 * @param taille et sa taille
 * @return
 */
ssize_t envoyer_message(int sockfd, const char *message, size_t taille) {
    if (sockfd < 0 || message == NULL) return -1;
    if (taille == 0) {
        taille = strlen(message);
    }

    ssize_t total_sent = 0;
    const char *ptr = message;
    while (total_sent < (ssize_t)taille) { //pour l'envoie totale des octets de données car send ne pas tout envoyer defois
        // on utilise total_sent pour justement repartir de la ou on cetait arréter on fait l'inverse pour la taille restant a envoyer
        ssize_t n = send(sockfd, ptr + total_sent, taille - (size_t)total_sent, 0);
        if (n < 0) {
            if (errno == EINTR) continue;
            perror("Erreur d'envoi");
            return -1;
        }
        total_sent += n;
    }
    /* debug */
    //printf("DEBUG: message envoyé (%zd octets) sur fd=%d\n msg : %s\n", total_sent, sockfd,message);
    return total_sent; // renvoie le nb de octects si -1 err
}
/* Envoie msg à tous les joueurs du tableau sauf celui dont id == id_exclu.
 * Vérifie que socket est valide (>=0). */
/**
 *
 * @param liste donc notre liste de joueur
 * @param nb le nb de joueurs
 * @param id_exclu donc le joueur a qui on veut pas envoyer le msg
 * @param msg le message en question
 */
void envoyer_a_tous_sauf(const Joueur liste[], int nb, int id_exclu, const char *msg) {
    if (liste == NULL || msg == NULL) return;
    for (int i = 0; i < nb; ++i) {
        if (liste[i].id == id_exclu) continue;
        if (liste[i].sock >= 0) {
            envoyer_message(liste[i].sock, msg, 0);
        }
    }
}


/* Envoie msg à tous les joueurs fournis */
/**
 *
 * @param liste liste de nos joueurs
 * @param nb le nb de joueurs dans le jeux
 * @param msg le messages
 */
void envoyer_messages_a_tous(const Joueur liste[], int nb, const char *msg) {
    if (liste == NULL || msg == NULL) return;
    for (int i = 0; i < nb; ++i) {
        if (liste[i].sock >= 0) {
            envoyer_message(liste[i].sock, msg, 0);
        }
    }
}


/* Reçoit au maximum taille-1 octets dans buffer, met un '\0'.
 * Retourne nombre d'octets reçus (>=0), 0 si fermeture, -1 si erreur.
 */
/**
 *
 * @param sockfd Le socket connecte
 * @param buffer message en question
 * @param taille Et sa taille
 * @return le nb d'octets
 */
ssize_t recevoir_message(int sockfd, char *buffer, size_t taille) {
    if (sockfd < 0 || buffer == NULL || taille == 0) return -1;
    memset(buffer, 0, taille); // pour eviter les restes d'anciens messages

    ssize_t n = recv(sockfd, buffer, taille - 1/*pour la place du dernier carac le vide '\0' */, 0);
    if (n < 0) {
        if (errno == EINTR) return recevoir_message(sockfd, buffer, taille);
        perror("Erreur lors de la réception du message");
        return -1;
    }
    if (n == 0) {
        /* socket fermé par le peer */
        return 0;
    }
    buffer[n] = '\0';
    printf("Message reçu: %s\n", buffer);
    return n;
}


/* Demande au joueur de choisir une ligne : renvoie l'indice de ligne ou -1 */
/**
 *
 * @param sock socket du client
 * @param j Et le joueur associé à ce socket
 * @return renvoie la validation du message
 */
int demander_ligne_res(int sock, Joueur *j) {
    if (sock < 0 || j == NULL) return -1;
    char buff[TAILLE_BUF];
    ssize_t n = recevoir_message(sock, buff, sizeof(buff)); // le msg que le client a envoyer dans main_client
    if (n <= 0) {
        fprintf(stderr, "Erreur/reception (ligne) joueur %d (n=%zd)\n", j->id, n);
        return -1;
    }

    int valeur = -1; // ici amélioration possible dans le renvoie d'un autre possibility avec une boucle de renvoie de messages
    if (sscanf(buff, "LIGNE %d", &valeur) != 1) {
        fprintf(stderr, "Message LIGNE mal formé reçu: %s\n", buff);
        return -1;
    }
    return valeur;
}
/* Demande au joueur (socket) de choisir une carte : renvoie la valeur choisie ou -1 */
/**
 *Symétrie par rapport à demander ligne
 * @param sock
 * @param j
 * @return
 */
int demander_carte_res(int sock, Joueur *j) {
    if (sock < 0 || j == NULL) return -1;
    char buff[TAILLE_BUF];
    ssize_t n = recevoir_message(sock, buff, sizeof(buff));
    if (n <= 0) {
        fprintf(stderr, "Erreur/reception (carte) joueur %d (n=%zd)\n", j->id, n);
        return -1;
    }

    int valeur = -1;
    if (sscanf(buff, "CARTE %d", &valeur) != 1) {
        /* message mal formé */
        fprintf(stderr, "Message CARTE mal formé reçu: %s\n", buff);
        return -1;
    }
    return valeur;
}

