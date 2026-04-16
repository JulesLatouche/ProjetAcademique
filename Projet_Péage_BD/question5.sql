SELECT u.UtilisateurID, u.Nom, u.Prenom, b.BadgeID, b.DateFinAbonnement
FROM gestionpeages.Badges b
INNER JOIN gestionpeages.Utilisateurs u ON b.UtilisateurID = u.UtilisateurID
WHERE b.DateFinAbonnement < CURRENT_DATE
  AND NOT EXISTS (
      SELECT 1
      FROM gestionpeages.Trajets t
      WHERE t.BadgeID = b.BadgeID AND t.DateSortie IS NULL
  );