
//  des.c
//  Des
//  Created by Guru Teja Mannava on 7/9/15.
//  Copyright (c) 2015 Guru Teja Mannava. All rights reserved.

#include <stdio.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <stdlib.h>
#include <time.h>

#include "des.h"

/* Initial Permutation Table */
static char Initial_Permutation_IP[] = {
    58, 50, 42, 34, 26, 18, 10,  2,
    60, 52, 44, 36, 28, 20, 12,  4,
    62, 54, 46, 38, 30, 22, 14,  6,
    64, 56, 48, 40, 32, 24, 16,  8,
    57, 49, 41, 33, 25, 17,  9,  1,
    59, 51, 43, 35, 27, 19, 11,  3,
    61, 53, 45, 37, 29, 21, 13,  5,
    63, 55, 47, 39, 31, 23, 15,  7
};

/* Inverse Initial Permutation Table */
static char Inverse_Initial_Permutation_PI[] = {
    40,  8, 48, 16, 56, 24, 64, 32,
    39,  7, 47, 15, 55, 23, 63, 31,
    38,  6, 46, 14, 54, 22, 62, 30,
    37,  5, 45, 13, 53, 21, 61, 29,
    36,  4, 44, 12, 52, 20, 60, 28,
    35,  3, 43, 11, 51, 19, 59, 27,
    34,  2, 42, 10, 50, 18, 58, 26,
    33,  1, 41,  9, 49, 17, 57, 25
};

/*Expansion table */
static char Expansion_table_E[] = {
    32,  1,  2,  3,  4,  5,
    4,  5,  6,  7,  8,  9,
    8,  9, 10, 11, 12, 13,
    12, 13, 14, 15, 16, 17,
    16, 17, 18, 19, 20, 21,
    20, 21, 22, 23, 24, 25,
    24, 25, 26, 27, 28, 29,
    28, 29, 30, 31, 32,  1
};

/* Post S-Box permutation */
static char Permutaion_Function_P[] = {
    16,  7, 20, 21,
    29, 12, 28, 17,
    1, 15, 23, 26,
    5, 18, 31, 10,
    2,  8, 24, 14,
    32, 27,  3,  9,
    19, 13, 30,  6,
    22, 11,  4, 25
};

/* The S-Box tables */
static char S_Boxes_S[8][64] = {{
    /* S1 */
    14,  4, 13,  1,  2, 15, 11,  8,  3, 10,  6, 12,  5,  9,  0,  7,
    0, 15,  7,  4, 14,  2, 13,  1, 10,  6, 12, 11,  9,  5,  3,  8,
    4,  1, 14,  8, 13,  6,  2, 11, 15, 12,  9,  7,  3, 10,  5,  0,
    15, 12,  8,  2,  4,  9,  1,  7,  5, 11,  3, 14, 10,  0,  6, 13
},{
    /* S2 */
    15,  1,  8, 14,  6, 11,  3,  4,  9,  7,  2, 13, 12,  0,  5, 10,
    3, 13,  4,  7, 15,  2,  8, 14, 12,  0,  1, 10,  6,  9, 11,  5,
    0, 14,  7, 11, 10,  4, 13,  1,  5,  8, 12,  6,  9,  3,  2, 15,
    13,  8, 10,  1,  3, 15,  4,  2, 11,  6,  7, 12,  0,  5, 14,  9
},{
    /* S3 */
    10,  0,  9, 14,  6,  3, 15,  5,  1, 13, 12,  7, 11,  4,  2,  8,
    13,  7,  0,  9,  3,  4,  6, 10,  2,  8,  5, 14, 12, 11, 15,  1,
    13,  6,  4,  9,  8, 15,  3,  0, 11,  1,  2, 12,  5, 10, 14,  7,
    1, 10, 13,  0,  6,  9,  8,  7,  4, 15, 14,  3, 11,  5,  2, 12
},{
    /* S4 */
    7, 13, 14,  3,  0,  6,  9, 10,  1,  2,  8,  5, 11, 12,  4, 15,
    13,  8, 11,  5,  6, 15,  0,  3,  4,  7,  2, 12,  1, 10, 14,  9,
    10,  6,  9,  0, 12, 11,  7, 13, 15,  1,  3, 14,  5,  2,  8,  4,
    3, 15,  0,  6, 10,  1, 13,  8,  9,  4,  5, 11, 12,  7,  2, 14
},{
    /* S5 */
    2, 12,  4,  1,  7, 10, 11,  6,  8,  5,  3, 15, 13,  0, 14,  9,
    14, 11,  2, 12,  4,  7, 13,  1,  5,  0, 15, 10,  3,  9,  8,  6,
    4,  2,  1, 11, 10, 13,  7,  8, 15,  9, 12,  5,  6,  3,  0, 14,
    11,  8, 12,  7,  1, 14,  2, 13,  6, 15,  0,  9, 10,  4,  5,  3
},{
    /* S6 */
    12,  1, 10, 15,  9,  2,  6,  8,  0, 13,  3,  4, 14,  7,  5, 11,
    10, 15,  4,  2,  7, 12,  9,  5,  6,  1, 13, 14,  0, 11,  3,  8,
    9, 14, 15,  5,  2,  8, 12,  3,  7,  0,  4, 10,  1, 13, 11,  6,
    4,  3,  2, 12,  9,  5, 15, 10, 11, 14,  1,  7,  6,  0,  8, 13
},{
    /* S7 */
    4, 11,  2, 14, 15,  0,  8, 13,  3, 12,  9,  7,  5, 10,  6,  1,
    13,  0, 11,  7,  4,  9,  1, 10, 14,  3,  5, 12,  2, 15,  8,  6,
    1,  4, 11, 13, 12,  3,  7, 14, 10, 15,  6,  8,  0,  5,  9,  2,
    6, 11, 13,  8,  1,  4, 10,  7,  9,  5,  0, 15, 14,  2,  3, 12
},{
    /* S8 */
    13,  2,  8,  4,  6, 15, 11,  1, 10,  9,  3, 14,  5,  0, 12,  7,
    1, 15, 13,  8, 10,  3,  7,  4, 12,  5,  6, 11,  0, 14,  9,  2,
    7, 11,  4,  1,  9, 12, 14,  2,  0,  6, 10, 13, 15,  3,  5,  8,
    2,  1, 14,  7,  4, 10,  8, 13, 15, 12,  9,  0,  3,  5,  6, 11
}};

/* Permuted Choice 1 Table */
static char Permuted_Choice_PC1[] = {
    57, 49, 41, 33, 25, 17,  9,
    1, 58, 50, 42, 34, 26, 18,
    10,  2, 59, 51, 43, 35, 27,
    19, 11,  3, 60, 52, 44, 36,
    
    63, 55, 47, 39, 31, 23, 15,
    7, 62, 54, 46, 38, 30, 22,
    14,  6, 61, 53, 45, 37, 29,
    21, 13,  5, 28, 20, 12,  4
};

/* Permuted Choice 2 Table */
static char Permuted_Choice_PC2[] = {
    14, 17, 11, 24,  1,  5,
    3, 28, 15,  6, 21, 10,
    23, 19, 12,  4, 26,  8,
    16,  7, 27, 20, 13,  2,
    41, 52, 31, 37, 47, 55,
    30, 40, 51, 45, 33, 48,
    44, 49, 39, 56, 34, 53,
    46, 42, 50, 36, 29, 32
};

/* Iteration Shift Array */
static char Left_Circular_Shift[] = {
    /* 1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16 */
    1,  1,  2,  2,  2,  2,  2,  2,  1,  2,  2,  2,  2,  2,  2,  1
};


void* DES_Algorithm(void *readparams_temp) {

    uint64_t key, input_text,cypher_text;
    int round_test;
    clock_t start;
    desparams params = *((desparams *) readparams_temp);
    key = params.key;
    input_text = params.input_text;
    start = params.start;
    round_test = params.round;
    cypher_text = params.cipher_Text;
    printf("Input: %016llx \n", input_text);
    printf("Key: %016llx \n", key);
    
    //keys
    for(long long int i = 1; i <= 1844674407370955162 ; i++ ) {
        char row, column;
        /* 28 bits */
        uint32_t left_key                  = 0;
        uint32_t right_key                 = 0;
        uint32_t s_output                  = 0;
        uint32_t f_function_res            = 0;
        uint32_t temp                      = 0;
        
        //Left 32 -bit & Right 32-bit
        uint32_t Left_text                 = 0;
        uint32_t Right_text                = 0;
        
        
        /* 48 bits */
        uint64_t sub_key[16]            =  {0};
        uint64_t s_input                =  0;
        
        /* 64 bits */
        uint64_t init_permutaion_res    = 0;
        uint64_t pre_output_final       = 0;
        uint64_t inv_init_permutaion    = 0;
        
        //caluculating keys
        uint64_t permuted_Choice_1 = 0;
        uint64_t permuted_Choice_2 = 0;
        
        //calculations on Key - permuted choice 1
        
        for(int i = 0; i < 56; i++){
            
            permuted_Choice_1 = permuted_Choice_1 << 1;
            permuted_Choice_1 = permuted_Choice_1 | ((key >> (64 - Permuted_Choice_PC1[i])) & LB64_MASK);
            
        }
        //printf("\n \n permuted choice 1 value is :%016llx", permuted_Choice_1);
        
        //left key & right key
        left_key = (uint32_t) ((permuted_Choice_1 >> 28) & 0x000000000fffffff);
        right_key = (uint32_t) ( permuted_Choice_1 & 0x000000000fffffff);
        
        //printf("\n \n left key is :%016x", left_key);
        //printf("\n \n right key is :%016x", right_key);
        
        
        /* 16 sub keys Calculation  */
        for (int i = 0; i< 16; i++) {
            
            /* key schedule */
            // shifting Ci and Di
            for (int j = 0; j < Left_Circular_Shift[i]; j++) {
                
                left_key  = (0x0fffffff & (left_key << 1))  | (0x00000001 & (left_key >> 27));
                right_key = (0x0fffffff & (right_key << 1)) | (0x00000001 & (right_key >> 27));
                
            }
            //printf("\n \n circlar shift left key is :%016x", left_key);
            //printf("\n \n circular shift right key is :%016x", right_key);
            
            
            //permuted choice 2 o/p: subkey
            permuted_Choice_2 = 0;
            
            //join left key and right key
            permuted_Choice_2 = (((uint64_t) left_key) << 28) | (uint64_t) right_key ;
            
            sub_key[i] = 0;
            
            for (int j = 0; j < 48; j++) {
                
                sub_key[i] <<= 1;
                sub_key[i]  |= (permuted_Choice_2 >> (56 - Permuted_Choice_PC2[j])) & LB64_MASK;
            }
            
            //printf("\n \n sub key is for round '%d' is :%016llx", i + 1, sub_key[i]);
        }
        
        
        
        // STEP @2
        // for rounds 1 - 16
        
        //For intial permutation calculation 64bit
        
        for(int i = 0; i < 64; i++){
            
            init_permutaion_res   = init_permutaion_res << 1;
            init_permutaion_res |= (input_text >> (64 - Initial_Permutation_IP[i])) & LB64_MASK ;
            //gives the value of text in that initial permuation position.
            //can remove LB64_MASK
        }
        
        //printf("\n \n inital_permutaion value is : %016llx",init_permutaion_res);
        
        
        //left text & right text
        Left_text = (uint32_t)(init_permutaion_res >> 32) & L64_MASK;
        Right_text = (uint64_t) init_permutaion_res & L64_MASK;
        
        //rounds
        for (int round = 0; round < round_test; round = round + 1) {
            // round 1 of DES
            // Expansion table with Right_Text
            s_input = 0;
            for (int j = 0; j< 48; j++) {
                
                s_input <<= 1;
                s_input |= (uint64_t) ((Right_text >> (32-Expansion_table_E[j])) & LB32_MASK);
                
            }
            //printf("\n \n expanded s_input for round '%d' is :%016llx", round+1, s_input);
            
            // Xor with Sub-Key Encryption
            s_input = s_input ^ sub_key[round];
            
            //printf("\n \n s_input(xor with subkey) for round '%d' is :%016llx", round+1, s_input);
            
            // S- Box calculation
            for (int j = 0; j < 8; j++) {
                
                row = (char) ((s_input & (0x0000840000000000 >> 6*j)) >> (42-6*j));
                row = (row >> 4) | (row & 0x01);
                
                column = (char) ((s_input & (0x0000780000000000 >> 6*j)) >> (43-6*j));
                
                s_output <<= 4;
                s_output |= (uint32_t) (S_Boxes_S[j][16*row + column] & 0x0f);
                
            }
            //printf("\n \n s_output from S_Box for round %d is %016x:", round + 1, s_output);
            
            //PermutaionFunction calculations
            f_function_res = 0;
            for (int j = 0; j < 32; j++) {
                
                f_function_res <<= 1;
                f_function_res |= (s_output >> (32 - Permutaion_Function_P[j])) & LB32_MASK;
                
            }
            //printf("\n \n permutation function output for round %d is %016x:", round + 1, f_function_res);
            
            // Right became left and left became right & Xor with f_function_res
            temp = Right_text;
            Right_text = Left_text ^ f_function_res;
            //printf("\n \n R1 after permutaion in round %d is %016x:", round + 1, Right_text);
            
            Left_text = temp;
            
        }
        
        //printf("\n \n left final text L is :%016x", Left_text);
        //printf("\n \n right final text R is :%016x", Right_text);
        
        //combining left and right
        uint64_t Right_temp, temp3;
        Right_temp = (uint32_t) Right_text;
        temp3 = (uint64_t)((Right_temp << 32 ))  ;//| 0xfffffffffff);
        //right_key = (uint32_t) ( permuted_Choice_1 & 0x000000000fffffff);
        Left_text = (uint32_t) (Left_text & 0x00000000ffffffff);
        
        //Right_text = (uint64_t) Right_text  >> 32  ;
        //printf("\n R' :%016llx",temp3);
        //printf("\n L' :%016x",Left_text);
        
        pre_output_final = (uint64_t) temp3 | (uint64_t) Left_text;
        
        
        //printf("\n \n final text (R L)is :%016llx", pre_output_final);
        
        //inverse invitational permutation
        for (int i = 0 ; i < 64; i++) {
            
            inv_init_permutaion <<= 1;
            inv_init_permutaion |= (pre_output_final >> (64 - Inverse_Initial_Permutation_PI[i])) & LB64_MASK;
        }
        
        
        printf("encryption result is :%016llX \n",inv_init_permutaion);
        
        //printf("Key is %016llx \n", key);
        
  
        if(inv_init_permutaion == cypher_text){
            
            printf("matched cypertext is %016llx for Key %016llx \n",cypher_text ,key);
        
            char hexaDecimal[1000];
            long int i=0;
            sprintf(hexaDecimal, "%016llx",key);
            printf("\nbinary value of key is: ");
            while(hexaDecimal[i]){
                switch(hexaDecimal[i]){
                    case '0': printf("0000"); break;
                    case '1': printf("0001"); break;
                    case '2': printf("0010"); break;
                    case '3': printf("0011"); break;
                    case '4': printf("0100"); break;
                    case '5': printf("0101"); break;
                    case '6': printf("0110"); break;
                    case '7': printf("0111"); break;
                    case '8': printf("1000"); break;
                    case '9': printf("1001"); break;
                    case 'A': printf("1010"); break;
                    case 'B': printf("1011"); break;
                    case 'C': printf("1100"); break;
                    case 'D': printf("1101"); break;
                    case 'E': printf("1110"); break;
                    case 'F': printf("1111"); break;
                    case 'a': printf("1010"); break;
                    case 'b': printf("1011"); break;
                    case 'c': printf("1100"); break;
                    case 'd': printf("1101"); break;
                    case 'e': printf("1110"); break;
                    case 'f': printf("1111"); break;
                    default:  printf("\nInvalid hexadecimal digit %c ",hexaDecimal[i]); return 0;
                }
                i++;
            }
            
            
            char *server_message = malloc(sizeof(*server_message)*(20 + 1));
            ssize_t read_size;
            server_message  = "key found at other end";
            printf("\nmessage being sent to socket is :%s \n",server_message);
            read_size = write(params.client_socket ,server_message, strlen(server_message));
            if(read_size > 0){
                printf("message sent to client \n");
                
                //end time
                char end_time[100];
                time_t now = time (0);
                strftime (end_time, 100, "%Y-%m-%d %H:%M:%S.000", localtime (&now));
                printf ("end time is %s \n", end_time);

                printf("killing threads \n");
                int pthread_kill(pthread_t thread, int sig);
                exit(0);
            };
            
        }
        else{
            printf("key not matched for cypher text:%016llX \n",key);
        }
        
        key = key + 1 ;        
    }
    
    return 0;
}


/* Getting current IP address */
char* getIP() {
    int fd;
    struct ifreq ifr;
    
    fd = socket(AF_INET, SOCK_DGRAM, 0);
    
    /* I want to get an IPv4 IP address */
    ifr.ifr_addr.sa_family = AF_INET;
    
    /* I want IP address attached to "eth0" */
    strncpy(ifr.ifr_name, "en1", IFNAMSIZ-1);
    
    ioctl(fd, SIOCGIFADDR, &ifr);
    
    close(fd);
    
    /* display result */
    char * ip_addr = inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr);
    printf("%s\n", ip_addr);
    
    return ip_addr;
}

#define Max 1000
char* DecimalToHexa(char binary_number[Max]){
    
    int temp;
    
    long int i=0,j=15;
    
    char hexaDecimal[Max] , hexaDecimal1[Max];
    
    hexaDecimal1[0] = '\0';
    
    hexaDecimal[0] = '\0';
    
    while(binary_number[i]){
        binary_number[i] = binary_number[i] - 48;
        ++i;
    }
    --i;
    
    while(i-2>=0){
        temp =  binary_number[i-3] *8 + binary_number[i-2] *4 +  binary_number[i-1] *2 + binary_number[i] ;
        if(temp > 9){
            hexaDecimal[j] = temp + 55;
            j = j-1;
        }
        else{
            hexaDecimal[j] = temp + 48;
            j = j-1;
        }
        i=i-4;
    }
    if(i ==1)
        hexaDecimal[j] = binary_number[i-1] *2 + binary_number[i] + 48 ;
    else if(i==0)
        hexaDecimal[j] =  binary_number[i] + 48 ;
    else
        --j;
    //    printf("\n hexadecimal value is %s \n",hexaDecimal);
    printf("\n Equivalent hexadecimal value: \n");
    int z = 15;
    while(z>=0){
        printf("%c",hexaDecimal[z]);
        z = z-1;
    }
    //hexaDecimal[16] = '\0';
    return hexaDecimal;
}



