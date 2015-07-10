//
//  main.c
//  DES_Algorithm
//
//  Created by Siva pratap reddy Devireddy on 7/3/15.
//  Copyright (c) 2015 Siva pratap reddy Devireddy. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <pthread.h>
#include <sys/fcntl.h>

#include "des.h"

/* text */
//static uint64_t input_text = 0x0123456789ABCDEF;

static char *ip_address;

desparams params[NTHREADS];
uint64_t keys[NTHREADS];

void initKeys() {
    keys[0]     =   0x0000000000000000;
    keys[1]     =   0x0000000000000010;
    keys[2]     =   0x0000000000000020;
    keys[3]     =   0x0000000000000030;
    keys[4]     =   0x0000000000000040;
}

int main(int argc, const char * argv[]) {
    
    ip_address = getIP();   //Fetch the current system IP Address
    
    /* socket - client connection */
    
    int welcomeSocket;
    struct sockaddr_in serverAddr;
    struct sockaddr client_addr;
    socklen_t addr_size;
    
    /*---- Create the socket. The three arguments are: ----*/
    welcomeSocket = socket(PF_INET, SOCK_STREAM, 0);
    int set = 1;
    setsockopt(welcomeSocket, SOL_SOCKET, SO_NOSIGPIPE, (void *)&set, sizeof(int));
    
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(9999);
    serverAddr.sin_addr.s_addr = inet_addr(ip_address);
    memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);
    
    bind(welcomeSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));
    
    if(listen(welcomeSocket, 5) == 0)
        printf("Listening\n");
    else
        printf("Error\n");
    
    /*---- Accept call creates a new socket for the incoming connection ----*/
    addr_size = sizeof client_addr;
    int clientsock = accept(welcomeSocket,
                           (struct sockaddr *) &client_addr,
                           &addr_size);
    
    if (clientsock < 0) {
        printf("Client not accepted");
        exit(0);
    } else {
        printf("Accepted Client");
        
        if(fcntl(clientsock, F_GETFL) & O_NONBLOCK) {
            printf("socket is non-blocking");
        } else {
            printf("socket is blocking");
        }
    }
    
    /* Params initialisation */
    
    initKeys();
    
    for (int i=0; i < NTHREADS; i++) {
        params[i].input_text = 0x0123456789ABCDEF;
        params[i].key = keys[i];
        params[i].client_socket = clientsock;
    }
    
    printf("\n client socket is : %d",params[0].client_socket);
    
    /* thread creation */
    
    pthread_t threads[NTHREADS];
    int rc,i;
    
    for (int i=0; i < NTHREADS; i++) {
        rc = pthread_create(&threads[i], NULL, DES_Algorithm,(void *)&params[i]);
        if (rc != 0) {
            printf("Thread %d creation failed !", i);
        }
    }
    
    /* wait for threads to finish */
    for (i=0; i < NTHREADS; ++i) {
        rc = pthread_join(threads[i], NULL);
    }
    
    printf("== Press any key to exit ==");
    return getchar();
}
