// Hardware-Assisted AV
// Hardware component - PIN tool
// Marcus Botacin - 2017
// Federal University of Paraná (UFPR)

// Include Block

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "pin.H"
#include<list>

// Bloom filter definition
#include"bloom_filter.hpp"

// Windows namespace, to not conflict with PIN.h one
namespace WINDOWS
{
    #include <Windows.h>
}

// Output files
ofstream OutFile;
ofstream OutFile2;
ofstream OutFile3;

// define if matching mode will be enabled
#define CHECK_WINDOW
// define Branch Window size
#define N 20
// define window itself
std::list<bool> window;

// define if bloom filter matching will be enabled
#define BLOOM
// bloom filter itself
bloom_filter *filter;

// maximum number of signatures
// limited for test purposes
#define MAX_SIGS 1024
// loaded signature count
UINT64 n_sigs=0;
// signature DB
UINT64 sigs[MAX_SIGS];

// ADD branch to current window
void window_add(VOID *taken)
{
    // interpret data as taken or not-taken
	BOOL value;
	if(taken){
		value=1;
	}else{
		value=0;
	}

    // always add to window
	window.push_back(value);
	
    // if size exceeds, remove to fit
	if(window.size()>N){
		window.pop_front();
	}
}

// convert bit position to their decimal representation
// used to convert a given branch pattern to integer and then
// add it to the bloom filter
UINT64 bit_to_dec(list<bool> l)
{
	UINT64 acc=0;
	int i=0;
    // for each bit in the pattern
	for (std::list<bool>::reverse_iterator rit=l.rbegin(); rit!=l.rend(); ++rit)
	{
        // exponentiate and increment
		acc+=(*rit<<i);
		i++;
	}
    // return the integer number
	return acc;
}

// signature match in the linear mode
BOOL detect_linear(list<bool> l)
{
    // check each loaded signature
	for(int i=0;i<n_sigs;i++)
	{
        // convert bit signature to integer
		UINT64 sig = bit_to_dec(l);
        // check match
		if(sig == sigs[i])
		{
            // detected
			return TRUE;
		}
	}
    // no match, false
	return FALSE;
}

// matching using bloom filter
BOOL detect_bloom(list<bool> l)
{
    // get number from pattern
	UINT64 sig = bit_to_dec(l);
    // bloom check
	return filter->contains(sig);
}

// Check current window
WINDOWS::UINT64 window_check()
{
	WINDOWS::UINT64 PID;

    // only check if window is complete
	if(window.size()<N){
		return 0;
	}
	
	BOOL res;
	int i;

// perform signature match in some mode
#ifdef BLOOM
	res = detect_bloom(window);
#else
	res = detect_linear(window);
#endif

    // if signature matches
	if(res){
        // identifies source process
		PID = PIN_GetPid();
        // debug prints
		OutFile3<<"Matching window: ";
		for (std::list<bool>::iterator it=window.begin(); it != window.end(); ++it)
		{
			OutFile3<<*it;
		}
		OutFile3<<endl;
		OutFile3<<"Detected on PID: "<<PID<<endl;
		return PID;
	}

	return 0;
}

// call an AV on demand
void run_AV(WINDOWS::UINT64 PID)
{
		char cmd[4096];
		BOOL _32bits;
		WINDOWS::HANDLE hProc;

        // target process can be 32 or 64 bits
        // first identifies
		hProc=WINDOWS::OpenProcess(PROCESS_QUERY_INFORMATION,FALSE,PID);
		WINDOWS::IsWow64Process(hProc,(WINDOWS::PBOOL)&_32bits);

        // if 32, call 32-bit AV
		if(!_32bits)
		{
            // debug print
			//printf("Calling 64bits\n");
			sprintf(cmd,"SimpleAV\\x64\\Debug\\SimpleAV.exe -s %d",PIN_GetPid());
		}
		else{
            // if 64, call 64-bit AV
            // debug print
			//printf("Calling 32bits\n");
			sprintf(cmd,"SimpleAV\\Debug\\SimpleAV.exe -s %d",PIN_GetPid());
		}

        // Create AV process
        // this code simulates the on-demand interrupt
		WINDOWS::STARTUPINFO si;
		WINDOWS::PROCESS_INFORMATION pi;
		ZeroMemory( &si, sizeof(si) );
		si.cb = sizeof(si);
		ZeroMemory( &pi, sizeof(pi) );
		WINDOWS::CreateProcess(NULL,cmd,NULL,NULL,FALSE,CREATE_NO_WINDOW,NULL,NULL,&si,&pi);
		WINDOWS::WaitForSingleObject( pi.hProcess, INFINITE );
		WINDOWS::DWORD exit;
		WINDOWS::GetExitCodeProcess(pi.hProcess,&exit);
        // log AV scan result
		OutFile3<<"AV Scan: "<<exit<<endl;
}

// Generate the branch history pattern
// taken = branch data
// Filter = main executable or lib
VOID check_branch(VOID *taken, BOOL Filter)
{
	WINDOWS::UINT64 PID;

    // interpret data as taken/not-taken
    // first log file consider all branches
	if(taken)
	{
		OutFile<<1;
	}else{
		OutFile<<0;
	}

    // second file considers only main images
	if(Filter)
	{
        // log
		if(taken)
		{
			OutFile2<<1;
		}else{
			OutFile2<<0;
		}

// if matching mode, match
#ifdef CHECK_WINDOW
        // add branch to window
			window_add(taken);
        // check if window match and retrieves PID if so
			PID=window_check();
        // if matches, run AV
			if(PID>0)
			{
				run_AV(PID);
			}
#endif
	}
}

// Instrumentation function
VOID Instruction(INS ins, VOID *v)
{	
	BOOL Filter=FALSE;
	
    // Only branch instructions, since we are emulating the branch unity
	if(!INS_IsBranchOrCall(ins))
	{
		return;
	}

    // Only Direct Branches
	if(!INS_IsDirectBranchOrCall(ins))
	{
		return;
	}
	
    // identifies code portion
	PIN_LockClient();
	IMG img = IMG_FindByAddress(INS_Address(ins));
	PIN_UnlockClient();
	if(IMG_Valid(img) && IMG_IsMainExecutable(img))
	{
		Filter=TRUE;
	}

    // callback
	INS_InsertCall(ins,IPOINT_BEFORE,(AFUNPTR)check_branch,IARG_BRANCH_TAKEN,IARG_BOOL,Filter,IARG_END);
}

// first log file - branch pattern including all code sections
KNOB<string> KnobOutputFile(KNOB_MODE_WRITEONCE, "pintool",
		"o", "branch.with.libs.out", "specify output file name");

// second log file - branch pattern excluding libraries
KNOB<string> KnobOutputFile2(KNOB_MODE_WRITEONCE, "pintool",
		"o2", "branch.without.libs.out", "specify output file name");

// third log file - AV logs and so on
KNOB<string> KnobOutputFile3(KNOB_MODE_WRITEONCE, "pintool",
		"o3", "branch.log.out", "specify output file name");

// At the end, close files
VOID Fini(INT32 code, VOID *v)
{
	OutFile.close();
	OutFile2.close();
    // log
	OutFile3<<"Finishing"<<endl;
	OutFile3.close();
}

// load signature definitions
void load_sigs()
{
	bloom_parameters parameters;
	ifstream s;
	string line;
    // open signature file
	string path = "sigs.txt";
	s.open(path);
	if(s.is_open())
	{
        // read it line by line
		while(getline(s,line))
		{
			sigs[n_sigs]=stoull(line,NULL,10);
			n_sigs++;
		}
	}

    // create, fit and insert definitions into bloom filter, when using it
	#ifdef BLOOM
	OutFile3<<"Using bloom filter"<<endl;
	parameters.projected_element_count = n_sigs;
	parameters.false_positive_probability = 0.0001; // 1 in 10000
	parameters.random_seed = 0xA5A5A5A5;
	parameters.compute_optimal_parameters();
	filter = new bloom_filter(parameters);
	for(int i=0;i<n_sigs;i++)
	{
		filter->insert(sigs[i]);
	}
	OutFile3<<"Loaded "<<n_sigs<<" signature definitions"<<endl;
#endif

}

// Entry Point
int main(int argc, char * argv[])
{
	// Initialize pin, no args except the binary
	if (PIN_Init(argc, argv)) return -1;

	// Open log files
	OutFile.open(KnobOutputFile.Value().c_str());
	OutFile2.open(KnobOutputFile2.Value().c_str());
	OutFile3.open(KnobOutputFile3.Value().c_str());

    // log
	OutFile3 << "Starting..." << endl;

    // load AV signatures
	load_sigs();

	// instruction-level instrumentation
	INS_AddInstrumentFunction(Instruction, 0);

	// Application exit callback
	PIN_AddFiniFunction(Fini, 0);

	// Start the program
	PIN_StartProgram();

	return 0;
}
