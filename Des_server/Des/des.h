
//  des.h
//  Des
//  Created by Guru Teja Mannava on 7/9/15.
//  Copyright (c) 2015 Guru Teja Mannava. All rights reserved.

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
    uint64_t key;
    uint64_t input_text,cipher_Text;
    int count;
    int client_socket;
    char server_message[10];
    char buffer[1024];
    int round;
    clock_t start;
} desparams;

//Performs DES Algorithm
void* DES_Algorithm(void *readparams_temp);

char * getIP();
char * bintohex();