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

int main(){

    int server_fd, new_socket, valread, valread1;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char player2[1024] = {0};
    int buffer[1024] = {0};
    char *hello = "Hello from server";

    int x=0;
       
    while(x==0){

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

        // Reception d'un nombre ( 0 / 1 )
        int test[1024] = {0};
        valread1 = read( new_socket , test, 1024);

        if(test[0]==1){
            printf("\n envoie du score \n");
            /////////////////////////////////
            char chaine[1000] = ""; // Chaîne vide de taille 1000
	    char all[10000] = "";
	    FILE* fichier = NULL;

	    fichier = fopen("score.txt", "r");

	    if (fichier != NULL)
	    {
		while(fgets(chaine, 100000, fichier) != NULL){
		     strcat(all,chaine);
		}
		
		//printf("%s", chaine); // On affiche la chaîne
	 
		fclose(fichier);
	    }
            /////////////////////////////////
            char *me = all;
            printf("%s",me);
            send(new_socket , me , 1024, 0 );
        }else if (test[0]==0){
            printf("\n ajoue de score");
            valread1 = read( new_socket , player2, 1024);
            //printf("%s a ete enregistrer !!!",player2);
            /////////////////////////////////
            FILE* fichier = NULL;

	    fichier = fopen("score.txt", "a+");

	    if (fichier != NULL)
	    {
	    	fprintf(fichier,"%s\n",player2);
	    	fclose(fichier);
	    }
            /////////////////////////////////
	    printf("%s a ete enregistrer !!!",player2);
        }

    }

    return 0;

}
