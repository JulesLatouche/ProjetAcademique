import java.io.*;
import java.net.*;
import java.util.*;

class Utilisateur implements Serializable {
    private static final long serialVersionUID = 1L;
    private String nomUtilisateur;
    private String motDePasse;

    public Utilisateur(String nomUtilisateur, String motDePasse) {
        this.nomUtilisateur = nomUtilisateur;
        this.motDePasse = motDePasse;
    }

    public String getNomUtilisateur() { return nomUtilisateur; }
    public String getMotDePasse() { return motDePasse; }
}
