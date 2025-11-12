/*
 *  PCode.c
 * 
 *  Created by Janin on 12/11/21.
 *  Copyright 2021 ENSEIRB. All rights reserved.
 *
 */


#include "PCode.h"
#include <stdio.h>

stack_value stack[SIZE];

int sp = 0; // première case libre
int bp = 0; // block pointer
int fp = 0; // frame pointer


// test pile vide, pile pleine et pile valide (pour le debug ?)

int empty_stack () {
  return sp <= 0;  // sp == 0 when empty and valid stack
}


int full_stack () {
  return sp >= SIZE;  // sp == SIZE when full and valid stack
}

int valid_stack (){
  return sp >= 0 && sp <= SIZE;
}


// imprime la pile

void print_top_stack_int()
{printf("Stack[%i] = %i (INT)\n",sp-1,stack[sp-1].int_value);}

void print_top_stack_float()
{printf("Stack[%i] = %f (FLOAT)\n",sp-1,stack[sp-1].float_value);}

void print_top_stack_index()
{printf("Stack[%i] = %i (INDEX)\n",sp-1,stack[sp-1].index_value);}


void print_top_stack_float();
// Imprime le sommet de pile comme un flottant


void test();

void test() {
  // testing macros' syntax (not to be ran)
  
  LOADI(22)
  LOADF(4.5)
  DROP(4)
  MINUSI
  MINUSF  
  I2F1
    
  ADDI
  MULTI
  SUBI
  DIVI
    
  ADDF
  MULTF
  SUBF
  DIVF
    
  LTI
  GTI
    //  LEQI
    //  GEQI
    
  EQI
  LTF
  GTF
    // LEQF
    // GEQF
  EQF
    
  IFT(toto)
  IFN(toto)
  GOTO(toto)

 toto: NOP
  LOADI(0)
  LOAD
  STORE
  SAVEBP
    
  CALL(test)

  RESTOREBP
  

  
}


