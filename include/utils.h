#ifndef UTILS_H
#define UTILS_H
#include <string.h>
#include <stdio.h>

// Fonction pour nettoyer le buffer d'entrée
void nettoyer_texte(char *text);

// Fonction pour vérifier la réponse de l'utilisateur
int verifier_reponse(char *reponse_user, char *mot_cle_attendu);

#endif