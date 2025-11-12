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

// Declare z of type int with offset 2 at depth 0
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
IFN(False_0)
// la condition 0 est vraie
SAVEBP // entering block
// Debut conditionelle 1
// Loading global var y adress (used at depth 2)
LOADI(1) // loading offset 1 of variable y
// Loading variable y (right) value
LOAD
IFN(False_1)
// la condition 1 est vraie
LOADI(1)
// Loading global var z adress (used at depth 2)
LOADI(2) // loading offset 2 of variable z
// Storing variable z (right) value
STORE
GOTO(End_1)
False_1:
// la condition 1 est fausse
LOADI(2)
// Loading global var z adress (used at depth 2)
LOADI(2) // loading offset 2 of variable z
// Storing variable z (right) value
STORE
End_1:
// Fin conditionelle 1
RESTOREBP // exiting block
GOTO(End_0)
False_0:
// la condition 0 est fausse
SAVEBP // entering block
// Debut conditionelle 2
// Loading global var z adress (used at depth 2)
LOADI(2) // loading offset 2 of variable z
// Loading variable z (right) value
LOAD
IFN(False_2)
// la condition 2 est vraie
LOADI(3)
// Loading global var z adress (used at depth 2)
LOADI(2) // loading offset 2 of variable z
// Storing variable z (right) value
STORE
GOTO(End_2)
False_2:
// la condition 2 est fausse
LOADI(4)
// Loading global var z adress (used at depth 2)
LOADI(2) // loading offset 2 of variable z
// Storing variable z (right) value
STORE
End_2:
// Fin conditionelle 2
RESTOREBP // exiting block
End_0:
// Fin conditionelle 0
// Loading global var z adress (used at depth 1)
LOADI(2) // loading offset 2 of variable z
// Loading variable z (right) value
LOAD
}
