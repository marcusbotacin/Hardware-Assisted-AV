// Hardware Assisted AV
// Software component
// Marcus Botacin - 2017
// Federal University of Paraná (UFPR)

#include<stdio.h>   // I/O header
#include "print.h"  // project header

// Initial banner
void print_head(char *msg)
{
	printf("[%s] %s\n",HEAD,msg);
}

// Close banner
void print_head_args(char *msg,char *args)
{
	printf("[%s] %s: %s\n",HEAD,msg,args);
}
