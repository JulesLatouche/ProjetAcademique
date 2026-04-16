/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/Classes/Class.java to edit this template
 */
package exploration;
import java.util.ArrayList;

/**
 *
 * @author jo
 */
public class Plateau
{
    private static final String BORD = "░";
    public static boolean VISIBLE = true; // mode déboggage : si true, toutes les salles doivent afficher leur contenu
    private Jeu jeu;
    public Jeu getJeu(){return this.jeu;}
    private void setJeu(Jeu jeu){this.jeu = jeu;}

    private boolean visible;
    public boolean isVisible(){return this.visible||Plateau.VISIBLE;}
    public void setVisible(boolean visible){this.visible = visible;}

    private Salle[][] grille;
    public int getNbLig(){return this.grille.length;}
    public int getNbCol(){return this.grille[0].length;}
    public int getNbSalles(){return this.getNbCol()*this.getNbLig();}
    public int NbSalles (){
        return this.getNbLig()*this.getNbCol();
    }
     public int NbSallesVides(){
        return (this.NbSalles()*this.jeu.getProportionVides())/100;
    }
    public int NbSallespleines(){
        return this.NbSalles()-this.NbSallesVides();
    }
    public Salle getSalle(Position p)
    {
        if(p.isValide()) return this.grille[p.getLig()-1][p.getCol()-1];
        else return null;
    }
    public Salle getSalle(int lig, int col){return this.getSalle(new Position(lig,col,this));}

    /**
     * Place une salle à une position donnée (Les positions en Lig et Col commencent à 1)
     * @param s Une salle
     */
    public void placeSalle(Salle s)
    {
        Position p = s.getPosition();
        if(!p.isValide()) throw new RuntimeException("Affectation de la salle à une position non valide");
        this.grille[p.getLig()-1][p.getCol()-1] = s;       
    }

    /**
     * Crée une salle contenant un objet référencé par o (vide si o = null) et l'affecte au plateau
     * @param p la position dans le plateau
     * @param o l'objet que la salle doit contenir
     */
    public void setNouvelleSalle(Position p, Objet o)
    {
        placeSalle(new Salle(p,this,o));       
    }
    public void setNouvelleSalle(Position p){setNouvelleSalle(p, null);}
    
    private Joueur joueur;
    public Joueur getJoueur(){return this.joueur;}
    private void setJoueur(Joueur joueur){this.joueur = joueur;}
    public Position getPosJoueur(){return this.getJoueur().getPosition();}

    @Override
    public String toString()
    {
       // restitue la chaîne qui représente l'ensemble du plateau en combinant les toString des salles (voir exemple dans le sujet)
        String res="";
        for(int i=0;i<this.getNbCol()+2;i++){
            res+=BORD;
        }
        for(int i=0;i<this.getNbLig();i++){
            res+="\n"+BORD;
            for(int j=0;j<getNbCol();j++){
                if(grille[i][j].getPosition().equals(this.getJoueur().getPosition())){
                    res+=this.getJoueur().getSymbole();
                    System.out.println("test");
                }
                else{
                    res+=res+grille[i][j].toString();
                }
            }
            res=res+BORD;
        }
        res+="\n";
        for(int i=0;i<this.getNbCol()+2;i++){
            res+=BORD;
        }
        return res;
    }
public Plateau(int nbLig, int nbCol, Jeu jeu)
    {
        // Crée un joueur et initialise le plateau selon les spécifications du jeu (Gros boulot)
        grille=new Salle[nbLig][nbCol];
        this.setJeu(jeu);
        for(int i=0;i<this.getNbSalles();i++){
            this.setNouvelleSalle(new Position(i, this));
        }
        this.initPlateau();System.out.println("d");
        this.setJoueur(new Joueur(new Position((NbSalles()-1)/2, this)));
        System.out.println(joueur.getPosition().getRang()); 
        System.out.println(NbSalles());
        System.out.println(getNbCol());
        System.out.println(getNbLig());
    }
public void initPlateau (){
        int i = (int)(Math.random()*4+1);
        if (i==1) {
            this.setNouvelleSalle(new Position(0,this),new Sortie()); // rang  ==  nblig et nbcol
        }
        if (i==2) {
            this.setNouvelleSalle(new Position(1,this.getNbCol(),this),new Sortie());
        }
        if(i==3){
            this.setNouvelleSalle(new Position(this.getNbLig(),1,this),new Sortie());
        }
        if(i==4){
            this.setNouvelleSalle(new Position(this.getNbLig(),this.getNbCol(),this),new Sortie());
        }

        this.placeObjet(this.getJeu().getListeCategories());

    }
private void placeObjet(Categorie... categories){
        //création d'un tableau d'objet du nombre de salle pleine pour l'<instant vide (null partout)
        Objet[] objets = new Objet[this.NbSallespleines()];
        int nbTotal = 0;
        for(int i = 0 ; i<categories.length ; i++) // Parcours des catégories
        {
            nbTotal = nbTotal+categories[i].getProportion();
        }
        // nbTotal contient désormais la somme des proportions des différents objets à créer
        // crée autant d'objets qu'il y a de cases dans le tableau d'objets dans les proportions voulues /
        int indiceObjet = 0;
        for(int indiceCategorie = 0 ; indiceCategorie<categories.length ; indiceCategorie++)
        {
            int nbObjets = objets.length*categories[indiceCategorie].getProportion()/nbTotal; // calcule le nombre d'objets nbObjets de la catégorie courante à créer
            System.out.println(nbObjets+" de la catégorie "+categories[indiceCategorie].getClasse().getSimpleName());

            for(int compteurObjets = 1 ; compteurObjets<= nbObjets ; compteurObjets++,indiceObjet++) // crée nbObjets instances de la catégorie courante
            {
                Objet o = categories[indiceCategorie].getNouveau();
                objets[indiceObjet] = o; // crée une instance de la catégorie courante avec le constructeur par défaut
                int randomRang;
                do{
                    randomRang=(int)(Math.random()*getNbSalles());
                }
                while (!getSalle(new Position(randomRang, this)).isVide()&&(NbSalles()-1)/2==randomRang);
                this.setNouvelleSalle(new Position(randomRang, this), o);
            }
        }

    }
    
}
