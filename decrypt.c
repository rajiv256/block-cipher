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
                        

//expansion keys 
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
                         
//keys for 10 rounds
u_int AllKeys[NROUNDS] ; 

u_int FIRST_HALF(u_int input){
    //printf("%u\n",input >>16) ; 
    return (input >> 16) ; 
}

u_int SECOND_HALF(u_int input){
    return ((input <<16)>>16) ; 
}

//returns the ith bit
u_int BIT(u_int n , int idx){
    
    if (n & (1<<idx)){
        return 1 ; 
    }
    return 0 ; 
}


void GENERATE_ALL_KEYS(){
    
   FILE *file = fopen ( "keys.txt", "r" );
   int i=0;
   char *ptr;
   if ( file != NULL )
   {
      char line [ 128 ]; /* or other suitable maximum line size */
      while ( fgets ( line, sizeof line, file ) != NULL ) /* read a line */
      {
         AllKeys[NROUNDS-i-1]=strtoul(line,&ptr,10);
      	 i++;
      }
      fclose ( file );
   }
   else
   {
      printf("Error opening file\n");
      exit(1); /* why didn't the file open? */
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
    //expansion 
    u_int AfterExpansion = EXPAND(iround,right) ; 
	
	printf("afterexpansion %u\n",AfterExpansion);
    
    //xoring with roundkey
    u_int AfterRoundKey = AfterExpansion^AllKeys[iround] ; 
    printf("afterroundkey %u\n",AfterRoundKey);
     
    u_int SboxIn[4] ; u_int temp = AfterRoundKey ; int i ; 
    memset(SboxIn,0,sizeof(SboxIn)) ; 
    
    //storing inputs of sbox in sboxin
    for(i = 0 ; i < 4 ; i++){
    
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
    //storing the outputs of sbox in sboxout
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
    u_int AfterPermutation = Permute(9-iround,AfterSbox) ;
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
        rtemp = l ;
        ltemp = r ^ FBOX(i,l) ; 
        
        l = ltemp ;
        r = rtemp ;  
        printf("leftpart -- %u\n",l);
        printf("rightpart -- %u\n",r);
    
    } 
    
    return ((l<<16)+(r<<0)) ; 
} 
                         

int main(){
    
    //initializing all the keys to 0 
    memset(AllKeys,0,sizeof(AllKeys)) ; 
    GENERATE_ALL_KEYS() ; 
    
    int z;
    for(z=0;z<10;z++){
    	printf("key %d %u\n ",z,AllKeys[z]);
    }
    
    FILE* fp = fopen("bob.txt","rb") ; 
    FILE* fw = fopen("malice.txt","w") ; 
     
    if (fp == NULL)
	{
    	printf("Error opening plaintext file!\n");
    	exit(1);
	}

	if (fw == NULL)
	{
    	printf("Error opening ciphertext file!\n");
    	exit(1);
	}
	
     char line [ 128 ]; /* or other suitable maximum line size */
 
     
    while ( fgets ( line, sizeof line, fp ) != NULL ) /* read a line */
    {
        //printf("%s\n",line);
        u_int block = 0 ;
        block=atoi(line);
  		printf("input %u\n",block);
  		u_int encrypted = DES_ENCRYPT(block,NROUNDS) ;
        printf("output %u\n\n",encrypted);
        
         	
        u_int num = encrypted;
		char c[4] ;
		
		
		c[3] = (int)(num <<24)>>24 ;
		//fprintf(fw,"%c\n",c[3]);
		num = encrypted ; 
		c[2] = (int)((num>>8)<<24)>>24 ; 
		num = encrypted ; 
		c[1] = (int)((num>>16)<<24)>>24 ; 
		num = encrypted ; 
		c[0] = (int)((num>>24)<<24)>>24 ;  

		printf("&&&&&&&&&&&\n%c\n %u &&&&&&&&",c[3],num );
        fprintf(fw,"%s",c) ; 
    	
    }
    fclose ( fw );
    
     
    fclose(fp);
    return 0 ; 
}
                         
