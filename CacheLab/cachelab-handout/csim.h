#ifndef CSAPP_CSIM_H
#define CSAPP_CSIM_H
#include <getopt.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
//!functions
void printUsage();

void parseInput();

void initCache();

void parseTrace();

void cleanUp();

//!global variables
typedef struct{
    int valid;                        //valid bit     --- 1 bit
    unsigned long long tag;           //tag           --- t bits
    char* block;                      //block         --- b bits long
    int timeStamp;
} Line;

typedef struct{
    Line *lines;
} Set;

typedef struct{
    Set *sets;
} Cache;

typedef struct{
    int s;          // num of set index bits
    int S; 
    int b;          // num of block bits
    int B;
    int E;          // num of lines per set
    int verbose_flag;
    int hit;
    int miss;
    int evict;
    int argc_count;
} cacheInfo;

cacheInfo cache_info;
Cache cache;
char *trace;
#endif //CSAPP_CSIM_H