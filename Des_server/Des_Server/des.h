//
//  des.h
//  Des
//
//  Created by Siva pratap reddy Devireddy on 7/9/15.
//  Copyright (c) 2015 Siva pratap reddy Devireddy. All rights reserved.
//

//socket
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define NTHREADS 5
#define LB32_MASK 0x00000001
#define LB64_MASK 0x0000000000000001
#define L64_MASK  0x00000000ffffffff
#define H64_MASK  0xffffffff00000000

typedef struct params {
    uint64_t key;  //cypher text for this key A956007AC5E6FCFE
    uint64_t input_text;
    int count;
    int client_socket;
    char server_message[10];
    char buffer[1024];
} desparams;

//Performs DES Algorithm
void* DES_Algorithm(void *readparams_temp);

char * getIP();