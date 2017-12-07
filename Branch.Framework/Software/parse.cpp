// Hardware Assisted AV
// Software component
// Marcus Botacin - 2017
// Federal University of Paraná (UFPR)

#include"parse.h"	// Header

DWORD Parse(int argc, char *argv[])
{
	int i; // for each argument
	for(i=0;i<argc-1;i++)
	{
		// case scan flag
		if(strcmp(argv[i],SCAN)==MATCHING_STRINGS)
		{	
			// get following PID
			return atoi(argv[i+1]);
		}
	}
	// no PID to scan
	return NO_PID;
}
