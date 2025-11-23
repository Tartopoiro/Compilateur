%{


#include "Table_des_symboles.h"

#include <stdio.h>
#include <stdlib.h>
  
extern int yylex();
extern int yyparse();

void yyerror (char* s) {
  printf ("%s\n",s);
  exit(0);
  }
		
int depth=0; // depth actuelle
int offset=0; // offset actuel
int offset_stack[256]={0}; // liste pour stocker l'offset max pour chaque depth : offset_stack[depth] contient l'offset max à depth
int offset_sp=0; // pointeur pour mémoriser où on en est dans offset_stack
  

%}

%union { 
  struct ATTRIBUTE * symbol_value;
  char * string_value;
  int int_value;
  float float_value;
  int type_value;
  int label_value;
  int offset_value;
}

%token <int_value> NUM
%token <float_value> DEC


%token INT FLOAT VOID

%token <string_value> ID
%token AO AF PO PF PV VIR
%token RETURN  EQ
%token <label_value> IF ELSE WHILE

%token <label_value> AND OR NOT DIFF EQUAL SUP INF
%token PLUS MOINS STAR DIV
%token DOT ARR

%nonassoc IFX
%left OR                       // higher priority on ||
%left AND                      // higher priority on &&
%left DIFF EQUAL SUP INF       // higher priority on comparison
%left PLUS MOINS               // higher priority on + - 
%left STAR DIV                 // higher priority on * /
%left DOT ARR                  // higher priority on . and -> 
%nonassoc UNA                  // highest priority on unary operator
%nonassoc ELSE


%{
char * type2string (int c) {
  switch (c)
    {
    case INT:
      return("int");
    case FLOAT:
      return("float");
    case VOID:
      return("void");
    default:
      return("type error");
    }  
};

int expressionPrinter(int t1, char * op, int t2){
  //Si les types different alors float obligatoire
  if(t1 != t2){
    if(t1 != INT){
      printf("I2F2\n");
    }else if(t2 != INT){
      printf("I2F1\n");
    }else{
      printf("!!! CAST ERROR IN EXPRESSION PRINTER !!!");
    }
    printf("%sF\n",op);
    return FLOAT;
  //identique et INT
  }else if(t1 == INT){
    printf("%sI\n",op);
    return INT;
  //identique et float
  }else{
    printf("%sF\n",op);
    return FLOAT;
  }
}

char whichType(int t){
  switch(t){
    case INT:
      return 'I';
    case FLOAT:
      return 'F';
    default:
      return 'V'; //error
  }

  }

 // dirty trick to end function init_glob_var() definition (see rule po : PO)
void end_glob_var_decl(){
  static int unfinished=1;
  if (unfinished) {
    unfinished = 0;
    printf("}\n\n");
  }
}

// Votre code C peut aller ci-dessous pour factoriser (un peu) le code des actions semantiques
 
  %}


%start prog  

// liste de tous les type des attributs des non terminaux que vous voulez manipuler l'attribut (il faudra en ajouter plein ;-) )
%type <type_value> type exp  typename
%type <string_value> fun_head

%%

 // O. Déclaration globale

prog : glob_decl_list              {}
;

glob_decl_list : glob_var_list glob_fun_list {}
;

glob_var_list : glob_var_list decl PV {} // reset current_decl_type après la déclaration des variables
| {printf("void init_glob_var(){\n"); // starting  function init_glob_var() definition in target code
}
;

glob_fun_list : glob_fun_list fun {}
| fun {}
;

// I. Functions

fun : type fun_head fun_body   {}
;

po: PO {end_glob_var_decl();}  // dirty trick to end function init_glob_var() definition in target code
  
fun_head : ID po PF            {
  // Pas de déclaration de fonction à l'intérieur de fonctions !
  if (depth>0) yyerror("Function must be declared at top level~!\n");
  else { printf("void pcode_%s()", $1);} // reset current_decl_type après la déclaration d'une fonction
  }

| ID po params PF              {
   // Pas de déclaration de fonction à l'intérieur de fonctions !
  if (depth>0) yyerror("Function must be declared at top level~!\n");
 }
;

params: type ID vir params     {} // récursion droite pour numéroter les paramètres du dernier au premier
| type ID                      {}


vir : VIR                      {}
;

fun_body : fao block faf       {}
;

fao : AO {
  depth++; // on entre dans un nouveau bloc
  /* Chaque instance de bloc réinitialise son compteur d'offset :
     depth 0 -> offsets globaux démarrent à 0
     depth >0 -> offsets locaux démarrent à 1
  */
  if (depth == 0) offset_stack[depth] = 0; else offset_stack[depth] = 1;
  offset = offset_stack[depth]; // initialise offset à l'offset de la depth courante
  printf(" {\n");
      }
;
faf : AF {
  depth--; // on sort du bloc
  offset = offset_stack[depth]; // on reset l'offset à celui de la depth précédente
  printf("}\n");
      }
;


// II. Block
block:
decl_list inst_list            {}
;

// III. Declarations

decl_list : decl_list decl PV   {}
|                               {}
;

decl: var_decl                  {}
;

var_decl : type vlist ;

vlist: vlist vir ID            {
                                  attribute a = makeSymbol($<type_value>0, offset_stack[depth]++, depth);  // créer le symbole 
                                  set_symbol_value($3, a);
                                  // garder offset à jour 
                                  offset = offset_stack[depth];
                                  if (a->type == FLOAT){
                                    printf("LOADF(0.0)\n");
                                    }
                                    else if(a->type == INT){
                                  printf("LOADI(0)\n");
                                    }
                                  }
| ID                           {
                                attribute a = makeSymbol($<type_value>0, offset_stack[depth]++, depth);
                                set_symbol_value($1, a);
                                offset = offset_stack[depth];
                                if (a->type == FLOAT){
                                  printf("LOADF(0.0)\n");
                                  }
                                  else if(a->type == INT){
                                printf("LOADI(0)\n");
                                  }
                                }
;

type
: typename                     { $$ = $1;}
;

typename // Utilisation des terminaux comme codage (entier) du type !!!
: INT                          {$$=INT;} 
| FLOAT                        {$$=FLOAT;}
| VOID                         {$$=VOID;}
;

// IV. Intructions

inst_list: inst_list inst   {} 
| inst                      {}
;

pv : PV                       {}
;
 
inst:
ao block af                   {}
| exp pv                      {}
| aff pv                      {}
| ret pv                      {}
| cond                        {}
| loop                        {}
| pv                          {}
;

// Accolades explicites pour gerer l'entrée et la sortie d'un sous-bloc
ao : AO                       { printf("SAVEBP\n");
                                // on entre dans un nouveau bloc
                                depth++;
                                /*
                                   depth 0 -> offsets globaux démarrent à 0
                                   depth >0 -> offsets locaux démarrent à 1
                                */
                                if (depth == 0) offset_stack[depth] = 0; else offset_stack[depth] = 1;
                                offset = offset_stack[depth];
                                }
;

af : AF                       { printf("RESTOREBP\n");
                                // on sort d'un bloc
                                offset_stack[depth] = offset; // on sauvegarde l'offset courant pour cette depth
                                depth--;
                                offset = offset_stack[depth]; // on reset l'offset à celui de la depth précédente
                              }
;


// IV.1 Affectations

aff : ID EQ exp               {
                                attribute a = get_symbol_value($1);
                                // $3 est le type_value retourné par exp 
                                if ($3 != a->type) {
                                  if (a->type == FLOAT && $3 == INT) {
                                    printf("I2F2\n");
                                  }
                                  if (a->type == INT && $3 == FLOAT) {
                                    printf("//!!! CAST ERROR IN AFFECTION !!!");
                                  }
                                }
                                if (a->depth == 0) { // variable globale
                                  printf("LOADI(%d)\nSTORE\n", a->offset);
                                } else if (a->depth == depth) {
                                  // locale au bloc actuel
                                  printf("LOADBP\nSHIFT(%d)\nSTORE\n", a->offset);
                                } else if (a->depth < depth) {
                                  // locale dans un bloc englobant : grimper la chaîne saved-bp
                                  printf("LOADBP\n");
                                  int climbs = depth - a->depth;
                                  while (climbs > 0) { printf("LOAD\n"); climbs--; } // on LOAD jusq'au bon bloc
                                  printf("SHIFT(%d)\nSTORE\n", a->offset);
                                } else {
                                  printf("// NON ACCESSIBLE VARIABLE !!!\n");
                                }
                                }
                                


// IV.2 Return
ret : RETURN exp              {}
| RETURN PO PF                {}
;

// IV.3. Conditionelles
//           N.B. ces rêgles génèrent un conflit déclage reduction
//           qui est résolu comme on le souhaite par un décalage (shift)
//           avec ELSE en entrée (voir y.output)

cond :
if bool_cond inst  elsop       {}
;

elsop : else inst              {}
|                  %prec IFX   {} // juste un "truc" pour éviter le message de conflit shift / reduce
;

bool_cond : PO exp PF         {}
;

if : IF                       {}
;

else : ELSE                   {}
;

// IV.4. Iterations

loop : while while_cond inst  {}
;

while_cond : PO exp PF        {}

while : WHILE                 {}
;


// V. Expressions

exp
// V.1 Exp. arithmetiques
: MOINS exp %prec UNA         {}
         // -x + y lue comme (- x) + y  et pas - (x + y)
| exp PLUS exp                {$$=expressionPrinter($1,"ADD",$3);}
| exp MOINS exp               {$$=expressionPrinter($1,"SUB",$3);}
| exp STAR exp                {$$=expressionPrinter($1,"MULT",$3);}
| exp DIV exp                 {$$=expressionPrinter($1,"DIV",$3);}
| PO exp PF                   {}//A revoir
| ID                          { attribute a = get_symbol_value($1);
                                if (a->depth == 0) { /* global */
                                  printf("LOADI(%d)\nLOAD\n", a->offset);
                                } else if (a->depth == depth) {
                                  // locale au bloc actuel
                                  printf("LOADBP\nSHIFT(%d)\nLOAD\n", a->offset);
                                } else if (a->depth < depth) {
                                  // locale dans un bloc englobant : grimper la chaîne saved-bp
                                  printf("LOADBP\n");
                                  int climbs = depth - a->depth;
                                  while (climbs > 0) { printf("LOAD\n"); climbs--; }
                                  printf("SHIFT(%d)\nLOAD\n", a->offset);
                                } else {
                                  printf("// NON ACCESSIBLE VARIABLE !!!\n");
                                }
                                $$ = a->type; }
                              // $$=a->type pour transmettre le type dans les expressions et éviter les conversions inutiles;
| app                         {}
| NUM                         {printf("LOADI(%d)\n",$1);$$=INT;}
| DEC                         {printf("LOADF(%f)\n",$1);$$=FLOAT;}


// V.2. Booléens

| NOT exp %prec UNA           {}
| exp INF exp                 {printf("LT%c\n", whichType($1));}
| exp SUP exp                 {}
| exp EQUAL exp               {}
| exp DIFF exp                {}
| exp AND exp                 {}
| exp OR exp                  {}

;

// V.3 Applications de fonctions


app : fid PO args PF          {}
;

fid : ID                      {}

args :  arglist               {}
|                             {}
;

arglist : arglist VIR exp     {} // récursion gauche pour empiler les arguements de la fonction de gauche à droite
| exp                         {}
;



%% 
int main () {

  /* Ici on peut ouvrir le fichier source, avec les messages 
     d'erreur usuel si besoin, et rediriger l'entrée standard 
     sur ce fichier pour lancer dessus la compilation.
   */

char * header=
"// PCode Header\n\
#include \"PCode.h\"\n\
\n\
void pcode_main();\n\
void init_glob_var();\n\
\n\
int main() {\n\
init_glob_var();\n\
pcode_main();\n\
return stack[sp-1].int_value;\n\
}\n\
\n";  

printf("%s\n",header); // ouput header
  
return yyparse (); // output your compilation
 
 
}

