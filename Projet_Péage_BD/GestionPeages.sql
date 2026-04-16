CREATE SCHEMA GestionPeages;
SET search_path TO GestionPeages;
CREATE TABLE Utilisateurs (
    UtilisateurID INTEGER PRIMARY KEY,
    Nom TEXT NOT NULL,
    Prenom TEXT NOT NULL,
    Email TEXT UNIQUE,
    Telephone TEXT,
    EstAbonne BOOLEAN DEFAULT FALSE
);

-- Table pour les abonnements
CREATE TABLE Abonnements (
    AbonnementID INTEGER PRIMARY KEY,
    Niveau TEXT NOT NULL,
    TarifMensuel FLOAT NOT NULL,
    TauxReduction FLOAT NOT NULL CHECK (TauxReduction BETWEEN 0 AND 100)
);

-- Table pour les badges d'abonnement
CREATE TABLE Badges (
    BadgeID INTEGER  PRIMARY KEY,
    UtilisateurID INTEGER NOT NULL,
    AbonnementID INTEGER NOT NULL,
    DateDebutAbonnement DATE NOT NULL,
    DateFinAbonnement DATE,
    FOREIGN KEY (UtilisateurID) REFERENCES Utilisateurs(UtilisateurID),
    FOREIGN KEY (AbonnementID) REFERENCES Abonnements(AbonnementID)
);

-- Table pour les péages
CREATE TABLE Peages (
    PeageID INTEGER PRIMARY KEY,
    NomPeage TEXT NOT NULL,
    Localisation TEXT NOT NULL
);
-- Table pour les tickets (non abonnés)
CREATE TABLE Tickets (
    TicketID INTEGER PRIMARY KEY,
    NumeroTicket TEXT UNIQUE NOT NULL,
    DateEmission DATE NOT NULL,
    PlaqueVehicule TEXT NOT NULL,
    PeageEntreeID INTEGER NOT NULL,
	UtilisateurID INTEGER NOT NULL,
    FOREIGN KEY (PeageEntreeID) REFERENCES Peages(PeageID),
	FOREIGN KEY(UtilisateurID) REFERENCES Utilisateurs(UtilisateurID)
);

-- Table pour les trajets
CREATE TABLE Trajets (
    TrajetID INTEGER PRIMARY KEY,
    BadgeID INTEGER,
    TicketID INTEGER,
    DateEntree DATE NOT NULL,
    PeageEntreeID INTEGER NOT NULL,
    PeageSortieID INTEGER,
    DateSortie DATE,
    Tarif FLOAT,
    FOREIGN KEY (TicketID) REFERENCES Tickets(TicketID),
    FOREIGN KEY (BadgeID) REFERENCES Badges(BadgeID),
    FOREIGN KEY (PeageEntreeID) REFERENCES Peages(PeageID),
    FOREIGN KEY (PeageSortieID) REFERENCES Peages(PeageID)
);



-- Table pour les factures mensuelles
CREATE TABLE Factures (
    FactureID INTEGER PRIMARY KEY,
    UtilisateurID INTEGER NOT NULL,
    Mois DATE NOT NULL,
    MontantTrajets FLOAT NOT NULL,
    TarifMensuelAbonnement FLOAT,
    TotalFacture FLOAT NOT NULL,
    FOREIGN KEY (UtilisateurID) REFERENCES Utilisateurs(UtilisateurID)
);

-- Table pour la sécurité (plaques des véhicules enregistrées)
CREATE TABLE PlaquesEnregistrees (
    PlaqueID INTEGER PRIMARY KEY,
    PlaqueVehicule TEXT NOT NULL,
    PeageID INTEGER NOT NULL,
    DateEnregistrement DATE NOT NULL,
    FOREIGN KEY (PeageID) REFERENCES Peages(PeageID)
);