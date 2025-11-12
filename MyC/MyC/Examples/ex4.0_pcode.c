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
// Declare x of type int with offset 0 at depth 0
LOADI(0)

// Declare y of type int with offset 1 at depth 0
LOADI(0)

}

void pcode_main() {
LOADI(3)
// Loading global var x adress (used at depth 1)
LOADI(0) // loading offset 0 of variable x
// Storing variable x (right) value
STORE
LOADI(5)
// Loading global var y adress (used at depth 1)
LOADI(1) // loading offset 1 of variable y
// Storing variable y (right) value
STORE
// Debut conditionelle 0
// Loading global var x adress (used at depth 1)
LOADI(0) // loading offset 0 of variable x
// Loading variable x (right) value
LOAD
// Loading global var y adress (used at depth 1)
LOADI(1) // loading offset 1 of variable y
// Loading variable y (right) value
LOAD
LTI
IFN(False_0)
// la condition 0 est vraie
// Loading global var y adress (used at depth 1)
LOADI(1) // loading offset 1 of variable y
// Loading variable y (right) value
LOAD
// Loading global var x adress (used at depth 1)
LOADI(0) // loading offset 0 of variable x
// Storing variable x (right) value
STORE
False_0:
// la condition 0 est fausse
// Fin conditionelle 0
// Loading global var x adress (used at depth 1)
LOADI(0) // loading offset 0 of variable x
// Loading variable x (right) value
LOAD
}
