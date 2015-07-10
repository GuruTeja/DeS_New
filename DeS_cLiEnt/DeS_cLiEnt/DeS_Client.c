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

//#include<unistd.h>  //write
#include "des.h"

static char *ip_address;

desparams params[NTHREADS];
uint64_t keys[NTHREADS];

void initKeys() {
    keys[0]     =   0x0000000000000050;
    keys[1]     =   0x0000000000000060;
    keys[2]     =   0x0000000000000070;
    keys[3]     =   0x0000000000000080;
    keys[4]     =   0x0000000000000090;
}

int main(int argc, const char * argv[]) {
    
    /* socket - client connection */
    //start client
    
    struct sockaddr_in serverAddr;
    socklen_t addr_size;
    
    
    /*---- Create the socket. The three arguments are: ----*/
    /* 1) Internet domain 2) Stream socket 3) Default protocol (TCP in this case) */
    int client_socket;
    client_socket = socket(PF_INET, SOCK_STREAM, 0);
    int set = 1;
    setsockopt(client_socket, SOL_SOCKET, SO_NOSIGPIPE, (void *)&set, sizeof(int));
    
    /*---- Configure settings of the server address struct ----*/
    /* Address family = Internet */
    serverAddr.sin_family = AF_INET;
    /* Set port number, using htons function to use proper byte order */
    serverAddr.sin_port = htons(9999);
    /* Set IP address to localhost */
    serverAddr.sin_addr.s_addr = inet_addr("10.205.1.170");
    /* Set all bits of the padding field to 0 */
    memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);
    
    /*---- Connect the socket to the server using the address struct ----*/
    addr_size = sizeof serverAddr;
    if (connect(client_socket, (struct sockaddr *) &serverAddr, addr_size) < 0){
        printf("\n not connected to server");
    }
    
    /* Params initialisation */
    
    initKeys();
    
    for (int i=0; i < NTHREADS; i++) {
        params[i].input_text = 0x0123456789ABCDEF;
        params[i].key = keys[i];
        params[i].client_socket = client_socket;
    }
    printf("\n client socket is : %d",params[0].client_socket);
    
    /* thread creation */
    
    pthread_t threads[NTHREADS];
    int thread_args[NTHREADS];
    int rc,i;
    
    for (int i=0; i < NTHREADS; i++) {
        thread_args[i] = i;
        rc = pthread_create(&threads[i], NULL, DES_Algorithm,(void *)&params[i]);
        if (rc != 0) {
            printf("Thread %d creation failed !", i);
        }
    }
    
    /* wait for threads to finish */
    for (i=0; i<NTHREADS; ++i) {
        rc = pthread_join(threads[i], NULL);
    }
    
    
    exit (0);
}






