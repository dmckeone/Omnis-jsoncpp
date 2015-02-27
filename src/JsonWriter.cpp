/* SIMPLE OBJECT (IMPLEMENTATION)
 * 
 * This object has some simple functionality to show how to do basic method calls and property setting.
 *
 * March 30, 2010 David McKeone (Created)
 */

#include "jsoncpp.he"
#include "JsonValue.he"
#include "JsonWriter.he"

using boost::shared_ptr;
using namespace OmnisTools;

/**************************************************************************************************
 **                       CONSTRUCTORS / DESTRUCTORS                                             **
 **************************************************************************************************/

JsonWriter::JsonWriter(qobjinst objinst) : NVObjBase(objinst)
{ }

JsonWriter::JsonWriter(qobjinst objinst, tThreadData *pThreadData) : NVObjBase(objinst)
{ }

JsonWriter::~JsonWriter()
{ }



/**************************************************************************************************
 **                              PROPERTY DECLERATION                                            **
 **************************************************************************************************/

// This is where the resource # of the methods is defined.  In this project it is also used as the Unique ID.
//
// NOTE: Json::Writer properties are on the 3300-3399 stripe
const static qshort cPropertyMyProperty = 3300;

/**************************************************************************************************
 **                               METHOD DECLERATION                                             **
 **************************************************************************************************/

// This is where the resource # of the methods is defined.  In this project is also used as the Unique ID.
//
// NOTE:Json::Writer has the 2300-2399 stripe
const static qshort cMethodWrite = 2300;

/**************************************************************************************************
 **                                 INSTANCE METHODS                                             **
 **************************************************************************************************/

// Call a method
qlong JsonWriter::methodCall( tThreadData* pThreadData )
{
	qshort funcId = (qshort)ECOgetId(pThreadData->mEci);
	qshort paramCount = ECOgetParamCount(pThreadData->mEci);
	
	switch( funcId )
	{
		case cMethodWrite:
			pThreadData->mCurMethodName = "$write";
			methodWrite(pThreadData, paramCount);
			break;
	}
	
	return 0L;
}

/**************************************************************************************************
 **                                        STATIC METHODS                                        **
 **************************************************************************************************/

/* METHODS */

// Table of parameter resources and types.
// Note that all parameters can be stored in this single table and the array offset can be  
// passed via the MethodsTable.
//
// Columns are:
// 1) Name of Parameter (Resource #)
// 2) Return type (fft value)
// 3) Parameter flags of type EXTD_FLAG_xxxx
// 4) Extended flags.  Documentation states, "Must be 0"
//
// NOTE: Json::Writer has the 4200-4299 parameter stripe
ECOparam cJsonWriterMethodsParamsTable[] = 
{
	4200, fftNumber, 0, 0
};

// Table of Methods available for Simple
// Columns are:
// 1) Unique ID 
// 2) Name of Method (Resource #)
// 3) Return Type 
// 4) # of Parameters
// 5) Array of Parameter Names (Taken from MethodsParamsTable.  Increments # of parameters past this pointer) 
// 6) Enum Start (Not sure what this does, 0 = disabled)
// 7) Enum Stop (Not sure what this does, 0 = disabled)
ECOmethodEvent cJsonWriterMethodsTable[] = 
{
	cMethodWrite, cMethodWrite, fftCharacter, 1, &cJsonWriterMethodsParamsTable[0], 0, 0
};

// List of methods in JsonWriter
qlong JsonWriter::returnMethods(tThreadData* pThreadData)
{
	const qshort cMethodCount = sizeof(cJsonWriterMethodsTable) / sizeof(ECOmethodEvent);
	
	return ECOreturnMethods( gInstLib, pThreadData->mEci, &cJsonWriterMethodsTable[0], cMethodCount );
}

/* PROPERTIES */

// Table of properties available from JsonWriter
// Columns are:
// 1) Unique ID 
// 2) Name of Property (Resource #)
// 3) Return Type 
// 4) Flags describing the property
// 5) Additional Flags describing the property
// 6) Enum Start (Not sure what this does, 0 = disabled)
// 7) Enum Stop (Not sure what this does, 0 = disabled)
ECOproperty cJsonWriterPropertyTable[] = 
{
	cPropertyMyProperty, cPropertyMyProperty, fftInteger, EXTD_FLAG_PROPCUSTOM, 0, 0, 0 /* Shows under Custom category */
};

// List of properties in Simple
qlong JsonWriter::returnProperties( tThreadData* pThreadData )
{
	const qshort propertyCount = sizeof(cJsonWriterPropertyTable) / sizeof(ECOproperty);
	
	return ECOreturnProperties( gInstLib, pThreadData->mEci, &cJsonWriterPropertyTable[0], propertyCount );
}

// Assignability of properties
qlong JsonWriter::canAssignProperty( tThreadData* pThreadData, qlong propID ) {
	switch (propID) {
		default:
			return qfalse;
	}
}

// Method to retrieve a property of the object
qlong JsonWriter::getProperty( tThreadData* pThreadData ) 
{
	EXTfldval fValReturn;
	
	qlong propID = ECOgetId( pThreadData->mEci );
	switch( propID ) {
		case cPropertyMyProperty:
			fValReturn.setLong(myProperty); // Put property into return value
			ECOaddParam(pThreadData->mEci, &fValReturn); // Return to caller
			break;	       
	}
	
	return 1L;
}

// Method to set a property of the object
qlong JsonWriter::setProperty( tThreadData* pThreadData )
{
	// Setup the EXTfldval with the parameter data
	EXTfldval fVal;
	
	if( getParamVar( pThreadData->mEci, 1, fVal ) == qfalse )
		return 0L;
	
	// Assign to the appropriate property
	qlong propID = ECOgetId( pThreadData->mEci );
	switch( propID ) {
		case cPropertyMyProperty:
			myProperty = fVal.getLong();
			break;
	}
	
	return 1L;
}

/**************************************************************************************************
 **                              CUSTOM (YOUR) METHODS                                           **
 **************************************************************************************************/

// Simple method that squares the result $method(5.0) = 25.0
void JsonWriter::methodWrite( tThreadData* pThreadData, qshort pParamCount )
{
	// Declare all EXTfldvals for the parameters
	EXTfldval fValRoot, fValReturn;  
	
	// Read all parameters
	if( getParamVar( pThreadData, 1, fValRoot ) == qfalse )
		return;
	
	// Read parameter into Omnis object
	JsonValue *jvRoot = getObjForEXTfldval<JsonValue>(pThreadData, fValRoot);
	
	// Write value object (If it exists)
	std::string output;
	if (jvRoot) {
		output = jsonWriter.write( jvRoot->getJsonValue() );
	} else {
		output = "";
	}
	
	// Return the output of the writer to the caller
	getEXTFldValFromString(fValReturn, output);
	ECOaddParam(pThreadData->mEci, &fValReturn);
	
}