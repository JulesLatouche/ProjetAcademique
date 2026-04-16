/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/Classes/Class.java to edit this template
 */
package exploration;


/**
 * À compléter
 * @author jo
 */

public class DetecteurMines extends Outil
{
    private Mine m;
    @Override
    public void interaction(Joueur j)
    {
        //code
        
    }
    public DetecteurMines()
    {
        super
        (
                "↺*",
                "Détecteur de mines",
                "<Descriptif de la fonctionnalité de l'outil>",
                3//coût énergétique de la détection des mines
        );
    }    

    @Override
    public void activation(Joueur j)
    {
        /* explore les salles autour de la salle du joueur
        (direction à droite par rapport à la position de la salle du joueur,
        puis positions successives qui sont fondées sur les directions successives dans le sens trigonométrique : voir direction et position),
        compte les mines et affiche le résultat
        */
    /*    int cptMines = 0;
        Plateau plateau = j.getSalle().getPlateau();
        Position positionJoueur = j.getPosition();

        // Parcourir toutes les directions
        Direction d = new Direction("haut");
        Position positionVoisine = positionJoueur.getSuivante(d);
        Salle salleVoisine = plateau.getSalle(positionVoisine);
            if (salleVoisine != null) {
                Objet contenuSalle = salleVoisine.getContenu();
                if (contenuSalle instanceof Mine) {
                    cptMines++;
                }
            }*/

        int cptMines = 0;
        Plateau plateau = j.getSalle().getPlateau();
        Position positionJoueur = j.getPosition();        
        Direction[] directions = {Direction.HAUT, Direction.BAS, Direction.GAUCHE, Direction.DROITE};// Parcours toutes les directions
        for (Direction d : directions) { //boucle for-each parcours chaque élément de la liste "directions"
            Position positionVoisine = positionJoueur.getSuivante(d);
            Salle salleVoisine = plateau.getSalle(positionVoisine);
            if (salleVoisine != null) {
                Objet contenuSalle = salleVoisine.getContenu();
                if (contenuSalle == m) {//verif si salle contient une mine
                    cptMines++;
                }
            }
        }
        // Afficher le résultat de la détection
        System.out.println("Nombre de mines détectées : " + cptMines);
    }
          
}
