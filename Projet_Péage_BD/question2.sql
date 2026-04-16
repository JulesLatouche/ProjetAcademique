SELECT u.nom, u.prenom,t.DateEntree, t.DateSortie, pe.NomPeage AS PeageEntree, ps.NomPeage AS PeageSortie, t.Tarif
FROM gestionpeages.Trajets t
INNER JOIN gestionpeages.Badges b ON t.BadgeID = b.BadgeID
INNER JOIN gestionpeages.Utilisateurs u ON b.UtilisateurID = u.UtilisateurID
INNER JOIN gestionpeages.Peages pe ON t.PeageEntreeID = pe.PeageID
LEFT JOIN gestionpeages.Peages ps ON t.PeageSortieID = ps.PeageID
WHERE u.UtilisateurID = 3;