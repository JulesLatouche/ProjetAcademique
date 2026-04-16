/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/Classes/Class.java to edit this template
 */
package exploration;

/**
 *
 * @author jo
 */
public class Mine extends Objet
{

    public Mine(String symbole, String nature) {
        super(symbole, nature);
    }
    
    @Override
    public void interaction(Joueur j)
    {
        j.setPerdant(true);
        
    }

    public Mine(){
        super("*","Mine");
    }
}
