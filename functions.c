/* 
 * File:   functions.c
 * Author: Sharad Sinha
 * Email: sharad_sinha@pmail.ntu.edu.sg
 * Rev: 1.0
 * Release Date: July 31,2012 (Rev 1.0)
 * © Center for High Performance Embedded Systems(CHiPES),
 * N4-02A-32,School of Computer Engineering, Nanyang Avenue
 * Nanyang Technological University
 * Singapore, 639798
 * This software is released without any warranty and CHiPES and
 * the author disclaims all liabilities arising out of its usage
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"

/* strindex: return index of t in s, -1 if none */
  int strindex(char s[], char t[])
  {
  int i, j, k;
     for (i = 0; s[i] != '\0'; i++)
   {
       for (j=i, k=0; t[k]!='\0' && s[j]==t[k]; j++, k++)
	   ;
          if (k > 0 && t[k] == '\0'){
           return i;
           printf ("\n %d",i);}
     }
  return -1;
  }
  
  // Generate a random number in the integer set [A,B)
  // The following function uses the formula: rand()%(max-min)+min
  int randomNumGen(int A, int B)
{
    int max, min, random;
    if(A>B)
    {
      max = A; min = B;
    }
    else
    {
      max = B; min = A;
    }
    random = rand()%(max-min)+min;
    return random;
}

 
