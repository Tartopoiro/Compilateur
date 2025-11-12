// Begin PCode Header
#include "PCode.h"

void pcode_main();
void init_glob_var();

int main() {
init_glob_var();
pcode_main();
return stack[sp-1].int_value;
}
// End PCode Header

void init_glob_var(){
}

void pcode_main() {
LOADI(1)
LOADI(2)
MULTI
LOADF(3.500000)
LOADI(4)
I2F2 // converting second arg to float
MULTF
I2F1 // converting first arg to float
ADDF
}
