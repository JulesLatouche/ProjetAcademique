cmake_minimum_required(VERSION 3.10)
project(projet C)

set(CMAKE_C_STANDARD 11)

# Serveur
add_executable(serveur
        Gestionnaire_Jeux.c
        reseau.c
        main_serveur.c
)

#  Client
add_executable(client
        reseau.c
        main_client.c
)

