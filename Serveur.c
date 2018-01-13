#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
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
//const char lettres[] = {'A','J', 'Q', 'K'};


typedef enum {
    as= 1, deux=2, trois=3, quatre=4, cinq=5, six=6, sept=7, huit=8, neuf=9, dix=10, valet=11, reine=12, roi=13
    
} Val;

typedef enum {
    carreau=1, coeur=2, pique=3, trefle=4
    
} Coul;

/*typedef enum {
    paire=1, doublePaire=2, brelan=3, couleur=4
    
} Combin;*/

struct carte{
        Val valeur; 
        Coul couleur;
};

struct joueur{
        int budget;
//         char*adresseConnex;//int adresseConnex;
        int noId;
};

            /* Nouvelle definition de Variables (struct) */
typedef struct carte Carte;
typedef struct joueur Joueur;

            /* Fonctions */
void initializerCartes(Carte*);
void initializerJoueurs(int, Joueur*);
void distribuirCartes(Carte*, int);
/*bool combPaire(Carte*);
bool combDoublePaire(Carte*);
bool combBrelan(Carte*);
bool combCouleur(Carte*);
int detecterCombinaison(Carte*, int);*/



void initializerCartes(Carte* tabCartes){
    int i;
    for(i=0; i<MAX_CARTES; i++){
        
        if(i+1<14){
            tabCartes[i].valeur=i+1;
            tabCartes[i].couleur=carreau;
                //if(i=1)tabCartes[i].valeur=
        }
        
        else if(i+1<27){
            tabCartes[i].valeur=i+1;
            tabCartes[i].couleur=coeur;
        }
        
        else if(i+1<40){
            tabCartes[i].valeur=i+1;
            tabCartes[i].couleur=pique;
        }
        
        else{
            tabCartes[i].valeur=i+1;
            tabCartes[i].couleur=trefle;
        }
    }
    printf("\nToutes les cartes initializées!!...\n");
}

void initializerJoueurs(int noJoueur, Joueur* J){
        J->budget = 100;
        J->noId = noJoueur;
        sleep(1);
        printf("\n\t  ===== Joueur n°%i viens de se connecter! ====\n",J->noId);
}


void distribuirCartes(Carte* tabCartes, int nbreCartes){
    srand(time(NULL));
    int i;
    for(i=0; i<nbreCartes; i++){
        tabCartes[i].valeur=rand()% MAX_VALEUR+1;
        tabCartes[i].couleur=rand()% MAX_COULEUR+1;
        
    }
    printf("\n-Cartes Distribuées!!...\n");
    
    
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


int main(){
  
  /* Déclaration des variables */
  int sockserveur,newsockfd[3],lg;
  struct sockaddr_in coord_client;
  struct sockaddr_in mes_coord;
  struct tm *m;
  char* pc;
  
  
  /* Création d'un socket */
  sockserveur=socket(AF_INET, SOCK_STREAM, 0);
  
  /* Serveur : appel BIND */
  bzero(&mes_coord,sizeof(mes_coord));
  mes_coord.sin_family=AF_INET;
  mes_coord.sin_port=htons(2064);
  mes_coord.sin_addr.s_addr=htonl(INADDR_ANY);
  
  if(bind(sockserveur,(struct sockaddr*)&mes_coord,sizeof(mes_coord))<0){
        printf("Erreur BIND\n");
        exit(0);
  }
  
  /* Serveur : appel LISTEN */
  if (listen(sockserveur,5)<0){
        printf("Erreur LISTEN\n");
        exit(0);
  }
  
  /* Serveur : appel ACCEPT */
        lg=sizeof(coord_client);
        int i,j, n=0;
        int idF;
        bool continuer, play;
        
        Carte C[MAX_CARTES];
        Carte CC[MAX_CARTES_MAIN];
        Joueur J[MAX_JOUEURS];
        
        int tabPid[MAX_JOUEURS];
        
        int tabTube[MAX_JOUEURS][2];
        
        char* buffer1 = malloc(100*sizeof(char));
        char* buffer2 = malloc(100*sizeof(char));
        char bufferMsg1[100], bufferMsg2[100];
        
        int reponse;
        
        printf("\n\t**************** Serveur Connecté!!... ***************\n");
        sleep(1);
        initializerCartes(C);
        
        //***** Initialization du "tabTube" *****
        for(i=0; i<MAX_JOUEURS; i++){
            for(j=0; j<2; j++){
                tabTube[i][j];
            }
            
        }
        
        // ******* Boucle principal : Création de Processus ******
        for(i=0; i<MAX_JOUEURS; i++){
                
                newsockfd[i]=accept(sockserveur,(struct sockaddr*)&coord_client,&lg);
                
                if(newsockfd[i]==(-1)){
                    
                            printf("Erreur ACCEPT\n");
                            exit(0);
                }
                
                else{
                            if(pipe(tabTube[i])==-1){
                                perror("tube");
                                exit(EXIT_FAILURE);
                            }
                            
                            else{
                                    
                                    tabPid[i] = fork();
                            
                                    if (tabPid[i] == -1){
                                        
                                                perror("Attention Fork");
                                                exit (EXIT_FAILURE);
                                    }
                                    
                                    else if(tabPid[i] == 0){

                                                if(i==0){   
                                                            printf("\n\n\t Processus n°1...\n");
                                                            read(tabTube[0][0], &reponse ,sizeof(int));
                                                            
                                                            if(reponse == 1){
                                                                        sleep(2);
                                                                        printf("\nClient n°%i est Ready\n",i+1);
                                                            }
                                                            else    printf("\nClient n°%i est en train d'attrendre\n",i+1);
                                                            //write(tabTube[1][1], &reponse, sizeof(int));
                                                            
                                                }
                                                else if(i==1){
                                                            printf("\n\n\t Processus n°2...\n");
                                                            read(tabTube[1][0], &reponse ,sizeof(int));
                                                            
                                                            if(reponse == 1){
                                                                        sleep(2);
                                                                        printf("\nClient n°%i est Ready\n",i+1);
                                                            }
                                                            else    printf("\nClient n°%i est en train d'attrendre\n",i+1);
                                                            //write(tabTube[2][1], &reponse, sizeof(int));

                                                }
                                                
                                                else {
                                                            printf("\n\n\t Processus n°3...\n");
                                                            read(tabTube[2][0], &reponse ,sizeof(int));
                                                            
                                                            if(reponse == 1){
                                                                        sleep(2);
                                                                        printf("\nClient n°%i est Ready\n",i+1);
                                                            }
                                                            else    printf("\nClient n°%i est en train d'attrendre\n",i+1);

                                                }
                    
                                    }
                                    
                                    else {      
                                                
                                                initializerJoueurs(i+1, &J[i]);
                                                write(newsockfd[i], &J[i], sizeof(Joueur));
                                                write(newsockfd[i], &n, sizeof(int));
                                                read(newsockfd[i], &play, sizeof(bool));
                                                
                                                if(play){
                                                            sleep(1);
                                                            printf("\n-Commençons!!...\n");
                                                            sleep(1);
                                                            distribuirCartes(C,2);
                                                            write(newsockfd[i], &C, MAX_CARTES*sizeof(Carte));
                                                            reponse = 1; //Ready;
                                                            write(tabTube[0][1], &reponse, sizeof(int));
                                                            write(tabTube[1][1], &reponse, sizeof(int));
                                                            write(tabTube[2][1], &reponse, sizeof(int));
                                                            
                                                            
                                                }
                                                else {      
                                                            sleep(2);
                                                            printf("\n-Jeu en attente! (En attentant le joueur suivant)...\n");
                                                            reponse = 0; // Attente;
                                                            write(tabTube[0][1], &reponse, sizeof(int));
                                                            write(tabTube[1][1], &reponse, sizeof(int));
                                                            write(tabTube[2][1], &reponse, sizeof(int));
                                                            
                                                            
                                                }
                                                
                                                read(newsockfd[i], &continuer, sizeof(bool));
                                                
                                                if(continuer){
                                                            sleep(2);
                                                            printf("\n-Continuons!!...\n");  
                                                            sleep(2);
                                                            printf("\n\tJe suis le Croupier/Serveur (Père)!!...\n");
                                                            distribuirCartes(C,3);
                                                            write(newsockfd[i], &C, MAX_CARTES*sizeof(Carte));
                                                                        
                                                }
                                                else {      
                                                            sleep(1);
                                                            read(newsockfd[i], &idF, sizeof(int));
                                                            printf("\n-Fin du jeux pour le Joueur n°%i !!...\n", idF);
                                                }
                                        
                                                int stat;
                                                waitpid(tabPid[i], &stat, 0);
                                                
                                                exit (EXIT_SUCCESS);
                                    }
                                
                           }
                }
               
        }
        
        close(newsockfd[i]);
  
  close(sockserveur);
  printf("\n");
  
}
