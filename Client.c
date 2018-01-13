#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdbool.h>


#define MAX_CARTES 52
#define MAX_CARTES_MAIN 5
#define MAX_VALEUR 13
#define MAX_COULEUR 4
#define MAX_BUDGET 100
#define MAX_JOUEURS 3

const char co[] = "\xe2\x99\xa5";
const char carr[] = "\xe2\x99\xa6";
const char piq[] = "\xe2\x99\xa4";
const char tr[] = "\xe2\x99\xa7";
const char lettres[] = {'A','J', 'Q', 'K'};


int intContinuer, intPlay;



typedef enum {
    as= 1, deux=2, trois=3, quatre=4, cinq=5, six=6, sept=7, huit=8, neuf=9, dix=10, valet=11, reine=12, roi=13
    
} Val;

typedef enum {
    carreau=1, coeur=2, pique=3, trefle=4
    
} Coul;

struct carte{
        Val valeur; 
        Coul couleur;
};

struct joueur{
        int budget;
       // char adresseConnex;//int adresseConnex;
        int noId;
};


typedef struct carte Carte;
typedef struct joueur Joueur;



void afficherEnsembleCartes(Carte*, int);
bool combPaire(Carte tabCartes[]);
bool combDoublePaire(Carte tabCartes[]);
bool combBrelan(Carte tabCartes[]);
bool combCouleur(Carte tabCartes[]);
//int detecterCombinaison(Carte tabCartes[], int);

bool commencerJeu(bool play){
    printf("\n\t --------------------------------\n");
    printf("\t|  1) Commencer le Jeu.\t\t |\n\t|  2) Attendre à un autre Joueur.|\n");
    printf("\t --------------------------------\n");
    scanf("%i",&intPlay);
    if(intPlay==1) return play = true;
    else if (intPlay == 2) return play = false;
}

bool continueJeu(bool continuer){
    printf("\n\t --------------------------------\n");
    printf("\t|  1) Continuer.\t\t |\n\t|  2) Arreter.\t\t\t |\n");
    printf("\t --------------------------------\n");
    //printf("\n1)Continuer.\n2)Arreter.\n");
    scanf(" %i",&intContinuer);
    if(intContinuer==1) return continuer = true;
    else if (intContinuer == 2) return continuer = false;
    
}

void afficherEnsembleCartes(Carte* tabCartes, int nbreCartes){
    int i;
    
    for(i=0; i<nbreCartes; i++){
        
        if(tabCartes[i].couleur==carreau){
                printf("\n\t\t(%d , %d) = %d de %s\n", tabCartes[i].valeur,tabCartes[i].couleur, tabCartes[i].valeur, carr);  
        }
        
        else if(tabCartes[i].couleur==coeur){
              printf("\n\t\t(%d , %d) = %d de %s\n", tabCartes[i].valeur,tabCartes[i].couleur, tabCartes[i].valeur, co);  
        }
        
        else if(tabCartes[i].couleur==pique){
              printf("\n\t\t(%d , %d) = %d de %s\n", tabCartes[i].valeur,tabCartes[i].couleur, tabCartes[i].valeur, piq);  
        }
        
        else {
              printf("\n\t\t(%d , %d) = %d de %s\n", tabCartes[i].valeur,tabCartes[i].couleur, tabCartes[i].valeur, tr);  
        }
    }
}


/*int detecterCombinaison(Carte tabCartes[], int nbreCartes){
    int paire1 = -1;
    int paire2 = -1;
    int brelan = -1;
    int couleur = -1;
    
    int i,j,k;
    
    for(i=0; i<nbreCartes; i++){
        for(j=i+1; j<nbreCartes; j++){
            
                if(tabCartes[i].valeur == tabCartes[j].valeur){
                        if(paire1==-1) pai1 = tabCartes[i].valeur;
                        else if(paire1 != tabCartes[i].valeur && paire2 == -1) paire2 = tabCartes[i].valeur;                
                }
                
            for(k=j+1; k<nbreCartes; k++){
                
                if(tabCartes[i].valeur == tabCartes[j] && tabCartes[j] == tabCartes[k])
            }
        }
    }
    
}*/

int main(int argc,char** argv){
  
  struct sockaddr_in coord_serveur;
  int sockclient;
  struct tm *m;
  char* pc;
  u_short port;
  int nb;
  
  
  
  //création de la socket
  if(argc!=3){
        printf("Usage: ./client <adresse ip> <port>\n");
        return 0;
  }
  
  port=atoi(argv[2]);
  sockclient = socket(AF_INET, SOCK_STREAM, 0);

  //Partie connect ()

  //préparation de la structure permettant de connecter le serveur
  bzero(&coord_serveur,sizeof(coord_serveur));
  coord_serveur.sin_family = AF_INET;
  coord_serveur.sin_port = htons(port);
  coord_serveur.sin_addr.s_addr = inet_addr(argv[1]);
  
  if(connect(sockclient, (struct sockaddr *) &coord_serveur, sizeof(coord_serveur)) <0){ 
        printf ("erreur de connexion \n");
        exit(0);
  }
	  
  //Traitement
  
    int n;
    Joueur Ju;
    Carte Ca[MAX_CARTES];
    Carte CaC[MAX_CARTES_MAIN];
    bool continuer, play;
    

        read(sockclient, &Ju, sizeof(Joueur)); //toujours mettre le "&"
        read(sockclient, &n, sizeof(int));
        printf("\n\t===========================================\n");
        printf("\t       B I E N V E N U E - POKER GAME\n");
        printf("\t===========================================\n\n");
        sleep(1);
        printf("\n\t************** Joueur n°%i ***************\n",Ju.noId);
        printf("\n-Votre budget initial est: %i\n",Ju.budget);
        printf("-Pari : %i\n",n);
        
        /* Demande de commencer le Jeu */
        sleep(1);
        play = commencerJeu(play);
        write(sockclient, &play, sizeof(bool));
        read(sockclient, &Ca,2*sizeof(Carte));
        sleep(2);
        printf("Vos cartes sont: ");
        afficherEnsembleCartes(Ca,2);
        
        /* Demande de continuer/arreter le Jeu */ 
        sleep(2);
        continuer = continueJeu(continuer);
        write(sockclient, &continuer, sizeof(bool));

        if(continuer){
                    sleep(1);
                    printf("\n\t\tContinuons!!...\n");
                    
                    
        }
        else{
                    sleep(1);
                    printf("\n\t\t*******  F I N  D U  J E U X  *******\n\n");
                    int id = Ju.noId;
                    write(sockclient, &Ju.noId, sizeof(int));
                    exit(1);
        }
        
        read(sockclient, &Ca, 3*sizeof(Carte));
        sleep(2);
        printf("\nLes cartes du Croupier sont: ");
        afficherEnsembleCartes(Ca,3);
        
            
  
        close(sockclient);
        printf("\n");
}


