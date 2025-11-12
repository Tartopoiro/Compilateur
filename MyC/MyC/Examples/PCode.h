/*
 *  PCode.h
 * 
 *  Created by Janin on 11/25.
 *  Copyright 2025 ENSEIRB. All rights reserved.
 *
 *  Le PCode présenté ici mime, via des macros C
 *  un langage d'assembleur avec pile.
 *
 *  Les concessions faites au C:
 *    - gestion du CO (conteur ordinal) via les appels de fonctions de C 
 *      sans arguements
 *    - gestion des types, alignements mémoires, conversion sur la pile
 *       via le type union en C  (avec un vrai assembleur il faudrait le 
 *       faire "à la main"...
 *
 *  La pile de PCode est donc codée comme un tableau C de valeurs typées selon 
 *  un type union qui rassemble les types des valeurs qu'on veut pouvoir stocker 
 *  sur la pile.
 * 
 *  Les "adresses" de PCode sont alors simplement codées comme des indices
 *  dans ce tableau. Autrement dit, on évite, pour la beauté du geste,
 *  d'utiliser explicitement les pointeurs C.
 *
 *  Bien sur stack[k] c'est la même chose que *(stack+k) il y a donc bien 
 *  des pointeurs C derrière notre codage du PCode en C.
 *
 *  WARNING : Codage du PCode en l'état, peut-être avec des erreurs (?) 
 *  et susceptible d'être étendu si besoin.
 *  Dans ce cas, il faudra détailler les extensions ou corrections proposées.
 *
 */


#ifndef PCODE_H
#define PCODE_H

#define SIZE 100


typedef union {
  int int_value;
  float float_value;
  int index_value;
}  stack_value;
  

extern stack_value stack[SIZE];
   

extern int sp;
     // Stack pointeur (ou index) qui pointe toujours sur la première
     // case LIBRE.
     // On empile en incrémentant sp !
     // On dépile en décrémentant sp !


extern int bp;
     // Block pointeur : un pointeur (ou index) qui marque une position fixe,
     // sur la pile, à partir de laquelle on peut retrouver les données
     // (paramètres, variables locales) propres au bloc courant.
     // A utiliser pour gerer les blocs imbriqués et les appels de fonctions.


/*************************************************/
// Instructions P-CODE, définit comme des macros C

/********** I. Empiler / depiler une valeur ***************/

// empiler 
#define LOADI(X)  stack[sp ++].int_value = X;

#define LOADF(F)  stack[sp ++].float_value = F;

// depiler n valeurs du sommet de pile
#define DROP(n)   sp = sp-n;

/********** II. Opérations arithmetiques binaires *********/

/* OPERATION UNAIRE */

#define MINUSI stack[sp-1].int_value = - stack[sp-1].int_value;
#define MINUSF stack[sp-1].float_value = - stack[sp-1].float_value;


/* CONVERSION */
#define I2F2 stack[sp-1].float_value = (float) stack[sp-1].int_value;
                   /* Conversion sommet de pile */
#define I2F1 stack[sp-2].float_value = (float) stack[sp-2].int_value;
                   /* Conversion sous sommet de pile */


/* OPERATIONS BINAIRES */

/* On dépile, on effectue le calcul
   on rempile le résultat.

   On peut ajouter des fonctions binaires de PCode si besoin.
   Ex : ==
*/

  /* on integer */
#define ADDI stack[sp - 2].int_value = stack[sp - 2].int_value + stack[sp - 1].int_value; sp--;
#define MULTI stack[sp - 2].int_value = stack[sp - 2].int_value * stack[sp - 1].int_value; sp--;
#define SUBI stack[sp - 2].int_value = stack[sp - 2].int_value - stack[sp - 1].int_value; sp--;
#define DIVI stack[sp - 2].int_value = stack[sp - 2].int_value / stack[sp - 1].int_value; sp--;

  /* on float */
#define ADDF stack[sp - 2].float_value = stack[sp - 2].float_value + stack[sp - 1].float_value; sp--;
#define MULTF stack[sp - 2].float_value = stack[sp - 2].float_value * stack[sp - 1].float_value; sp--;
#define SUBF stack[sp - 2].float_value = stack[sp - 2].float_value - stack[sp - 1].float_value; sp--;
#define DIVF stack[sp - 2].float_value = stack[sp - 2].float_value / stack[sp - 1].float_value; sp--;

/********** III. Opérations comparaison binaires *********/

  /* on integer */
#define LTI stack[sp - 2].int_value = stack[sp - 2].int_value < stack[sp - 1].int_value; sp--;
#define GTI stack[sp - 2].int_value = stack[sp - 2].int_value > stack[sp - 1].int_value; sp--;
#define EQI stack[sp - 2].int_value = stack[sp - 2].int_value == stack[sp - 1].int_value; sp--;
#define NEQI stack[sp - 2].int_value = stack[sp - 2].int_value != stack[sp - 1].int_value; sp--;
 
  /* on float */
#define LTF stack[sp - 2].int_value = (int) stack[sp - 2].float_value < stack[sp - 1].float_value; sp--;
#define GTF stack[sp - 2].int_value = (int) stack[sp - 2].float_value > stack[sp - 1].float_value; sp--;
#define EQF stack[sp - 2].int_value = (int) stack[sp - 2].float_value == stack[sp - 1].float_value; sp--;
#define NEQF stack[sp - 2].int_value = (int) stack[sp - 2].float_value != stack[sp - 1].float_value; sp--;



/********** IV. Branchements ******************/
// La condition est en sommet de pile.
// Elle est toujours dépilée.

      // branchement conditionel
#define IFT(L) if (stack[--sp].int_value) goto L;

      // branchement conditionel avec negation
#define IFN(L) if (!(stack[--sp].int_value)) goto L;

      // branchement inconditionel
#define GOTO(L) goto L;

/**********  V. NOP ************************************/

#define NOP ;


/********** V. Opérations booléennes *********/
/* codé par évaluation paresseuse ? */

/*
  // boolean op. over integer coded boolean (cas non paresseux)
#define OR stack[sp - 2].int_value = stack[sp - 2].int_value || stack[sp - 1].int_value; sp--;
#define AND stack[sp - 2].int_value = stack[sp - 2].int_value && stack[sp - 1].int_value; sp--;
#define NOT stack[sp - 1].int_value = ! stack[sp - 1].int_value;

*/

/********** VI. Arithmétique des pointeurs *********/

#define LOADBP stack[sp++].index_value = bp;

#define SHIFT(N) stack[sp-1].index_value = stack[sp-1].index_value + N;
/* Shift l'index de sommet de pile d'un offset N. 
   L'offset N pourra aussi être négatif */

#define LOAD stack[sp-1] = stack[stack[sp-1].index_value];
    /* Remplace l'index de sommet de pile par la valeur stockée à cet index */

    /* Par exemple :
       pour compiler la lecture d'une variable globale x d'offset o
       LOADI 0  // accès à une variable globale
       SHIFT(o) // offset de la variable x
                // l'index (valeur droite) de x est en sommet de pile
       LOAD     // chargement de la valeur (gauche) de x


       pour compiler la lecture d'une variable local y de depth m>0 
       et d'offset o
       dans un bloc de depth n >= m
       LOADBP // chargement du block pointer
       [LOAD] a faire n-m fois pour "remonter" dans les blocs
       SHIFT(o) // offset de la variable y
                // l'index (valeur droite) de y est en sommet de pile
       LOAD     // chargement de la valeur (gauche) de y
    */
 

#define STORE stack[stack[sp-1].index_value] = stack[sp-2]; sp=sp-2;
    /* Stocke à l'index de sommet de pile la valeur sous le sommet de pile,
       et dépile d'autant. */
    /* Par exemple : pour compiler x = 32 de type int
       LOADI 32
       [... charger l'index (valuer droite) de x dans la pile ...]
       STORE
    */

/* REMARQUE : dans ces deux dernières operations, on copie des valeurs 
   de type union. La vérification de type devra être faites à la compilation !
 */


/********** VII. Gestion des blocs et des variables locales ***/

/*  Etats de la pile souhaité pendant l'execution d'un gestion d'un bloc
    avec m variables locales  Varloc1, ....,  Varlocm

       ---------
bp+m  | VarLocm | 
...   | ....... |
...   | ....... |
bp+1  | Varloc1 |
bp    | OldBp   |  // to be restored upon exiting the block with oldSP = bp
       ---------

Autrement dit, l'adresse dans le code cible des variables locales
au block est donné, dans l'ordre, par bp+1, bp+2, ..., bp+n
Le i de la variable Varloci est appelé l'offset de cette variable.
Il devra être stocké, à la compilation, dans la table des symboles.

*/


/* ***** Entrée dans un bloc ***** 
   Sauvegarde de bp en sommet de pile et positione bp à cet endroit. 
*/
#define SAVEBP stack[sp].index_value = bp; bp = sp++;
// Remarque : il faudra après ça reserver les emplacements dans la pile
// ce qui se fera dans le traitement des déclarations locales à ce block.

/* ***** Sortir d'un bloc ***** */
#define RESTOREBP  sp = bp; \
                   bp = stack[bp].index_value;

/* Ce qui fait, dans l'ordre:
   1) restoration sp
   2) restauration bp
*/

/********* VII. Gestion des appels de fonctions ***************/

/* 
  Remarque: lors d'un appel type toto(e1,...,en) 

  1) AVANT la compilation des arguements, on veillera à réserver sur la pile
  l'emplacement necessaire au stockage du résultat de l'appel 
  (avec LOADI 0 ou LOADF 0 selon le type du résulta).

  2) la compilation des n arguments, 
  dans l'ordre, produit un code cible qui, à l'execution, revient à 
  empiler les valeurs  de e1, e2, ..., en.

  3) on  sauvergarde le block-pointer avec SAVEBP

  On appel alors sur le bloc du corp de la fonction toto compilée 
  en une fonction void pcode_toto()

*/



#define CALL(L)  L(); // L est le symbole pcode_toto dans notre exemple

/* au début de l'appel, par CALL(toto), à dire appel à la fonction 
   pcode_toto(), le stack est dans l'état suivant:

sp       |         |
          ---------
bp       | oldbp   | (positionnée par SAVEBP au début bloc de la fonction)
          ---------
bp -1    |  expn   | 
...      | ....... |
...      | ....... |
bp -n    |  exp1   |
          ---------
bp -n -1 |    0    | emplacement pour valeur de retour
          ---------
         |  Pile0  |  pile avant l'appel

On constate que les arguements de la fonction sont donc, dans l'ordre, aux index
bp -n pour exp1, bp -(n-1) pour exp2, ..., bp -1 pour expn,
l'emplacement réservé pour le résultat étant à l'index bp -(n+1)

A la fin de l'appel de CALL(pcode_toto), la pile est (ou pourrait être ;-) ) 
dans l'état suivant: (pour une fonction avec m variable locale)


sp       |         |
...      | ....... |
...      | ....... |
bp +m    | varLocm |         
...      | ....... |
...      | ....... |
bp +1    | varLoc1 |
          ---------
bp       | oldbp   | (positionnée par SAVEBP au début bloc de la fonction)
          ---------
bp -1    |  expn   | 
...      | ....... |
...      | ....... |
bp -n    |  exp1   |
          ---------
bp -n -1 |  ret_val| valeur de retour
          ---------
         |  Pile0  |  pile avant l'appel

  
On lance donc, après l'appel de  
RESTOREBP
DROP(n)

pour arriver dans l'état de pile

sp-1     |  ret_val| valeur de retour
          ---------
         |  Pile0  |  pile avant l'appel

i.e. l'appel de la fonction revient, in fine, à empiler sa valeur de retour

*/


/* 

Autrement dit, pour un appel de fonction toto(e1,...,en)
on doit produire du code qui fait, dans l'ordre
1) reservation d'un espace sur la pile pour valeur de retour
2) empilement des arguements
3) sauvergarde du bp
4) execution du code de la fonction (attention au return inclus dans 
   des sous-blocs, il faudra bien finir l'execution avec la bonne 
   valeur du bp) 
5) restoration du bp 
6) dépilement des arguements

*/

/* ATTENTION : on se simplifiera la vie
en traitant arguments de fonctions et variable locales à une fonction
comme des variables de profondeur 1, d'offset négatif pour les arguments,
d'offset positives pour les variables locales.
*/

/***************************************************/
/* Autres fonctions de "PCODE" pour le debug.
   Vous pouvez ajouter les fonctions que vous voulez !
 */

int empty_stack ();

int full_stack ();

int valid_stack ();

void print_top_stack_int();
// Imprime le sommet de pile comme un entier

void print_top_stack_float();
// Imprime le sommet de pile comme un flottant

void print_top_stack_index();
// Imprime le sommet de pile comme un index (de pile)

#endif

