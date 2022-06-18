#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>

#define NB_LIGNES_JOUEURS 5
#define NB_COLONNES_JOUEURS 16

#define NB_LIGNES_HEROS 5
#define NB_COLONNES_HEROS 7

#define NB_LIGNES_UNITES 12
#define NB_COLONNES_UNITES 8

#define NB_LIGNES_HEROSJOUEURS 25
#define NB_COLONNES_HEROSJOUEURS 11

#define NB_LIGNES_UNITESJOUEURS 60
#define NB_COLONNES_UNITESJOUEURS 11

#define NB_LIGNES_CARTE 10
#define NB_COLONNES_CARTE 10

#define NB_EMPLACEMENT 6
#define NB_ESPACES_CENTRE 46
#define WIDTH_C_CASE_JOUEUR 40
#define WIDTH_C_VIDE 14
#define WIDTH_NB_C_C_J 5
#define WIDTH_MENU 39

#define TAILLE_BLOC 40


#define NB_LIGNES_UNITESBATAILLE 12
#define NB_COLONNES_UNITESBATAILLE 13

#define NB_LIGNES_PLATEAUBATAILLE 12
#define NB_COLONNES_PLATEAUBATAILLE 12

#define JOUEUR_HUMAIN 0
#define LARGEUR_ECRAN_EN_CARACTERES 151
#define NB_EMPLACEMENTS_UNITES 6



typedef enum {NUM_JOUEUR_J, LIGNE_J, COLONNE_J, OR, BOIS, PIERRE, FER,
SOUFFRE,SULFURE, GEMME_BLEU, GEMME_ROUGE, GEMME_JAUNE, NB_UNITES_TUEES,
NB_BATAILLE_GAGNES, NB_UNITES_PERDUES, NB_BATAILLE_PERDUES} E_JOUEURS;

typedef enum {NUM_HERO_H, BONUS_ATTAQUE_H, BONUS_DEFENSE_H,
CHANCE_CRITIQUE_H, MORAL_H, POINTS_ATTAQUE_SPECIALE_H, PRIX_H} E_HEROS;

typedef enum {NUM_UNITE_U, TYPE_U, SANTE_U, ATTAQUE_U, DEFENSE_U, DEGATS_U,
MOUVEMENTS_U, PRIX_U} E_UNITES;

typedef enum {NUM_JOUEUR_HJ, NUM_HERO_HJ, HERO_PRINCIPAL, POINTS_EXPERIENCE,
NIVEAU_HJ, POINTS_COMPETENCE_HJ, BONUS_ATTAQUE_HJ, BONUS_DEFENSE_HJ,
CHANCE_CRITIQUE_HJ, MORAL_HJ, POINTS_ATTAQUE_SPECIALE_HJ} E_HEROSJOUEURS;

typedef enum {NUM_JOUEUR_UJ, NUM_UNITE_UJ, TYPE_UJ, SANTE_UJ, ATTAQUE_UJ,
DEFENSE_UJ, DEGATS_UJ, MOUVEMENTS_UJ, NOMBRE_UJ, NIVEAU_UJ, UNITE_ACTIVE} E_UNITESJOUEURS;

typedef enum { NUM_JOUEUR_UB, NUM_UNITE_UB, TYPE_UB, SANTE_RESTANTE_UB,
ATTAQUE_UB, DEFENSE_UB, DEGATS_UB, MOUVEMENTS_UB, NOMBRE_UNITES_UB, LIGNE_UB,
COLONNE_UB, FINTOUR_UB, SANTE_UNITAIRE_UB} E_UNITESBATAILLE;

void chargerFichierVersTableau(
  int nb_lignes,
  int nb_colonnes,
  int tab[nb_lignes][nb_colonnes],
  char *nomFichier){
      FILE *filePointer = fopen(nomFichier,"r");
        for(int i=0; i<nb_lignes; i++){
          for(int j=0; j<nb_colonnes; j++){
            fscanf(filePointer, "%d", &tab[i][j]);
          }
        }
      fclose(filePointer);
}//PASS

void sauvegarderTableauDansFichier(
  int nb_lignes,
  int nb_colonnes,
  int tab[nb_lignes][nb_colonnes],
  char *nomFichier){
    FILE *filePointer = fopen(nomFichier, "w");
      for(int i=0; i<nb_lignes; i++){
        for(int j=0; j<nb_colonnes; j++){
          (j + 1 != nb_colonnes) ? fprintf(filePointer, "%d ",tab[i][j]) : fprintf(filePointer, "%d",tab[i][j]);
        }
        (i + 1 != nb_lignes) ? fprintf(filePointer, "\n") : 0;
      }
      fclose(filePointer);
}//PASS

void chargerJeuComplet(
  int carte[NB_LIGNES_CARTE][NB_COLONNES_CARTE],
  int joueurs[NB_LIGNES_JOUEURS][NB_COLONNES_JOUEURS],
  int heros[NB_LIGNES_HEROS][NB_COLONNES_HEROS],
  int herosJoueurs[NB_LIGNES_HEROSJOUEURS][NB_COLONNES_HEROSJOUEURS],
  int unites[NB_LIGNES_UNITES][NB_COLONNES_UNITES],
  int unitesJoueurs[NB_LIGNES_UNITESJOUEURS][NB_COLONNES_UNITESJOUEURS] ){

    chargerFichierVersTableau(NB_LIGNES_JOUEURS,NB_COLONNES_JOUEURS,joueurs,"sauvegarde/joueurs_sauvegarde.txt");
    chargerFichierVersTableau(NB_LIGNES_HEROS,NB_COLONNES_HEROS,heros,"sauvegarde/heros_sauvegarde.txt");
    chargerFichierVersTableau(NB_LIGNES_UNITES,NB_COLONNES_UNITES,unites,"sauvegarde/unites_sauvegarde.txt");
    chargerFichierVersTableau(NB_LIGNES_HEROSJOUEURS,NB_COLONNES_HEROSJOUEURS,herosJoueurs,"sauvegarde/herosJoueurs_sauvegarde.txt");
    chargerFichierVersTableau(NB_LIGNES_UNITESJOUEURS,NB_COLONNES_UNITESJOUEURS,unitesJoueurs,"sauvegarde/unitesJoueurs_sauvegarde.txt");
    chargerFichierVersTableau(NB_LIGNES_CARTE,NB_COLONNES_CARTE,carte,"sauvegarde/carte_sauvegarde.txt");
}//PASS


void initialiserNouveauJeu(
  int carte[NB_LIGNES_CARTE][NB_COLONNES_CARTE],
  int joueurs[NB_LIGNES_JOUEURS][NB_COLONNES_JOUEURS],
  int heros[NB_LIGNES_HEROS][NB_COLONNES_HEROS],
  int herosJoueurs[NB_LIGNES_HEROSJOUEURS][NB_COLONNES_HEROSJOUEURS],
  int unites[NB_LIGNES_UNITES][NB_COLONNES_UNITES],
  int unitesJoueurs[NB_LIGNES_UNITESJOUEURS][NB_COLONNES_UNITESJOUEURS] ){

    chargerFichierVersTableau(NB_LIGNES_JOUEURS,NB_COLONNES_JOUEURS,joueurs,"original/joueurs_original.txt");
    chargerFichierVersTableau(NB_LIGNES_HEROS,NB_COLONNES_HEROS,heros,"original/heros_original.txt");
    chargerFichierVersTableau(NB_LIGNES_UNITES,NB_COLONNES_UNITES,unites,"original/unites_original.txt");
    chargerFichierVersTableau(NB_LIGNES_HEROSJOUEURS,NB_COLONNES_HEROSJOUEURS,herosJoueurs,"original/herosJoueurs_original.txt");
    chargerFichierVersTableau(NB_LIGNES_UNITESJOUEURS,NB_COLONNES_UNITESJOUEURS,unitesJoueurs,"original/unitesJoueurs_original.txt");
    chargerFichierVersTableau(NB_LIGNES_CARTE,NB_COLONNES_CARTE,carte,"original/carte_original.txt");
}//PASS

void sauvegarderJeuComplet(
  int carte[NB_LIGNES_CARTE][NB_COLONNES_CARTE],
  int joueurs[NB_LIGNES_JOUEURS][NB_COLONNES_JOUEURS],
  int heros[NB_LIGNES_HEROS][NB_COLONNES_HEROS],
  int herosJoueurs[NB_LIGNES_HEROSJOUEURS][NB_COLONNES_HEROSJOUEURS],
  int unites[NB_LIGNES_UNITES][NB_COLONNES_UNITES],
  int unitesJoueurs[NB_LIGNES_UNITESJOUEURS][NB_COLONNES_UNITESJOUEURS] ){

    sauvegarderTableauDansFichier(NB_LIGNES_JOUEURS,NB_COLONNES_JOUEURS,joueurs,"sauvegarde/joueurs_sauvegarde.txt");
    sauvegarderTableauDansFichier(NB_LIGNES_HEROS,NB_COLONNES_HEROS,heros,"sauvegarde/heros_sauvegarde.txt");
    sauvegarderTableauDansFichier(NB_LIGNES_UNITES,NB_COLONNES_UNITES,unites,"sauvegarde/unites_sauvegarde.txt");
    sauvegarderTableauDansFichier(NB_LIGNES_HEROSJOUEURS,NB_COLONNES_HEROSJOUEURS,herosJoueurs,"sauvegarde/herosJoueurs_sauvegarde.txt");
    sauvegarderTableauDansFichier(NB_LIGNES_UNITESJOUEURS,NB_COLONNES_UNITESJOUEURS,unitesJoueurs,"sauvegarde/unitesJoueurs_sauvegarde.txt");
    sauvegarderTableauDansFichier(NB_LIGNES_CARTE,NB_COLONNES_CARTE,carte,"sauvegarde/carte_sauvegarde.txt");
}//PASS

int chercherIndiceAvecUneCondition(
  int colonneConcernee,
  int valeur,
  int nb_lignes,
  int nb_colonnes,
  int tab[nb_lignes][nb_colonnes] ){
    int i=0;
    while(i<(nb_lignes-1) && tab[i][colonneConcernee] != valeur){
      i++;
    }
    if(tab[i][colonneConcernee] == valeur){
      return i;
    }
    return -1;
}//PASS

int chercherIndiceAvecDeuxConditions(
  int colonneConcernee1,
  int valeur1,
  int colonneConcernee2,
  int valeur2,
  int nb_lignes,
  int nb_colonnes,
  int tab[nb_lignes][nb_colonnes] ){
    int i=0;

    while(i<(nb_lignes-1) && (tab[i][colonneConcernee1] != valeur1 || tab[i][colonneConcernee2] != valeur2)){
      i++;
    }
    if(tab[i][colonneConcernee1] == valeur1 && tab[i][colonneConcernee2] == valeur2){
      return i;
    }
    return -1;
}//PASS

void success(SDL_Surface* ecran, char* text, int val){
  SDL_Surface *menu = NULL;
  SDL_Rect positionMenu;
  SDL_Event event;
  TTF_Font *font = NULL, *text_font = NULL, *small_font = NULL;
  TTF_Font *title_font= NULL;
  SDL_Surface *texte = NULL;
  SDL_Surface *montant = NULL;
  char value[20];
  int continuer = 1;

    ecran = SDL_SetVideoMode(400, 400, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    SDL_Init(SDL_INIT_VIDEO);
    //here condition
    if(text == "Un tresor a ete decouvert !"){
      menu = IMG_Load("images/or.png");
      sprintf(value,"%d Piece d\'Or",val);
    }else if(text == "Un Coffre d\'experience a ete decouvert !"){
      menu = IMG_Load("images/experience.png");
      sprintf(value,"%d Point",val);
    }else if(text == "la bataille va commencer !"){
      menu = IMG_Load("images/success.png");
      sprintf(value,"Attque ennemi %d",val);
    }else{
      menu = IMG_Load("images/success.png");
      sprintf(value,"Success !");
    }
    positionMenu.x = 0;
    positionMenu.y = 0;

    TTF_Init();
    text_font = TTF_OpenFont("Teko-Medium.ttf", 27);
    small_font = TTF_OpenFont("BlackOpsOne-Regular.ttf", 15);
    font = TTF_OpenFont("BlackOpsOne-Regular.ttf", 19);


    SDL_Color blanc = {255,255,255};
    SDL_Color jaune = {255,196,87};
    SDL_Color rouge = {255,0,0};
    SDL_Rect positionText;

    while(continuer){
      SDL_WaitEvent( & event);
      if(event.type == SDL_QUIT){
        continuer = 0;
      }else if(event.type == SDL_KEYDOWN){
        if(event.key.keysym.sym == SDLK_ESCAPE){
          continuer = 0;
        }else if(event.key.keysym.sym == SDLK_RETURN || event.key.keysym.sym == SDLK_KP_ENTER){
          continuer = 0;
        }
      }
      SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran -> format, 0, 0, 0));
      SDL_BlitSurface(menu, NULL, ecran, & positionMenu);

      if(font != 0){
        char* phrase = "Clicker sur entrer pour continuer...";
          texte = TTF_RenderText_Blended(text_font, text, jaune);
          positionText.y = 240;
          positionText.x = (400 - strlen(text)*8.6)/2;
          SDL_BlitSurface(texte, NULL, ecran, &positionText);

          montant = TTF_RenderText_Blended(small_font, value, rouge);
          positionText.y = 333;
          positionText.x = (400 - strlen(value)*10)/2;
          SDL_BlitSurface(montant, NULL, ecran, &positionText);

          texte = TTF_RenderText_Blended(font, phrase, blanc);
          positionText.y = 365;
          positionText.x = 20;
          SDL_BlitSurface(texte, NULL, ecran, &positionText);
      }
      SDL_Flip(ecran);
    }
    TTF_CloseFont(font);
    TTF_CloseFont(text_font);
    SDL_FreeSurface(texte);
    SDL_FreeSurface(menu);
}

int alert(SDL_Surface* ecran,char* text){
  SDL_Surface *menu = NULL;
  SDL_Rect positionMenu;
  SDL_Event event;
  TTF_Font *font = NULL, *text_font = NULL;
  TTF_Font *title_font= NULL;
  SDL_Surface *texte = NULL;
  int continuer = 1;
  int indiceOption = 0;

    ecran = SDL_SetVideoMode(400, 400, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    SDL_Init(SDL_INIT_VIDEO);
    menu = IMG_Load("images/alert.png");
    positionMenu.x = 0;
    positionMenu.y = 0;

    TTF_Init();
    text_font = TTF_OpenFont("Teko-Medium.ttf", 35);
    font = TTF_OpenFont("BlackOpsOne-Regular.ttf", 35);

    SDL_Color blanc = {255,255,255};
    SDL_Color jaune = {255,196,87};
    SDL_Color rouge = {255,0,0};
    SDL_Color vert = {34,139,24};
    SDL_Rect positionText;

    while (continuer){
      SDL_WaitEvent( & event);
      if(event.type == SDL_QUIT){
        continuer = 0;
      }else if(event.type == SDL_KEYDOWN){
        if(event.key.keysym.sym == SDLK_ESCAPE){
          continuer = 0;
        }else if(event.key.keysym.sym == SDLK_RETURN || event.key.keysym.sym == SDLK_KP_ENTER){
          if(indiceOption == 0){
            return 1;
          }else if(indiceOption == 1){
            return 0;
          }
        }else if(event.key.keysym.sym == SDLK_LEFT){
          if(indiceOption > 0){
            indiceOption--;
          }
        }else if(event.key.keysym.sym == SDLK_RIGHT){
          if(indiceOption < 1){
            indiceOption++;
          }
        }
      }
      SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran -> format, 0, 0, 0));
      SDL_BlitSurface(menu, NULL, ecran, & positionMenu);

      if(font != 0){
        char * phrase[2];
          texte = TTF_RenderText_Blended(text_font, text, jaune);
          positionText.y = 210;
          positionText.x = (400 - strlen(text)*11)/2;
          SDL_BlitSurface(texte, NULL, ecran, & positionText);

          phrase[0] = "OUI";
          phrase[1] = "NON";
        int x_depart = 90;
        SDL_Color couleur_a_utiliser;
        for (int i = 0; i < 2; i++){
          if(i == indiceOption){
            if(i == 0){
              couleur_a_utiliser = vert;
            }else{
              couleur_a_utiliser = rouge;
            }
            positionText.y = 300;
          }else{
            couleur_a_utiliser = blanc;
            positionText.y = 305;
          }
          texte = TTF_RenderText_Blended(font, phrase[i], couleur_a_utiliser);
          positionText.x = x_depart + i * 150;
          SDL_BlitSurface(texte, NULL, ecran, & positionText);
        }
      }
      SDL_Flip(ecran);
    }
    TTF_CloseFont(font);
    TTF_CloseFont(text_font);
    SDL_FreeSurface(texte);
    SDL_FreeSurface(menu);
}

void info(SDL_Surface* ecran, char* text, char* img){
  SDL_Surface *menu = NULL;
  SDL_Rect positionMenu;
  SDL_Event event;
  TTF_Font *font = NULL, *text_font = NULL;
  TTF_Font *title_font= NULL;
  SDL_Surface *texte = NULL;
  int continuer = 1;

    ecran = SDL_SetVideoMode(400, 400, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    SDL_Init(SDL_INIT_VIDEO);
    menu = IMG_Load(img);
    positionMenu.x = 0;
    positionMenu.y = 0;

    TTF_Init();
    if(strlen(text) >= 37){
      text_font = TTF_OpenFont("Teko-Medium.ttf", 20);
    }else{
      text_font = TTF_OpenFont("Teko-Medium.ttf", 27);
    }
    font = TTF_OpenFont("BlackOpsOne-Regular.ttf", 19);

    SDL_Color blanc = {255,255,255};
    SDL_Color jaune = {255,196,87};
    SDL_Rect positionText;

    while (continuer){
      SDL_WaitEvent( & event);
      if(event.type == SDL_QUIT){
        continuer = 0;
      }else if(event.type == SDL_KEYDOWN){
        if(event.key.keysym.sym == SDLK_ESCAPE){
          continuer = 0;
        }else if(event.key.keysym.sym == SDLK_RETURN || event.key.keysym.sym == SDLK_KP_ENTER){
          continuer = 0;
        }
      }
      SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran -> format, 0, 0, 0));
      SDL_BlitSurface(menu, NULL, ecran, & positionMenu);

      if(font != 0){
        char* phrase = "Clicker sur entrer pour continuer...";
          texte = TTF_RenderText_Blended(text_font, text, jaune);
          positionText.y = 230;
          if(strlen(text) >= 37){
            positionText.x = (400 - strlen(text)*6.4)/2;
          }else{
            positionText.x = (400 - strlen(text)*8.6)/2;
          }
          SDL_BlitSurface(texte, NULL, ecran, & positionText);
          positionText.y = 325;
          positionText.x = 20;
          texte = TTF_RenderText_Blended(font, phrase, blanc);
          SDL_BlitSurface(texte, NULL, ecran, & positionText);
      }
      SDL_Flip(ecran);
    }
    TTF_CloseFont(font);
    TTF_CloseFont(text_font);
    SDL_FreeSurface(texte);
    SDL_FreeSurface(menu);
}

void ouvrir_caise(SDL_Surface* ecran, char* text,int amount, char* img){
  SDL_Surface *menu = NULL;
  SDL_Rect positionMenu;
  SDL_Event event;
  TTF_Font *font = NULL, *text_font = NULL;
  TTF_Font *title_font= NULL;
  SDL_Surface *texte = NULL;
  int continuer = 1;


    SDL_Init(SDL_INIT_VIDEO);
    menu = IMG_Load(img);
    positionMenu.x = 0;
    positionMenu.y = 0;

    TTF_Init();
    text_font = TTF_OpenFont("Teko-Medium.ttf", 27);
    font = TTF_OpenFont("BlackOpsOne-Regular.ttf", 19);

    SDL_Color blanc = {255,255,255};
    SDL_Color jaune = {255,196,87};
    SDL_Rect positionText;

    while (continuer){
      SDL_WaitEvent( & event);
      if(event.type == SDL_QUIT){
        continuer = 0;
      }else if(event.type == SDL_KEYDOWN){
        if(event.key.keysym.sym == SDLK_ESCAPE){
          continuer = 0;
        }else if(event.key.keysym.sym == SDLK_RETURN || event.key.keysym.sym == SDLK_KP_ENTER){
          continuer = 0;
        }
      }
      SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran -> format, 0, 0, 0));
      SDL_BlitSurface(menu, NULL, ecran, & positionMenu);

      if(font != 0){
        char* phrase = "Clicker sur entrer pour continuer...";
          texte = TTF_RenderText_Blended(text_font, text, jaune);
          positionText.y = 230;
          positionText.x = (400 - strlen(text)*8.6)/2;
          SDL_BlitSurface(texte, NULL, ecran, & positionText);
          positionText.y = 325;
          positionText.x = 20;
          texte = TTF_RenderText_Blended(font, phrase, blanc);
          SDL_BlitSurface(texte, NULL, ecran, & positionText);
      }
      SDL_Flip(ecran);
    }
    TTF_CloseFont(font);
    TTF_CloseFont(text_font);
    SDL_FreeSurface(texte);
    SDL_FreeSurface(menu);
}




void executerAction(//here
  SDL_Surface* ecran,
  int carte[NB_LIGNES_CARTE][NB_COLONNES_CARTE],
  int joueurs[NB_LIGNES_JOUEURS][NB_COLONNES_JOUEURS],
  int herosJoueurs[NB_LIGNES_HEROSJOUEURS][NB_COLONNES_HEROSJOUEURS],
  int unitesJoueurs[NB_LIGNES_UNITESJOUEURS][NB_COLONNES_UNITESJOUEURS] ){
    int choix = 0, ind_J_humain = 0, unite = 0, dizaine = 0, centaine = 0, num_lig_J = 0, num_col_J = 0, ind_H_Principal = 0;

    ind_J_humain = chercherIndiceAvecUneCondition(NUM_JOUEUR_J,JOUEUR_HUMAIN,NB_LIGNES_JOUEURS,NB_COLONNES_JOUEURS,joueurs);
    num_lig_J = joueurs[ind_J_humain][LIGNE_J];
    num_col_J = joueurs[ind_J_humain][COLONNE_J];
    dizaine = (carte[num_lig_J][num_col_J]/10)%10;
    centaine = (carte[num_lig_J][num_col_J]/100);

    if((dizaine)==0){
      info(ecran, "Case Vide !", "images/alert.png");
    }else if((dizaine)==1){
      char* amount;
      sprintf(amount, "Vouz voulez attacker l\'ennemi %d ?", centaine);
      if(alert(ecran, amount)){
        success(ecran,"la bataille va commencer !",centaine);
        // deroulerBataille(JOUEUR_HUMAIN,centaine,joueurs,herosJoueurs,unitesJoueurs);
      }
    }else if((dizaine)==2){
      if(centaine != 0){
        if(alert(ecran,"Vouz voulez ouvrir cette coffre ?")){
          success(ecran,"Un tresor a ete decouvert !", centaine);
          joueurs[ind_J_humain][OR] += centaine;
          carte[num_lig_J][num_col_J] %= 100;
        }
      }else{
        info(ecran,"Coffre de tresor deja ouvert !", "images/alert.png");
      }
    }else if((dizaine)==3){
      if(centaine != 0){
        if(alert(ecran,"Vouz voulez ouvrir cette caisse ?")){
          ind_H_Principal = chercherIndiceAvecDeuxConditions(NUM_JOUEUR_HJ,JOUEUR_HUMAIN,HERO_PRINCIPAL,1,NB_LIGNES_HEROSJOUEURS,NB_COLONNES_HEROSJOUEURS,herosJoueurs);
          if(ind_H_Principal != -1){
            success(ecran,"Un Coffre d\'experience a ete decouvert !", centaine);
            herosJoueurs[ind_H_Principal][POINTS_EXPERIENCE] += centaine;
            carte[num_lig_J][num_col_J] %= 100;
          }else{
            info(ecran,"Vous n\'avez pas un hero principal","images/alert.png");
          }
        }
      }else{
        info(ecran,"Caisse d\'experience deja ouvert !","images/alert.png");
      }
    }
}//PASS

void degagerNuages(
  int lig,
  int col,
  int carte[NB_LIGNES_CARTE][NB_COLONNES_CARTE] ){
    int lig_min = lig-2, lig_max = lig+2, col_min = col-2, col_max = col+2, contenu_case = 0;

    for(int i=lig_min; i<=lig_max; i++){
      for(int j=col_min; j<=col_max; j++){
        contenu_case = carte[i][j] % 10;
        if(contenu_case == 1){
          if((i >= 0) && (i <= NB_LIGNES_CARTE-1) && (j >= 0) && (j <= NB_COLONNES_CARTE-1)){
               carte[i][j] /= 10;
               carte[i][j] *= 10;
          }
        }
      }
    }
}//PASS

void avancer(
  char action,

  int carte[NB_LIGNES_CARTE][NB_COLONNES_CARTE],
  int joueurs[NB_LIGNES_JOUEURS][NB_COLONNES_JOUEURS] ){
    int ind_J_humain = 0, ans_lig_J = 0, ans_col_J = 0, existe_J = 0, nouv_lig_J = 0, nouv_col_J = 0, etat_change = 0;

    ind_J_humain = chercherIndiceAvecUneCondition(NUM_JOUEUR_J,JOUEUR_HUMAIN,NB_LIGNES_JOUEURS,NB_COLONNES_JOUEURS,joueurs);
    ans_lig_J = joueurs[ind_J_humain][LIGNE_J];
    ans_col_J = joueurs[ind_J_humain][COLONNE_J];
    existe_J = carte[ans_lig_J][ans_col_J] % 10 ;

    if((existe_J == 2)){
      if(action == 'h' && ans_lig_J > 0){
        nouv_lig_J = --joueurs[ind_J_humain][LIGNE_J];
        nouv_col_J = ans_col_J;
        etat_change = 1;
      }
      else if(action == 'b' && ans_lig_J < NB_LIGNES_CARTE-1){
        nouv_lig_J = ++joueurs[ind_J_humain][LIGNE_J];
        nouv_col_J = ans_col_J;
        etat_change = 1;
      }
      else if(action == 'g' && ans_col_J > 0){
        nouv_col_J = --joueurs[ind_J_humain][COLONNE_J];
        nouv_lig_J = ans_lig_J;
        etat_change = 1;
      }
      else if(action == 'd' && ans_col_J < NB_COLONNES_CARTE-1){
        nouv_col_J = ++joueurs[ind_J_humain][COLONNE_J];
        nouv_lig_J = ans_lig_J;
        etat_change = 1;
      }
      if(etat_change){
        degagerNuages(nouv_lig_J,nouv_col_J,carte);
        carte[ans_lig_J][ans_col_J] /= 10;
        carte[ans_lig_J][ans_col_J] *= 10;
        carte[nouv_lig_J][nouv_col_J] /= 10;
        carte[nouv_lig_J][nouv_col_J] *= 10;
        carte[nouv_lig_J][nouv_col_J] += 2;
      }
    }
}//PASS

void lancerMenuAchatHeros(//here
  SDL_Surface* ecran,
  int numJoueur,
  int joueurs[NB_LIGNES_JOUEURS][NB_COLONNES_JOUEURS],
  int heros[NB_LIGNES_HEROS][NB_COLONNES_HEROS],
  int herosJoueurs[NB_LIGNES_HEROSJOUEURS][NB_COLONNES_HEROSJOUEURS] ){
    int choix=1, achat=0, confirm=0, ind_case_vide=0, ind_J_humain=0, existe_humain=0, ind_choix=0, choix_deja_achete=0, solde=0, prix=0;
    char text[100];

    SDL_Surface *menu = NULL, *uncommon = NULL, *hero = NULL;
    SDL_Rect positionMenu;
    SDL_Event event;
    TTF_Font *font = NULL;
    TTF_Font *title_font= NULL;
    SDL_Surface *texte = NULL;
    int continuer = 1;
    int indiceOption = 1;

        SDL_Init(SDL_INIT_VIDEO);
        menu = IMG_Load("images/background2.png");
        uncommon = IMG_Load("images/uncommon.png");
        or = IMG_Load("images/gold.png");
        positionMenu.x = 0;
        positionMenu.y = 0;

        TTF_Init();
        title_font = TTF_OpenFont("BlackOpsOne-Regular.ttf", 35);
        font = TTF_OpenFont("BlackOpsOne-Regular.ttf", 20);

        while (continuer){
          ind_choix = chercherIndiceAvecUneCondition(NUM_HERO_H,choix,NB_LIGNES_HEROS,NB_COLONNES_HEROS,heros);
          choix_deja_achete = chercherIndiceAvecDeuxConditions(NUM_JOUEUR_HJ,numJoueur,NUM_HERO_HJ,choix,NB_LIGNES_HEROSJOUEURS,NB_COLONNES_HEROSJOUEURS,herosJoueurs);
          ind_J_humain = chercherIndiceAvecUneCondition(NUM_JOUEUR_J,numJoueur,NB_LIGNES_JOUEURS,NB_COLONNES_JOUEURS,joueurs);
          ind_case_vide = chercherIndiceAvecUneCondition(NUM_JOUEUR_HJ,-1,NB_LIGNES_HEROSJOUEURS,NB_COLONNES_HEROSJOUEURS,herosJoueurs);
          prix = heros[ind_choix][PRIX_H];
          solde = joueurs[ind_J_humain][OR];

          SDL_WaitEvent( & event);
          if(event.type == SDL_QUIT){
            continuer = 0;
          }else if(event.type == SDL_KEYDOWN){
            if(event.key.keysym.sym == SDLK_ESCAPE){
              continuer = 0;
            }else if(event.key.keysym.sym == SDLK_RETURN || event.key.keysym.sym == SDLK_KP_ENTER){
              for(int i=0; i<=NB_LIGNES_JOUEURS; i++){
                if(indiceOption == i){
                  if(ind_choix == -1){
                    sprintf(text, "l\'hero numero %d est non disponible !",choix);
                    info(ecran,text, "images/alert.png");
                  }else if(choix_deja_achete != -1){
                    info(ecran,"Achat annule, hero deja achete !", "images/alert.png");
                  }else if(ind_case_vide == -1){
                    info(ecran,"Vous avez atteint le nombre d\'achat maximal", "images/alert.png");
                  }else if(solde < prix){
                    info(ecran,"Achat annule, solde insuffisant pour acheter cet hero !", "images/alert.png");
                  }else if(solde >= prix){
                    sprintf(text, "Confirmer l'achat de l\'hero %d ?",choix);
                    confirm = alert(ecran,text);
                    if(confirm == 1){
                      if(ind_case_vide != -1){
                        herosJoueurs[ind_case_vide][NUM_JOUEUR_HJ] = 0;
                        herosJoueurs[ind_case_vide][NUM_HERO_HJ] = choix;
                        herosJoueurs[ind_case_vide][HERO_PRINCIPAL] = 0;
                        herosJoueurs[ind_case_vide][POINTS_EXPERIENCE] = 0;
                        herosJoueurs[ind_case_vide][NIVEAU_HJ] = 0;
                        herosJoueurs[ind_case_vide][POINTS_COMPETENCE_HJ] = 0;
                        herosJoueurs[ind_case_vide][BONUS_ATTAQUE_HJ] = heros[ind_choix][BONUS_ATTAQUE_H];
                        herosJoueurs[ind_case_vide][BONUS_DEFENSE_HJ] = heros[ind_choix][BONUS_DEFENSE_H];
                        herosJoueurs[ind_case_vide][CHANCE_CRITIQUE_HJ] = heros[ind_choix][CHANCE_CRITIQUE_H];
                        herosJoueurs[ind_case_vide][MORAL_HJ] = heros[ind_choix][MORAL_H];
                        herosJoueurs[ind_case_vide][POINTS_ATTAQUE_SPECIALE_HJ] = heros[ind_choix][POINTS_ATTAQUE_SPECIALE_H];
                        joueurs[ind_J_humain][OR] -= prix;
                        success(ecran,"Achat effectue avec succes !",0);
                      }
                    }
                  }
                  ecran = SDL_SetVideoMode(520, 400, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
                }
              }
            }else if(event.key.keysym.sym == SDLK_LEFT){
              if(indiceOption > 1){
                indiceOption--;
              }
            }else if(event.key.keysym.sym == SDLK_RIGHT){
              indiceOption++;
              choix = indiceOption;
              ind_choix = chercherIndiceAvecUneCondition(NUM_HERO_H,choix,NB_LIGNES_HEROS,NB_COLONNES_HEROS,heros);
              if(ind_choix == -1){
                indiceOption--;
              }
            }
            choix = indiceOption;
          }
          SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran -> format, 0, 0, 0));
          positionMenu.y = 0;
          positionMenu.x = 0;
          SDL_BlitSurface(menu, NULL, ecran, & positionMenu);


          SDL_Color blanc = {255,255,255};
          SDL_Color green = {0, 255, 0};
          SDL_Color dore = {255,140,0};
          SDL_Rect positionText;

          positionMenu.y = 10;
          positionMenu.x = 8;
          SDL_BlitSurface(uncommon, NULL, ecran, &positionMenu);

          if(font != 0){
            char text[10];

            int num_hero = chercherIndiceAvecUneCondition(NUM_HERO_H,indiceOption,NB_LIGNES_HEROS,NB_COLONNES_HEROS,heros);

            positionText.y = 0;
            positionText.x = 370;
            SDL_BlitSurface(or, NULL, ecran, & positionText);

            sprintf(text, "%*d",4, joueurs[JOUEUR_HUMAIN][OR]);
            texte = TTF_RenderText_Blended(font, text, dore);
            positionText.y = 19;
            positionText.x = 410;
            SDL_BlitSurface(texte, NULL, ecran, &positionText);

            sprintf(text, "Hero %d", heros[num_hero][NUM_HERO_H]);
            texte = TTF_RenderText_0Blended(title_font, text, blanc);
            positionText.y = 140;
            positionText.x = 75;
            SDL_BlitSurface(texte, NULL, ecran, & positionText);

            for(int i=0; i<5; i++){
              sprintf(text, "%d", heros[num_hero][i+1]);
              texte = TTF_RenderText_Blended(font, text, blanc);
              positionText.y = 123 + i*32;
              positionText.x = 420;
              SDL_BlitSurface(texte, NULL, ecran, & positionText);
            }

            sprintf(text, "Prix %d", heros[num_hero][PRIX_H]);
            texte = TTF_RenderText_Blended(title_font, text, green);
            positionText.y = 283;
            positionText.x = 255;
            SDL_BlitSurface(texte, NULL, ecran, & positionText);

            sprintf(text, "images/heros/hero%d.png", indiceOption);
            hero = IMG_Load(text);
            positionText.y = 10;
            positionText.x = 80;
            SDL_BlitSurface(hero, NULL, ecran, & positionText);



            // NUM_HERO_H, BONUS_ATTAQUE_H, BONUS_DEFENSE_H,
            // CHANCE_CRITIQUE_H, MORAL_H, POINTS_ATTAQUE_SPECIALE_H, PRIX_H



            // for (int i = 0; i < 2; i++){
            //   if(i == indiceOption){
            //     couleur_a_utiliser = blanc;
            //     positionText.x = 80;
            //     positionText.x = 75;
            //   }
            //
            //   texte = TTF_RenderText_Blended(font, phrase[i], couleur_a_utiliser);
            //   positionText.y = y_depart + i * 30;
            //   SDL_BlitSurface(texte, NULL, ecran, & positionText);
            // }

          }


          SDL_Flip(ecran);
    }
    SDL_FreeSurface(hero);
    SDL_FreeSurface(uncommon);
    SDL_FreeSurface(texte);
    TTF_CloseFont(font);
    SDL_FreeSurface(menu);
  }//PASS


void deroulerJeu(
  SDL_Surface* ecran,
  int carte[NB_LIGNES_CARTE][NB_COLONNES_CARTE],
  int joueurs[NB_LIGNES_JOUEURS][NB_COLONNES_JOUEURS],
  int heros[NB_LIGNES_HEROS][NB_COLONNES_HEROS],
  int herosJoueurs[NB_LIGNES_HEROSJOUEURS][NB_COLONNES_HEROSJOUEURS],
  int unites[NB_LIGNES_UNITES][NB_COLONNES_UNITES],
  int unitesJoueurs[NB_LIGNES_UNITESJOUEURS][NB_COLONNES_UNITESJOUEURS] ){
    SDL_Surface *joueur = NULL, *terrain_invisible = NULL, *terrain_visible = NULL,
      *coffre_or = NULL, *coffre_or_overt = NULL, *caisse_xp = NULL, *ennemi1 = NULL,
      *ennemi2 = NULL, *ennemi3 = NULL, *ennemi4 = NULL, *materials[14], *rock = NULL, *rock2 = NULL, *caisse_xp_overt = NULL, *materials_frame = NULL;
    TTF_Font *font = NULL;
    SDL_Rect position, positionJoueur;
    SDL_Event event;
    char text[10];
    int continuer = 1;

    int unite = 0, dizaine = 0, centaine = 0, milliers = 0;
    joueur = IMG_Load("images/joueur.png");
    terrain_invisible = IMG_Load("images/terrain_invisible.png");
    terrain_visible = IMG_Load("images/terrain_visible.png");
    coffre_or = IMG_Load("images/coffre_or.png");
    coffre_or_overt = IMG_Load("images/coffre_or_overt.png");
    caisse_xp = IMG_Load("images/caisse_xp.png");
    caisse_xp_overt = IMG_Load("images/xp.png");
    ennemi1 = IMG_Load("images/ennemi1.png");
    ennemi2 = IMG_Load("images/ennemi2.png");
    ennemi3 = IMG_Load("images/ennemi3.png");
    ennemi4 = IMG_Load("images/ennemi4.png");
    materials_frame = IMG_Load("images/materials.png");
    rock = IMG_Load("images/rock.jpg");
    rock2 = IMG_Load("images/rock2.jpg");

    ecran = SDL_SetVideoMode(520, 400, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    SDL_EnableKeyRepeat(100, 100);
    while(continuer){
      SDL_WaitEvent(&event);
      if(event.type == SDL_QUIT){
        continuer = 0;
      }else if(event.type == SDL_KEYDOWN){
        if(event.key.keysym.sym == SDLK_ESCAPE){
          continuer = 0;
        }else if(event.key.keysym.sym == SDLK_UP){
          avancer('h',carte,joueurs);
        }else if(event.key.keysym.sym == SDLK_DOWN){
          avancer('b',carte,joueurs);
        }else if(event.key.keysym.sym == SDLK_RIGHT){
          avancer('d',carte,joueurs);
          joueur = IMG_Load("images/joueur.png");
        }else if(event.key.keysym.sym == SDLK_LEFT){
          avancer('g',carte,joueurs);
          joueur = IMG_Load("images/joueur_g.png");
        }else if(event.key.keysym.sym == SDLK_RETURN){
          ecran = SDL_SetVideoMode(400, 400, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
          executerAction(ecran,carte,joueurs,herosJoueurs,unitesJoueurs);
          ecran = SDL_SetVideoMode(520, 400, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
          sauvegarderJeuComplet(carte,joueurs,heros,herosJoueurs,unites,unitesJoueurs);
        }
      }

      SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 89, 158, 42));
      for (int i = 0 ; i < NB_LIGNES_CARTE; i++){
          for (int j = 0 ; j < NB_COLONNES_CARTE + 3; j++){//here
              position.y = i * TAILLE_BLOC;
              position.x = j * TAILLE_BLOC;
              unite = carte[i][j]%10;
              dizaine = (carte[i][j]/10)%10;
              centaine = carte[i][j]/100;
              if(i >= 8 && j >= NB_COLONNES_CARTE){
                SDL_BlitSurface(rock2, NULL, ecran, &position);
              }else if(j >= NB_COLONNES_CARTE){
                SDL_BlitSurface(rock, NULL, ecran, &position);
              }else{
                if(unite == 0){
                  SDL_BlitSurface(terrain_visible, NULL, ecran, &position);
                  if(dizaine == 1){
                    if(centaine == 1){
                      SDL_BlitSurface(ennemi1, NULL, ecran, &position);
                    }else if(centaine == 2){
                      SDL_BlitSurface(ennemi2, NULL, ecran, &position);
                    }else if(centaine == 3){
                      SDL_BlitSurface(ennemi3, NULL, ecran, &position);
                    }else if(centaine == 4){
                      SDL_BlitSurface(ennemi4, NULL, ecran, &position);
                    }
                  }else if(dizaine == 2){
                    if(centaine != 0){
                      SDL_BlitSurface(coffre_or, NULL, ecran, &position);
                    }else{
                      SDL_BlitSurface(coffre_or_overt, NULL, ecran, &position);
                    }
                  }else if(dizaine == 3){
                    if(centaine != 0){
                      SDL_BlitSurface(caisse_xp, NULL, ecran, &position);
                    }else{
                      SDL_BlitSurface(caisse_xp_overt, NULL, ecran, &position);
                    }
                  }
                }else if(unite == 1){
                  SDL_BlitSurface(terrain_invisible, NULL, ecran, &position);
                }else if(unite == 2){
                  SDL_BlitSurface(terrain_visible, NULL, ecran, &position);
                  if(centaine == 1){
                    SDL_BlitSurface(ennemi1, NULL, ecran, &position);
                  }else if(centaine == 2){
                    SDL_BlitSurface(ennemi2, NULL, ecran, &position);
                  }else if(centaine == 3){
                    SDL_BlitSurface(ennemi3, NULL, ecran, &position);
                  }else if(centaine == 4){
                    SDL_BlitSurface(ennemi4, NULL, ecran, &position);
                  }else if(dizaine == 2){
                    SDL_BlitSurface(coffre_or_overt, NULL, ecran, &position);
                  }else if(dizaine == 3){
                    SDL_BlitSurface(caisse_xp_overt, NULL, ecran, &position);
                  }
                  SDL_BlitSurface(joueur, NULL, ecran, &position);
                }
              }
          }
      }
      position.y = 0;
      position.x = NB_COLONNES_CARTE * TAILLE_BLOC;
      SDL_BlitSurface(materials_frame, NULL, ecran, &position);
      {
        TTF_Init();
        font = TTF_OpenFont("BlackOpsOne-Regular.ttf", 15.5);
        SDL_Color blanc = {255,255,255};
        SDL_Color gold = {255, 140, 0};
        SDL_Color rouge = {255, 0, 0};
        SDL_Color green = {0, 255, 0};
        SDL_Rect positionText;

        sprintf(text,"%*d",6,joueurs[JOUEUR_HUMAIN][OR]);
        materials[13] = TTF_RenderText_Blended(font, text, gold);
        position.y = 21.5;
        position.x = NB_COLONNES_CARTE * TAILLE_BLOC + 45;
        SDL_BlitSurface(materials[13], NULL, ecran, &position);

        for(int i=0; i<8; i++){
          sprintf(text,"%*d",6,joueurs[JOUEUR_HUMAIN][i+4]);
          materials[i] = TTF_RenderText_Blended(font, text, blanc);
          position.y = 64.5 + i*31.5;
          position.x = NB_COLONNES_CARTE * TAILLE_BLOC + 45;
          SDL_BlitSurface(materials[i], NULL, ecran, &position);
        }

        font = TTF_OpenFont("BlackOpsOne-Regular.ttf", 13.5);

        sprintf(text, "U.Tuees: %*d",10,joueurs[JOUEUR_HUMAIN][NB_UNITES_TUEES]);
        materials[12] = TTF_RenderText_Blended(font, text, green);
        position.y = 325;
        position.x = NB_COLONNES_CARTE * TAILLE_BLOC + 10;
        SDL_BlitSurface(materials[12], NULL, ecran, &position);


        sprintf(text, "B.Gagnes: %*d",7,joueurs[JOUEUR_HUMAIN][NB_BATAILLE_GAGNES]);
        materials[12] = TTF_RenderText_Blended(font, text, green);
        position.y = 343;
        position.x = NB_COLONNES_CARTE * TAILLE_BLOC + 10;
        SDL_BlitSurface(materials[12], NULL, ecran, &position);

        sprintf(text, "U.Perdues: %*d",4,joueurs[JOUEUR_HUMAIN][NB_UNITES_PERDUES]);
        materials[12] = TTF_RenderText_Blended(font, text, rouge);
        position.y = 361;
        position.x = NB_COLONNES_CARTE * TAILLE_BLOC + 10;
        SDL_BlitSurface(materials[12], NULL, ecran, &position);

        sprintf(text, "B.Perdues: %*d",4,joueurs[JOUEUR_HUMAIN][NB_BATAILLE_PERDUES]);
        materials[12] = TTF_RenderText_Blended(font, text, rouge);
        position.y = 379;
        position.x = NB_COLONNES_CARTE * TAILLE_BLOC + 10;
        SDL_BlitSurface(materials[12], NULL, ecran, &position);
      }
      SDL_Flip(ecran);
    }

    SDL_EnableKeyRepeat(0, 0);
    SDL_FreeSurface(materials_frame);
    SDL_FreeSurface(joueur);
    SDL_FreeSurface(caisse_xp_overt);
    SDL_FreeSurface(coffre_or_overt);
    SDL_FreeSurface(terrain_visible);
    SDL_FreeSurface(terrain_invisible);
    SDL_FreeSurface(coffre_or);
    SDL_FreeSurface(caisse_xp);
    SDL_FreeSurface(rock);
    SDL_FreeSurface(ennemi1);
    SDL_FreeSurface(ennemi2);
    SDL_FreeSurface(ennemi3);
    SDL_FreeSurface(ennemi4);
}

void lancerMenuCampement(
  SDL_Surface* ecran,
  int joueurs[NB_LIGNES_JOUEURS][NB_COLONNES_JOUEURS],
  int herosJoueurs[NB_LIGNES_HEROSJOUEURS][NB_COLONNES_HEROSJOUEURS],
  int unitesJoueurs[NB_LIGNES_UNITESJOUEURS][NB_COLONNES_UNITESJOUEURS] ){

    SDL_Surface *menu = NULL;
    SDL_Rect positionMenu;
    SDL_Event event;
    TTF_Font *font = NULL;
    TTF_Font *title_font= NULL;
    SDL_Surface *texte = NULL;
    int continuer = 1;
    int indiceOption = 0;

    SDL_Init(SDL_INIT_VIDEO);


    menu = IMG_Load("images/background.jpg");
    positionMenu.x = 0;
    positionMenu.y = 0;

    TTF_Init();
    title_font = TTF_OpenFont("BlackOpsOne-Regular.ttf", 35);
    font = TTF_OpenFont("PressStart2P-Regular.ttf", 15);


    SDL_Color blanc = {255,255,255};
    SDL_Color bleu = {130, 236, 251};
    SDL_Rect positionText;

    while (continuer){
      SDL_WaitEvent( & event);
      if(event.type == SDL_QUIT){
        continuer = 0;
      }else if(event.type == SDL_KEYDOWN){
        if(event.key.keysym.sym == SDLK_ESCAPE){
          continuer = 0;
        }else if(event.key.keysym.sym == SDLK_RETURN || event.key.keysym.sym == SDLK_KP_ENTER){
          if(indiceOption == 0){

          }else if(indiceOption == 1){

          }
        }else if(event.key.keysym.sym == SDLK_UP){
          if(indiceOption > 0){
            indiceOption--;
          }
        }else if(event.key.keysym.sym == SDLK_DOWN){
          if(indiceOption < 1){
            indiceOption++;
          }
        }
      }
      SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran -> format, 0, 0, 0));
      SDL_BlitSurface(menu, NULL, ecran, & positionMenu);

      if(font != 0){
        char * phrase[2];
        texte = TTF_RenderText_Blended(title_font, "Campement", bleu);
        positionText.y = 100;
        positionText.x = 90;
        SDL_BlitSurface(texte, NULL, ecran, & positionText);
        phrase[0] = "Configuration Heros";
        phrase[1] = "Configuration Unites";
        int y_depart = 210;
        SDL_Color couleur_a_utiliser;
        for(int i = 0; i < 2; i++){
          if(i == indiceOption){
            couleur_a_utiliser = bleu;
            positionText.x = 70;
          }else{
            couleur_a_utiliser = blanc;
            positionText.x = 65;
          }
          texte = TTF_RenderText_Blended(font, phrase[i], couleur_a_utiliser);
          positionText.y = y_depart + i * 30;
          SDL_BlitSurface(texte, NULL, ecran, & positionText);
        }
      }
      SDL_Flip(ecran);
    }
    SDL_FreeSurface(texte);
    TTF_CloseFont(font);
    SDL_FreeSurface(menu);
  }

void lancerMenuMagasin(
  SDL_Surface* ecran,
  int joueurs[NB_LIGNES_JOUEURS][NB_COLONNES_JOUEURS],
  int heros[NB_LIGNES_HEROS][NB_COLONNES_HEROS],
  int herosJoueurs[NB_LIGNES_HEROSJOUEURS][NB_COLONNES_HEROSJOUEURS],
  int unites[NB_LIGNES_UNITES][NB_COLONNES_UNITES],
  int unitesJoueurs[NB_LIGNES_UNITESJOUEURS][NB_COLONNES_UNITESJOUEURS]){

    SDL_Surface *menu = NULL;
    SDL_Rect positionMenu;
    SDL_Event event;
    TTF_Font *font = NULL;
    TTF_Font *title_font= NULL;
    SDL_Surface *texte = NULL;
    int continuer = 1;
    int indiceOption = 0;

      SDL_Init(SDL_INIT_VIDEO);
      menu = IMG_Load("images/background.jpg");
      positionMenu.x = 0;
      positionMenu.y = 0;

      TTF_Init();
      title_font = TTF_OpenFont("BlackOpsOne-Regular.ttf", 35);
      font = TTF_OpenFont("PressStart2P-Regular.ttf", 15);
      SDL_Color blanc = {255,255,255};
      SDL_Color bleu = {130, 236, 251};
      SDL_Rect positionText;

      while (continuer){
        SDL_WaitEvent( & event);
        if(event.type == SDL_QUIT){
          continuer = 0;
        }else if(event.type == SDL_KEYDOWN){
          if(event.key.keysym.sym == SDLK_ESCAPE){
            continuer = 0;
          }else if(event.key.keysym.sym == SDLK_RETURN || event.key.keysym.sym == SDLK_KP_ENTER){
            if(indiceOption == 0){
              ecran = SDL_SetVideoMode(520, 400, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
              lancerMenuAchatHeros(ecran,JOUEUR_HUMAIN,joueurs,heros,herosJoueurs);
              ecran = SDL_SetVideoMode(400, 400, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
            }else if(indiceOption == 1){
              lancerMenuAchatHeros(ecran,JOUEUR_HUMAIN,joueurs,heros,herosJoueurs);//here
            }else if(indiceOption == 2){
              lancerMenuAchatHeros(ecran,JOUEUR_HUMAIN,joueurs,heros,herosJoueurs);
            }else if(indiceOption == 3){
              ///lancerMenuAchatUnites(ecran,JOUEUR_HUMAIN,joueurs,heros,herosJoueurs);
            }
          }else if(event.key.keysym.sym == SDLK_UP){
            if(indiceOption > 0){
              indiceOption--;
            }
          }else if(event.key.keysym.sym == SDLK_DOWN){
            if(indiceOption < 3){
              indiceOption++;
            }
          }
        }
        SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran -> format, 0, 0, 0));
        SDL_BlitSurface(menu, NULL, ecran, & positionMenu);

        if(font != 0){
          char * phrase[4];
            texte = TTF_RenderText_Blended(title_font, "Magasin", bleu);
            positionText.y = 100;
            positionText.x = 125;
            SDL_BlitSurface(texte, NULL, ecran, & positionText);
            phrase[0] = "Acheter des Heros";
            phrase[1] = "Voir les Heros";
            phrase[2] = "Acheter des Unites";
            phrase[3] = "Voir les Unites";
          int y_depart = 180;
          SDL_Color couleur_a_utiliser;
          for (int i = 0; i < 4; i++){
            if(i == indiceOption){
              couleur_a_utiliser = bleu;
              positionText.x = 80;
            }else{
              couleur_a_utiliser = blanc;
              positionText.x = 75;
            }
            texte = TTF_RenderText_Blended(font, phrase[i], couleur_a_utiliser);
            positionText.y = y_depart + i * 30;
            SDL_BlitSurface(texte, NULL, ecran, & positionText);
          }
        }
        SDL_Flip(ecran);
      }
      SDL_FreeSurface(texte);
      TTF_CloseFont(font);
      SDL_FreeSurface(menu);
}


int main(int argc, char * argv[]){
  SDL_Surface *ecran = NULL, *menu = NULL, *load = NULL;
  SDL_Rect positionMenu;
  SDL_Event event;
  TTF_Font *font = NULL;
  TTF_Font *title_font= NULL;
  SDL_Surface *texte = NULL;
  int continuer = 1;
  int screen = 0;
  int nb_choix = 0;

    int joueurs       [NB_LIGNES_JOUEURS][NB_COLONNES_JOUEURS];
    int heros         [NB_LIGNES_HEROS][NB_COLONNES_HEROS];
    int unites        [NB_LIGNES_UNITES][NB_COLONNES_UNITES];
    int herosJoueurs  [NB_LIGNES_HEROSJOUEURS][NB_COLONNES_HEROSJOUEURS];
    int unitesJoueurs [NB_LIGNES_UNITESJOUEURS][NB_COLONNES_UNITESJOUEURS];
    int carte         [NB_LIGNES_CARTE][NB_COLONNES_CARTE];
    int indiceOption = 0;

      SDL_Init(SDL_INIT_VIDEO);
      SDL_WM_SetIcon(IMG_Load("images/heros_icon.png"), NULL);
      ecran = SDL_SetVideoMode(400, 400, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
      SDL_WM_SetCaption("Heros Magic War", NULL);

      menu = IMG_Load("images/background.jpg");
      load = IMG_Load("images/load.png");
      positionMenu.x = 0;
      positionMenu.y = 0;

      TTF_Init();
      title_font = TTF_OpenFont("BlackOpsOne-Regular.ttf", 35);
      font = TTF_OpenFont("PressStart2P-Regular.ttf", 15);


      SDL_Color blanc = {255,255,255};
      SDL_Color bleu = {130, 236, 251};
      SDL_Rect positionText;

      SDL_EnableKeyRepeat(100, 100);
      SDL_BlitSurface(load, NULL, ecran, &positionMenu);
      SDL_Flip(ecran);
      SDL_Delay(1000);
      while(continuer){

        SDL_WaitEvent( & event);
        if(event.type == SDL_QUIT){
          continuer = 0;
        }else if(event.type == SDL_KEYDOWN){
          if(event.key.keysym.sym == SDLK_ESCAPE){
            continuer = 0;
          }else if(event.key.keysym.sym == SDLK_RETURN || event.key.keysym.sym == SDLK_KP_ENTER){
            if(indiceOption == 0 && screen == 0){
              initialiserNouveauJeu(carte,joueurs,heros,herosJoueurs,unites,unitesJoueurs);
              sauvegarderJeuComplet(carte,joueurs,heros,herosJoueurs,unites,unitesJoueurs);
              screen = 1;
            }else if(indiceOption == 1 && screen == 0){
              chargerJeuComplet(carte,joueurs,heros,herosJoueurs,unites,unitesJoueurs);
              screen = 1;
              indiceOption = 0;
            }else if(indiceOption == 0 && screen == 1){
              lancerMenuMagasin(ecran,joueurs,heros,herosJoueurs,unites,unitesJoueurs);
              sauvegarderJeuComplet(carte,joueurs,heros,herosJoueurs,unites,unitesJoueurs);
            }else if(indiceOption == 1 && screen == 1){
              lancerMenuCampement(ecran,joueurs,herosJoueurs,unitesJoueurs);
              sauvegarderJeuComplet(carte,joueurs,heros,herosJoueurs,unites,unitesJoueurs);
            }else if(indiceOption == 2 && screen == 1){
              deroulerJeu(ecran,carte,joueurs,heros,herosJoueurs,unites,unitesJoueurs);
              ecran = SDL_SetVideoMode(400, 400, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
              sauvegarderJeuComplet(carte,joueurs,heros,herosJoueurs,unites,unitesJoueurs);
            }else if(indiceOption == 3 && screen == 1){
              initialiserNouveauJeu(carte,joueurs,heros,herosJoueurs,unites,unitesJoueurs);
              deroulerJeu(ecran,carte,joueurs,heros,herosJoueurs,unites,unitesJoueurs);
              ecran = SDL_SetVideoMode(400, 400, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
              sauvegarderJeuComplet(carte,joueurs,heros,herosJoueurs,unites,unitesJoueurs);
            }else if(indiceOption == 4 && screen == 1){
              if(alert(ecran,"Vouz Voulez Quitter ?")){
                continuer = 0;
              }
            }
          }else if(event.key.keysym.sym == SDLK_UP){
            if(indiceOption > 0){
              indiceOption--;
            }
          }else if(event.key.keysym.sym == SDLK_DOWN){
            if(screen == 0 && indiceOption < 1){
                indiceOption++;
            }else if(screen ==1 && indiceOption < 4){
                indiceOption++;
            }
          }
        }

        SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran -> format, 0, 0, 0));
        SDL_BlitSurface(menu, NULL, ecran, & positionMenu);
        if(font != 0){
          char * phrase[5];
          if(screen == 0){
            texte = TTF_RenderText_Blended(title_font, "HEROS MAGIC WAR", bleu);
            positionText.y = 100;
            positionText.x = 30;
            SDL_BlitSurface(texte, NULL, ecran, & positionText);
            phrase[0] = "NOUVELLE PARTIE";
            phrase[1] = "CHARGER PARTIE";
            nb_choix = 2;
          }else if(screen == 1){
            texte = TTF_RenderText_Blended(title_font, "Menu Principal", bleu);
            positionText.x = 60;
            positionText.y = 90;
            SDL_BlitSurface(texte, NULL, ecran, & positionText);
            phrase[0] = "Magasin";
            phrase[1] = "Campement";
            phrase[2] = "Reprendre le jeu en cours";
            phrase[3] = "Nouvelle partie";
            phrase[4] = "Quitter";
            font = TTF_OpenFont("PressStart2P-Regular.ttf", 12);
            nb_choix = 5;
          }

          int y_depart = 210;
          SDL_Color couleur_a_utiliser;
          if(screen == 1){
            y_depart = 155;
          }
          for(int i = 0; i < nb_choix; i++){
            if(i == indiceOption){
              couleur_a_utiliser = bleu;
              positionText.x = 100;
            }else{
              couleur_a_utiliser = blanc;
              positionText.x = 95;
            }
            texte = TTF_RenderText_Blended(font, phrase[i], couleur_a_utiliser);
            positionText.y = y_depart + i * 30;
            SDL_BlitSurface(texte, NULL, ecran, & positionText);
          }
        }
        SDL_Flip(ecran);
      }
      SDL_EnableKeyRepeat(0, 0);
      SDL_FreeSurface(texte);
      TTF_CloseFont(font);
      SDL_FreeSurface(menu);
      SDL_FreeSurface(load);
      TTF_Quit();
      SDL_Quit();

      return EXIT_SUCCESS;
}

// if(alerte == 0){
//   alerte = 1;
//   if(alert(ecran,"Vouz voulez ouvrir cette caisse ?")){
//     return;
//   }
// }
