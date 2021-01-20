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

#include "userFunctions.h"

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT   27015

pthread_t hSender;
pthread_t hReceiver;

int sock;
struct sockaddr_in server;

void ThreadDestroyer() 
{
    printf("Canceling threads...\n");
    pthread_cancel(hSender);
    pthread_cancel(hReceiver);
    close(sock);
}

void ErrorMessage(char* message)
{
    printf("Error: %s\n", message);
}

void* Receiver()
{
    int read_size;
    char message[DEFAULT_BUFLEN];
    while( (read_size = recv(sock , message , DEFAULT_BUFLEN , 0)) > 0 )
    {
        message[read_size] = '\0';
        printf("%s\n", message);   
    }
    
    if(read_size == 0)
        ErrorMessage("Client disconnected");
    else if(read_size == -1)
        ErrorMessage("recv failed");

    ThreadDestroyer();

    return 0;
}

void* Sender()
{
    char message[DEFAULT_BUFLEN];
    int input1, input2;
    char str1[DEFAULT_BUFLEN/3 - 1];
    char str2[DEFAULT_BUFLEN/3 - 1];
    char str3[DEFAULT_BUFLEN/3 - 1];
    char ch;
    int brojac;

    while(1)
    {   
        input1 = meni(); //exit, sub, unsub, publish
        printf("\n");

        if(input1 == 0) //exit
            ThreadDestroyer();
    
        if(input1 == 1 || input1 == 2) //sub, unsub
        {
            input2 = meniTopic();
            printf("\n");

            if(input2 == 0) //exit
                ThreadDestroyer();

            inputToString(input1, str1);
            inputTopicToString(input2, str2);

            if(input2 == 3 || input2 == 5) //if city needs to be choosen
            {
                input2 = meniCity();

                if(input2 == 0) //exit
                    ThreadDestroyer();

                inputCityToString(input2, str3);

                sprintf(message, "%s%s%s%c", str1, str2, str3, 0);
            }
            else
                sprintf(message, "%s%s%c", str1, str2, 0);
        }
        else
        {
            printf("Publish something on your own\n");
            printf("It should be in the following format: pub -t \"topic_name\" -m \"message\"\n");
            
            brojac = 0;
            ch = 0;
            getchar(); //clear '\n'
            while( (ch = getchar()) != '\n')
            {
                message[brojac] = ch;
                if(++brojac == DEFAULT_BUFLEN)
                {
                    ErrorMessage("BufferOverflow");
                    continue;
                }
            }

            message[brojac] = '\0';
        }

        //Send data
        if( send(sock , message , strlen(message), 0) < 0)
        {
            ErrorMessage("Send failed");
            ThreadDestroyer();
        }
    }

    return 0;
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

    //Thread create
    pthread_create(&hSender, NULL, Sender, 0);
    pthread_create(&hReceiver, NULL, Receiver, 0);

    pthread_join(hSender, 0);
    pthread_join(hReceiver, 0);

    return 0;
}