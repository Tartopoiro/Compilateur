// PCode Header
#include "PCode.h"

void pcode_main();
void init_glob_var();

int main() {
init_glob_var();
pcode_main();
return stack[sp-1].int_value;
}


void init_glob_var(){
}

void pcode_main() {
LOADI(1)
LOADI(2)
MULTI
LOADI(3)
LOADI(4)
MULTI
ADDI
}
