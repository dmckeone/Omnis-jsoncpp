/* STATIC METHODS (IMPLEMENTATION)
 * 
 * This file implements all static methods for the external.
 *
 * February 18, 2011 David McKeone (Created)
 */

#include <extcomp.he>
#include "OmnisTools.he"
#include "Static.he"

using namespace OmnisTools;

// Define static methods
const static qshort cStaticMethodEmpty = 20000;

// Parameters for Static Methods
// Columns are:
// 1) Name of Parameter (Resource #)
// 2) Return type (fft value)
// 3) Parameter flags of type EXTD_FLAG_xxxx
// 4) Extended flags.  Documentation states, "Must be 0"
ECOparam cStaticMethodsParamsTable[] = 
{
	// $empty
    20800, fftCharacter  , 0, 0
};

// Table of Methods available
// Columns are:
// 1) Unique ID 
// 2) Name of Method (Resource #)
// 3) Return Type 
// 4) # of Parameters
// 5) Array of Parameter Names (Taken from MethodsParamsTable.  Increments # of parameters past this pointer) 
// 6) Enum Start (Not sure what this does, 0 = disabled)
// 7) Enum Stop (Not sure what this does, 0 = disabled)
ECOmethodEvent cStaticMethodsTable[] = 
{
	cStaticMethodEmpty,     cStaticMethodEmpty,     fftNone,      1, &cStaticMethodsParamsTable[0], 0, 0
};

// List of methods in Simple
qlong returnStaticMethods(tThreadData* pThreadData)
{
	const qshort cStaticMethodCount = sizeof(cStaticMethodsTable) / sizeof(ECOmethodEvent);
	
	return ECOreturnMethods( gInstLib, pThreadData->mEci, &cStaticMethodsTable[0], cStaticMethodCount );
}

// Implementation of single static method
void methodStaticEmpty(tThreadData* pThreadData, qshort paramCount) {
	return;
}

// Static method dispatch
qlong staticMethodCall( OmnisTools::tThreadData* pThreadData ) {
	
	qshort funcId = (qshort)ECOgetId(pThreadData->mEci);
	qshort paramCount = ECOgetParamCount(pThreadData->mEci);
	
	switch( funcId )
	{
		case cStaticMethodEmpty:
			pThreadData->mCurMethodName = "$empty";
			methodStaticEmpty(pThreadData, paramCount);
			break;
    }
	
	return 0L;
}

