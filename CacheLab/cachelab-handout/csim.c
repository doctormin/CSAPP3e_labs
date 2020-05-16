#include "cachelab.h"
#include "csim.h"
/*
 * Yimin Zhao (a.k.a. 赵一民)
 * 518030910188
 */

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

void parseInput(int argc, char **argv){
    cache_info.verbose_flag = 0;
    cache_info.argc_count = 0;
    int input;
    while((input = getopt(argc, argv, "hvs:E:b:t:")) != -1){
        switch(input){
            case 'h':
                printUsage();
                exit(0);
            case 's':
                ++cache_info.argc_count;
                cache_info.s = atoi(optarg);
                break;
            case 'E':
                ++cache_info.argc_count;
                cache_info.E = atoi(optarg);
                break;
            case 'b':
                ++cache_info.argc_count;
                cache_info.b = atoi(optarg); 
                break;   
            case 'v':
                cache_info.verbose_flag = 1;
                break;
            case 't':
                ++cache_info.argc_count;
                trace = optarg;
                break;
            default:
                printUsage();
                exit(-1);
        }
    }
    //The following code handles cases where the number of parameters is insufficient
    switch(cache_info.argc_count){
        case 4:
            break;
        default:
            printUsage();
            exit(-1);
    }
}

void initCache(){
    cache_info.S = pow(2, cache_info.s);
    cache_info.B = pow(2, cache_info.b);
    cache_info.miss = 0;
    cache_info.hit = 0;
    cache_info.evict = 0;
    cache.sets = (Set*) calloc (cache_info.S, sizeof(Set));
    for(int i = 0; i < cache_info.S; i++){
        (cache.sets)[i].lines = (Line*) calloc (cache_info.E, sizeof(Line));
        for(int k = 0; k < cache_info.E; k++){
            ((cache.sets)[i].lines)[k].block = (char*) calloc (cache_info.B, sizeof(char));
        }
    }
}

void cleanUp(){
    for(int i = 0; i < cache_info.S; i++){  
        for(int k = 0; k < cache_info.E; k++){
            free(((cache.sets)[i].lines)[k].block);
        }
        free((cache.sets)[i].lines);
    }
    free(cache.sets);
}

void parseTrace(){

}
int main(int argc, char **argv)
{
    parseInput(argc, argv);
    initCache();
    parseTrace();

    printSummary(cache_info.hit, cache_info.miss, cache_info.evict);
    printf("%d", cache_info.verbose_flag);
    cleanUp();
    return 0;
}