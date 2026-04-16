import java.io.*;
import java.net.*;
import java.util.*;

public class BaseDeDonnees {
    private Map<String, ObjetStocke> objets = new HashMap<>();
    private final String fichierSauvegarde = "base_de_donnees.ser";

    // constructeur de base
    public BaseDeDonnees() {
        charger();
    }

    public synchronized void ajouterObjet(ObjetStocke objet) {
        objets.put(objet.getId(), objet);
        sauvegarder();
    }

    public synchronized ObjetStocke rechercherObjet(String id) {
        return objets.get(id);
    }

    public synchronized void supprimerObjet(String id) {
        objets.remove(id);
        sauvegarder();
    }

    private void sauvegarder() {
        try (ObjectOutputStream out = new ObjectOutputStream(new FileOutputStream(fichierSauvegarde))) {
            out.writeObject(objets);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    @SuppressWarnings("unchecked")
    private void charger() {
        try (ObjectInputStream in = new ObjectInputStream(new FileInputStream(fichierSauvegarde))) {
            objets = (Map<String, ObjetStocke>) in.readObject();
        } catch (IOException | ClassNotFoundException e) {
            objets = new HashMap<>();
        }
    }

    // Retourne une copie des objets sous forme de liste
    public synchronized List<ObjetStocke> getTousLesObjets() {
        return new ArrayList<>(objets.values());
    }

    // Supprime tous les objets de notre map
    public synchronized void supprimerTousLesObjets() {
        objets.clear();
        sauvegarder();
    }


}