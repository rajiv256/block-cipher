#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#define u_int unsigned int 
#define FIRST_BIT (1<<23)
#define NROUNDS 10 
#define KEY_SIZE 24 
#define BLOCK_SIZE 32
#define HALF_BLOCK BLOCK_SIZE/2


// Permutations for each round -- Assuming 10 rounds.
u_int SboxPermutations[NROUNDS][HALF_BLOCK] = {{2, 6, 13, 4, 8, 11, 1, 15, 12, 14, 0, 5, 10, 3, 7, 9},
                            {13, 3, 7, 5, 0, 8, 15, 2, 1, 11, 6, 4, 14, 10, 9, 12},
                            {11, 6, 15, 9, 1, 0, 4, 12, 5, 2, 7, 10, 8, 13, 14, 3},
                            {5, 14, 13, 12, 6, 2, 10, 0, 9, 11, 3, 1, 4, 7, 15, 8},
                            {10, 15, 2, 13, 3, 9, 11, 1, 14, 7, 4, 8, 0, 12, 5, 6},
                            {5, 7, 14, 13, 10, 11, 12, 2, 1, 3, 0, 15, 9, 6, 4, 8},
                            {6, 4, 11, 14, 5, 0, 13, 8, 3, 15, 7, 10, 2, 9, 1, 12},
                            {11, 1, 7, 15, 9, 8, 5, 6, 0, 2, 3, 4, 13, 14, 12, 10},
                            {1, 12, 0, 8, 9, 4, 13, 14, 10, 2, 15, 7, 5, 11, 3, 6},
                            {8, 4, 3, 13, 15, 7, 5, 6, 12, 0, 14, 11, 9, 10, 1, 2}} ; 

// S-boxes for i-th round -- Assuming a total of 10 rounds.
u_int Sboxes[4][4][HALF_BLOCK] = {{{2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9},
                        {14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6},
                        {4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14},
                        {11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3}},
                        
                        {{2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9},
                        {14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6},
                        {4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14},
                        {11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3}},
                        
                        {{2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9},
                        {14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6},
                        {4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14},
                        {11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3}},
                        
                        {{2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9},
                        {14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6},
                        {4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14},
                        {11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3}}} ; 
                        
 
u_int Expansion[NROUNDS][KEY_SIZE] = {{6, 5, 12, 11, 7, 3, 1, 13, 14, 10, 4, 9, 0, 5, 8, 14, 1, 6, 13, 15, 2, 7, 15, 2},
                         {6, 5, 12, 11, 7, 3, 1, 13, 14, 10, 4, 9, 0, 5, 8, 14, 1, 6, 13, 15, 2, 7, 15, 2},
                         {6, 5, 12, 11, 7, 3, 1, 13, 14, 10, 4, 9, 0, 5, 8, 14, 1, 6, 13, 15, 2, 7, 15, 2},
                         {6, 5, 12, 11, 7, 3, 1, 13, 14, 10, 4, 9, 0, 5, 8, 14, 1, 6, 13, 15, 2, 7, 15, 2},
                         {6, 5, 12, 11, 7, 3, 1, 13, 14, 10, 4, 9, 0, 5, 8, 14, 1, 6, 13, 15, 2, 7, 15, 2},
                         {6, 5, 12, 11, 7, 3, 1, 13, 14, 10, 4, 9, 0, 5, 8, 14, 1, 6, 13, 15, 2, 7, 15, 2},
                         {6, 5, 12, 11, 7, 3, 1, 13, 14, 10, 4, 9, 0, 5, 8, 14, 1, 6, 13, 15, 2, 7, 15, 2},
                         {6, 5, 12, 11, 7, 3, 1, 13, 14, 10, 4, 9, 0, 5, 8, 14, 1, 6, 13, 15, 2, 7, 15, 2},
                         {6, 5, 12, 11, 7, 3, 1, 13, 14, 10, 4, 9, 0, 5, 8, 14, 1, 6, 13, 15, 2, 7, 15, 2},
                         {6, 5, 12, 11, 7, 3, 1, 13, 14, 10, 4, 9, 0, 5, 8, 14, 1, 6, 13, 15, 2, 7, 15, 2}} ;
                         
u_int AllKeys[NROUNDS] ; 

u_int FIRST_HALF(u_int input){
    //printf("%u\n",input >>16) ; 
    return (input >> 16) ; 
}
u_int SECOND_HALF(u_int input){
    return ((input <<16)>>16) ; 
}
u_int BIT(u_int n , int idx){
    if (n & (1<<idx)){
        return 1 ; 
    }
    return 0 ; 
}

u_int KEY_GEN() {
    u_int key = 0  ;
     
    
    int parity_bit = 0 ; int i ; 
    for (i = 0 ; i < KEY_SIZE ; i++){
        if (i%4 ==3){
            if (parity_bit)
                 key |= (FIRST_BIT >> i) ; 
            parity_bit = 0 ;
        }
        else{
            if (rand()%2 == 1){
                key += (FIRST_BIT >> i) ; 
                parity_bit = (parity_bit == 0 ? 1 : 0) ; 
            }
        }
    } 
    
    return key ; 
    
}

void GENERATE_ALL_KEYS(){
    int i ; 
    //printf("jdhfifi\n") ; 
    memset(AllKeys,0,sizeof(AllKeys)) ; 
    for(i = 0 ; i < NROUNDS ; i++){
        AllKeys[i] = KEY_GEN() ;
        //printf("%u  ",AllKeys[i] ) ;  
    }
    
    // Regenerate if weak keys are found.
    int j ; 
    for(i = 0 ; i < NROUNDS ; i++){
        for(j = i+1 ; j < NROUNDS ; j++){
            
            if (AllKeys[i] == AllKeys[j]){
                GENERATE_ALL_KEYS() ; 
            }
        }
    }
    return ; 
}



// Expansion corresponding to i-th round. 
u_int EXPAND(u_int iround , u_int right){
    u_int result = 0 ; int i ; 
    for(i = 23 ; i >= 0 ; i--){
        result |= (BIT(right,Expansion[iround][KEY_SIZE-1-i])) << i ;
    }
    return result ; 
}

// Applies the permutation pertaining to i-th round. 
u_int Permute(u_int iround, u_int input){
    u_int result = 0 ;
    int i ; 
    for(i = 0 ; i < 16 ; i++){
        result |= (BIT(input,SboxPermutations[iround][i]) << (15-i)) ; 
    }
    return result ; 
}

u_int FBOX(u_int iround, u_int right){
        printf("fboxinput -- %u\n",right);
    u_int AfterExpansion = EXPAND(iround,right) ; 
	printf("afterexpansion %u\n",AfterExpansion);
    u_int AfterRoundKey = AfterExpansion^AllKeys[iround] ; 
         printf("afterroundkey %u\n",AfterRoundKey);
    u_int SboxIn[4] ; u_int temp = AfterRoundKey ; int i ; 
    memset(SboxIn,0,sizeof(SboxIn)) ; 
    for(i = 0 ; i < 4 ; i++){
            	printf("s-boxinput %u\n",SboxIn[i]);
        int j ; 
        for(j =0 ; j< 6 ; j++){
            SboxIn[3-i] |= (temp&1) << j ; temp >> 1 ; 
        }  
   	  
    }
    u_int SboxOut[4] ; 
    memset(SboxOut,0,sizeof(SboxOut)) ; 
   
	for(i=0;i<4;i++){
    	printf("s-boxinput %u\n",SboxIn[i]);
    }
    
    for(i = 0 ; i < 4 ; i++){
        
        int x = 0 ; x += (SboxIn[i] &1) ; x += 2*((SboxIn[i] & (1<< 5)) >>5) ; 
       
        //printf("%d %u\n",x,SboxIn[i]) ; 
        u_int y = 0 ;  
        u_int j ; 
        for(j = 1 ; j < 5 ; j++){
            y |= (SboxIn[j] &(1<<j)) ; 
        }  
        y >> 1; 
        SboxOut[i] = Sboxes[i][x][y] ;  
   		    	printf("s-boxoutput %u\n",SboxOut[i]);
    }
    u_int AfterSbox = 0 ; 
    for(i = 0 ; i < 4 ; i++){
        AfterSbox |= SboxOut[i] ; 
        AfterSbox = AfterSbox << 4 ; 
    }
    	printf ("After s-box %u\n",AfterSbox) ;    
    //printf("After Sb : %u\n",AfterSbox) ; 
    u_int AfterPermutation = Permute(iround,AfterSbox) ;
       printf("After permutation %u\n",AfterPermutation);
    return AfterPermutation ; 
     
}



u_int DES_ENCRYPT (u_int input, int nrounds){
    u_int l = FIRST_HALF(input) ; 
    u_int r = SECOND_HALF(input) ;
    u_int i ; 
     printf("leftpart -- %u\n",l);
    printf("rightpart -- %u\n",r);
    for(i = 0 ; i < nrounds ; i++){
        
        u_int ltemp = l ; u_int rtemp = r ;
        ltemp = r ;
        rtemp = l ^ FBOX(i,r) ; 
        
        l = ltemp ;
        r = rtemp ;  
	printf("leftpart -- %u\n",l);
        printf("rightpart -- %u\n",r);
                printf("After one round %u\n",(l<<16)+r);
    } 

    return ((l<<16)+r) ; 
 } 
                         
int main(){
    
     
    memset(AllKeys,0,sizeof(AllKeys)) ; 
    GENERATE_ALL_KEYS() ; 
    
    FILE* fk = fopen("keys.txt","w");
    FILE* fp = fopen("alice.txt","rb") ; 
    FILE* fw = fopen("bob.txt","w") ; 
    int z;
    for(z=0;z<NROUNDS;z++){
    	fprintf(fk,"%u\n",AllKeys[z]);
    } 
    fclose(fk);
    
    fseek(fp,0,SEEK_END) ; 
    long FILE_SIZE = ftell(fp) ; 
    rewind(fp) ; 
    char* s = (char*)malloc(sizeof(char)*(FILE_SIZE+2)) ; 
    fread(s,sizeof(char),FILE_SIZE,fp) ; 
    fclose(fp) ;   
    int SIZE = (FILE_SIZE/4)*4  ; 
    
    int i = 0 ; 
    for(i = 0 ; i < SIZE ; ){
        u_int block = 0 ; 
        block += s[i] ; block = block << 8 ;
        block += s[i+1] ; block = block << 8 ;
        block += s[i+2] ; block = block << 8 ;
        block += s[i+3] ; //block = block << 8 ;
        u_int encrypted = DES_ENCRYPT(block,NROUNDS) ;
          		printf("input %u\n",block);
        fprintf(fw,"%u\n",encrypted) ; 
        printf("output %u\n\n",encrypted);
        i += 4 ; 
        
    }
    fclose(fw) ; 
    
    return 0 ; 
}
                         
