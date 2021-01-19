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

#include<stdio.h>
#include<string.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write
#include<pthread.h>

#define DEFAULT_BUFLEN 1024
#define DEFAULT_PORT   27015

int brojac = 0;
int socket_desc, client_sock[2] , c , read_size;
    struct sockaddr_in server , client[2];
    char client_message[DEFAULT_BUFLEN];

void* Accepter()
{
    //accept connection from an incoming client
    while(brojac != 2)
    {
        client_sock[brojac] = accept(socket_desc, (struct sockaddr *)&client[brojac], (socklen_t*)&c);
        if (client_sock[brojac] < 0)
        {
            perror("accept failed");
            return 0;
        }
        puts("Connection accepted");
        brojac++;
    }

    return 0;
}

int main(int argc , char *argv[])
{
   
    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");

    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(DEFAULT_PORT);

    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        //print the error message
        perror("bind failed. Error");
        return 1;
    }
    puts("bind done");

    //Listen
    listen(socket_desc , 3);

    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);

    pthread_t hAccepter;
    pthread_create(&hAccepter, NULL, Accepter, 0);
    pthread_join(hAccepter, 0);

    //Receive a message from client
    while( (read_size = recv(client_sock[1], client_message , DEFAULT_BUFLEN , 0)) > 0 )
    {
        printf("Bytes received: %d\n", read_size);
        client_message[read_size] = '\0';
        printf("%s\n", client_message);

        if( send(client_sock[0], client_message , strlen(client_message), 0) < 0)
        {
            puts("Send failed");
        }
    }
    
    if(read_size == 0)
    {
        puts("Client disconnected");
        fflush(stdout);
    }
    else if(read_size == -1)
    {
        perror("recv failed");
    }

    return 0;
}

