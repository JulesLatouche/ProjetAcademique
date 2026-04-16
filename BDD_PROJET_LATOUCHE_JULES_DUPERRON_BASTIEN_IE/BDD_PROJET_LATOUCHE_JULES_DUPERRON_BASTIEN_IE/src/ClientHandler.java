import java.io.*;
import java.net.*;
import java.util.*;

public class ClientHandler implements Runnable {
    private Socket socket;
    private BaseDeDonnees baseDeDonnees;
    private GestionUtilisateurs gestionUtilisateurs;
    private boolean authentifie = false;
    private String utilisateurActuel = null;

    public ClientHandler(Socket socket, BaseDeDonnees baseDeDonnees, GestionUtilisateurs gestionUtilisateurs) {
        this.socket = socket;
        this.baseDeDonnees = baseDeDonnees;
        this.gestionUtilisateurs = gestionUtilisateurs;
    }

    @Override
    public void run() {
        try (ObjectInputStream in = new ObjectInputStream(socket.getInputStream());
             ObjectOutputStream out = new ObjectOutputStream(socket.getOutputStream())) {

            // partie verifiation de l'autenticité de l'utilisateur
            while (!authentifie) {
                out.writeObject("Veuillez vous connecter (LOGIN;nom;motdepasse) ou créer un compte (REGISTER;nom;motdepasse)");
                out.flush();

                String commande = (String) in.readObject();
                String[] parts = commande.split(";");

                if (parts.length < 3) {
                    out.writeObject("Format incorrect. Essayez LOGIN;nom;motdepasse ou REGISTER;nom;motdepasse");
                    out.flush();
                    continue;
                }

                if (parts[0].equalsIgnoreCase("LOGIN")) {
                    if (gestionUtilisateurs.verifierUtilisateur(parts[1], parts[2])) {
                        authentifie = true;
                        utilisateurActuel = parts[1];
                        out.writeObject("Connexion réussie !");
                    } else {
                        out.writeObject("Échec de l'authentification.");
                    }
                } else if (parts[0].equalsIgnoreCase("REGISTER")) {
                    if (gestionUtilisateurs.ajouterUtilisateur(parts[1], parts[2])) {
                        out.writeObject("Utilisateur créé avec succès.");
                    } else {
                        out.writeObject("Utilisateur déjà existant.");
                    }
                } else {
                    out.writeObject("Commande invalide.");
                }
                out.flush();
            }

            // partie commande saisie par l'utilisateur
            while (authentifie) {
                try {
                    out.writeObject("Bienvenue, " + utilisateurActuel + " ! Entrez une commande (AJOUTER;id;nom;desc | RECHERCHER;id | SUPPRIMER;id |SUPPR_TOUT | AFFICHER_TOUT | QUITTER ) :");
                    out.flush();
                    String commande = (String) in.readObject();
                    String[] parts = commande.split(";");

                    if (parts.length == 0) {
                        out.writeObject("Commande invalide.");
                        out.flush();
                        continue;
                    }

                    switch (parts[0].toUpperCase()) {
                        case "AJOUTER":
                            if (parts.length == 4) {
                                ObjetStocke objet = new ObjetStocke(parts[1], parts[2], parts[3]);
                                baseDeDonnees.ajouterObjet(objet);
                                out.writeObject("Objet ajouté avec succès.");
                            } else {
                                out.writeObject("Format incorrect. Utilisez AJOUTER;id;nom;description");
                            }
                            break;
                        case "RECHERCHER":
                            if (parts.length == 2) {
                                ObjetStocke objet = baseDeDonnees.rechercherObjet(parts[1]);
                                out.writeObject((objet == null) ? "Objet non trouvé" : objet.toString());
                            } else {
                                out.writeObject("Format incorrect. Utilisez RECHERCHER;id");
                            }
                            break;
                        case "SUPPRIMER":
                            if (parts.length == 2) {
                                baseDeDonnees.supprimerObjet(parts[1]);
                                out.writeObject("Objet supprimé.");
                            } else {
                                out.writeObject("Format incorrect. Utilisez SUPPRIMER;id");
                            }
                            break;
                        case "SUPPR_TOUT":
                            if (parts.length == 1) {
                                baseDeDonnees.supprimerTousLesObjets();
                                out.writeObject("Tous les objets ont été supprimés.");
                            }else {
                                out.writeObject("Format incorrect. Utilisez SUPPRIMER;id");
                            }
                            break;
                        case "AFFICHER_TOUT":
                            if(parts.length == 1){
                                List<ObjetStocke> objets = baseDeDonnees.getTousLesObjets();
                                out.writeObject(objets.isEmpty() ? "Aucun objet enregistré." : objets);
                            }else {
                                out.writeObject("Format incorrect");
                            }
                            break;
                        case "QUITTER":
                            authentifie = false;
                            out.writeObject("Déconnexion réussie.");
                            out.flush();
                            break;
                        default:
                            out.writeObject("Commande inconnue.");
                    }
                    out.flush();
                } catch (IOException | ClassNotFoundException e) {
                    System.out.println(" Erreur de connexion avec le client : " + e.getMessage());
                    break; // Quitte la boucle proprement
                }
            }

        } catch (IOException | ClassNotFoundException e) {
            System.out.println(" Une erreur s'est produite avec le client : " + e.getMessage());
        } finally {
            try {
                // fermeture propre
                socket.close();
            } catch (IOException e) {
                System.out.println("️ Impossible de fermer le socket : " + e.getMessage());
            }
        }
    }
}


