/* 
    ********************************************************************
    Odsek:          Elektrotehnika i racunarstvo
    Departman:      Racunarstvo i automatika
    Katedra:        Racunarska tehnika i racunarske komunikacije (RT-RK)
    Predmet:        Osnovi Racunarskih Mreza 1
    Godina studija: Treca (III)
    Skolska godina: 2020
    Semestar:       Zimski (V)
    
    Ime fajla:      client.c
    Opis:           TCP/IP klijent
    
    Platforma:      Raspberry Pi 2 - Model B
    OS:             Raspbian
    ********************************************************************
*/

#include<stdio.h>      //printf
#include<string.h>     //strlen
#include<sys/socket.h> //socket
#include<arpa/inet.h>  //inet_addr
#include <fcntl.h>     //for open
#include <unistd.h>    //for close

#include<stdlib.h>
#include<pthread.h>
#include<string.h>
#include<semaphore.h>

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT   27015

int sock;
struct sockaddr_in server;

void* Receiver()
{
    return 0;
}

void* Sender()
{
    //Send some data
    /*
    if( send(sock , message , strlen(message), 0) < 0)
    {
        puts("Send failed");
        return 1;
    }
    puts("Client message:");
    puts(message);
    */

    return 0;
}

/*
kada se unese nesto sa tastature treba da se pozove ova funkcija 
koja prekida niti i zavrsava program
*/
void ThreadDestroyer() 
{
    close(sock);
}

void ErrorMessage(char* message, int errorCode)
{
    printf("Error: %s\n", message);
    exit(errorCode);
}

int main(int argc , char *argv[])
{
    //Create socket
    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");

    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons(DEFAULT_PORT);

    //Connect to remote server
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("connect failed. Error");
        return 1;
    }

    puts("Connected\n");

    //Thread init

    return 0;
}