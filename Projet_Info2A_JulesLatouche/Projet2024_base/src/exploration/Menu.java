/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/Classes/Class.java to edit this template
 */
package exploration;

/**
 *
 * @author latou
 */
public class Menu {
     String choix;
    int nbLign;
    int nbCol;
    Categorie[] listeCategories;
    int proportionVides;
    Jeu jeu ;

    

    public Menu() {
        System.out.println("[1]parametre par defaut\n[2]Modifier Paramètre");
        choix=Lire.S();
        switch (choix) {
            case "1":
                System.out.println("...");
                Categorie
                    grenades = new Categorie(4, CaisseGrenades.class),
                    mines = new Categorie(3, Mine.class),
                    energie = new Categorie(8, ReserveEnergie.class),
                    scannerDirectionnel = new Categorie(10, ScannerUnidirectionnel.class),
                    detecteurMine = new Categorie(2, DetecteurMines.class);
                nbLign=5;
                nbCol=11;
                listeCategories=new Categorie[]{grenades,mines,energie,scannerDirectionnel,detecteurMine};
                

                break;
            case "2":
                System.out.println("Entrée taille de la carte:\nLongeur:");
                nbLign=Lire.i();
                System.out.println("Largeur:");                //verification a mettre+autre parametre
                nbCol=Lire.i();
                break;  
        
            default:
                break;
        }
    }



    public String getChoix() {
        return choix;
    }



    public void setChoix(String choix) {
        this.choix = choix;
    }



    public int getNbLign() {
        return nbLign;
    }



    public void setNbLign(int nbLign) {
        this.nbLign = nbLign;
    }



    public int getNbCol() {
        return nbCol;
    }



    public void setNbCol(int nbCol) {
        this.nbCol = nbCol;
    }



    public Categorie[] getListeCategories() {
        return listeCategories;
    }



    public void setListeCategories(Categorie[] listeCategories) {
        this.listeCategories = listeCategories;
    }



    public int getProportionVides() {
        return proportionVides;
    }



    public void setProportionVides(int proportionVides) {
        this.proportionVides = proportionVides;
    }



    public Jeu getJeu() {
        return jeu;
    }



    public void setJeu(Jeu autJeu) {
        this.jeu = autJeu;
    }
}
