#include <stdlib.h>
#include <stdio.h>
#include <string.h> 
#include <time.h>
#include <sys/stat.h>

#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#define PORT 8080

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

void jeuTitre(){
	printf("\t#######  #######  ##   ##           ######      ##         ##  ##       ##       ##   ##  ##   ##  #######   ######   ######  #######\n");
	printf("\t#######  #######  ##   ##           #######    ###        ###  ##       ##       ##   ##  ### ###  #######   ######   ######  #######\n");
	printf("\t                        #                 #    ##           #                          #        #	                                   \n");
	printf("\t   ##    ####     ##   ##           ##   ##              ####  ##       ##       ##   ##  ## # ##  ####        ##       ##    ####   \n");
	printf("\t## ##    ##       ##   ##           ##   ##             #####  ##       ##       ##   ##  ##   ##  ##          ##       ##    ##     \n");
	printf("\t#####    #######  #######           #######            ##  ##  #######  #######  #######  ##   ##  #######     ##       ##    #######\n");
	printf("\t ###     #######   #####            ######            ##   ##  #######  #######   #####   ##   ##  #######     ##       ##    #######\n");
	printf("\n\n\n\n");
}

void save_score(char player1[], char player2[], int coupsj1, int coupsj2, char ip_server[]){

	int server_fd, new_socket, valread, valread1;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    int buffer[1024] = {0};
    char *hello = "Hello from server";
       
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
       
    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                                                  &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
       
    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address, 
                                 sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, 
                       (socklen_t*)&addrlen))<0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    buffer[0] = 1;
    send(new_socket , buffer , 1024 , 0 );

    /////////////////////////////////////////////////////

	system("clear");
	printf("\n %s",player1);
	printf(" : %d",coupsj1);
	printf(" vs %s",player2);
	printf(" : %d\n",coupsj2);
	sleep(10);

}

int j(int all,char name[],int nbp){
	printf("\n %s veuillez saisir le nombre d'allumettes a retirer compris entre 1 et %d : ",name,nbp);
	int nb;
	scanf("%d",&nb);
	while(nb<=0 | nb>nbp){
		printf("\n Veuillez saisir un nombre compris entre 1 et %d :",nbp);
		scanf("%d",&nb);
	}
	int result = all-nb;
	return result;
}

int ai(int all, int nbp){
	int i;
	int result;
	for(i=1;i<=nbp;i++){
		result=all-i;
		if(result==1){
			printf("Trouvé !!!");
			return result;
		}
	}
    srand(time(NULL));
    int nbgen=rand()%nbp+1;
    result=all-nbgen;
    return result;
}

void jouer(char player1[], char player2[], int nballumettes, int nbprallumettes){
	
	#define NOMBREALLUMETTE nballumettes
	#define NOMBREDEPRISEMAX nbprallumettes
	#define NAMEJOUEUR1 player1
	#define NAMEJOUEUR2 player2
	
	int allumette;
	
	allumette = NOMBREALLUMETTE;
	
	int coupsj1 = 0;
	int coupsj2 = 0;
	
	while(allumette>0){
		
		system("clear");
		jeuTitre();
		printf("Nombre d'allumette disponible : ");
		int i;
		for(i=0;i<allumette;i++){
			printf("|");
		}
		printf(" ( %d allumettes restants )\n",allumette);
		allumette = j(allumette,NAMEJOUEUR1,NOMBREDEPRISEMAX);
		coupsj1++;
		if (allumette<=0){
			printf("%s",NAMEJOUEUR2);
			printf(" à gagné en %d coups\n",coupsj2);
			sleep(5);
			break;
		}
		
		system("clear");
		jeuTitre();
		printf("Nombre d'allumette disponible : ");
		for(i=0;i<allumette;i++){
			printf("|");
		}
		printf(" ( %d allumettes restants )\n",allumette);
		allumette = ai(allumette,NOMBREDEPRISEMAX);
		coupsj2++;
		if (allumette<=0){
			printf("%s",NAMEJOUEUR1);
			printf(" à gagné en %d coups\n",coupsj1);
			sleep(5);
			break;
		}
	}
}

void heberger(char player1[], int nballumettes, int nbprallumettes, char ip_server[]){

	printf("\n En Attende du joueur 2 ... \n");

	int server_fd, new_socket, valread, valread1;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char player2[1024] = {0};
    int buffer[1024] = {0};
    char *hello = "Hello from server";
       
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
       
    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                                                  &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
       
    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address, 
                                 sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, 
                       (socklen_t*)&addrlen))<0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    // Reception du nom du joueur 2 puis envoie du nom du joueur 1
    valread1 = read( new_socket , player2, 1024);
    printf("%s est connectée\n",player2);
    char *me = player1;
    send(new_socket , me , strlen(me) , 0 );

    // Envoie du nombre d'allumette et du nombre de prise
    buffer[0] = nballumettes;
    buffer[1] = nbprallumettes;
    send(new_socket , buffer , 1024 , 0 );

    ///////////////////////////////////////////////////

    int allumette;
	
	allumette = nballumettes;
	
	int coupsj1 = 0;
	int coupsj2 = 0;

    int x=0;
    while(x==0){

    	system("clear");
		jeuTitre();
		printf("Nombre d'allumette disponible : ");
		int i;
		for(i=0;i<allumette;i++){
			printf("|");
		}
		printf(" ( %d allumettes restants )\n",allumette);
		allumette = j(allumette,player1,nbprallumettes);
		coupsj1++;
		if (allumette<=0){
			printf("%s",player2);
			printf(" à gagné en %d coups\n",coupsj2);
			//
			buffer[0] = allumette;
	    	buffer[1] = nbprallumettes;
	    	buffer[2] = 2;
	    	buffer[3] = coupsj1;
	    	buffer[4] = coupsj2;
	    	send(new_socket , buffer , 1024 , 0 );
			//
			sleep(10);
			save_score(player1,player2,coupsj1,coupsj2,ip_server);
			break;
		}

		system("clear");
		jeuTitre();
		printf("Nombre d'allumette disponible : ");
		for(i=0;i<allumette;i++){
			printf("|");
		}
		printf(" ( %d allumettes restants )\n",allumette);

		//////////////////////////////////////
		printf("C'est au tour de %s de jouer : ",player2);
		buffer[0] = allumette;
    	buffer[1] = nbprallumettes;
    	buffer[2] = 0;
    	buffer[3] = coupsj1;
    	buffer[4] = coupsj2;
    	send(new_socket , buffer , 1024 , 0 );
    	//buffer[1024] = {0};
    	valread = read( new_socket , buffer, 1024);
    	allumette = buffer[0];
		//////////////////////////////////////
		coupsj2++;
		if (allumette<=0){
			printf("%s",player1);
			printf(" à gagné en %d coups\n",coupsj1);
			//
			buffer[0] = allumette;
	    	buffer[1] = nbprallumettes;
	    	buffer[2] = 1;
	    	buffer[3] = coupsj1;
	    	buffer[4] = coupsj2;
	    	send(new_socket , buffer , 1024 , 0 );
			//
			sleep(10);
			save_score(player1,player2,coupsj1,coupsj2,ip_server);
			break;
		}
    	////////////////////////////////////////////////////
        
    }

}

void rejoindre(char player1[]){

	char ip[15];

	printf("Veuillez saisir l'adresse ip de l'hébergeur : ");
	scanf("%s",ip);

	int sock = 0, valread, valread1;
    struct sockaddr_in serv_addr;
    int tab[] = {0};
    int *hello = &tab[0]; //"Hello from client";
    char buffer[1024] = {0};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
    }
   
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
       
    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, ip, &serv_addr.sin_addr)<=0) 
    {
        printf("\nInvalid address/ Address not supported \n");
    }
   
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
    }

    // Envoie du nom du joueur 2 puis reception du nom du joueur 1
    char *me = player1; 
    send(sock , me , 1024, 0 );
    char player2[1024] = {0};
    valread1 = read( sock , player2, 1024);
    printf("%s est connectée\n",player2);

    // Reception du nombre d'allumette et du nombre de prise max possible 
    int test[1024] = {0};
    valread = read( sock , test, 1024);

    ////////////////////////////////////////////

    int i;

    system("clear");
	jeuTitre();
	printf("Nombre d'allumette disponible : ");
	for(i=0;i<test[0];i++){
		printf("|");
	}
	printf(" ( %d allumettes restants )\n",test[0]);
	printf("C'est au tour de %s de jouer : ",player2);

    int x=0;
    while(x==0){

    	//test[1024] = {0};
    	valread = read( sock , test, 1024);
    	//sleep(10);
    	if(test[2]==0){
        	system("clear");
			jeuTitre();
			printf("Nombre d'allumette disponible : ");
			for(i=0;i<test[0];i++){
				printf("|");
			}
			printf(" ( %d allumettes restants )\n",test[0]);
			test[0]=j(test[0],player1,test[1]);
        	send(sock , test , 1024, 0 );

        	///////
        	
        	system("clear");
			jeuTitre();
			printf("Nombre d'allumette disponible : ");
			for(i=0;i<test[0];i++){
				printf("|");
			}
			printf(" ( %d allumettes restants )\n",test[0]);
			printf("C'est au tour de %s de jouer : ",player2);
    	}else if(test[2]==1){
    		system("clear");
			jeuTitre();
			printf("Nombre d'allumette disponible : ");
			for(i=0;i<test[0];i++){
				printf("|");
			}
			printf(" ( %d allumettes restants )\n",test[0]);
			printf("%s",player2);
			printf(" à gagné en %d coups\n",test[3]);
			sleep(10);
			x=1;
    	}else if (test[2]==2){
    		system("clear");
			jeuTitre();
			printf("Nombre d'allumette disponible : ");
			for(i=0;i<test[0];i++){
				printf("|");
			}
			printf(" ( %d allumettes restants )\n",test[0]);
			printf("%s",player1);
			printf(" à gagné en %d coups\n",test[4]);
			sleep(10);
			x=1;
    	}
    	

        /////////////////////////////////////////

        //printf("Hello message sent\n");

    }

}

void versus(char player1[], int nballumettes, int nbprallumettes, char ip_server[]){
	
	#define HEBERGER 1
	#define REJOINDRE 2
	#define QUITTER 0
	
	int i = 0;
	
	int selected;
	
	while(i==0){
	
		system("clear");
		
		jeuTitre();
		
		printf("\n\t VERSUS :");
		
		printf("\n\t\t\t 1] HEBERGER");
		printf("\n\t\t\t 2] REJOINDRE");
		printf("\n\t\t\t 0] QUITTER");
		
		printf("\n\n\n\n\n\t\t\t Faite votre choix : ");
		
		scanf("%d",&selected);
		
		while(selected<0 || selected>2){
			scanf("%d",&selected);
		}
		
		if(selected==0){
			break;
		}else if (selected==1){
			heberger(player1, nballumettes, nbprallumettes, ip_server);
		}else if (selected==2){
			rejoindre(player1);
		}
	
	}
	
}

void option(){
	
    char* filename = "config.txt";

	FILE *in_file = fopen(filename, "r");

    struct stat sb;
    stat(filename, &sb);

    char *file_contents = malloc(sb.st_size);

    int inc=0;

    // variable récuperer
    char player1[12];
    int nballumettes;
    int nbprallumettes;
    char ip_server[35];

    while (fscanf(in_file, "%[^\n] ", file_contents) != EOF) {
    	if(inc==0){
    		int m;
    		for(m=0;m<sb.st_size;m++){
    			player1[m]=file_contents[m];
    		}
    	}else if(inc==1){
    		nballumettes=((int)file_contents[0]-48)*10+((int)file_contents[1]-48);
    	}else if(inc==2){
    		nbprallumettes=(int)file_contents[0]-48;
    	}else if(inc==3){
    		int m;
    		for(m=0;m<sb.st_size;m++){
    			ip_server[m]=file_contents[m];
    		}
    	}else{
    		break;
    	}
    	inc++;
    }

    fclose(in_file);

    //////////////////////////////////////////////////////
	
	int i=0;
	
	while(i==0){
	
		system("clear");
		jeuTitre();
		printf("\t OPTION :");
		
		printf("\n\t\t\t 0] Retour");
		printf("\n\t\t\t 1] Player1 name : %s",player1);
		printf("\n\t\t\t 2] Nombre d'allumette : %d",nballumettes);
		printf("\n\t\t\t 3] Nombre de prise d'allumette : %d",nbprallumettes);
		printf("\n\t\t\t 4] Ip du server : %s",ip_server);

		printf("\n\n\n\n\n\t\t\t Choisissez la ligne a modifier : ");
		int selected;
		scanf("%d",&selected);
		
		while(selected<0 || selected>4){
			scanf("%d",&selected);
		}
		
		if(selected==0){
			break;
		}else if (selected==1){
			player1[11];
			printf("Veuillez entrer le nom du player1 :");
			scanf("%s",player1);
			player1[10]='\n';
			FILE* fichier2 = NULL;
			fichier2 = fopen("config.txt", "w+");
			fprintf(fichier2, "%s\n%d\n%d\n%s",player1,nballumettes,nbprallumettes,ip_server);
	        fclose(fichier2);
	        break;
		}else if (selected==2){
			printf("Veuillez entrer le nombre d'allumette :");
			scanf("%d",&nballumettes);
			FILE* fichier2 = NULL;
			fichier2 = fopen("config.txt", "w+");
			fprintf(fichier2, "%s\n%d\n%d\n%s",player1,nballumettes,nbprallumettes,ip_server);
	        fclose(fichier2);
	        break;
		}else if (selected==3){
			printf("Veuillez entrer le nombre de prise d'allumette :");
			scanf("%d",&nbprallumettes);
			FILE* fichier2 = NULL;
			fichier2 = fopen("config.txt", "w+");
			fprintf(fichier2, "%s\n%d\n%d\n%s",player1,nballumettes,nbprallumettes,ip_server);
	        fclose(fichier2);
	        break;
		}else{
			printf("Veuillez entrer le nombre de prise d'allumette :");
			scanf("%s",ip_server);
			FILE* fichier2 = NULL;
			fichier2 = fopen("config.txt", "w+");
			fprintf(fichier2, "%s\n%d\n%d\n%s",player1,nballumettes,nbprallumettes,ip_server);
	        fclose(fichier2);
	        break;
		}
	}	
}

void menu(){
	
	#define JOUER 1
	#define VERSUS 2
	#define OPTION 3
	#define SCORE 4
	#define CREDIT 5
	#define QUITTER 0
	
	int i = 0;
	
	int selected;
	
	while(i==0){

		/////////////////////////////////////////////

		char* filename = "config.txt";

		FILE *in_file = fopen(filename, "r");

	    struct stat sb;
	    stat(filename, &sb);

	    char *file_contents = malloc(sb.st_size);

	    int inc=0;

	    // variable récuperer
	    char player1[12];
	    int nballumettes;
	    int nbprallumettes;
	    char ip_server[35];

	    while (fscanf(in_file, "%[^\n] ", file_contents) != EOF) {
	    	if(inc==0){
	    		int m;
	    		for(m=0;m<sb.st_size;m++){
	    			player1[m]=file_contents[m];
	    		}
	    	}else if(inc==1){
	    		nballumettes=((int)file_contents[0]-48)*10+((int)file_contents[1]-48);
	    	}else if(inc==2){
	    		nbprallumettes=(int)file_contents[0]-48;
	    	}else if(inc==3){
	    		int m;
	    		for(m=0;m<sb.st_size;m++){
	    			ip_server[m]=file_contents[m];
	    		}
	    	}else{
	    		break;
	    	}
	    	inc++;
	    }

	    fclose(in_file);

		///////////////////////////////////////////////

		system("clear");
		
		jeuTitre();
		
		printf("\n\t MENU :");
		
		printf("\n\t\t\t 1] JOUER");
		printf("\n\t\t\t 2] VERSUS");
		printf("\n\t\t\t 3] OPTION");
		printf("\n\t\t\t 4] SCORE");
		printf("\n\t\t\t 5] CREDIT");
		printf("\n\t\t\t 0] QUITTER");
		
		printf("\n\n\n\n\n\t\t\t Faite votre choix : ");
		
		scanf("%d",&selected);
		
		while(selected<0 || selected>3){
			scanf("%d",&selected);
		}
		
		if(selected==0){
			break;
		}else if (selected==1){
			jouer(player1,"IA",nballumettes,nbprallumettes);
		}else if (selected==2){
			versus(player1,nballumettes,nbprallumettes,ip_server);
		}else if (selected==3){
			option();
		}
	}
}

int main(){

	FILE* fichier = NULL;

    fichier = fopen("config.txt", "r");
    
    if (fichier != NULL){
    	printf("Chargement de la config ....\n");
    	sleep(5);
    }else{
    	FILE* fichier2 = NULL;
		fichier2 = fopen("config.txt", "w+");
        fputs("player1\n19\n3\n127.0.0.1", fichier2);
        fclose(fichier2);
    }
    fclose(fichier);
	menu();
	return 0;
}
