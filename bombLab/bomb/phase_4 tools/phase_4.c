#include<stdio.h>
/*
 *This file is for searching for the answer of phase_4. 
 *In phase_4, a function "func4(14, 0,int x)" must return 0 (in eax)
 *It's hard to analyse based on the .asm, so this reverse .c is written to find the appropriate x 
*/
int func(int edx, int esi, int x){
    //x is the first input in phase_4
    int ecx = (edx - esi) >> 31;
    int eax = ((edx-esi)+ecx) >> 1;
    ecx = eax + esi;
    if(ecx == 0)
        return 0;
    if(ecx < x){
         esi = ecx + 0x1;
         eax = func(edx, esi, x);
         return 2*eax + 1;
    }
    else{
        edx = ecx - 1;
        eax = func(edx, esi, x);
        return 2*eax;
    }
}

//use brute force to find the needed input---x
int main(){
    for (int i = 0; i <= 14; i++){
        if(func(14, 0, i) == 0)
            printf("%d", i);
    }
    printf("%s", "finished!");
}