import java.io.*;
import java.net.*;
import java.util.*;

class Client {
    private static final String HOST = "localhost";
    private static final int PORT = 8080;

    public static void main(String[] args) {
        try (Socket socket = new Socket(HOST, PORT);
             ObjectOutputStream out = new ObjectOutputStream(socket.getOutputStream());
             ObjectInputStream in = new ObjectInputStream(socket.getInputStream());
             Scanner scanner = new Scanner(System.in)) {

            while (true) {
                String message = (String) in.readObject();
                System.out.println(message); // Message de connexion

                String commande = scanner.nextLine();
                out.writeObject(commande);
                out.flush();

                String reponse = (String) in.readObject();
                System.out.println("Réponse du serveur: " + reponse);

                if (reponse.contains("Connexion réussie")) {
                    break; // Passe à la boucle de commandes
                }
            }

            while (true) {
                String message = (String) in.readObject();
                System.out.println(message); // Invite de commande

                String commande = scanner.nextLine();
                out.writeObject(commande);
                out.flush();

                Object  reponse = in.readObject();
                //peut pas savoir à l'avance quel type d'objet on va recevoir
                if (reponse instanceof List) {//verif si c'est une liste
                    @SuppressWarnings("unchecked")
                    List<String> objets = (List<String>) reponse;
                    if (objets.isEmpty()) {
                        System.out.println("Aucun objet enregistré.");
                    } else {
                        System.out.println("Objets dans la base :");
                        for (Object obj : objets) {//parcours la lst d'objets de la bdd
                            System.out.println(obj);
                        }
                    }
                } else {
                    System.out.println("Réponse du serveur: " + reponse);
                }

                if (reponse.equals("Déconnexion réussie.")) {
                    break; // Quitte le programme proprement
                }
            }

        } catch (IOException | ClassNotFoundException e) {
            System.out.println("Erreur de connexion : " + e.getMessage());
        }
    }
}
