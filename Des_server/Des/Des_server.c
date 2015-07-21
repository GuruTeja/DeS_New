//
//  main.c
//  DES_Algorithm
//
//  Created by Guru Teja Mannava on 7/3/15.
//  Copyright (c) 2015 Guru Teja Mannava. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <pthread.h>
#include <sys/fcntl.h>
#include <sys/time.h>

#include "des.h"

static char *ip_address;

desparams params[NTHREADS];
uint64_t keys[NTHREADS];
uint64_t Input_Text;
uint64_t Cypher_Text;
void initKeys() {
    keys[0]     =   0x0000000000000000;
    keys[1]     =   0x199999999999999A;
    keys[2]     =   0x3333333333333334;
    keys[3]     =   0x4CCCCCCCCCCCCCCE;
    keys[4]     =   0x6666666666666668;
}

void * handleHostMessages(void * args) {
    //clock_t start, end;
   // start = clock();
    struct timeval tv;
    gettimeofday(&tv,NULL);
    
    char *msg = malloc(20);
    int clientsock = *((int *)args);
    printf("Thread :: Sock is %d \n", clientsock);
    while (1) {
        ssize_t size = recv(clientsock, msg, 2000, 0);
        msg[size] = '\0';
        if (size > 0) {
            printf("Thread :: %ld bits - %s \n", size, msg);
            printf("killing threads \n");
            struct timeval tv;
            gettimeofday(&tv,NULL);
            int pthread_kill(pthread_t thread, int sig);
            exit(0);
        }
    }
}
#define MAX 1000
int main(int argc, const char * argv[]) {
    
    /* enter inputs here */
    char binaryNumber[MAX],hexaDecimal[MAX];
    char *in;
    long int i=0,j=15;
    int temp;
    in = malloc(sizeof(in)*MAX);
    
    printf("Enter binary number: ");
    scanf("%s",binaryNumber);
    
    while(binaryNumber[i]){
        binaryNumber[i] = binaryNumber[i] -48;
        ++i;
    }
    
    --i;
    while(i-2>=0){
        temp =  binaryNumber[i-3] *8 + binaryNumber[i-2] *4 +  binaryNumber[i-1] *2 + binaryNumber[i] ;
        if(temp > 9){
            hexaDecimal[j] = temp + 55;
            j=j-1;
        }
        else{
            hexaDecimal[j] = temp + 48;
            j=j-1;
        }
        i=i-4;
    }
    
    if(i ==1)
        hexaDecimal[j] = binaryNumber[i-1] *2 + binaryNumber[i] + 48 ;
    else if(i==0)
        hexaDecimal[j] =  binaryNumber[i] + 48 ;
    else
        j++;
    
    printf("Equivalent hexadecimal value: ");
    int z = 15;
    while(z>=0){
        printf("%c",hexaDecimal[z]);
        z = z-1;
    }
    
    in = hexaDecimal;
    Input_Text = strtoull(in, (char **)NULL, 16);
    printf("\n Input text is : %016llx", Input_Text);
    

    
    //for cipher
    char binaryNumber1[MAX],hexaDecimal1[MAX];
    char *in1;
    long int i1=0,j1=15;
    int temp1;
    in1 = malloc(sizeof(in)*MAX);
    
    printf("\n Enter cipher text number: ");
    scanf("%s",binaryNumber1);
    
    while(binaryNumber1[i1]){
        binaryNumber1[i1] = binaryNumber1[i1] -48;
        ++i1;
    }
    
    --i1;
    while(i1-2>=0){
        temp1 =  binaryNumber1[i1-3] *8 + binaryNumber1[i1-2] *4 +  binaryNumber1[i1-1] *2 + binaryNumber1[i1] ;
        if(temp1 > 9){
            hexaDecimal1[j1] = temp1 + 55;
            j1=j1-1;
        }
        else{
            hexaDecimal1[j1] = temp1 + 48;
            j1=j1-1;
        }
        i1=i1-4;
    }
    
    if(i1 ==1)
        hexaDecimal1[j1] = binaryNumber1[i1-1] *2 + binaryNumber1[i1] + 48 ;
    else if(i1==0)
        hexaDecimal1[j1] =  binaryNumber1[i1] + 48 ;
    else
        j1++;
    
    printf("Equivalent hexadecimal value: ");
    int z1 = 15;
    while(z1>=0){
        printf("%c",hexaDecimal1[z1]);
        z1 = z1-1;
    }
    
    in1 = hexaDecimal1;
    Cypher_Text = strtoull(in1, (char **)NULL, 16);
    printf("\n cypher text is : %016llx", Cypher_Text);
    
    
    int round_input;
    printf("\n no of rounds:");
    scanf("%d",&round_input);
    
    clock_t start1;
    start1 = clock();
    
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
        printf("Listening \n");
    else
        printf("Error \n");
    
    /* Accept call creates a new socket for the incoming connection */
    addr_size = sizeof client_addr;
    int clientsock = accept(welcomeSocket,
                           (struct sockaddr *) &client_addr,
                           &addr_size);
    
    if (clientsock < 0) {
        printf("Client not accepted \n");
        exit(0);
    } else {
        printf("Accepted Client \n");
    }
    
    
    //creating a thread for listing from client all time of process
    pthread_t sockThread;
    int res = pthread_create(&sockThread, NULL, handleHostMessages,(void *)&clientsock);

    
    /* Params initialisation */
    
    initKeys();

    for (int i=0; i < NTHREADS; i++) {
        params[i].input_text = Input_Text;
        params[i].key = keys[i];
        params[i].client_socket = clientsock;
        params[i].start = start1;
        params[i].round = round_input;
        params[i].cipher_Text= Cypher_Text;
    }
    
    printf("client socket is : %d \n",params[0].client_socket);
    
    /* thread creation */
    
    pthread_t threads[NTHREADS];
    int rc;
    
    for (int i=0; i < 1; i++) {
        rc = pthread_create(&threads[i], NULL, DES_Algorithm,(void *)&params[i]);
        if (rc != 0) {
            printf("Thread %d creation failed ! \n", i);
        }
    }
    
    /* wait for threads to finish */
    for (int i=0; i < 1; ++i) {
        rc = pthread_join(threads[i], NULL);
    }
    
    
    printf("== Press any key to exit ==");
    return getchar();
}
