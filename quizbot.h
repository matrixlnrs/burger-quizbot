#ifndef QUIZBOT_H
#define QUIZBOT_H

#define _CRT_SECURE_NO_WARNINGS 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

/* --- Fichiers de sauvegarde --- */
#define FICHIER_UTILISATEURS "users.dat"
#define FICHIER_QUESTIONS    "questions.dat"
#define FICHIER_HISTORIQUE   "history.dat"
#define FICHIER_INIT_TEXTE   "questions.txt"

#define MAX_QUESTIONS_JEU 10
#define NIVEAU_MIXTE 4

/* --- Structures --- */

typedef struct {
    char pseudo[64];
    int est_admin;      // 1 = Oui, 0 = Non
    int meilleur_score;
} Utilisateur;

typedef struct {
    int id;
    int difficulte;     // 1, 2 ou 3
    int type;           // 0 pour Texte
    char theme[64];
    char enonce[256];
    char reponse[256];  // Contient les réponses séparées par |
} Question;

typedef struct {
    long long date_timestamp;
    char pseudo[64];
    int score;
    int difficulte;
} Historique;

/* --- Fonctions --- */

// Outils texte
void nettoyer_texte(char* texte);
void mettre_en_majuscule(char* texte);
int lire_clavier(char* buffer, int taille, const char* message);
int lire_entier(int min, int max, const char* message);

// Gestion Fichiers (Plus explicite, moins générique)
int charger_utilisateurs(Utilisateur** tableau, int* nombre);
int charger_questions(Question** tableau, int* nombre);
int charger_historique(Historique** tableau, int* nombre);

void sauvegarder_utilisateurs(Utilisateur* tableau, int nombre);
void ajouter_historique(const char* pseudo, int score, int diff);
void ajouter_question_db(Question* q);

// Logique Jeu
void init_banque_questions(void);
int verifier_reponse(const char* saisie_joueur, const char* reponse_attendue);
int connexion_joueur(Utilisateur* utilisateur_actuel);
void lancer_partie(Utilisateur* utilisateur_actuel);

// Affichage
void afficher_historique(const char* pseudo);
void afficher_classement(void);
void menu_admin(Utilisateur* utilisateur_actuel);
void menu_principal(Utilisateur* utilisateur_actuel);

#endif // QUIZBOT_H