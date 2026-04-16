
import java.sql.*;

public class Code_java {

    public static void main(String[] args) {
        String url = "jdbc:postgresql://kafka.iem/bd058766";
        String user = "bd058766";
        String password = "bd058766";

        try (Connection connection = DriverManager.getConnection(url, user, password)) {
            System.out.println("Connexion établie");

            // Exécute les différentes requêtes
            afficherPeagesLesPlusEmpruntes(connection);
            afficherTrajetsAbonne(connection, 1);  // Exemple d'UtilisateurID
            afficherAutomobilistesNonAbonnes(connection);
            //afficherPortionsPlusEmpruntees(connection);
            //afficherBadgesExpirés(connection);

        } catch (SQLException e) {
            System.out.println("Erreur lors de la connexion à la base");
            e.printStackTrace();
        }
    }

    // 1. Quels sont les péages les plus empruntés ?
    public static void afficherPeagesLesPlusEmpruntes(Connection connection) throws SQLException {
        String query = """
            SELECT p.NomPeage, COUNT(t.TrajetID) AS NombrePassages
            FROM GestionPeages.Peages p
            LEFT JOIN GestionPeages.Trajets t ON p.PeageID IN (t.PeageEntreeID, t.PeageSortieID)
            GROUP BY p.NomPeage
            ORDER BY NombrePassages DESC;
        """;

        try (Statement statement = connection.createStatement();
             ResultSet resultSet = statement.executeQuery(query)) {
            System.out.println("\n--- Péages les plus empruntés ---");
            while (resultSet.next()) {
                System.out.printf("Péage: %s | Passages: %d%n",
                        resultSet.getString("NomPeage"), resultSet.getInt("NombrePassages"));
            }
        }
    }

    // 2. Liste des trajets pour un utilisateur abonné donné
    public static void afficherTrajetsAbonne(Connection connection, int utilisateurID) throws SQLException {
        String query = """
            SELECT u.Nom, u.Prenom, t.DateEntree, pe.NomPeage AS PeageEntree, 
                   ps.NomPeage AS PeageSortie, t.Tarif
            FROM GestionPeages.Trajets t
            INNER JOIN GestionPeages.Badges b ON t.BadgeID = b.BadgeID
            INNER JOIN GestionPeages.Utilisateurs u ON b.UtilisateurID = u.UtilisateurID
            INNER JOIN GestionPeages.Peages pe ON t.PeageEntreeID = pe.PeageID
            LEFT JOIN GestionPeages.Peages ps ON t.PeageSortieID = ps.PeageID
            WHERE u.UtilisateurID = ?;
        """;

        try (PreparedStatement statement = connection.prepareStatement(query)) {
            statement.setInt(1, utilisateurID);
            ResultSet resultSet = statement.executeQuery();

            System.out.print("\n--- Trajets de l'utilisateur abonné ---");
            while (resultSet.next()) {  
                String nom = resultSet.getString("Nom");  // Colonne SQL "Nom"
                String prenom = resultSet.getString("Prenom");  // Colonne SQL "Prenom"
                String peageEntree = resultSet.getString("PeageEntree");  // Colonne SQL "PeageEntree"
                String peageSortie = resultSet.getString("PeageSortie");  // Colonne SQL "PeageSortie"
                double tarif = resultSet.getDouble("Tarif");  // Colonne SQL "Tarif"

                System.out.printf("Utilisateur: %s %s  | Entrée: %s | Sortie: %s | Tarif: %.2f€%n", 
                                  nom, prenom, peageEntree, peageSortie, tarif);
}

        }
    }

    // 3. Automobilistes non abonnés effectuant souvent le même trajet
    public static void afficherAutomobilistesNonAbonnes(Connection connection) throws SQLException {
        String query = """
            SELECT 
                u.UtilisateurID,
                u.Nom,
                u.Prenom,
                COUNT(t.TrajetID) AS NombreDeTrajets,
                t.PeageEntreeID,
                t.PeageSortieID
            FROM 
                gestionpeages.Utilisateurs u
            INNER JOIN 
                gestionpeages.Tickets tk ON u.UtilisateurID = tk.UtilisateurID
            INNER JOIN 
                gestionpeages.Trajets t ON tk.TicketID = t.TicketID
            WHERE 
                u.EstAbonne = FALSE
                AND t.PeageSortieID IS NOT NULL -- Exclure les trajets sans sortie
            GROUP BY 
                u.UtilisateurID, u.Nom, u.Prenom, t.PeageEntreeID, t.PeageSortieID
            HAVING 
                COUNT(t.TrajetID) > 0 -- Définir "souvent" comme au moins 3 fois
            ORDER BY 
                NombreDeTrajets DESC;
        """;

        try (Statement statement = connection.createStatement();
             ResultSet resultSet = statement.executeQuery(query)) {
            System.out.println("\n--- Automobilistes non abonnés souvent sur le même trajet ---");
            while (resultSet.next()) {
                System.out.printf("Plaque: %s | Entrée: %s | Sortie: %s | Nombre de trajets: %d%n",
                        resultSet.getString("PlaqueVehicule"), resultSet.getString("PeageEntree"),
                        resultSet.getString("PeageSortie"), resultSet.getInt("NombreTrajets"));
            }
        }
    }

    // 4. Portions d'autoroute les plus empruntées
    public static void afficherPortionsPlusEmpruntees(Connection connection) throws SQLException {
        String query = """
            SELECT pe1.NomPeage AS PeageEntree, pe2.NomPeage AS PeageSortie, 
                   COUNT(*) AS NombreTrajets
            FROM GestionPeages.Trajets t
            INNER JOIN GestionPeages.Peages pe1 ON t.PeageEntreeID = pe1.PeageID
            INNER JOIN GestionPeages.Peages pe2 ON t.PeageSortieID = pe2.PeageID
            GROUP BY PeageEntree, PeageSortie
            ORDER BY NombreTrajets DESC;
        """;

        try (Statement statement = connection.createStatement();
             ResultSet resultSet = statement.executeQuery(query)) {
            System.out.println("\n--- Portions d'autoroute les plus empruntées ---");
            while (resultSet.next()) {
                System.out.printf("Entrée: %s | Sortie: %s | Passages: %d%n",
                        resultSet.getString("PeageEntree"), resultSet.getString("PeageSortie"),
                        resultSet.getInt("NombreTrajets"));
            }
        }
    }

    // 5. Automobilistes dont l’abonnement est terminé mais qui ont encore un badge
    public static void afficherBadgesExpirés(Connection connection) throws SQLException {
        String query = """
            SELECT u.UtilisateurID, u.Nom, u.Prenom, b.BadgeID, b.DateFinAbonnement
            FROM GestionPeages.Badges b
            INNER JOIN GestionPeages.Utilisateurs u ON b.UtilisateurID = u.UtilisateurID
            WHERE b.DateFinAbonnement < CURRENT_DATE;
        """;

        try (Statement statement = connection.createStatement();
             ResultSet resultSet = statement.executeQuery(query)) {
            System.out.println("\n--- Badges expirés ---");
            while (resultSet.next()) {
                System.out.printf("Utilisateur ID: %d | Nom: %s %s | Badge ID: %d | Fin Abonnement: %s%n",
                        resultSet.getInt("UtilisateurID"), resultSet.getString("Nom"),
                        resultSet.getString("Prenom"), resultSet.getInt("BadgeID"),
                        resultSet.getDate("DateFinAbonnement"));
            }
        }
    }
}
