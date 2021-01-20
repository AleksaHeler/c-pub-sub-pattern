/* 
    ********************************************************************
    Odsek:          Elektrotehnika i racunarstvo
    Departman:      Racunarstvo i automatika
    Katedra:        Racunarska tehnika i racunarske komunikacije (RT-RK)
    Predmet:        Osnovi Racunarskih Mreza 1
    Godina studija: Treca (III)
    Skolska godina: 2020
    Semestar:       Zimski (V)
    
    Ime fajla:      server.c
    Opis:           TCP/IP server
    
    Platforma:      Raspberry Pi 2 - Model B
    OS:             Raspbian
    ********************************************************************
*/

/*
 * TODO: proveriti da li je struktura ok, tj da li je sve sto je potrebno tu
 * 
 * 
*/

#include <stdio.h>
#include <string.h>     //strlen
#include <sys/socket.h>
#include <arpa/inet.h>  //inet_addr
#include <unistd.h>     //write
#include <stdlib.h>
#include <pthread.h>    //pthread
#include <string.h>
#include <semaphore.h>

#define DEFAULT_BUFLEN  512
#define DEFAULT_PORT    27015
#define MAX_CLIENTS     10

struct client {
    int socket_desc;
    struct sockaddr_in socket;
    char topics[50][100]; // max 50 topica sa po 100 karaktera za svaki
};

// Funkcije za akcije koje klijent ima (za pthread)
void* pub_thread(void *);
void* sub_thread(void *);
void* unsub_thread(void *);

int main(int argc , char *argv[])
{
    int socket_desc , client_sock , c;
    struct sockaddr_in server , client_sockadd_in;
    int *new_sock;
   
    //Create socket
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1)
        printf("[Error] Could not create socket!");
    puts("Socket created");

    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(DEFAULT_PORT);

    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0) {
        perror("[Error] Bind failed!");
        return 1;
    }
    puts("bind done");

    //Listen
    listen(socket_desc , 3);

    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);

    while(1){
        //accept connection from an incoming client
        client_sock = accept(socket_desc, (struct sockaddr *)&client_sockadd_in, (socklen_t*)&c);
        if (client_sock < 0) {
            perror("[Error] Accept failed!");
            return 1;
        }
        puts("Connection accepted");

        // Create thread vars, and prepare parameter
        pthread_t sub;
        new_sock = malloc(1);
        *new_sock = client_sock;
        
        if(pthread_create(&sub, NULL, sub_thread, (void *)new_sock) < 0){
            perror("[Error] Thread create failed!");
        }
    }

    return 0;
}

void* sub_thread(void *new_sock){
    int sock = *(int*)new_sock;
    int read_size;
    char client_message[DEFAULT_BUFLEN];

    // Receive logika
    while( (read_size = recv(sock , client_message , DEFAULT_BUFLEN , 0)) > 0 ){
        printf("Bytes received: %d\n", read_size);
        printf("Message received: %s\n", client_message);
    }

    if(read_size == 0) {
        puts("Client disconnected");
        fflush(stdout);
    }
    else if(read_size == -1)
        perror("[Error] Recv failed");

    // Proveriti da li je format ispravan
    // [sub -t "weather"]


    free(new_sock);
    return 0;
}