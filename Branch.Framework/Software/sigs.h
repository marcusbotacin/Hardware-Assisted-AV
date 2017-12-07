// Hardware Assisted AV
// Software component
// Marcus Botacin - 2017
// Federal University of Paraná (UFPR)

#include<Windows.h>	// System header
#pragma once		// avoid multiple includes

#define SIGS_FILE "sigs.txt"

// max number of signatures
// limited for test purposes
#define MAX_SIGS 10

// max string size
#define MAX_STR 64

// signature definition structure
typedef struct _sig
{
	char name[MAX_STR];	// threat name
	DWORD bytes;		// unique identifier
}signature,*psignature;

// prototype
BOOL load_av_sigs(psignature db);
