INSERT INTO GestionPeages.Utilisateurs (UtilisateurID, Nom, Prenom, Email, Telephone, EstAbonne)
    VALUES 
        (1, 'Dupont', 'Jean', 'jean.dupont@email.com', '0612345678', TRUE), --  utilisateur 1
        (2, 'Martin', 'Alice', 'alice.martin@email.com', '0623456789', FALSE), -- utilisateur 2
        (3,'Chameroixx', 'Bertrand','bc.ch@email.com','0719457456',TRUE), --utilisateur 3
        (4, 'laurent', 'lafite', 'lafit.lr@email.com', '0578998856', FALSE), -- utilisateur 4
        (5,'auchard', 'luca','luca.hau@email.com','0658998896',FALSE), --utilisateur 5
        (6, 'Durand', 'Claire', 'claire.durand@email.com', '0634123456', TRUE), -- Nouvelle abonnée
        (7, 'Petit', 'Marc', 'marc.petit@email.com', '0645123457', FALSE); -- Non abonné



INSERT INTO GestionPeages.Abonnements (AbonnementID, Niveau, TarifMensuel, TauxReduction) 
    VALUES 
    (1, 'simple', 10.0, 5),  -- niveaux des abonements 
    (2, 'avancé', 20.0, 10), 
    (3, 'prenuim', 35.0, 15);


INSERT INTO GestionPeages.Badges (BadgeID, UtilisateurID, AbonnementID, DateDebutAbonnement, DateFinAbonnement) 
    VALUES 
        (1, 1, 2, '2024-01-01', '2024-12-31'), -- Badge pour Jean Dupont
        (2, 3, 3,'2024-01-01','2024-12-31'),-- badge pour bertrand 
        (3, 4, 1,'2024-12-31','2023-01-01'),-- badge pour laurent périmée
        (4, 6, 3, '2024-01-01', '2024-12-31'); -- Badge pour Claire Durand


INSERT INTO GestionPeages.Peages (PeageID, NomPeage, Localisation) 
    VALUES  
        (1, 'Peage Nord', 'Paris'), 
        (2, 'Peage Sud', 'Lyon'), 
        (3, 'Peage Est', 'Strasbourg'), 
        (4, 'Peage Ouest', 'Nantes');
		
INSERT INTO GestionPeages.Tickets (TicketID, NumeroTicket, DateEmission, PlaqueVehicule, PeageEntreeID, UtilisateurID)
VALUES 
(1, 'TICKET123', '2024-10-11', 'AB-123-CD', 3, 2), -- ticket Peage Est
(2, 'TICKET124', '2024-09-04', 'EF-456-GH', 3, 4), -- autre ticket 
(3, 'TICKET125', '2024-10-15', 'GH-789-IJ', 3, 5), -- pareil
(4, 'TICKET126', '2024-10-15', 'IJ-111-KL', 3, 5),
(5, 'TICKET127', '2024-10-15', 'KL-222-MN', 3, 5),
(6, 'TICKET128', '2024-10-15', 'MN-333-OP', 3, 5);

-- Abonnés
INSERT INTO GestionPeages.Trajets (TrajetID, BadgeID, TicketID, DateEntree, PeageEntreeID, DateSortie, PeageSortieID, Tarif)
VALUES 
(1, 1, NULL, '2024-10-10', 1, '2024-10-10', 2, 15.0), -- Jean Dupont, trajet entre Peage Nord et Peage Sud
(2, 2, NULL, '2024-04-04', 3, '2024-04-04', 4, 10.0), -- Bertrand, trajet entre Peage Est et Peage Ouest
(5, 4, NULL, '2024-10-12', 2, '2024-10-12', 1, 25.0); -- Claire Durand, retour Sud -> Nord

-- Non abonnés
INSERT INTO GestionPeages.Trajets (TrajetID, TicketID, DateEntree, PeageEntreeID, DateSortie, PeageSortieID, Tarif)
VALUES 
(3, 1, '2024-10-11', 3, '2024-10-11', 4, 20.0), -- Ticket 1, Peage Est -> Peage Ouest
(4, 2, '2024-09-04', 3, '2024-09-04', 1, 50.0), -- Ticket 2, Peage Est -> Peage Nord
(6, 3, '2024-10-15', 3, '2024-10-15', 4, 22.0), -- Ticket 3, Peage Est -> Peage Ouest
(7, 4, '2024-10-15', 3, '2024-10-15', 4, 22.0),
(8, 5, '2024-10-15', 3, '2024-10-15', 4, 22.0),
(9, 6, '2024-10-15', 3, '2024-10-15', 4, 22.0);





INSERT INTO GestionPeages.PlaquesEnregistrees (PlaqueID, PlaqueVehicule, PeageID, DateEnregistrement)
VALUES 
(1, 'AB-123-CD', 3, '2024-10-11'),
(2, 'EF-456-GH', 3, '2024-09-04'),
(3, 'GH-789-IJ', 4, '2024-10-15'); 

INSERT INTO GestionPeages.Factures (FactureID, UtilisateurID, Mois, MontantTrajets, TarifMensuelAbonnement, TotalFacture)
VALUES 
(1, 1, '2024-10-01', 45.0, 20.0, 65.0), -- Facture pour Jean Dupont
(2,3,'2024-10-01',35.0,20.0,65.0), -- pour bertrand
(3,4,'2024-10-01',0,10.0,120.0),
(4, 6, '2024-10-01', 35, 35, 50.0); -- Facture pour Claire Duran
