BEGIN {
    print "\\documentclass{article}"
    print "\\usepackage[utf8]{inputenc}"
    print "\\title{Statistiques de la partie}"
    print "\\date{}"
    print "\\begin{document}"
    print "\\maketitle"
    print "\\section*{Résumé}"
}

/^DEMANDER_CARTE/ { # motif d'exe
    for (i=1; i<=NF; i++) { #donc ici on parcours l'esemble de notre ligne NF nombre de champs
        # $0 => DEMANDER_CARTE joueur=1 time=19384918
        # $1 =>  demander_carte
        # $2 => joueur=1
        # $3 => time=19384918
        if ($i ~ /^joueur=/) {
            split($i,a,"="); j=a[2] # ici on decoupe la chaine dans un tableau en fonction des =
            #donc en dans a[1] = joueur et dans a[2] on a 1 l'id du joueur
        }
        else if ($i ~ /^time=/) {
            split($i,b,"="); t=b[2]
        } # meme logique pour le temps donc j a l'id du joueur et t a le temps
    }
    demande_time[j] = t # tableau associatif
}
/^REPONSE_CARTE/ {
    for (i=1; i<=NF; i++) {
        if ($i ~ /^joueur=/) {
            split($i,a,"="); j=a[2]
        }
        else if ($i ~ /^time=/) {
            split($i,b,"="); t=b[2]
        }
    }

    if (j in demande_time) {
        delta = t - demande_time[j]
        total_temps[j] += delta
        nb_reponses[j]++
        delete demande_time[j]
    }
}

/^JOUE/ {
    joueur = carte = boeufs = "" # eviter les err de stats
    #JOUE joueur=2 carte=83 boeufs=1
    for (i = 1; i <= NF; i++) {
        if ($i ~ /^joueur=/) {
            split($i, a, "=")
            joueur = a[2]
        }
        else if ($i ~ /^carte=/) {
            split($i, b, "=")
            carte = b[2]
        }
    }
    cartes_jouees[joueur]++ # en fonction des id des joueurs on compte les cartes
    somme_cartes[joueur] += carte # pour la moyenne

    # on ajoute la carte a notre ensemble de carte jouer
    if (cartes_liste[joueur] == "")
        cartes_liste[joueur] = carte
    else
        cartes_liste[joueur] = cartes_liste[joueur] ", " carte
}


/^SCORE/ {
    #SCORE joueur=1 total=0
    for (i = 1; i <= NF; i++) {
        if ($i ~ /^joueur=/) {
            split($i, a, "="); joueur = a[2]
        }
        if ($i ~ /^total=/) {
            split($i, b, "="); total[joueur] = b[2] # en fonction du joueur on stock son score dans le total
        }
    }
}

/^PARTIE_END/ {
    #PARTIE_END gagnant=2 avec le score de 16
    for (i = 1; i <= NF; i++) {
        if ($i ~ /^gagnant=/) {
            split($i, a, "="); gagnant = a[2] # on stock donc l'id du gagant
            victoires[gagnant]++ # on augmente la stat de vitoire du gagant donc si c'est 2(id du joueur) ce sera le joueur 2
        }
    }
    parties++ #
}

END {
    print "\\section*{Statistiques par joueur}"

    for (j in total ) { # on traite que les joeur qui on reçu un score
        if (cartes_jouees[j] > 0) # calule de la moy
            moyenne_cartes = somme_cartes[j] / cartes_jouees[j]
        else
            moyenne_cartes = 0
        # calcule du taux de victoire
        if (parties > 0)
            taux = (victoires[j] / parties) * 100
        else
            taux = 0
        # pour le calcule du temps
        moyenne = total_temps[j] / nb_reponses[j]

        print "\\subsection*{Joueur " j "}" # en fonction du joueur on fait une section
        print "\\begin{itemize}"
        print "\\item Points(têtes de boeufs) : " total[j]
        print "\\item Victoires : " (victoires[j] + 0) #affichage meme si le joueur n'a jamais gagner avec le +0
        print "\\item Nombre de cartes jouées : " (cartes_jouees[j] + 0)
        print "\\item Valeur moyenne des cartes : " moyenne_cartes
        print "\\item Cartes jouées : " (cartes_liste[j] != "" ? cartes_liste[j] : "Aucune")
        print "\\item Taux de victoire : " taux "\\%"
        print "\\end{itemize}"
        print "Joueur " j " : " moyenne " ms\\\\"
    }

    print "\\end{document}"
}
