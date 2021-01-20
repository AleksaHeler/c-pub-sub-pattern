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

#include"userFunctions.h"

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT   27015

pthread_t hSender;
pthread_t hReceiver;

int sock;
struct sockaddr_in server;

int publishers[11];
int timer_finished = 0;
int continue_recv = 1;
int check_recv = 0;

void ThreadDestroyer() 
{
    printf("Canceling threads...\n");
    pthread_cancel(hSender);
    pthread_cancel(hReceiver);
    close(sock);
    exit(1);
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
        if(continue_recv == 1)
        {
            message[read_size] = '\0';
            printf("Received message %s\n", message);
            if(strcmp(message, "\"crypto\"") == 0)
                publishers[0] = 1;
            else if(strcmp(message, "\"numbers\"") == 0)
                publishers[1] = 1;
            else if(strcmp(message, "\"pollution/belgrade\"") == 0)
                publishers[4] = 1;
            else if(strcmp(message, "\"pollution/novisad\"") == 0)
                publishers[5] = 1;
            else if(strcmp(message, "\"pollution/kragujevac\"") == 0)
                publishers[6] = 1;   
            else if(strcmp(message, "\"weather/belgrade\"") == 0)
                publishers[7] = 1;
            else if(strcmp(message, "\"weather/novisad\"") == 0)
                publishers[8] = 1;
            else if(strcmp(message, "\"weather/kragujevac\"") == 0)
                publishers[9] = 1;
            else if(strcmp(message, "\"weather/nice\"") == 0)
                publishers[10] = 1;
            else if(strcmp(message, "\"trump\"") == 0)
                publishers[2] = 1;
            else if(strcmp(message, "\"years\"") == 0)
                publishers[3] = 1;
        }
        else
        {
            check_recv = 1;
        }
    }
    
    if(read_size == 0)
        ErrorMessage("Client disconnected");
    else if(read_size == -1)
        ErrorMessage("recv failed");

    ThreadDestroyer();

    return 0;
}

void* timerThread()
{
    timer_finished = 0;
    sleep(30); //30 seconds
    timer_finished = 1;
    return 0;
}

void* Sender()
{
    char message[22][DEFAULT_BUFLEN];
       
    sprintf(message[0], "%s", "sub -t \"crypto\"");
    sprintf(message[1], "%s", "sub -t \"numbers\"");
    sprintf(message[2], "%s", "sub -t \"trump\"");
    sprintf(message[3], "%s", "sub -t \"years\"");

    sprintf(message[4], "%s", "sub -t \"pollution/belgrade\"");
    sprintf(message[5], "%s", "sub -t \"pollution/novisad\"");
    sprintf(message[6], "%s", "sub -t \"pollution/kragujevac\"");
    sprintf(message[7], "%s", "sub -t \"weather/belgrade\"");
    sprintf(message[8], "%s", "sub -t \"weather/novisad\"");
    sprintf(message[9], "%s", "sub -t \"weather/kragujevac\"");
    sprintf(message[10], "%s", "sub -t \"weather/nice\"");
    

    for(int i = 0; i<11; i++)
    {
        publishers[i] = 0;
        printf("%s\n", message[i]);

        sleep(1);
        //Send data
        if( send(sock , message[i] , strlen(message[i]), 0) < 0)
        {
            ErrorMessage("Send failed");
            ThreadDestroyer();
        }  
    }

    pthread_t hTimerThread;
    pthread_create(&hTimerThread, NULL, timerThread, 0);

    while(timer_finished == 0);

    for(int i = 0; i<11; i++)
    {
        if(publishers[i] == 0)
        {
            printf("Didn't receive messages from all subs: %d\n", i);
            ThreadDestroyer();
        }
    }
    
    sprintf(message[11], "%s", "unsub -t \"crypto\"");
    sprintf(message[12], "%s", "unsub -t \"numbers\"");
    sprintf(message[13], "%s", "unsub -t \"pollution/belgrade\"");
    sprintf(message[14], "%s", "unsub -t \"pollution/novisad\"");
    sprintf(message[15], "%s", "unsub -t \"pollution/kragujevac\"");
    sprintf(message[16], "%s", "unsub -t \"weather/belgrade\"");
    sprintf(message[17], "%s", "unsub -t \"weather/novisad\"");
    sprintf(message[18], "%s", "unsub -t \"weather/kragujevac\"");
    sprintf(message[19], "%s", "unsub -t \"weather/nice\"");
    sprintf(message[20], "%s", "unsub -t \"trump\"");
    sprintf(message[21], "%s", "unsub -t \"years\"");

    printf("\nReceived messages from all subs\n");
    
    for(int i = 11; i<22; i++)
    {
        printf("%s\n", message[i]);

        sleep(1);
        //Send data
        if( send(sock , message[i] , strlen(message[i]), 0) < 0)
        {
            ErrorMessage("Send failed");
            ThreadDestroyer();
        }  
    }

    sleep(5);
    continue_recv = 0;
    pthread_create(&hTimerThread, NULL, timerThread, 0);

    if(check_recv == 1)
    {
        printf("unsub error");
    }
    else
        printf("ALL TESTS PASSED SUCCESSFULLY\n");

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

    //pthread_join(hSender, 0);
    //pthread_join(hReceiver, 0);

    sleep(120);

    return 0;
}
