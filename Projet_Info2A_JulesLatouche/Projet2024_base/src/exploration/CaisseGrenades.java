/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/Classes/Class.java to edit this template
 */
package exploration;

/**
 * À compléter
 * @author jo
 */
public class CaisseGrenades extends Objet
{
    private final static int MAX = 9;// nombre maximal dans les caisses de grenades trouvées dans les salles
    private int nbGrenades;


    
    
    @Override
    public void interaction(Joueur j)
    {
    if (nbGrenades == 0) {
        throw new RuntimeException("La caisse est vide.");
    }
    
    if (nbGrenades != 0) {
        int n = 0;
        boolean valide;
        do {
            System.out.println("Combien de grenade voulez-vous prendre ?");
            n = Lire.i(); // Utilisation de la méthode Lire.i() pour lire un entier
            valide = (n >= 0 && n <= nbGrenades);
            if (!valide) {
                System.out.println("Veuillez saisir un nombre valide de grenades.");
            }
        } while (!valide);
        
        j.ajouterGrenade(n);
        setNbGrenades(nbGrenades - n);
    }
}
        /* S'il reste des grenades dans cette réserve,
           propose au joueur d'en prendre un certain nombre,
           ce qui augmente les grenades du joueur et diminue d'autant cette réserve
        */
    

    public CaisseGrenades(int nbGrenades)
    {
        super("☌","Caisse de grenades");
        this.nbGrenades=nbGrenades;
    }
    
    public CaisseGrenades(){
        super("☌","Caisse de grenades");
        int j =(int)(Math.random()*MAX)+1;
        this.nbGrenades=j;
    }
    public void setNbGrenades(int nbGrenades) {
        this.nbGrenades = nbGrenades;
    }

    public static int getMAX() {
        return MAX;
    }

    public int getNbGrenades() {
        return nbGrenades;
    }
}

