#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <assert.h>
#include <set>
#include "pin.H"

using namespace std;

ofstream OutFile;

// The array storing the spacing frequency between two dependant instructions
UINT64 *dependancySpacing;

// Output file name
INT32 maxSize;

// Trace variable
UINT64 instructionCount = 0;
UINT64 lastWriteTime[REG_LAST];

// This knob sets the output file name
KNOB<string> KnobOutputFile(KNOB_MODE_WRITEONCE, "pintool", "o", "result.csv", "specify the output file name");

// This knob will set the maximum spacing between two dependant instructions in the program
KNOB<string> KnobMaxSpacing(KNOB_MODE_WRITEONCE, "pintool", "s", "100", "specify the maximum spacing between two dependant instructions in the program");

VOID docount() {
    instructionCount++;
}

// This function is called before every instruction is executed. Have to change
// the code to send in the register names from the Instrumentation function
VOID updateSpacingInfo(REG reg, BOOL isWrite){
    REG fullReg = REG_FullRegName(reg);

    if (isWrite) {
        lastWriteTime[fullReg] = instructionCount;
    } else {
        UINT64 lastWrite = lastWriteTime[fullReg];
        UINT64 distance = instructionCount - lastWrite;

        if (distance < (UINT64)maxSize) {
            dependancySpacing[distance]++;
        }
    }
}

// Pin calls this function every time a new instruction is encountered
VOID Instruction(INS ins, VOID *v)
{
    // Insert a call to updateSpacingInfo before every instruction.
    // You may need to add arguments to the call.
    INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)docount, IARG_END);

    std::set<REG> processedRegs;
    UINT32 numRead = INS_MaxNumRRegs(ins);
    for (UINT32 i = 0; i < numRead; i++) {
        REG r = INS_RegR(ins, i);
        if (REG_valid(r)) {
            REG fullR = REG_FullRegName(r);
            if (processedRegs.find(fullR) == processedRegs.end()) {
                INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)updateSpacingInfo, IARG_UINT32, r, IARG_BOOL, FALSE, IARG_END);
                processedRegs.insert(fullR);
            }
        }
    }

    UINT32 numWrite = INS_MaxNumWRegs(ins);
    for (UINT32 i = 0; i < numWrite; i++) {
        REG w = INS_RegW(ins, i);
        if (REG_valid(w)) {
            INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)updateSpacingInfo, IARG_UINT32, w, IARG_BOOL, TRUE, IARG_END);
        }
    }
}

// This function is called when the application exits
VOID Fini(INT32 code, VOID *v)
{
    // Write to a file since cout and cerr maybe closed by the application
    OutFile.open(KnobOutputFile.Value().c_str());
    OutFile.setf(ios::showbase);
	for(INT32 i = 0; i < maxSize; i++)
		OutFile << dependancySpacing[i]<<",";
    OutFile.close();
}

// argc, argv are the entire command line, including pin -t <toolname> -- ...
int main(int argc, char * argv[])
{
    // Initialize pin
    if ( PIN_Init(argc, argv)) return 1;

    // printf("Warning: Pin Tool not implemented\n");

    maxSize = atoi(KnobMaxSpacing.Value().c_str());

    // Initializing depdendancy Spacing
    dependancySpacing = new UINT64[maxSize];

    for (int i=0; i < maxSize; i++) {
        dependancySpacing[i] = 0;
    }

    for (int i = 0; i < REG_LAST; i++) {
        lastWriteTime[i] = 0;
    }

    // Register Instruction to be called to instrument instructions
    INS_AddInstrumentFunction(Instruction, 0);

    // Register Fini to be called when the application exits
    PIN_AddFiniFunction(Fini, 0);
    
    // Start the program, never returns
    PIN_StartProgram();
    
    return 0;
}

