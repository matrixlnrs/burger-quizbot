===================================================
Document: Spécifications "Burger QuizBot"
Rôle: Concepteur
===================================================

1. TITRE DU PROJET
[cite_start]Projet Semestre 1: Burger QuizBot [cite: 1]

2. OBJECTIF
[cite_start]Développer un jeu textuel interactif en langage C[cite: 2, 61]. Le jeu est un quiz (type "Burger Quiz") où l'utilisateur interagit via des commandes textuelles dans un terminal.

3. INTERFACE
Exclusivement textuelle (dans le terminal). Aucune IHM graphique.

4. [cite_start]ARCHITECTURE DES DONNÉES (LES 3 "BASES DE DONNÉES") [cite: 71]

   4.1. Fichier "utilisateurs.dat" (Base de Données Utilisateurs)
   - Objectif: Stocker les profils des joueurs.
   - Champs de structure:
     - identifiant (string, ex: "pseudo")
     - importance (int, ex: 0=Joueur, 1=Admin)
     - meilleur_score (int)

   4.2. Fichier "questions.dat" (Base de Données des Réponses)
   - Objectif: Stocker la banque de questions/réponses du quiz.
   - Champs de structure:
     - id_question (int, unique)
     - texte_question (string)
     - option_A (string)
     - option_B (string)
     - option_C (string)
     - index_bonne_reponse (int)
     - (Futur: theme (string) - voir section 6)

   4.3. Fichier "historique.dat" (Base de Données Historique)
   - Objectif: Sauvegarder les interactions (l'historique des parties).
   - Champs de structure:
     - timestamp (date/heure de la partie)
     - pseudo_joueur (string)
     - score_partie (int)

5. MODES DE FONCTIONNEMENT

   5.1. Mode Utilisation (Joueur)
   - Objectif: Permettre à l'utilisateur de jouer au quiz.
   - Fonctionnalités:
     - Authentification: L'utilisateur saisit un pseudo.
       - Si le pseudo n'existe pas, il est créé (importance=0, meilleur_score=0).
       - [cite_start]Si le pseudo existe, ses informations sont chargées[cite: 68].
     - Menu Joueur:
       - 1. Lancer une partie.
       - [cite_start]2. Consulter l'historique de ses scores (Req 10)[cite: 86].
       - 3. Quitter.
     - [cite_start]Déroulé du jeu (Interaction - Req 9)[cite: 86]:
       - Le bot pose des questions lues depuis "questions.dat".
       - L'utilisateur saisit une réponse.
       - [cite_start]Le bot génère une réponse contextuelle (Bonne/Mauvaise réponse, score)[cite: 67].
       - À la fin de la partie, le score est enregistré dans "historique.dat" et le "meilleur_score" est mis à jour dans "utilisateurs.dat" si nécessaire.

   5.2. [cite_start]Mode Administration (Maître du Jeu) [cite: 69]
   - [cite_start]Objectif: Permettre la gestion du jeu sans recompiler le programme[cite: 69].
   - Accès: L'utilisateur doit être identifié comme "Admin" (via 'importance' == 1 dans "utilisateurs.dat").
   - Fonctionnalités:
     - [cite_start]1. Gestion des Questions (Req 8)[cite: 86]:
       - Ajouter une question (et ses réponses) au fichier "questions.dat".
       - Modifier une question existante.
       - Supprimer une question.
     - [cite_start]2. Gestion des Utilisateurs (Req 7)[cite: 86]:
       - Lister tous les utilisateurs.
       - Modifier un utilisateur (ex: promouvoir un joueur en Admin).
       - Supprimer un utilisateur.

6. [cite_start]FONCTIONNALITÉS EXTENSIBLES (Pour le futur) [cite: 70]
   - Thèmes de questions: Ajouter le champ "theme" (ex: "Sel", "Poivre", "Menus") à la structure Question.
   - Épreuves: Permettre au joueur de choisir une épreuve (un thème).
   - Épreuve finale: Créer un thème spécial "Burger de la Mort" avec des règles différentes.