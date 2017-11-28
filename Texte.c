#include "Sous_programmes.h"

/// Merci à http://www.cplusplus.com/reference/cstring/strstr/ pour l'explication de la fonction strstr

//permet d'aller jeusqu'au label dans un fichier type 1: pour la langue, type 2: pour le numéro
int fic_goto(FILE *fic, int num, int type, int depart)
{ ///LE FICHIER "fic" DOIT DÉJÀ ETRE OUVERT EN MODE LECTURE
    // type 1 est pour checher la langue, 2 le début du texte, 3 la fin du texte

    char a, b, test1;
    int rep, i, test2; //i est le numéro de l'octet à chaque fois

    rep = 0;

    switch (type) //pour voir quel type de label faut chercher
    {
        case 1:
        a = label1;
        b = num;
        break;
        case 2:
        case 3:
        a = label3;
        b = num;
        break;
    }

    fseek(fic, depart, SEEK_SET); //on va au depart (spécifié dans les paramètres)

    while (rep==0) //on teste chaque octet du fichier jusqu'à avoir le label cherché
    {
        if (type==3) //pour le case 3 on prend la position avant le label
        {
            i = ftell(fic);
        }

        fscanf(fic, "%c", &test1); //on scanne l'octet

        if (test1==a) //on voit si y'a le truc cherché (une paranthèse/crochet ouvrante)
        {
            fscanf(fic, "%d", &test2); //on scanne le numéro qui suit

            if (type==1 || type==2) //pour les cases 1 et 2 on prend la position après le label
            {
                i = ftell(fic);
            }

            if (test2==b) //on regarde si le numéro est le bon
            {
                rep = i;
            }
        }
    }
    return rep;
}

//entre le numéro d'un texte, la langue, et les paramètres éventuels dans joueur e la couleur
void texte(int num, int joueur, int langue, char *couleurp[4])
{
    char *pch;
    int a,b,c;

    FILE* fic = NULL;

    //ouverture du fichier
    fic = fopen ("texte.txt", "r");

    if (fic==NULL) exit(EXIT_FAILURE);//si on arrive pas à ouvrir le fichier ça renvoit une erreur

    a = fic_goto(fic, langue, 1, 0); //a est le début de la langue
    b = fic_goto(fic, num, 2, a); //b+4 est le début du texte
    c = fic_goto(fic, num+1, 3, a); //c-3 est la fin du texte

    // le texte va de b+4 à c-3 (fseek)

    char rep[(c-3)-(b+4)+2]; //création de la chaine de charactères de la bonne taille
                             //+2 discutablement inutile

    fseek(fic, b+4, SEEK_SET); //on va au début du texte

    fgets(rep, ((c-3)-(b+4)+1), fic); //on prend le string de la bonne taille

    //fermeture du fichier
    fclose(fic);


    //on remplace les # par des \n
    pch = strstr(rep,"#");
    while (pch!=NULL) //tant qu'il y a des #
    {
        strncpy (pch,"\n",1);
        pch = strstr (rep,"#");
    }


    switch (num)
    {
        ///Juste du texte
        case 1: //1) ATENTION! Agrandir la fenêtre! plein écran recommandé
        case 2: //2) Voulez-vous voir les règles du jeu? (0- non / 1- oui)
        case 3: //3) présentation et règles du jeu
        case 6: //6) Voulez vous rejouer? (0 - non / 1 - oui)
        case 7: //7) merci à Artemii Lazovskii pour la traduction en russe
        case 23: //23) Voulez vous charger une partie sauvegardée?
        case 24: //24) ***Pause***
        case 25: //25)  - Reprendre la partie
        case 26: //26)  - Sauvegarder la partie
        case 27: //27) ***Partie enregistrée***
        case 28: //28)  -  Quitter le jeu
        case 31: //31)  - azerty
        case 32: //32)  - qwerty
        case 34: //34) Combien de maisons?
        case 36: //36) Choisissez pour chaque maison si vous voullez une Intelligence Artificielle ou un Joueur
        case 40: //40) Confirmer
        case 22: //22) Utilisez Z pour vous déplacer vers le haut, S pour le bas, Q pour gauche D pour droite, ENTRÉE pour valider le choix et P pour mettre le jeu en pause
        case 33: //33) Utilisez Z pour déplacer le curseur vers le haut, S pour le bas et ENTRÉE pour selectionner
        case 42: //42) Partie %d
        case 43: //43) Quelle sauvegarde?
        case 44: //44) Annuler
        printf("%s", rep); //on pourrait mettre printf(rep); car rep est un string, mais un warning arrive.
        break;

        ///Du texte avec un %d (joueur)
        case 35: //35) %d Maisons
        case 41: //41) Nouvelle partie
        printf(rep, joueur);
        break;

        ///Du texte avec des %s%s (couleurp[joueur-1], RESET)
        case 19: //19) ***FIN DU TOUR***
        case 38: //38) PL
        case 39: //39) IA
        printf(rep, couleurp[joueur-1], RESET);
        break;

        /// Du texte avec des %s%d%s (couleurp[joueur-1], joueur, RESET)
        case 5: //5) Le joueur x a gagné
        case 9: //9) c'est au joueur x de jouer
        case 37: //37) Maison n°%d
        printf(rep, couleurp[joueur-1], joueur, RESET);
        break;

        default: //discutablement inutile
        break;
    }
}

//fin du fichier
