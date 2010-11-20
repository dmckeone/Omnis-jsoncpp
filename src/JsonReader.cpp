/* SIMPLE OBJECT (IMPLEMENTATION)
 * 
 * This object has some simple functionality to show how to do basic method calls and property setting.
 *
 * March 30, 2010 David McKeone (Created)
 */

#include <extcomp.he>
#include "JsonValue.he"
#include "JsonReader.he"

using namespace OmnisTools;

/**************************************************************************************************
 **                       CONSTRUCTORS / DESTRUCTORS                                             **
 **************************************************************************************************/

JsonReader::JsonReader(qobjinst objinst) : NVObjBase(objinst)
{ }

JsonReader::JsonReader(qobjinst objinst, tThreadData *pThreadData) : NVObjBase(objinst)
{ }

JsonReader::~JsonReader()
{ }



/**************************************************************************************************
 **                              PROPERTY DECLERATION                                            **
 **************************************************************************************************/

// This is where the resource # of the methods is defined.  In this project it is also used as the Unique ID.
//
// NOTE: Json::Reader properties are on the 3200-3299 stripe
const static qshort cPropertyMyProperty = 3200;

/**************************************************************************************************
 **                               METHOD DECLERATION                                             **
 **************************************************************************************************/

// This is where the resource # of the methods is defined.  In this project is also used as the Unique ID.
//
// NOTE:Json::Reader has the 2200-2299 stripe
const static qshort cMethodParse = 2200,
                    cMethodGetFormattedErrorMessages = 2201;

/**************************************************************************************************
 **                                 INSTANCE METHODS                                             **
 **************************************************************************************************/

// Call a method
qlong JsonReader::methodCall( tThreadData* pThreadData )
{
	qshort funcId = (qshort)ECOgetId(pThreadData->mEci);
	qshort paramCount = ECOgetParamCount(pThreadData->mEci);
	
	switch( funcId )
	{
		case cMethodParse:
			pThreadData->mCurMethodName = "$parse";
			methodParse(pThreadData, paramCount);
			break;
		case cMethodGetFormattedErrorMessages:
			pThreadData->mCurMethodName = "$getFormattedErrorMessages";
			methodGetFormattedErrorMessages(pThreadData, paramCount);
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
// NOTE: Json::Reader has the 4100-4199 parameter stripe
ECOparam cJsonReaderMethodsParamsTable[] = 
{
	4100, fftNumber, 0, 0
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
ECOmethodEvent cJsonReaderMethodsTable[] = 
{
	cMethodParse, cMethodParse, fftObject, 1, &cJsonReaderMethodsParamsTable[0], 0, 0,
	cMethodGetFormattedErrorMessages, cMethodGetFormattedErrorMessages, fftNone, 0, 0, 0, 0
};

// List of methods in JsonReader
qlong JsonReader::returnMethods(EXTCompInfo* pEci)
{
	const qshort cMethodCount = sizeof(cJsonReaderMethodsTable) / sizeof(ECOmethodEvent);
	
	return ECOreturnMethods( gInstLib, pEci, &cJsonReaderMethodsTable[0], cMethodCount );
}

/* PROPERTIES */

// Table of properties available from JsonReader
// Columns are:
// 1) Unique ID 
// 2) Name of Property (Resource #)
// 3) Return Type 
// 4) Flags describing the property
// 5) Additional Flags describing the property
// 6) Enum Start (Not sure what this does, 0 = disabled)
// 7) Enum Stop (Not sure what this does, 0 = disabled)
ECOproperty cJsonReaderPropertyTable[] = 
{
	cPropertyMyProperty, cPropertyMyProperty, fftInteger, EXTD_FLAG_PROPCUSTOM, 0, 0, 0 /* Shows under Custom category */
};

// List of properties in Simple
qlong JsonReader::returnProperties( EXTCompInfo* pEci )
{
	const qshort propertyCount = sizeof(cJsonReaderPropertyTable) / sizeof(ECOproperty);
	
	return ECOreturnProperties( gInstLib, pEci, &cJsonReaderPropertyTable[0], propertyCount );
}

// Assignability of properties
qlong JsonReader::canAssignProperty( EXTCompInfo* pEci, qlong propID ) {
	switch (propID) {
		default:
			return qfalse;
	}
}

// Method to retrieve a property of the object
qlong JsonReader::getProperty( EXTCompInfo* pEci ) 
{
	EXTfldval fValReturn;
	
	qlong propID = ECOgetId( pEci );
	switch( propID ) {
		case cPropertyMyProperty:
			fValReturn.setLong(myProperty); // Put property into return value
			ECOaddParam(pEci, &fValReturn); // Return to caller
			break;	       
	}
	
	return 1L;
}

// Method to set a property of the object
qlong JsonReader::setProperty( EXTCompInfo* pEci )
{
	// Retrieve value to set for property, always in first parameter
	EXTParamInfo* param = ECOfindParamNum( pEci, 1 );
	if( !param ) { return 0L; }
	
	// Setup the EXTfldval with the parameter data
	EXTfldval fVal;
	fVal.setFldVal((qfldval)param->mData);
	fVal.setReadOnly( qfalse );
	
	// Assign to the appropriate property
	qlong propID = ECOgetId( pEci );
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

// Parsing method -- $parse(std::string document, Json::Value root, bool comments) 
void JsonReader::methodParse( tThreadData* pThreadData, qshort pParamCount )
{	
	// Declare all EXTfldvals for the parameters
	EXTfldval fValDocument, fValRoot, fValComments, fValReturn;  
	
	// Read all parameters
	if( getParamVar( pThreadData, 1, fValDocument ) == qfalse )
		return;
	if( getParamVar( pThreadData, 2, fValRoot ) == qfalse )
		return;
	if( getParamVar( pThreadData, 3, fValComments ) == qfalse )
		return;
	
	// Setup parameters for call to Json::Reader
	std::string document = getStringFromEXTFldVal(fValDocument);
	bool collectComments = getBoolFromQBool(fValComments.getBool());
	Json::Value root;
	
	// Parse
	bool success = jsonReader.parse(document, root, collectComments);
	
	// Read parse document into Omnis object
	// Get instance
	JsonValue *jvRoot = getObjForEXTfldval<JsonValue>(pThreadData, fValRoot);
	
	// Set the value in the object instance and notify Omnis that the parameter changed
	jvRoot->setJsonValue(root);
	ECOsetParameterChanged( pThreadData->mEci, 2 );
	
	// Return the status of the parse to the caller
	fValReturn.setBool(getQBoolFromBool(success));
	ECOaddParam(pThreadData->mEci, &fValReturn);
}

void JsonReader::methodGetFormattedErrorMessages( tThreadData* pThreadData, qshort pParamCount )
{
	std::string errorMessages = jsonReader.getFormatedErrorMessages();
	EXTfldval fValReturn;
	getEXTFldValFromString(fValReturn, errorMessages);
	ECOaddParam(pThreadData->mEci, &fValReturn);
}
