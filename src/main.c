#include <stdio.h>
#include <string.h>
#include "../include/structures.h"
#include "../include/utils.h"

// On déclare les fonctions de analyse.c ici (ou on crée un analyse.h, 
// mais pour compiler simple on peut inclure le .c en bas ou linker)
// Pour faire propre, ajoutons les prototypes ici :
void nettoyer_texte(char *texte);
int verifier_reponse(char *reponse_user, char *mot_cle_attendu);

int main() {
    // --- Étape 2 du PDF : Création des données de test ---
    Question tab_questions[3];

    // Question 1
    tab_questions[0].id = 1;
    strcpy(tab_questions[0].enonce, "Quelle est la capitale de la France ?");
    strcpy(tab_questions[0].mot_cle, "paris"); // Le mot magique
    strcpy(tab_questions[0].explication, "Paris est la capitale.");

    // Question 2
    tab_questions[1].id = 2;
    strcpy(tab_questions[1].enonce, "Quelle est la couleur du cheval blanc d'Henri IV ?");
    strcpy(tab_questions[1].mot_cle, "blanc");
    strcpy(tab_questions[1].explication, "Il est blanc, c'est dans la question !");

    // Question 3
    tab_questions[2].id = 3;
    strcpy(tab_questions[2].enonce, "Combien font 2 + 2 ?");
    strcpy(tab_questions[2].mot_cle, "4"); // On peut stocker "quatre" aussi si on veut
    strcpy(tab_questions[2].explication, "Ca fait 4.");

    // --- Étape 3 du PDF : La boucle de jeu ---
    int score = 0;
    char reponse_user[256]; // Buffer pour stocker la phrase de l'utilisateur

    printf("=== BURGER QUIZ BOT (Version Reponse Libre) ===\n");
    printf("Repondez aux questions (ecrivez des phrases completes si vous voulez).\n\n");

    for (int i = 0; i < 3; i++) {
        // A. Affichage
        printf("QUESTION %d : %s\n", i + 1, tab_questions[i].enonce);
        
        // B. Saisie (On utilise fgets au lieu de scanf pour lire les espaces)
        // scanf s'arrête au premier espace, fgets lit toute la phrase.
        printf("Votre reponse > ");
        fgets(reponse_user, 256, stdin);

        // C. Vérification (Appel à notre logique intelligente)
        if (verifier_reponse(reponse_user, tab_questions[i].mot_cle)) {
            printf("[OUI] Bonne reponse !\n");
            score++;
        } else {
            printf("[NON] Rate.\n");
            printf("On attendait le mot-cle : %s\n", tab_questions[i].mot_cle);
        }
        
        printf("-> %s\n", tab_questions[i].explication);
        printf("-----------------------------------\n");
    }

    // Affichage score final (comme dans le PDF)
    printf("Partie terminee ! Score : %d / 3\n", score);

    return 0;
}