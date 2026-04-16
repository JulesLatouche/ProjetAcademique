import java.io.*;
import java.net.*;
import java.util.*;

public class GestionUtilisateurs {
    private Map<String, String> utilisateurs = new HashMap<>();
    private final String fichierUtilisateurs = "utilisateurs.ser";

    public GestionUtilisateurs() {
        chargerUtilisateurs();
    }

    public synchronized boolean ajouterUtilisateur(String nom, String motDePasse) {
        if (utilisateurs.containsKey(nom)) {
            return false; // L'utilisateur existe déjà
        }
        utilisateurs.put(nom, motDePasse);
        sauvegarderUtilisateurs();
        return true;
    }

    public synchronized boolean verifierUtilisateur(String nom, String motDePasse) {
        return utilisateurs.containsKey(nom) && utilisateurs.get(nom).equals(motDePasse);
    }

    private void sauvegarderUtilisateurs() {
        try (ObjectOutputStream out = new ObjectOutputStream(new FileOutputStream(fichierUtilisateurs))) {
            out.writeObject(utilisateurs);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    @SuppressWarnings("unchecked")
    private void chargerUtilisateurs() {
        try (ObjectInputStream in = new ObjectInputStream(new FileInputStream(fichierUtilisateurs))) {
            utilisateurs = (Map<String, String>) in.readObject();
        } catch (IOException | ClassNotFoundException e) {
            utilisateurs = new HashMap<>();
        }
    }
}
