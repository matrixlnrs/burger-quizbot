#ifndef STRUCTURES_H
#define STRUCTURES_H

// Structure représentant un Joueur
typedef struct {
    char pseudo[50];
    int score_actuel;
    int meilleur_score;
} User;

// Structure représentant une Question
typedef struct {
    int id;
    char enonce[256];       // Le texte de la question
    
    char mot_cle[50];    // Le mot clé associé à la question

    char explication[512]; // Explications ou détails supplémentaires sur la question
} Question;

#endif