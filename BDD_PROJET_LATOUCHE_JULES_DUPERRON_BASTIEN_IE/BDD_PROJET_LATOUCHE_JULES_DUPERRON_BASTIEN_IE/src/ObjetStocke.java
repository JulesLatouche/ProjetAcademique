import java.io.*;
import java.net.*;
import java.util.*;


class ObjetStocke implements Serializable {
    private static final long serialVersionUID = 1L;
    private String id;
    private String nom;
    private String description;

    public ObjetStocke(String id, String nom, String description) {
        this.id = id;
        this.nom = nom;
        this.description = description;
    }

    public String getId() { return id; }
    public String getNom() { return nom; }
    public String getDescription() { return description; }

    @Override
    public String toString() {
        return "ObjetStocke{id='" + id + "', nom='" + nom + "', description='" + description + "'}";
    }
}
