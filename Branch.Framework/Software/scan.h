// Hardware Assisted AV
// Software component
// Marcus Botacin - 2017
// Federal University of Paraná (UFPR)

#include<Windows.h> // System header
#include "sigs.h"   // Project header

// region to be scanned
#define CODE_PORTION 0

// states

// clean file
#define CLEAN -1

// failed scan
#define FAIL -2

// prototype
int scan(DWORD pid, psignature db);
