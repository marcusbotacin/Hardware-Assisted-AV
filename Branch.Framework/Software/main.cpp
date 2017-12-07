// Hardware Assisted AV
// Software component
// Marcus Botacin - 2017
// Federal University of Paraná (UFPR)

#include<stdio.h>   // I/O headers
#include<Windows.h> // System headers
#include "print.h"  // project headers
#include "parse.h"
#include "sigs.h"
#include "scan.h"

int main(int argc,char *argv[])
{
	int index;
	signature db[MAX_SIGS];
	DWORD pid;

    // display hello message
	print_head(START_MESSAGE);
	
    // first, load signatures
	if(load_av_sigs(db))
	{
        // then, get pid
		pid = Parse(argc,argv);
		if(pid)
		{
            // scan given PID against loaded signatures db
			index=scan(pid,db);
			if(index>=0)
			{
                // threat detected
                // index is DB threat entry
				print_head_args("Found threat",db[index].name);
			}else if(index==FAIL){
                // scan failed
				print_head("Failed to scan");
			}else{
                // nothing found
				print_head("Clean file");
			}
		}else{
            // could not get PID
			print_head("Failed to parse PID");
            // display usage
			print_head(USAGE_MESSAGE);
		}
	}
    // AV could not be loaded
	else{
		print_head("Failed to load signatures");
	}

    // display exit message
	print_head(END_MESSAGE);
	return 0;
}
