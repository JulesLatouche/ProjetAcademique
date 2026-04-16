SELECT p.NomPeage, COUNT(*) AS NombrePassages
FROM gestionpeages.trajets t
INNER JOIN gestionpeages.Peages p ON t.PeageEntreeID = p.PeageID
GROUP BY p.NomPeage
ORDER BY NombrePassages DESC;