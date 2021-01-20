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
#define MAX_CLIENTS     50
#define MAX_TOPICS      50

struct client {
    int socket;
    char topics[MAX_TOPICS][DEFAULT_BUFLEN];
};
struct client clients[MAX_CLIENTS];

void* ClientHandlerThread(void *);

int main(int argc , char *argv[])
{
    int socket_desc , client_sock , c, i;
    struct sockaddr_in server, cli_sockaddr_in;
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
        client_sock = accept(socket_desc, (struct sockaddr *)&cli_sockaddr_in, (socklen_t*)&c);
        if (client_sock < 0) {
            perror("[Error] Accept failed!");
            return 1;
        }
        puts("Connection accepted");

        // Create thread vars, and prepare parameter
        pthread_t sub;
        new_sock = (int*)malloc(sizeof(int));
        *new_sock = client_sock;
        
        // Try to find the client
        int new_client = 1;
        for (i = 0; i < MAX_CLIENTS; i++){
            if(clients[i].socket == client_sock){
                new_client = 0;
                printf("Client found (socket %d)\n\n", client_sock);
                break;
            }
        }

        // If no client found then create one
        if(new_client == 1){
            for (i = 0; i < MAX_CLIENTS; i++){
                if(clients[i].socket == 0){
                    clients[i].socket = client_sock;
                    printf("Created client (socket %d)\n\n", client_sock);
                    break;
                }
            }
        }

        if(pthread_create(&sub, NULL, ClientHandlerThread, (void *)new_sock) < 0){
            perror("[Error] Thread create failed!");
        }
    }

    return 0;
}

void* ClientHandlerThread(void *new_sock){
    int sock = *(int*)new_sock;
    int read_size, i, j;
    char *token, *state;
    char tokens[100][DEFAULT_BUFLEN];
    char msg[DEFAULT_BUFLEN];

    // Receive logika
    while( (read_size = recv(sock , msg , DEFAULT_BUFLEN , 0)) > 0 ){
        msg[read_size] = '\0';

        // Extract individual tokens from the message
        // pub -t "weather" -m "22C"
        i = 0;
        for (token = strtok_r(msg, " ", &state); token != NULL; token = strtok_r(NULL, " ", &state)) {
            strcpy(tokens[i++], token);
            if(strcmp(tokens[i-1], "-m") == 0){
                strcpy(tokens[i++], state);
            }
        }

        // Check what kind of request is it
        if(strcmp(tokens[0], "sub") == 0){ // SUBSCRIBE
            printf("-SUBSCRIBE-\n");
            printf("Topic: %s\n", tokens[2]);

            for (i = 0; i < MAX_CLIENTS; i++){ // Go trough clients
                if(clients[i].socket == sock){ // If it is the one that sent this request
                    printf("Found the client that sent sub request! (socket %d)\n", clients[i].socket);
                    int subscribed = 0;
                    for(j = 0; j < MAX_TOPICS; j++){ // Check if already subscribed
                        if(strcmp(clients[i].topics[j], tokens[2]) == 0){
                            printf("Client already subscribed! (topic %s)\n\n", tokens[2]);
                            subscribed = 1;
                            break;
                        }
                    }
                    if(subscribed == 0){ // If not subscribed
                        for(j = 0; j < MAX_TOPICS; j++){ // Find an empty string and set it
                            if(!(clients[i].topics[j] && clients[i].topics[j][0])){ // Is it empty?
                                printf("Subscribed! (topic %s)\n\n", tokens[2]);
                                strcpy(clients[i].topics[j], tokens[2]);
                                break;
                            }
                        }
                    }
                    break;
                }
            }
        } else if(strcmp(tokens[0], "unsub") == 0){ // UNSUBSCRIBE
            printf("-UNSUBSCRIBE-\n");
            printf("Topic: %s\n", tokens[2]);

            for (i = 0; i < MAX_CLIENTS; i++){ // Go trough clients
                if(clients[i].socket == sock){ // If it is the one that sent this request
                    printf("Found the client that sent unsub request! (socket %d)\n", clients[i].socket);
                    int subscribed = 0;
                    for(j = 0; j < MAX_TOPICS; j++){ // Check if already subscribed
                        if(strcmp(clients[i].topics[j], tokens[2]) == 0){
                            subscribed = 1;
                            break;
                        }
                    }
                    if(subscribed == 1){ // If subscribed
                        for(j = 0; j < MAX_TOPICS; j++){ // Find the topic and empty it
                            if(strcmp(clients[i].topics[j], tokens[2]) == 0){
                                printf("Unsubscribed! (topic %s)\n\n", tokens[2]);
                                clients[i].topics[j][0] = '\0';
                                break;
                            }
                        }
                    } else{
                        printf("Client was not even subscribed! (topic %s)\n\n", tokens[2]);
                    }
                    break;
                }
            }
        } else if(strcmp(tokens[0], "pub") == 0){ // PUBLISH
            // Go trough all clients and compare topics, if it is equal, send them message
            printf("-PUBLISH-\n");
            printf("Topic: %s, Message: %s\n", tokens[2], tokens[4]);

            for (i = 0; i < MAX_CLIENTS; i++){ // Go trough clients
                if(clients[i].socket != 0){ // If the client exists
                    for(j = 0; j < MAX_TOPICS; j++){ // Go trough all its topics
                        if(strcmp(clients[i].topics[j], tokens[2]) == 0){ // If it is subscribed
                            printf("Found a client subscribed to topic %s! (socket %d)\n", tokens[2], clients[i].socket);
                            
                            if(send(clients[i].socket, tokens[2], strlen(tokens[2]), 0) < 0){ //tokens[2] is set for testing, in actuall program tokens[4] is sent
                                puts("[Error] Send failed\n\n");
                            }else{
                                printf("Message sent to client %s! (socket %d)\n", tokens[2], clients[i].socket); 
                            }
                            break; // We can break, because client can be subscribed to a topic only once
                        }
                    }
                }
            }
        }
    }

    if(read_size == 0) {
        puts("Client disconnected");
        fflush(stdout);
    }
    else if(read_size == -1){
        perror("[Error] Recv failed");
    }

    free(new_sock);
    return 0;
}