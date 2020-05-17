#ifndef CSAPP_CSIM_H
#define CSAPP_CSIM_H
#include <getopt.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

typedef unsigned long long addr_t;

typedef addr_t time_t;

//!functions
void printUsage();

void parseInput();

void initCache();

void parseTrace();

void cleanUp();

int simulate(char op, addr_t address);

//!global variables
typedef struct{
    int valid;                        //valid bit     --- 1 bit
    addr_t tag;           //tag           --- t bits
    char* block;                      //block         --- b bits long
    time_t timeStamp;
} Line;

typedef struct{
    Line *lines;
} Set;

typedef struct{
    Set *sets;
} Cache;

typedef struct{
    int s;          // num of set index bits
    int S;          // num of sets
    int b;          // num of block bits
    int B;          // num of blocks
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
time_t timeGlobal;
#endif //CSAPP_CSIM_H