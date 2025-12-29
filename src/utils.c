#include "../include/utils.h"
#include <ctype.h>

//----------- FONCTIONS TRAITEMENT TEXTE ---------------

/**
 * @brief Nettoie le texte en convertissant en minuscule et en supprimant le retour à la ligne.
 * @param text Le texte à nettoyer.
 */
void nettoyer_texte(char *text) {
    for(int i = 0; text[i]; i++) {
        // Conversion en minuscule
        text[i] = tolower((unsigned char)text[i]);
        
        // Remplacement du retour à la ligne (\n) par fin de chaine (\0)
        // C'est un classique avec fgets()
        if(text[i] == '\n') {
            text[i] = '\0';
            break; 
        }
    }
}

/**
 * @brief Vérifie si la réponse de l'utilisateur contient le mot clé attendu.
 * @param reponse_user La réponse fournie par l'utilisateur.
 * @param mot_cle_attendu Le mot clé attendu pour valider la réponse.
 * @return 1 si le mot clé est trouvé dans la réponse, 0 sinon.
 */
// Renvoie 1 (Vrai) si le mot clé est trouvé, 0 (Faux) sinon.
int verifier_reponse(char *reponse_user, char *mot_cle_attendu) {
    // 1. On crée des copies temporaires pour ne pas abîmer les originaux
    char reponse_temp[256];
    char mot_cle_temp[50];
    
    // Copie sécurisée
    strcpy(reponse_temp, reponse_user);
    strcpy(mot_cle_temp, mot_cle_attendu);
    
    // 2. On nettoie les deux (tout en minuscule)
    nettoyer_texte(reponse_temp);
    nettoyer_texte(mot_cle_temp);
    
    // 3. Recherche de sous-chaîne (strstr)
    // strstr cherche si "mot_cle_temp" existe dans "reponse_temp"
    // Exemple : cherche "paris" dans "c'est paris je crois" -> Trouvé !
    if (strstr(reponse_temp, mot_cle_temp) != NULL) {
        return 1; // VRAI
    }
    
    return 0; // FAUX
}

//----------- FIN FONCTIONS TRAITEMENT TEXTE ---------------