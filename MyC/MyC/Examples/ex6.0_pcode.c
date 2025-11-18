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
LOADI(0)
}

void pcode_main() {
LOADI(1)
LOADI(3)
LOADI(1)
STORE
LOADI(1)
LOAD
LOADI(0)
STORE
LOADI(0)
LOAD
}
