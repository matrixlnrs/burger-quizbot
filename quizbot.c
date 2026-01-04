/**
 * @file quizbot.c
 * @brief Programme principal du jeu BurgerQuizBot.
 * @details Contient la logique du jeu, la gestion des fichiers et l'interface utilisateur.
 * Compile avec : gcc -std=c11 -Wall -Wextra -o quizbot quizbot.c
 */

#include "quizbot.h"

/* =========================================================
   OUTILS DE TEXTE
   ========================================================= */

/**
 * @brief Nettoie une chaîne de caractères (trimming).
 * @details Enlève les espaces, retours à la ligne ('\n', '\r') à la fin,
 * ainsi que les espaces au début de la chaîne par décalage.
 * * @param texte La chaîne de caractères à nettoyer (modifiée sur place).
 */
void nettoyer_texte(char* texte) {
    if (!texte) return;
    
    // 1. Enlever la fin (espaces ou \n)
    int len = strlen(texte);
    while (len > 0 && (texte[len - 1] == ' ' || texte[len - 1] == '\n' || texte[len - 1] == '\r')) {
        texte[len - 1] = '\0';
        len--;
    }

    // 2. Enlever le début (décalage vers la gauche)
    int debut = 0;
    while (texte[debut] == ' ' && texte[debut] != '\0') {
        debut++;
    }

    if (debut > 0) {
        int i = 0;
        while (texte[debut + i] != '\0') {
            texte[i] = texte[debut + i];
            i++;
        }
        texte[i] = '\0';
    }
}

/**
 * @brief Convertit une chaîne de caractères en majuscules.
 * * @param texte La chaîne à convertir (modifiée sur place).
 */
void mettre_en_majuscule(char* texte) {
    for (int i = 0; texte[i]; i++) {
        texte[i] = toupper((unsigned char)texte[i]);
    }
}

/**
 * @brief Normalise une chaîne pour faciliter la comparaison.
 * @details Ne conserve que les caractères alphanumériques (lettres et chiffres)
 * et convertit le tout en majuscules. Supprime la ponctuation.
 * * @param source La chaîne d'origine.
 * @param dest Le buffer de destination (doit être suffisamment grand).
 */
void normaliser(char* source, char* dest) {
    int j = 0;
    for (int i = 0; source[i] != '\0'; i++) {
        char c = source[i];
        if (isalnum((unsigned char)c)) {
            dest[j++] = toupper((unsigned char)c);
        }
    }
    dest[j] = '\0';
}

/**
 * @brief Lit une chaîne de caractères depuis l'entrée standard (clavier).
 * @details Utilise fgets pour une lecture sécurisée et nettoie le résultat.
 * * @param buffer Le tableau où stocker la saisie.
 * @param taille La taille maximale du buffer.
 * @param message Le message à afficher à l'utilisateur avant la saisie (peut être NULL).
 * @return int 1 si la lecture a réussi, 0 sinon.
 */
int lire_clavier(char* buffer, int taille, const char* message) {
    if (message) printf("%s", message);
    if (fgets(buffer, taille, stdin) != NULL) {
        nettoyer_texte(buffer);
        return 1;
    }
    return 0;
}

/**
 * @brief Demande à l'utilisateur de saisir un nombre entier dans une plage donnée.
 * @details Boucle tant que la saisie n'est pas un entier valide compris entre min et max.
 * * @param min La valeur minimale acceptée.
 * @param max La valeur maximale acceptée.
 * @param message Le message à afficher à l'utilisateur.
 * @return int L'entier validé saisi par l'utilisateur.
 */
int lire_entier(int min, int max, const char* message) {
    char buffer[100];
    int valeur;
    while (1) {
        lire_clavier(buffer, sizeof(buffer), message);
        valeur = atoi(buffer); 
        if (valeur >= min && valeur <= max && strlen(buffer) > 0) {
            return valeur;
        }
        printf("Erreur : Veuillez entrer un nombre entre %d et %d.\n", min, max);
    }
}

/* =========================================================
   GESTION DES FICHIERS (Version Simple)
   ========================================================= */

/**
 * @brief Charge la liste des utilisateurs depuis le fichier binaire.
 * @details Alloue dynamiquement la mémoire pour le tableau d'utilisateurs.
 * * @param tableau Pointeur vers un pointeur d'Utilisateur (sera alloué par la fonction).
 * @param nombre Pointeur vers un entier pour stocker le nombre d'éléments lus.
 * @return int 1 en cas de succès, 0 si le fichier n'existe pas ou erreur d'allocation.
 */
int charger_utilisateurs(Utilisateur** tableau, int* nombre) {
    FILE* f = fopen(FICHIER_UTILISATEURS, "rb");
    if (!f) { *nombre = 0; return 0; }

    fseek(f, 0, SEEK_END);
    long taille_fichier = ftell(f);
    rewind(f);

    int nb_elements = taille_fichier / sizeof(Utilisateur);
    *tableau = malloc(nb_elements * sizeof(Utilisateur));
    
    if (*tableau) {
        fread(*tableau, sizeof(Utilisateur), nb_elements, f);
        *nombre = nb_elements;
    }
    fclose(f);
    return 1;
}

/**
 * @brief Charge la liste des questions depuis le fichier binaire.
 * * @param tableau Pointeur vers le tableau de questions (sortie).
 * @param nombre Pointeur vers le nombre de questions lues (sortie).
 * @return int 1 en cas de succès, 0 sinon.
 */
int charger_questions(Question** tableau, int* nombre) {
    FILE* f = fopen(FICHIER_QUESTIONS, "rb");
    if (!f) { *nombre = 0; return 0; }

    fseek(f, 0, SEEK_END);
    long taille = ftell(f);
    rewind(f);

    int nb = taille / sizeof(Question);
    *tableau = malloc(nb * sizeof(Question));
    
    if (*tableau) {
        fread(*tableau, sizeof(Question), nb, f);
        *nombre = nb;
    }
    fclose(f);
    return 1;
}

/**
 * @brief Charge l'historique des parties depuis le fichier binaire.
 * * @param tableau Pointeur vers le tableau d'historique (sortie).
 * @param nombre Pointeur vers le nombre d'entrées lues (sortie).
 * @return int 1 en cas de succès, 0 sinon.
 */
int charger_historique(Historique** tableau, int* nombre) {
    FILE* f = fopen(FICHIER_HISTORIQUE, "rb");
    if (!f) { *nombre = 0; return 0; }

    fseek(f, 0, SEEK_END);
    long taille = ftell(f);
    rewind(f);

    int nb = taille / sizeof(Historique);
    *tableau = malloc(nb * sizeof(Historique));
    
    if (*tableau) {
        fread(*tableau, sizeof(Historique), nb, f);
        *nombre = nb;
    }
    fclose(f);
    return 1;
}

/**
 * @brief Sauvegarde la liste complète des utilisateurs dans le fichier.
 * @details Écrase le contenu précédent du fichier ("wb"). Utile pour les mises à jour.
 * * @param tableau Le tableau des utilisateurs à sauvegarder.
 * @param nombre Le nombre d'utilisateurs dans le tableau.
 */
void sauvegarder_utilisateurs(Utilisateur* tableau, int nombre) {
    FILE* f = fopen(FICHIER_UTILISATEURS, "wb");
    if (f) {
        fwrite(tableau, sizeof(Utilisateur), nombre, f);
        fclose(f);
    }
}

/**
 * @brief Ajoute une nouvelle entrée à l'historique des parties.
 * @details Ajoute à la fin du fichier ("ab") sans écraser le reste.
 * * @param pseudo Le pseudo du joueur.
 * @param score Le score obtenu.
 * @param diff Le niveau de difficulté choisi.
 */
void ajouter_historique(const char* pseudo, int score, int diff) {
    Historique h;
    h.date_timestamp = (long long)time(NULL);
    strcpy(h.pseudo, pseudo);
    h.score = score;
    h.difficulte = diff;

    FILE* f = fopen(FICHIER_HISTORIQUE, "ab");
    if (f) {
        fwrite(&h, sizeof(Historique), 1, f);
        fclose(f);
    }
}

/**
 * @brief Ajoute une nouvelle question à la base de données.
 * * @param q Pointeur vers la structure Question à ajouter.
 */
void ajouter_question_db(Question* q) {
    FILE* f = fopen(FICHIER_QUESTIONS, "ab");
    if (f) {
        fwrite(q, sizeof(Question), 1, f);
        fclose(f);
    }
}

/* =========================================================
   LOGIQUE FLOUE (Levenshtein simplifié)
   ========================================================= */

/**
 * @brief Retourne le minimum entre trois entiers.
 * * @param a Premier entier.
 * @param b Deuxième entier.
 * @param c Troisième entier.
 * @return int La valeur la plus petite.
 */
int minimum(int a, int b, int c) {
    int min = a;
    if (b < min) min = b;
    if (c < min) min = c;
    return min;
}

/**
 * @brief Calcule la distance de Levenshtein (version dynamique).
 * @details Cette fonction mesure la différence entre deux chaînes.
 * Elle utilise l'allocation dynamique (malloc) pour créer les tableaux de calcul,
 * ce qui permet de comparer des chaînes de n'importe quelle longueur.
 * * L'algorithme utilise une optimisation spatiale : au lieu de stocker toute la matrice
 * (ce qui prendrait beaucoup de mémoire), on ne garde que deux lignes :
 * - La ligne précédente (v_prec)
 * - La ligne en cours de calcul (v_cour)
 * * @param s1 La première chaîne de caractères.
 * @param s2 La deuxième chaîne de caractères.
 * @return int Le nombre minimum d'opérations pour transformer s1 en s2.
 */
int distance_levenshtein(const char *s1, const char *s2) {
    int len1 = strlen(s1);
    int len2 = strlen(s2);

    // Si une des chaînes est vide, la distance est la longueur de l'autre
    // (car il faut tout ajouter ou tout supprimer)
    if (len1 == 0) return len2;
    if (len2 == 0) return len1;

    /* --- 1. Allocation Dynamique --- */
    // On alloue deux tableaux d'entiers de la taille de la deuxième chaîne (+1 pour la case vide)
    // v_prec : stockera les coûts de la ligne précédente
    // v_cour : stockera les coûts de la ligne qu'on est en train de calculer
    int *v_prec = malloc((len2 + 1) * sizeof(int));
    int *v_cour = malloc((len2 + 1) * sizeof(int));

    // Sécurité : Si l'ordinateur n'a plus de RAM, malloc renvoie NULL.
    if (!v_prec || !v_cour) {
        if (v_prec) free(v_prec);
        if (v_cour) free(v_cour);
        return -1; // Code d'erreur
    }

    /* --- 2. Initialisation --- */
    // On remplit la première ligne (0, 1, 2, 3...)
    // Cela correspond à transformer une chaîne vide en s2 (coût = nombre d'ajouts)
    for (int j = 0; j <= len2; j++) {
        v_prec[j] = j;
    }

    /* --- 3. Calcul (Boucle principale) --- */
    // On parcourt chaque lettre de s1 (i)
    for (int i = 0; i < len1; i++) {
        
        // La première case de la ligne courante est toujours i+1
        // (Correspond à transformer s1[0..i] en chaîne vide -> i+1 suppressions)
        v_cour[0] = i + 1;

        // On parcourt chaque lettre de s2 (j) pour remplir la ligne
        for (int j = 0; j < len2; j++) {
            
            // Coût de substitution : 0 si les lettres sont identiques, 1 sinon
            int cout_sub = (s1[i] == s2[j]) ? 0 : 1;

            // Le cœur de l'algo : on prend le minimum des 3 actions possibles
            v_cour[j + 1] = minimum(
                v_prec[j + 1] + 1,  // Suppression (case du haut)
                v_cour[j] + 1,      // Insertion (case de gauche)
                v_prec[j] + cout_sub // Substitution (case diagonale haut-gauche)
            );
        }

        /* --- 4. Échange des pointeurs (Le "Swap") --- */
        // Pour le prochain tour de boucle, la ligne "courante" devient la ligne "précédente".
        // Au lieu de copier les valeurs (lent), on échange juste les adresses mémoire.
        int *temp = v_prec;
        v_prec = v_cour;
        v_cour = temp;
    }

    /* --- 5. Résultat et Nettoyage --- */
    // Le résultat final se trouve dans la dernière case de v_prec 
    // (car on a swapé les pointeurs à la fin de la boucle, v_prec contient la dernière ligne calculée)
    int resultat = v_prec[len2];

    // Très important : libérer la mémoire manuellement en C !
    free(v_prec);
    free(v_cour);

    return resultat;
}
/**
 * @brief Vérifie si la réponse du joueur correspond à la réponse attendue.
 * @details Gère plusieurs cas :
 * 1. Correspondance exacte.
 * 2. Conversion V/F vers VRAI/FAUX.
 * 3. Présence du mot clé dans la phrase.
 * 4. Tolérance aux fautes de frappe via Levenshtein.
 * * @param saisie_joueur La chaîne tapée par le joueur.
 * @param reponse_attendue La réponse correcte (peut contenir des variantes séparées par '|').
 * @return int 1 si la réponse est acceptée, 0 sinon.
 */
int verifier_reponse(const char* saisie_joueur, const char* reponse_attendue) {
    char saisie_clean[256];
    char copie_reponses[256];
    
    normaliser((char*)saisie_joueur, saisie_clean);
    strcpy(copie_reponses, reponse_attendue);

    // Cas spécial Vrai/Faux
    if (strcmp(reponse_attendue, "VRAI") == 0 || strcmp(reponse_attendue, "FAUX") == 0) {
        if (strcmp(saisie_clean, "V") == 0) strcat(saisie_clean, "RAI"); // V -> VRAI
        if (strcmp(saisie_clean, "F") == 0) strcat(saisie_clean, "AUX"); // F -> FAUX
    }

    // On découpe les réponses possibles avec strtok (standard C)
    char* token = strtok(copie_reponses, "|");
    while (token != NULL) {
        char rep_clean[256];
        normaliser(token, rep_clean);

        // 1. Égalité parfaite
        if (strcmp(saisie_clean, rep_clean) == 0) return 1;

        // 2. Le mot clé est dans la phrase (ex: "Paris" dans "C'est Paris")
        if (strlen(rep_clean) > 3 && strstr(saisie_clean, rep_clean) != NULL) return 1;

        // 3. Distance de Levenshtein (fautes de frappe)
        int tolerance = 1;
        if (strlen(rep_clean) > 5) tolerance = 2;
        if (strlen(rep_clean) > 9) tolerance = 3;

        if (distance_levenshtein(saisie_clean, rep_clean) <= tolerance) return 1;

        token = strtok(NULL, "|");
    }
    return 0;
}

/* =========================================================
   INITIALISATION ET IMPORT
   ========================================================= */

/**
 * @brief Initialise la banque de questions au démarrage.
 * @details Si le fichier binaire des questions est vide ou inexistant,
 * cette fonction tente d'importer les questions depuis un fichier texte CSV
 * défini par FICHIER_INIT_TEXTE.
 */
void init_banque_questions(void) {
    Question* tab = NULL;
    int nb = 0;
    
    // Si le fichier binaire n'existe pas ou est vide
    if (!charger_questions(&tab, &nb) || nb == 0) {
        if (tab) free(tab);
        
        printf("Initialisation depuis le fichier texte '%s'...\n", FICHIER_INIT_TEXTE);
        FILE* f_txt = fopen(FICHIER_INIT_TEXTE, "r");
        if (!f_txt) {
            printf("Erreur : Impossible d'ouvrir %s\n", FICHIER_INIT_TEXTE);
            return;
        }

        char ligne[512];
        int id = 1;
        
        // On recrée le fichier binaire à zéro
        FILE* f_bin = fopen(FICHIER_QUESTIONS, "wb");

        while (fgets(ligne, sizeof(ligne), f_txt)) {
            if (ligne[0] == '#' || strlen(ligne) < 5) continue;
            
            ligne[strcspn(ligne, "\n")] = 0;

            Question q;
            memset(&q, 0, sizeof(Question));
            q.id = id++;
            q.type = 0;

            char* ptr = strtok(ligne, ";");
            if (!ptr) continue;
            strcpy(q.theme, ptr);

            ptr = strtok(NULL, ";");
            if (!ptr) continue;
            q.difficulte = atoi(ptr);

            ptr = strtok(NULL, ";");
            if (!ptr) continue;
            strcpy(q.enonce, ptr);

            ptr = strtok(NULL, ";");
            if (!ptr) continue;
            strcpy(q.reponse, ptr);

            if (f_bin) fwrite(&q, sizeof(Question), 1, f_bin);
        }
        
        fclose(f_txt);
        if (f_bin) fclose(f_bin);
        printf("Importation terminee.\n");
    } else {
        free(tab);
    }
}

/* =========================================================
   GESTION UTILISATEURS
   ========================================================= */

/**
 * @brief Gère la connexion ou l'inscription d'un joueur.
 * @details Demande le pseudo. Si le joueur existe, charge ses infos.
 * Sinon, crée un nouveau profil. Le premier joueur créé devient Admin.
 * * @param utilisateur_actuel Pointeur vers la structure qui recevra les infos du joueur.
 * @return int 1 si la connexion est réussie.
 */
int connexion_joueur(Utilisateur* utilisateur_actuel) {
    char saisie[64];
    lire_clavier(saisie, sizeof(saisie), "Entrez votre pseudo : ");
    
    if (strlen(saisie) == 0) strcpy(saisie, "Invité");

    Utilisateur* tab = NULL;
    int nb = 0;
    charger_utilisateurs(&tab, &nb);

    // Recherche si existe
    int index_trouve = -1;
    for (int i = 0; i < nb; i++) {
        if (strcmp(tab[i].pseudo, saisie) == 0) {
            index_trouve = i;
            break;
        }
    }

    if (index_trouve != -1) {
        *utilisateur_actuel = tab[index_trouve];
        free(tab);
        return 1;
    }

    // Nouveau joueur
    Utilisateur nouveau;
    strcpy(nouveau.pseudo, saisie);
    nouveau.meilleur_score = 0;
    
    // Le tout premier est Admin
    if (nb == 0) nouveau.est_admin = 1;
    else nouveau.est_admin = 0;

    // Ajout dans le fichier
    FILE* f = fopen(FICHIER_UTILISATEURS, "ab");
    fwrite(&nouveau, sizeof(Utilisateur), 1, f);
    fclose(f);

    *utilisateur_actuel = nouveau;
    free(tab);
    return 1;
}

/* =========================================================
   JEU ET MENUS
   ========================================================= */

/**
 * @brief Lance une session de jeu (le Quiz).
 * @details 1. Demande la difficulté.
 * 2. Sélectionne et mélange les questions.
 * 3. Pose les questions et vérifie les réponses.
 * 4. Enregistre le score et met à jour le record si nécessaire.
 * * @param utilisateur_actuel Pointeur vers le joueur en cours.
 */
void lancer_partie(Utilisateur* utilisateur_actuel) {
    Question* tab = NULL;
    int nb_total = 0;
    
    if (!charger_questions(&tab, &nb_total)) {
        printf("Aucune question trouvee !\n");
        return;
    }

    printf("\n--- DIFFICULTE ---\n1. Facile\n2. Moyen\n3. Difficile\n4. Tout melanger\n");
    int diff_choisie = lire_entier(1, 4, "Choix : ");

    // Création d'un tableau d'index pour mélanger
    int* indices = malloc(nb_total * sizeof(int));
    int nb_selection = 0;

    for (int i = 0; i < nb_total; i++) {
        if (diff_choisie == 4 || tab[i].difficulte == diff_choisie) {
            indices[nb_selection] = i;
            nb_selection++;
        }
    }

    if (nb_selection == 0) {
        printf("Pas de questions pour ce niveau.\n");
        free(indices); free(tab); return;
    }

    // Mélange simple (Fisher-Yates)
    for (int i = nb_selection - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = indices[i];
        indices[i] = indices[j];
        indices[j] = temp;
    }

    int score = 0;
    int max_questions = (nb_selection < MAX_QUESTIONS_JEU) ? nb_selection : MAX_QUESTIONS_JEU;

    printf("\n=== C'EST PARTI ! (%d questions) ===\n", max_questions);

    for (int i = 0; i < max_questions; i++) {
        Question* q = &tab[indices[i]]; // Pointeur vers la question actuelle
        
        printf("\nQ%d [%s] : %s\n", i+1, q->theme, q->enonce);
        char rep_user[256];
        lire_clavier(rep_user, sizeof(rep_user), "Reponse > ");

        if (verifier_reponse(rep_user, q->reponse)) {
            printf("BRAVO !\n");
            score++;
        } else {
            // Affiche la réponse proprement (coupe au premier |)
            char solution[256];
            strcpy(solution, q->reponse);
            char* pipe = strchr(solution, '|');
            if (pipe) *pipe = '\0';
            printf("Dommage... La reponse etait : %s\n", solution);
        }
    }

    printf("\n=== FIN DE PARTIE ===\nScore : %d / %d\n", score, max_questions);

    // Sauvegarde historique
    ajouter_historique(utilisateur_actuel->pseudo, score, diff_choisie);

    // Mise à jour High Score si besoin
    if (score > utilisateur_actuel->meilleur_score) {
        printf("Nouveau record personnel !\n");
        utilisateur_actuel->meilleur_score = score;
        
        // Mise à jour laborieuse mais simple (on recharge tout, on modifie, on sauvegarde tout)
        Utilisateur* tous_users = NULL;
        int nb_users = 0;
        charger_utilisateurs(&tous_users, &nb_users);
        
        for (int k = 0; k < nb_users; k++) {
            if (strcmp(tous_users[k].pseudo, utilisateur_actuel->pseudo) == 0) {
                tous_users[k].meilleur_score = score;
                break;
            }
        }
        sauvegarder_utilisateurs(tous_users, nb_users);
        free(tous_users);
    }

    free(indices);
    free(tab);
}

/**
 * @brief Affiche les 10 dernières parties d'un joueur spécifique.
 * * @param pseudo Le pseudo du joueur dont on veut voir l'historique.
 */
void afficher_historique(const char* pseudo) {
    Historique* tab = NULL;
    int nb = 0;
    charger_historique(&tab, &nb);

    printf("\n--- HISTORIQUE DE %s ---\n", pseudo);
    int trouve = 0;
    // On parcourt à l'envers pour avoir les plus récents
    for (int i = nb - 1; i >= 0; i--) {
        if (strcmp(tab[i].pseudo, pseudo) == 0) {
            // Conversion timestamp -> date lisible
            time_t t = (time_t)tab[i].date_timestamp;
            struct tm *tm_info = localtime(&t);
            char buffer_date[26];
            strftime(buffer_date, 26, "%d/%m/%Y %H:%M", tm_info);

            printf("[%s] Score: %d (Diff: %d)\n", buffer_date, tab[i].score, tab[i].difficulte);
            trouve++;
            if (trouve >= 10) break; // Limite aux 10 derniers
        }
    }
    if (trouve == 0) printf("Aucune partie jouée.\n");
    free(tab);
}

/**
 * @brief Affiche les 10 dernières parties jouées (tous joueurs confondus).
 * @note Fonction simplifiée : n'effectue pas de tri par score.
 */
void afficher_classement(void) {
    Historique* tab = NULL;
    int nb = 0;
    charger_historique(&tab, &nb);
    
    printf("\n--- DERNIERES PARTIES JOUEES (GLOBAL) ---\n");
    for (int i = nb - 1; i >= 0 && i > nb - 11; i--) {
         printf("%s : %d pts\n", tab[i].pseudo, tab[i].score);
    }
    free(tab);
}

/* =========================================================
   MENU PRINCIPAL
   ========================================================= */

/**
 * @brief Affiche et gère le menu réservé aux administrateurs.
 * @details Permet d'ajouter de nouvelles questions à la base de données.
 * * @param utilisateur_actuel Le joueur connecté (doit être admin).
 */
void menu_admin(Utilisateur* utilisateur_actuel) {
    while (1) {
        printf("\n=== ADMINISTRATION (%s) ===\n", utilisateur_actuel->pseudo);
        printf("1. Ajouter une question\n2. Retour\n");
        int choix = lire_entier(1, 2, "Choix : ");

        if (choix == 2) break;
        if (choix == 1) {
            Question q;
            memset(&q, 0, sizeof(Question));
            
            // Calcul du nouvel ID
            Question* temp = NULL; int n = 0;
            charger_questions(&temp, &n);
            int max_id = 0;
            for(int i=0; i<n; i++) if(temp[i].id > max_id) max_id = temp[i].id;
            q.id = max_id + 1;
            free(temp);

            lire_clavier(q.theme, sizeof(q.theme), "Theme : ");
            lire_clavier(q.enonce, sizeof(q.enonce), "Question : ");
            lire_clavier(q.reponse, sizeof(q.reponse), "Reponse(s) : ");
            q.difficulte = lire_entier(1, 3, "Difficulte (1-3) : ");
            
            ajouter_question_db(&q);
            printf("Question ajoutee !\n");
        }
    }
}

/**
 * @brief Affiche le menu principal et gère la navigation.
 * * @param utilisateur_actuel Le joueur connecté.
 */
void menu_principal(Utilisateur* utilisateur_actuel) {
    while (1) {
        printf("\n=================================\n");
        printf("   MENU PRINCIPAL - %s\n", utilisateur_actuel->pseudo);
        printf("=================================\n");
        printf("1. Jouer au Quiz\n");
        printf("2. Mon historique\n");
        printf("3. Voir les scores\n");
        if (utilisateur_actuel->est_admin) {
            printf("4. Administration\n");
        }
        printf("0. Quitter\n");

        int choix = lire_entier(0, 4, "Votre choix : ");

        if (choix == 0) break;
        if (choix == 1) lancer_partie(utilisateur_actuel);
        if (choix == 2) afficher_historique(utilisateur_actuel->pseudo);
        if (choix == 3) afficher_classement();
        if (choix == 4) {
            if (utilisateur_actuel->est_admin) menu_admin(utilisateur_actuel);
            else printf("Acces interdit !\n");
        }
    }
}

/**
 * @brief Point d'entrée du programme.
 * @details Initialise le générateur aléatoire, les fichiers et la banque de questions.
 * Gère la boucle principale du programme.
 * * @return int 0 si le programme s'est terminé correctement.
 */
int main(void) {
    srand(time(NULL));

    // Création des fichiers vides s'ils n'existent pas
    FILE* f = fopen(FICHIER_UTILISATEURS, "ab"); if(f) fclose(f);
    f = fopen(FICHIER_QUESTIONS, "ab"); if(f) fclose(f);
    f = fopen(FICHIER_HISTORIQUE, "ab"); if(f) fclose(f);

    init_banque_questions();

    printf("\nBIENVENUE DANS BURGERQUIZBOT !\n");
    
    Utilisateur joueur;
    if (connexion_joueur(&joueur)) {
        menu_principal(&joueur);
    }

    printf("Au revoir !\n");
    return 0;
}