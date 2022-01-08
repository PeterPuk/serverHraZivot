//
// Created by Martin on 08/01/2022.
//
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void zapisSubor(int newsockfd);

int prijmiVolbuNacitanie(int newsockfd);

void posliSubor(FILE* subor,int newsockfd);

int mainServer(int argc, char* argv[]) {
    int volbaNacitania = 0;
    FILE* subor;
    char *nazovSuboru = "vystup.txt";
    int sockfd, newsockfd;
    socklen_t cli_len;
    struct sockaddr_in serv_addr, cli_addr;

    if (argc < 2)
    {
        fprintf(stderr,"Malo argumentov v program:%s\n", argv[0]);
        exit(1);
    }

    bzero((char*)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(atoi(argv[2]));

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("Chyba pri vytvarani socketu");
        exit(1);
    }

    printf("Socket sa vytvoril.\n");

    if (bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("Adresu sa nepodarilo priradit k socketu");
        exit(1);
    }
    printf("Adresa priradena.\n");

    int e = listen(sockfd, 5);

    if(e == 0){
        printf("Server pocuva...\n");
    }else{
        perror("Chyba pri pocuvani.\n");
        exit(1);
    }
    cli_len = sizeof(cli_addr);
    newsockfd = accept(sockfd,(struct sockaddr*)&cli_addr,&cli_len);
    volbaNacitania = prijmiVolbuNacitanie(newsockfd);
    if(volbaNacitania==1){
        subor = fopen(nazovSuboru,"r");
        if(subor== NULL){
            perror("Chyba pri citani suboru\n");
            exit(1);
        }
        posliSubor(subor,newsockfd);
        printf("Data sa poslali uspesne\n");

    }else{
        zapisSubor(newsockfd);
    }
    close(newsockfd);
    close(sockfd);
}

void zapisSubor(int newsockfd){
    int n;
    FILE *subor;
    char *nazovSuboru = "vystup.txt";
    char buffer[1024];
    subor = fopen(nazovSuboru,"w");
    if(subor== NULL){
        perror("Chyba pri vytvarani suboru\n");
        exit(1);
    }
    while(1){
        n = recv(newsockfd, buffer,1024,0);
        if(n <=0){
            break;
        }
        fprintf(subor,"%s",buffer);
        printf("%s",buffer);
        bzero(buffer,1024);
    }
}

void posliSubor(FILE* subor,int newsockfd){
    char data[1024] = {0};
    while (fgets(data,1024,subor) != NULL){
        if(send(newsockfd, data, sizeof(data),0) == -1){
            perror("Chyba pri posielani suboru.\n");
            exit(1);
        }
        bzero(data,1024);
    }
}


int prijmiVolbuNacitanie(int newsockfd){
    int vyberS = -1;
    while(vyberS == -1) {
        read(newsockfd, &vyberS, sizeof(int));
    }
    printf("Poziadavka od klienta je %d\n", vyberS);
    return vyberS;
}

