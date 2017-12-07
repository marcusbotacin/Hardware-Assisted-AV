// Hardware Assisted AV
// Software component
// Marcus Botacin - 2017
// Federal University of Paraná (UFPR)

#include<stdio.h>   // I/O header
#include "sigs.h"	// Project header

BOOL load_av_sigs(psignature db)
{
	int i=0;
	char name[MAX_STR];
	char bytes[MAX_STR];

    // open signature file
	FILE *f = fopen(SIGS_FILE,"r");
	if(!f)
        // no signature file
		return FALSE;

    // for each signature definition
    // get name
	while(fscanf(f,"%s",name)!=EOF)
	{
        // get bytes (signature)
		fscanf(f,"%s",bytes);
        // store db
		strcpy(db[i].name,name);
		db[i].bytes = atoi(bytes);
        // count signatures
		i++;
	}
	return TRUE;
}
