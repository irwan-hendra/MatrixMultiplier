/******************************************************************
 *
 * Matrix Multiply LAB 3
 *
 * 
 *
 * Matrix Multiplication
 * Copyright Irwan Hendra ID: 91998888
 *           Xuong Thong  ID: 21434525
 *
 *
 ******************************************************************/

#define BLOCK_SIZE			32
#define NUMBER_BLOCKS		20

#define MAX					BLOCK_SIZE*NUMBER_BLOCKS
#include <stdio.h>
#define CLOCK

/******************************************************************
 *****!!! For your Assembly, you should remove this #define !!!****
 *****************************************************************/
//#define C_RUN

#include "type.h"

/******************************************************************
 *	FUNCTION:	blocked_mm()
 ******************************************************************/
#ifndef C_RUN


#endif
void blocked_mm(double *c, double *a, double *b ) { 
  
  int i,j,k;

#ifdef C_RUN
  /******************************************************************
   ******* This is one algorithm in C to solve the problem **********
   *****************************************************************/
  for (i=0;i<BLOCK_SIZE;i++)
    for (j=0;j<BLOCK_SIZE;j++)
      for (k=0;k<BLOCK_SIZE;k++)
	    c[i*MAX+j] += a[i*MAX+k]*b[k*MAX+j];
  /*****************************************************************/
#else
  __asm
	{
		// pushing registers onto stack
		push eax;
		push ebx;
		push ecx;
		push edx;
		push esi;
		push edi;

		/**************** BEGIN YOUR CODE HERE ****************/
		/**
		 * Matrix Multiplication Code
		 * Author: Loc Xoung Thong, Irwan Hendra
		 * Version: 1.3
		 * Last Modified: Dec 3, 2004 9:50 PM
		 *
		 */

		mov eax, 0               // int i = 0
FIRST_LOOP:                      //first loop goes here
		mov ebx, 0               // int j = 0

		mov edx, c               //edx is the address of c
		mov esi, eax             //esi becomes a copy of eax
		imul esi, MAX            //i*MAX 
		shl esi, 3               //total * 8 because of double value type
	    add edx, esi             //this is address of c with i*MAX added

SECOND_LOOP:                     //second loop jump goes here
    
		fld qword ptr [edx]      //load the value pointed by edx to the floating stack
		push edx                 //free edx               

		mov ecx, 0               //int k = 0

		mov edi, a               //edi is address of a
		mov edx, eax             //since edx is now free, use it as copy of ecx (i)
		imul edx, MAX            //i*MAX
		shl edx, 3               //total * 8
		add edi, edx             //edi now has the value of address a and i*MAX double
                                 //value added to it

		mov esi, b               //esi is address of b
		mov edx, ebx             //use edx again as copy of ebx (j)
	    shl edx, 3               //j * 8 make j value as double value type
		add esi, edx             //esi now has the address of b and j double value added
                                 //to it
THIRD_LOOP:                      //third loop execution
		
		
		fld qword ptr[edi]       //load the value in a array pointed by edi to the floating stack
        fld qword ptr[esi]       //load the value in b array pointed by esi to the floating stack

    	fmul                     //pop st(0) and st(1), multiply and store to st(0)
		fadd                     //pop st(0) and st(1), add and store to st(0)
        
		mov edx, MAX             //use edx as a copy of MAX
		shl edx, 3               //turns edx into double value type
		add esi, edx             //increment esi by MAX (double) for each loop

		add edi, 8               //increment edi by 8 (double) for each loop

		inc ecx                  //increment k value by 1
		
		cmp ecx, BLOCK_SIZE      // as long as ecx is smaller than BLOCK_SIZE keep looping this
    	jl THIRD_LOOP            //THIRD_LOOP
        
		inc ebx                  //increment j value by 1
		pop edx                  //retrieve the value of edx from stack
		fstp qword ptr[edx]      //write the result of MM to the memory location pointed by edx
		add edx, 8               //increment edx by 8 (double value)
		cmp ebx, BLOCK_SIZE      //as long as j is smaller than BLOCK_SIZE keep looping this
		jl SECOND_LOOP           //SECOND_LOOP

		inc eax                  //increment i value by 1
		cmp eax, BLOCK_SIZE      //as long as i is smaller than BLOCK_SIZE keep looping this
		jl FIRST_LOOP            //FIRST_LOOP
		                         //if not then just pass through this code and end automatically
		

		
		/***************** END YOUR CODE HERE *****************/
		
		// popping from the stack
		pop edi;
		pop esi;
		pop edx;
		pop ecx;
		pop ebx;
		pop eax;
	}
#endif
} /* End of blocked_mm() */


double c[MAX*MAX],a[MAX*MAX], b[MAX*MAX];

int i,j,k;

int m,n = 0; 


/******************************************************************
 *	FUNCTION:		main()
 *
 *	One Expected Output:	
 *					How many times? 4
 *					get time 22.2800
 *					OUT 2561.0000
 *					MFLOPS: 4.706355e+001
 ******************************************************************/
int main() { 

  for (i=0;i<MAX;i++) {
    for (j=0;j<MAX;j++) {
      c[i*MAX+j] = 1;
      a[i*MAX+j] = 1;
      b[i*MAX+j] = 1;
    }
  }

  n = 1;
  printf("How many times?");
  scanf("%d", &n);

  START_CLOCK;

  for (m=0; m<n;m++)
    for (i=0;i<MAX;i+=BLOCK_SIZE)
      for (j=0;j<MAX;j+=BLOCK_SIZE)
        for (k=0;k<MAX;k+=BLOCK_SIZE)
	      blocked_mm(c+i*MAX+j,a+i*MAX+k, b+k*MAX+j);
	

  END_CLOCK;
  printf("OUT %f\n", c[0]);
  printf("MFLOPS: %e\n", (((double)MAX*MAX*MAX*n)/duration)/1000000);
  
  return 0;
} /* END of main() */