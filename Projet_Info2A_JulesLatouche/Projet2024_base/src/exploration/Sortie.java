package exploration;

/**
 * Sortie
 */
public class Sortie extends Objet{

  
    public Sortie(){
        super("<<<","Sortie"); 

    }




    /**
     * interraction avec le joueur est la fin du jeux 
     */
    @Override
    public void interaction(Joueur j) {
        
            if (j.getSalle().getContenu() == this) { // fin du jeu
                 j.setPerdant(false);
            }
    }
    
    
}