// Hardware Assisted AV
// Software component
// Marcus Botacin - 2017
// Federal University of Paraná (UFPR)

#include<Windows.h> // System header

#define SCAN "-s"               // scan flah
#define MATCHING_STRINGS 0      // matching string
#define NO_PID 0                // default PID: not found

// function prototype
DWORD Parse(int argc, char *argv[]);
