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
    timeGlobal = 0;
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
            ((cache.sets)[i].lines)[k].timeStamp = 0;
            ((cache.sets)[i].lines)[k].valid = 0;
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
    FILE* f_p = fopen(trace, "r");
    char instruction;
    addr_t address;
    int size;
    while(fscanf(f_p, " %c %llx,%d", &instruction, &address, &size) != EOF){
        //printf("%c, %llx, %d\n", instruction, address, size);
        if(instruction == 'I')
            continue;
        else{
            int result = simulate(instruction, address);
            if(result != -1 && cache_info.verbose_flag){
                printf("%c, %llx, %d", instruction, address, size);
                if(result == 0b100) printf(" hit");
                if(result == 0b010) printf(" miss");
                if(result == 0b011) printf(" miss evict");
                if(result == 0b110) printf(" miss hit");
                if(result == 0b111) printf(" miss evict hit");
                printf("\n");
            }
        }
            
    }
    fclose(f_p);
}

int simulate(char op, addr_t address){
    int    block_l     = cache_info.b;
    int    index_l     = cache_info.s;
    int    tag_l       = 64 - block_l - index_l;
    addr_t tag         = address >> (block_l + index_l);
    addr_t index       = ((address >> block_l)<<(block_l + tag_l))>>(block_l + tag_l);
    //printf("addr  = %llx\n", address);
    //printf("tag   = %llx\n", tag);
    //printf("index = %llx\n", index);
    Set    current_set = cache.sets[index];
    switch(op){
        case 'S':
        case 'L':
            for(int i = 0; i < cache_info.E; i++){
                if(current_set.lines[i].tag == tag && current_set.lines[i].valid){
                    //! hit
                    ++cache_info.hit;
                    return 0b100;
                }
            }
            ++cache_info.miss;
            int    available_line = -1;
            int    replaced_line  = -1;
            time_t minTimeStamp   = timeGlobal + 1;
            for(int i = 0; i < cache_info.E; i++){
                if(current_set.lines[i].valid == 0){
                    available_line = i;
                    break;
                }
                if(minTimeStamp > current_set.lines[i].timeStamp){
                    minTimeStamp = current_set.lines[i].timeStamp;
                    replaced_line = i;
                }
            }
            if(available_line != -1){
                //! miss without eviction
                current_set.lines[available_line].valid = 1;
                current_set.lines[available_line].tag = tag;
                current_set.lines[available_line].timeStamp = ++timeGlobal;
                return 0b010;
            }else{
                //! miss & eviction -> evict replaced_line
                ++cache_info.evict;
                current_set.lines[replaced_line].tag = tag;
                current_set.lines[replaced_line].timeStamp = ++timeGlobal;
                return 0b011;
            }
        case 'M':
            return simulate('L', address) | simulate('S', address);
        default:
            return -1;
    }
}

int main(int argc, char **argv)
{
    parseInput(argc, argv);
    initCache();
    parseTrace();
    printSummary(cache_info.hit, cache_info.miss, cache_info.evict);
    cleanUp();
    return 0;
}