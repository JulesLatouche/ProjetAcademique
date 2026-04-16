SELECT pe1.NomPeage AS PeageEntree, pe2.NomPeage AS PeageSortie, COUNT(*) AS NombreTrajets
FROM gestionpeages.Trajets t
INNER JOIN gestionpeages.Peages pe1 ON t.PeageEntreeID = pe1.PeageID
INNER JOIN gestionpeages.Peages pe2 ON t.PeageSortieID = pe2.PeageID
GROUP BY PeageEntree, PeageSortie
ORDER BY NombreTrajets DESC;