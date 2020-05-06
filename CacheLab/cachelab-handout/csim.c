#include "cachelab.h"
#include <getopt.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

void printUsage();

typedef struct{
    int valid;                        //valid bit     --- 1 bit
    unsigned long long tag;           //tag           --- t bits
    char* block;                      //block         --- b bits long
    int timeStamp;
} setLine;

typedef struct{
    setLine *lines;
} cacheSet;

typedef struct{
    cacheSet *sets;
} cache;

struct cachePara{
    int s;          // num of set index bits
    int S;
    int b;          // num of block bits
    int B;
    int E;          // num of lines per set
    int verbose_flag;
    int hit;
    int miss;
    int evict;
};

int main(int argc, char **argv)
{
    struct cachePara cache_info; 
    cache_info.verbose_flag = 0;
    char *trace;
    int input;
    while((input = getopt(argc, argv, "hvs:E:b:t:") != -1)){
        switch(input){
            case 's':
                cache_info.s = atoi(optarg);
                break;
            case 'E':
                cache_info.E = atoi(optarg);
                break;
            case 'b':
                cache_info.b = atoi(optarg); 
                break;   
            case 'v':
                cache_info.verbose_flag = 1;
                break;
             case 't':
                trace = optarg;
                break;
            case 'h':
                printUsage();
                exit(0);
            default:
                printUsage();
                exit(-1);
        }
    }
    printSummary(0, 0, 0);
    printf("%d", cache_info.verbose_flag);
    return 0;
}

void printUsage(){
    printf(
           "Usage: ./csim-ref [-hv] -s <num> -E <num> -b <num> -t <file>\n"
           "Options:\n"
           "-h         Print this help message.\n"
           "-v         Optional verbose flag.\n"
           "-s <num>   Number of set index bits.\n"
           "-E <num>   Number of lines per set.\n"
           "-b <num>   Number of block offset bits.\n"
           "-t <file>  Trace file.\n"
           "\n"
           "Examples:\n"
           "linux>  ./csim-ref -s 4 -E 1 -b 4 -t traces/yi.trace\n"
           "linux>  ./csim-ref -v -s 8 -E 2 -b 4 -t traces/yi.trace\n"
          );
}