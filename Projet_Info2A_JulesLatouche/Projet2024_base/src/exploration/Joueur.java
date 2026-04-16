/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/Classes/Class.java to edit this template
 */
package exploration;

/**
 * À compléter
 * @author jo
 */
public class Joueur
{    
    private String nom;
    private int nbGrenades;
    private int nbEnergie;
    private String symbole="♛♛";
    
    public int getNbEnergie() {
        return nbEnergie;
    }

    public void setNbEnergie(int nbEnergie) {
        this.nbEnergie = nbEnergie;
    }
    

    public void setNbGrenades(int nbGrenades) {
        this.nbGrenades = nbGrenades;
    }

    public int getNbGrenades() {
        return nbGrenades;
    }
    
    public String getNom(){return this.nom;}
    private void setNom(String nom){this.nom = nom;}
    private void setNom()
    {
        setNom(Lire.S("Quel est votre nom"));
    }
    
    private Position position;
    /**
     * Position du joueur
     * @return une référence à une position
     */
    public Position getPosition(){return this.position;}    
    /**
     * Salle où se situe le joueur
     * @return une référence à une salle
     */
    public Salle getSalle()
    {
        return this.position.getPlateau().getSalle(this.position);
    }
    private void setPosition(Position position)
    {
        // affectation de la position en déclenchant la méthode d'entrée de la salle
        this.position = position;
        this.position.getPlateau().getSalle(this.position).entree(this);        
    }
        
    /**
     * Avance dans une direction donnée à condition que le mur soit ouvert dans cette direction
     * @param d direction dans laquelle avancer
     */
    public void avance(Direction d)
    {
        /*
            code : il faut gérer la sortie du plateau, le fait qu'un mur soit ou non ouvert et remettre à jour la position du joueur        
        */
        if(this.position.isValide() && d.isValide()){
            if(this.position.getPlateau().getSalle(position).isVisible()){
                this.position.getSuivante(d);
            }
            else{
                System.out.println("Le mur est ouvert.");
                this.avance(d);
            }                          
        }else{
            System.out.println("Position invalide !");
        }
    }
    
    private LesOutils outils;
    public LesOutils getOutils()
    {
        return this.outils;
    }
    private void setOutils(LesOutils outils)
    {
        this.outils = outils;
    }

    /**
     * Récupération d'un outil trouvé dans une salle
     */
    public void recupere()
    {
        //Cette méthode est déclenchée par l'interaction avec l'outil
        //récup outil au sol?
        
        System.out.println("Vous avez trouvé un outil ! Voulez-vous le prendre?");
        String choix=Lire.S("Tapez o pour prendre l'outil ou n pour le laisser.");
        switch(choix){
            case"o":
                outils.ajoute((Outil)this.getSalle().getContenu());
                System.out.println("Vous prenez l'outil.");break;               
            case"n":
                System.out.println("Vous laissez l'outil par terre.");break;
        }       
    }    
    
    private boolean perdant;
    public boolean isPerdant(){
        return this.perdant;
    }
    public void setPerdant(boolean perdant){
        this.perdant = perdant;
    }
    
    public int ajouterGrenade(int n){
        return this.nbGrenades+n;           
    }
    public int ajouterEnergie(int n){
        return this.nbEnergie+n;           
    }
    public void reserveGrenade(){
        this.nbGrenades=10;
    }
    public void reserveEnergie(){
        this.nbEnergie=20;
    }
    public void lanceGrenade(Direction d)
    {
        /*
            La grenade est perdue si un mur est déjà ouvert dans la direction spécifiée
            Sinon,un accès est ajouté à la salle courante vers la salle contiguë dans la direction d (et réciproquement)
            et le joueur est « aspiré » dans la salle nouvellement ouverte. La réserve de grenades du joueur est décrémentée
        */
        if(this.position.isValide() && d.isValide()){
            Position posBase= this.position;
            this.position.getSuivante(d);
            if(this.position.getPlateau().getSalle(position).isVisible()){
                this.setPosition(posBase);
            }
            else if(this.position.getPlateau().getSalle(this.position).getContenu() instanceof Mine){
                 this.setPosition(position);
            }
        }
        this.nbGrenades= nbGrenades--;
    }
 
    public Joueur(String nom,Position position)
    {
        this.nom=nom;
        this.position=position;
    }
    public Joueur(Position position)
    {
        this.setNom();        
        this.position=position;
    }
   
    public String getSymbole(){
        return symbole;
    }
}
