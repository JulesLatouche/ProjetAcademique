SELECT 
  	u.UtilisateurID,
    u.Nom,
    u.Prenom,
    COUNT(t.TrajetID) AS NombreDeTrajets,
    t.PeageEntreeID,
    t.PeageSortieID
FROM gestionpeages.Utilisateurs u
INNER JOIN gestionpeages.Tickets tk ON u.UtilisateurID = tk.UtilisateurID
INNER JOIN gestionpeages.Trajets t ON tk.TicketID = t.TicketID
WHERE u.EstAbonne = FALSE AND t.PeageSortieID IS NOT NULL -- Exclure les trajets sans sortie
GROUP BY u.UtilisateurID, u.Nom, u.Prenom, t.PeageEntreeID, t.PeageSortieID
HAVING COUNT(t.TrajetID) > 2 -- Définir "souvent" comme au moins 3 fois
ORDER BY NombreDeTrajets DESC;