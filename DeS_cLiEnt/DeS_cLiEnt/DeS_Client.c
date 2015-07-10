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

desparams params[NTHREADS];
uint64_t keys[NTHREADS];

static char *ip_address;

int main(int argc, const char * argv[]) {
    
    
    
    /* socket - client connection */
    //start client
    
    struct sockaddr_in serverAddr;
    socklen_t addr_size;
    
    
    /*---- Create the socket. The three arguments are: ----*/
    /* 1) Internet domain 2) Stream socket 3) Default protocol (TCP in this case) */
    
    
    readParams.client_socket = socket(PF_INET, SOCK_STREAM, 0);
    int set = 1;
    setsockopt(readParams.client_socket, SOL_SOCKET, SO_NOSIGPIPE, (void *)&set, sizeof(int));
    
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
    if (connect(readParams.client_socket, (struct sockaddr *) &serverAddr, addr_size) < 0){
        printf("\n not connected to server");
    }
    
    /* initialize values to all pthreads structures*/
    readParams1.client_socket = readParams.client_socket;
    readParams2.client_socket = readParams.client_socket;
    readParams3.client_socket = readParams.client_socket;
    readParams4.client_socket = readParams.client_socket;
    
    
    //ack/read from server
    ssize_t readsize = recv(readParams.client_socket, readParams.server_buffer, 20, 0);
    printf("\n message from server is %s",readParams.server_buffer);
    if(readsize > 0){
        printf("\n ack received from server");
        //key from server
        readParams.server_buffer[readsize] = '\0';
        printf("\n key found by server is : %s",readParams.server_buffer);
        printf("\n stop all threads");
        int pthread_kill(pthread_t thread, int sig);
        printf("\n killed all threads");
        printf("\n exiting from code");
    }
    
    printf("ur here2");
    
    /* thread creation */
    readParams.input_text = 0x0123456789ABCDEF;
    pthread_t threads[NTHREADS];
    int thread_args[NTHREADS];
    int rc,i;
    for( i = 0; i < NTHREADS; i++){
        printf("\n message from server is %s",readParams.server_buffer);
        if(readsize > 0){
            printf("\n ack received from server");
            //key from server
            readParams.server_buffer[readsize] = '\0';
            printf("\n key found by server is : %s",readParams.server_buffer);
            printf("\n stop all threads");
            int pthread_kill(pthread_t thread, int sig);
            printf("\n killed all threads");
            printf("\n exiting from code");
        }
        thread_args[i] = i;
        if(i == 0){
            readParams.count = 0;
            readParams.key   = 0x0000000000000050;
            rc = pthread_create(&threads[i], NULL,DES_Algorithm,(void *)&readParams);
        }
        else if(i == 1){
            readParams1.count = 1;
            readParams1.input_text = 0x0123456789ABCDEF;
            readParams1.key   = 0x0000000000000060;
            rc = pthread_create(&threads[i], NULL,DES_Algorithm,(void *)&readParams1);
        } else if(i == 2){
            readParams2.count = 2;
            readParams2.input_text = 0x0123456789ABCDEF;
            readParams2.key  = 0x000000000000070;
            rc = pthread_create(&threads[i], NULL,DES_Algorithm,(void *)&readParams2);
        } else if(i == 3){
            readParams3.count = 3;
            readParams3.input_text = 0x0123456789ABCDEF;
            readParams3.key  = 0x0000000000000080;
            rc = pthread_create(&threads[i], NULL,DES_Algorithm,(void *)&readParams3);
        } else if(i == 4){
            readParams4.key  = 0x0000000000000090;
            readParams4.input_text = 0x0123456789ABCDEF;
            readParams.count = 4;
            rc = pthread_create(&threads[i], NULL,DES_Algorithm,(void *)&readParams4);
        }
        
        
    }
    
    /* wait for threads to finish */
    for (i=0; i<NTHREADS; ++i) {
        rc = pthread_join(threads[i], NULL);
    }
    
    
    exit (0);
}






