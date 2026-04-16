import java.io.*;
import java.net.*;
import java.util.*;
class Serveur {
    private static final int PORT = 8080;
    private BaseDeDonnees baseDeDonnees = new BaseDeDonnees();
    private GestionUtilisateurs gestionUtilisateurs = new GestionUtilisateurs();

    public void demarrer() {
        try (ServerSocket serverSocket = new ServerSocket(PORT)) {
            System.out.println("Serveur en écoute sur le port " + PORT);
            while (true) {
                Socket clientSocket = serverSocket.accept();
                System.out.println("un nouveau client est connecté avec le socket : " + clientSocket);
                new Thread(new ClientHandler(clientSocket, baseDeDonnees, gestionUtilisateurs)).start();
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public static void main(String[] args) {
        new Serveur().demarrer();
    }
}
