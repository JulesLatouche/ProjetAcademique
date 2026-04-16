/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/Classes/Class.java to edit this template
 */
package exploration;

/**
 * Création d'un tableau de catégories et d'un tableau d'objets de ces catégories
 * @author jo
 */
public class DemoCategories
{
    public static void creeInstances(Categorie... categories)
    {
        //création d'un tableau de 100 objets pour l'instant vide (null partout)
        Objet[] objets = new Objet[100];
        int nbTotal = 0;
        for(int i = 0 ; i<categories.length ; i++) // Parcours des catégories
        {
            nbTotal = nbTotal+categories[i].getProportion();
        }
        // nbTotal contient désormais la somme des proportions des différents objets à créer
        /* crée autant d'objets qu'il y a de cases dans le tableau d'objets (ici 100) dans les proportions voulues */
        int indiceObjet = 0;
        for(int indiceCategorie = 0 ; indiceCategorie<categories.length ; indiceCategorie++)
        {
            int nbObjets = objets.length*categories[indiceCategorie].getProportion()/nbTotal; // calcule le nombre d'objets nbObjets de la catégorie courante à créer
            System.out.println(nbObjets+" de la catégorie "+categories[indiceCategorie].getClasse().getSimpleName());

            for(int compteurObjets = 1 ; compteurObjets<= nbObjets ; compteurObjets++,indiceObjet++) // crée nbObjets instances de la catégorie courante
            {   
                Objet o = categories[indiceCategorie].getNouveau();
                objets[indiceObjet] = o; // crée une instance de la catégorie courante avec le constructeur par défaut

            }
        }
        for(int i = 0 ; i < objets.length ; i++) System.out.print(objets[i]+", ");
        // à noter qu'avec la division entière, il peut manquer des objets à la fin (d'où le null)
    }
    
    public static void main (String[] args)
    {
        creeInstances
        (
                new Categorie(2,Mine.class),
                new Categorie(1,CaisseGrenades.class),
                new Categorie(3,DetecteurMines.class),
                new Categorie(1,ScannerUnidirectionnel.class),
                new Categorie(2,ReserveEnergie.class)
        );
        /* les chiffres sont des proportions à créer de chaque catégorie. 
            Cela signifie que, pour 10 objets créés (somme des proportions), il doit y avoir 3 mines, 1 caisse de grenades, 4 Détecteurs…
        */
    }
}
