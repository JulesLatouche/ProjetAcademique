/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/Classes/Class.java to edit this template
 */
package exploration;

/**
 *
 * @author jo
 */
public class ReserveEnergie extends Objet
{
    private final static int MAX = 9;
    private int nbEnergie;

    public static int getMAX() {
        return MAX;
    }

    public int getNbEnergie() {
        return nbEnergie;
    }

    public void setNbEnergie(int nbEnergie) {
        this.nbEnergie = nbEnergie;
    }
    
    @Override
    public void interaction(Joueur j)
    {
    if (nbEnergie == 0) {
        throw new RuntimeException("La caisse est vide.");
    }
    
    if (nbEnergie != 0) {
        int n = 0;
        boolean valide;
        do {
            System.out.println("Combien d'energie voulez-vous prendre ?");
            n = Lire.i(); // Utilisation de la méthode Lire.i() pour lire un entier
            valide = (n >= 0 && n <= nbEnergie);
            if (!valide) {
                System.out.println("Veuillez saisir un nombre valide d'energie.");
            }
        } while (!valide);
        
        j.ajouterEnergie(n);
        setNbEnergie(nbEnergie - n);
    }
        /* S'il reste des unités d'énergie dans cette réserve,
           propose au joueur d'en prendre un certain nombre,
           ce qui augmente l'énergie du joueur et diminue d'autant cette réserve
        */
        
    }

    public ReserveEnergie(int disponible)
    {
        super("⚛","Réserve d'énergie");
        this.nbEnergie=nbEnergie;
    }
    
    public ReserveEnergie()
    {
        super("⚛","Réserve d'énergie");
        int j = (int) (Math.random() * MAX) + 1;
        this.nbEnergie = j;        
    }    
}
