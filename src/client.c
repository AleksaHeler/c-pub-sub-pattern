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

#include"userFunctions.c"

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT   27015

pthread_t hSender;
pthread_t hReceiver;

int sock;
struct sockaddr_in server;

void ThreadDestroyer() 
{
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
    char message[4];
    int input;
    while(1)
    {   
        meni();
        scanf("%d", &input);
        
        if(checkInputMeni(input))
        {
            if(input == 100)
            {
                ThreadDestroyer();
            }

            if(input < 7)
                message[0] = 's'; //subscribe
            else 
                message[0] = 'u'; //unsubscribe

            if(input == 1 || input == 11)
                message[1] = 'w'; //weather
            else if(input == 2 || input == 22)
                message[1] = 'p'; //polution
            else
            {
                if(input == 3 || input == 33)
                    message[1] = 'y'; //years
                else if(input == 4 || input == 44)
                    message[1] = 'n'; //numbers
                else if(input == 5 || input == 55)
                    message[1] = 't'; //Trump
                else // else if(input == 6 || input == 66)
                    message[1] = 'c'; //crypto market cap
            
                message[2] = '\0';
            }

            if(input == 1 || input == 11 || input == 2 || input == 22)
            {
                while(1)
                {
                    cityMeni();
                    scanf("%d", &input);

                    if(checkInputCityMeni(input) == 1)
                    {
                        message[2] = input + 48;
                        break;
                    }
                        else
                            printf("\nInvalid input\n");
                }

                message[3] = '\0';
            }
        }
        else
        {
            puts("Invalid input");
            continue;
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

