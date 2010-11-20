/* JSON VALUE (IMPLEMENTATION)
 * 
 * This object wraps the functionality of the Json::Value class
 *
 * March 30, 2010 David McKeone (Created)
 */

#include "jsoncpp.he"
#include "JsonValue.he"

#include <string>

// Resource #'s for Value Type constants
const static qshort kConstValueTypeStart = 23001,
                    kConstValueTypeEnd   = 23008;


using namespace OmnisTools;

/**************************************************************************************************
 **                       CONSTRUCTORS / DESTRUCTORS                                             **
 **************************************************************************************************/

JsonValue::JsonValue(qobjinst objinst) : NVObjBase(objinst)
{ }

JsonValue::JsonValue(qobjinst objinst, tThreadData *pThreadData) : NVObjBase(objinst)
{ 
	// Constructor for $new(), interpret parameters (if any)
	pThreadData->mCurMethodName = "$new";
	qshort paramCount = ECOgetParamCount(pThreadData->mEci);
		
	methodInitialize(pThreadData, paramCount);
}

JsonValue::~JsonValue()
{ }

void JsonValue::copy( NVObjBase* pObj ) {
	// Copy in super class (This does *this = *pObj)
	NVObjBase::copy(pObj);
	
	// Copy the Json::Value into the new object
	jsonValue = dynamic_cast<JsonValue*>(pObj)->jsonValue;
}

// Get the internal Json::Value
Json::Value JsonValue::getJsonValue() {
	return jsonValue;
}

// Set the internal Json::Value
void JsonValue::setJsonValue(Json::Value v) {
	// Set the new value
	jsonValue = v;
}

/**************************************************************************************************
 **                              PROPERTY DECLERATION                                            **
 **************************************************************************************************/

// This is where the resource # of the methods is defined.  In this project it is also used as the Unique ID.
//
// NOTE: Json::Value properties are on the 3100-3199 stripe
const static qshort cPropertyValueType     = 3100,
                    cPropertyValueTypeDesc = 3101,
                    cPropertyContents      = 3102;

/**************************************************************************************************
 **                               METHOD DECLERATION                                             **
 **************************************************************************************************/

// This is where the resource # of the methods is defined.  In this project is also used as the Unique ID.
//
// NOTE:Json::Value has the 2100-2199 stripe
const static qshort cMethodConstruct  = 2100,
                    cMethodInitialize = 2101,
                    cMethodIsNull     = 2102,
                    cMethodIsBool     = 2103,
                    cMethodIsInt      = 2104,
                    cMethodIsUInt     = 2105,
                    cMethodIsIntegral = 2106,
                    cMethodIsDouble   = 2107,
                    cMethodIsNumeric  = 2108,
                    cMethodIsString   = 2109,
                    cMethodIsArray    = 2110,
                    cMethodIsObject   = 2111,
                    cMethodGet        = 2112,
                    cMethodSize       = 2113,
                    cMethodEmpty      = 2114,
                    cMethodClear      = 2115,
                    cMethodIsValidIndex = 2116,
                    cMethodIsMember = 2117,
                    cMethodGetMemberNames = 2118;

/**************************************************************************************************
 **                                 INSTANCE METHODS                                             **
 **************************************************************************************************/

// Call a method
qlong JsonValue::methodCall( tThreadData* pThreadData )
{
	qshort funcId = (qshort)ECOgetId(pThreadData->mEci);
	qshort paramCount = ECOgetParamCount(pThreadData->mEci);

	switch( funcId )
	{
		case cMethodConstruct:
			pThreadData->mCurMethodName = "$construct";
			methodConstruct(pThreadData, paramCount);
			break;
		case cMethodInitialize:
			pThreadData->mCurMethodName = "$initialize";
			methodInitialize(pThreadData, paramCount);
			break;
		case cMethodIsNull:
			pThreadData->mCurMethodName = "$isNull";
			methodIsNull(pThreadData, paramCount);
			break;
		case cMethodIsBool:
			pThreadData->mCurMethodName = "$isBool";
			methodIsBool(pThreadData, paramCount);
			break;
		case cMethodIsInt:
			pThreadData->mCurMethodName = "$isInt";
			methodIsInt(pThreadData, paramCount);
			break;
		case cMethodIsUInt:
			pThreadData->mCurMethodName = "$isUInt";
			methodIsUInt(pThreadData, paramCount);
			break;
		case cMethodIsIntegral:
			pThreadData->mCurMethodName = "$isIntegral";
			methodIsIntegral(pThreadData, paramCount);
			break;
		case cMethodIsDouble:
			pThreadData->mCurMethodName = "$isDouble";
			methodIsDouble(pThreadData, paramCount);
			break;
		case cMethodIsNumeric:
			pThreadData->mCurMethodName = "$isNumeric";
			methodIsNumeric(pThreadData, paramCount);
			break;
		case cMethodIsString:
			pThreadData->mCurMethodName = "$isString";
			methodIsString(pThreadData, paramCount);
			break;
		case cMethodIsArray:
			pThreadData->mCurMethodName = "$isArray";
			methodIsArray(pThreadData, paramCount);
			break;
		case cMethodIsObject:
			pThreadData->mCurMethodName = "$isObject";
			methodIsObject(pThreadData, paramCount);
			break;
		case cMethodGet:
			pThreadData->mCurMethodName = "$get";
			methodGet(pThreadData, paramCount);
			break;
		case cMethodSize:
			pThreadData->mCurMethodName = "$size";
			methodSize(pThreadData, paramCount);
			break;
		case cMethodEmpty:
			pThreadData->mCurMethodName = "$empty";
			methodEmpty(pThreadData, paramCount);
			break;
		case cMethodClear:
			pThreadData->mCurMethodName = "$clear";
			methodClear(pThreadData, paramCount);
			break;
		case cMethodIsValidIndex:
			pThreadData->mCurMethodName = "$isValidIndex";
			methodIsValidIndex(pThreadData, paramCount);
			break;
		case cMethodIsMember:
			pThreadData->mCurMethodName = "$isMember";
			methodIsMember(pThreadData, paramCount);
			break;
		case cMethodGetMemberNames:
			pThreadData->mCurMethodName = "$getMemberNames";
			methodGetMemberNames(pThreadData, paramCount);
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
// NOTE: Json::Value has the 4000-4099 parameter stripe
ECOparam cJsonValueMethodsParamsTable[] = 
{
	4000, fftCharacter, 0, 0,
	4001, fftCharacter, 0, 0,
	4002, fftCharacter, 0, 0,
	4003, fftObject, EXTD_FLAG_PARAMOPT, 0,
	4004, fftInteger, 0, 0,
	4005, fftCharacter, 0, 0
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
ECOmethodEvent cJsonValueMethodsTable[] = 
{
	cMethodConstruct, cMethodConstruct, fftCharacter, 1, &cJsonValueMethodsParamsTable[0], 0, 0,
	cMethodInitialize, cMethodInitialize, fftCharacter, 1, &cJsonValueMethodsParamsTable[1], 0, 0,
	cMethodIsNull, cMethodIsNull, fftNone, 0, 0, 0, 0,
	cMethodIsBool, cMethodIsBool, fftNone, 0, 0, 0, 0,
	cMethodIsInt, cMethodIsInt, fftNone, 0, 0, 0, 0,
	cMethodIsUInt, cMethodIsUInt, fftNone, 0, 0, 0, 0,
	cMethodIsIntegral, cMethodIsIntegral, fftNone, 0, 0, 0, 0,
	cMethodIsDouble, cMethodIsDouble, fftNone, 0, 0, 0, 0,
	cMethodIsNumeric, cMethodIsNumeric, fftNone, 0, 0, 0, 0,
	cMethodIsString, cMethodIsString, fftNone, 0, 0, 0, 0,
	cMethodIsArray, cMethodIsArray, fftNone, 0, 0, 0, 0,
	cMethodIsObject, cMethodIsObject, fftNone, 0, 0, 0, 0,
	cMethodGet, cMethodGet, fftObject, 2, &cJsonValueMethodsParamsTable[2], 0, 0,
	cMethodSize, cMethodSize, fftInteger, 0, 0, 0, 0,
	cMethodEmpty, cMethodEmpty, fftBoolean, 0, 0, 0, 0,
	cMethodClear, cMethodClear, fftNone, 0, 0, 0, 0,
	cMethodIsValidIndex, cMethodIsValidIndex, fftBoolean, 1, &cJsonValueMethodsParamsTable[4], 0, 0,
	cMethodIsMember, cMethodIsMember, fftBoolean, 1, &cJsonValueMethodsParamsTable[5], 0, 0,
	cMethodGetMemberNames, cMethodGetMemberNames, fftList, 0, 0, 0, 0
};

// List of methods in Simple
qlong JsonValue::returnMethods(EXTCompInfo* pEci)
{
	const qshort cMethodCount = sizeof(cJsonValueMethodsTable) / sizeof(ECOmethodEvent);
	
	return ECOreturnMethods( gInstLib, pEci, &cJsonValueMethodsTable[0], cMethodCount );
}

/* PROPERTIES */

// Table of properties available from Simple
// Columns are:
// 1) Unique ID 
// 2) Name of Property (Resource #)
// 3) Return Type 
// 4) Flags describing the property
// 5) Additional Flags describing the property
// 6) Enum Start (Not sure what this does, 0 = disabled)
// 7) Enum Stop (Not sure what this does, 0 = disabled)
ECOproperty cJsonValuePropertyTable[] = 
{
	cPropertyValueType    , cPropertyValueType    , fftInteger  , EXTD_FLAG_PROPCUSTOM|EXTD_FLAG_EXTCONSTANT, 0, kConstValueTypeStart, kConstValueTypeEnd, /* Shows under Custom category */
	cPropertyValueTypeDesc, cPropertyValueTypeDesc, fftCharacter, EXTD_FLAG_PROPCUSTOM, 0, 0, 0, /* Shows under Custom category */
	cPropertyContents, cPropertyContents, fftCharacter, EXTD_FLAG_PROPCUSTOM, 0, 0, 0  /* Shows under Custom category */
};

// List of properties in this component
qlong JsonValue::returnProperties( EXTCompInfo* pEci )
{
	const qshort propertyCount = sizeof(cJsonValuePropertyTable) / sizeof(ECOproperty);

	return ECOreturnProperties( gInstLib, pEci, &cJsonValuePropertyTable[0], propertyCount );
}

// Assignability of properties
qlong JsonValue::canAssignProperty( EXTCompInfo* pEci, qlong propID ) {
	switch (propID) {
		case cPropertyValueType:
			return qfalse;
		case cPropertyValueTypeDesc:
			return qfalse;
		case cPropertyContents:
			return qtrue;
		default:
			return qfalse;
	}
}

// Method to retrieve a property of the object
qlong JsonValue::getProperty( EXTCompInfo* pEci ) 
{
	EXTfldval fValReturn;
	
	qlong propID = ECOgetId( pEci );
	switch( propID ) {
		case cPropertyValueType:
			propertyValueType(fValReturn); // Put property into return value
			break;
		case cPropertyValueTypeDesc:
			propertyValueTypeDesc(fValReturn); // Put property into return value
			break;
		case cPropertyContents:
			getPropertyContents(fValReturn); // Put property into return value
			break;		    
	}
	
	ECOaddParam(pEci, &fValReturn); // Return to caller
	
	return 1L;
}

// Method to set a property of the object
qlong JsonValue::setProperty( EXTCompInfo* pEci )
{	
	// Retrieve value to set for property, always in first parameter
	EXTfldval fVal;
	if( getParamVar( pEci, 1, fVal) == qfalse ) 
		return qfalse;
	
	// Assign to the appropriate property
	qlong propID = ECOgetId( pEci );
	switch( propID ) {
		case cPropertyValueType:
			// Value type is not an assignable property
			return qfalse;
		case cPropertyValueTypeDesc:
			// Value type description is not an assignable property
			return qfalse;
		case cPropertyContents:
			// Value type description is not an assignable property
			setPropertyContents(fVal);
			return qtrue;
		default:
			return qfalse;
	}
}

/**************************************************************************************************
 **                                         PROPERTIES                                           **
 **************************************************************************************************/

void JsonValue::propertyValueType(EXTfldval &retVal) {
	switch (jsonValue.type()) {
		case Json::nullValue:
			retVal.setConstant(kConstValueTypeStart, kConstValueTypeEnd, 0);
			break;
		case Json::intValue:
			retVal.setConstant(kConstValueTypeStart, kConstValueTypeEnd, 1);
			break;
		case Json::uintValue:
			retVal.setConstant(kConstValueTypeStart, kConstValueTypeEnd, 2);
			break;
		case Json::realValue:
			retVal.setConstant(kConstValueTypeStart, kConstValueTypeEnd, 3);
			break;
		case Json::stringValue:
			retVal.setConstant(kConstValueTypeStart, kConstValueTypeEnd, 4);
			break;
		case Json::booleanValue:
			retVal.setConstant(kConstValueTypeStart, kConstValueTypeEnd, 5);
			break;
		case Json::arrayValue:
			retVal.setConstant(kConstValueTypeStart, kConstValueTypeEnd, 6);
			break;
		case Json::objectValue:
			retVal.setConstant(kConstValueTypeStart, kConstValueTypeEnd, 7);
			break;
		default:
			break;
	}
}

// Returns the text version of the constant name
void JsonValue::propertyValueTypeDesc(EXTfldval &retVal) {
	switch (jsonValue.type()) {
		case Json::nullValue:
			getEXTFldValFromString(retVal,"kJSONNullValueType");
			break;
		case Json::intValue:
			getEXTFldValFromString(retVal,"kJSONIntValueType");
			break;
		case Json::uintValue:
			getEXTFldValFromString(retVal,"kJSONUIntValueType");
			break;
		case Json::realValue:
			getEXTFldValFromString(retVal,"kJSONRealValueType");
			break;
		case Json::stringValue:
			getEXTFldValFromString(retVal,"kJSONStringValueType");
			break;
		case Json::booleanValue:
			getEXTFldValFromString(retVal,"kJSONBooleanValueType");
			break;
		case Json::arrayValue:
			getEXTFldValFromString(retVal,"kJSONArrayValueType");
			break;
		case Json::objectValue:
			getEXTFldValFromString(retVal,"kJSONObjectValueType");
			break;
		default:
			break;
	}
}

// Return the contents of this value to the caller
void JsonValue::getPropertyContents(EXTfldval &retVal) {
	switch (jsonValue.type()) {
		case Json::nullValue:
			retVal.setNull(fftCharacter, dpFcharacter);
			break;
		case Json::intValue:
			retVal.setLong(jsonValue.asInt());
			break;
		case Json::uintValue:
			retVal.setLong(jsonValue.asUInt());
			break;
		case Json::realValue:
			retVal.setNum(jsonValue.asDouble());
			break;
		case Json::stringValue:
			getEXTFldValFromString(retVal,jsonValue.asString());
			break;
		case Json::booleanValue:
			retVal.setBool( getQBoolFromBool(jsonValue.asBool()) );
			break;
		case Json::arrayValue:
			getEXTFldValFromString(retVal,"<JSON Array>");
			break;
		case Json::objectValue:
			getEXTFldValFromString(retVal,"<JSON Object>");
			break;
		default:
			break;
	}
}

// Helper method to initialize the jsonValue to a new value.
void JsonValue::setValueFromEXTfldval(EXTfldval &fVal) {
	jsonValue = Json::Value();
	
	// Get the data type of the parameter
	ffttype valType; fVal.getType(valType);
	
	// Perform appropriate initialization for each type
	if (valType == fftCharacter) {
		jsonValue = getStringFromEXTFldVal(fVal);
	} else if (valType == fftInteger) {
		jsonValue = static_cast<int>( fVal.getLong() );
	} else if (valType == fftNumber) {
		jsonValue = getDoubleFromEXTFldVal(fVal);
	} else if (valType == fftBoolean) {
		jsonValue = getBoolFromQBool(fVal.getBool());
	}
	
	// If no parameters were passed or an object couldn't be found, then init to NULL
	if ( jsonValue.isNull() ) {
		jsonValue = Json::Value();
	}
}

// Helper method to set the jsonValue for a specific array position
void JsonValue::setValueFromEXTfldval(EXTfldval &fVal, int group) {
	jsonValue[group] = Json::Value();
	
	// Get the data type of the parameter
	ffttype valType; fVal.getType(valType);
	
	// Perform appropriate initialization for each type
	if (valType == fftCharacter) {
		jsonValue[group] = getStringFromEXTFldVal(fVal);
	} else if (valType == fftInteger) {
		jsonValue[group] = static_cast<int>( fVal.getLong() );
	} else if (valType == fftNumber) {
		jsonValue[group] = getDoubleFromEXTFldVal(fVal);
	} else if (valType == fftBoolean) {
		jsonValue[group] = getBoolFromQBool(fVal.getBool());
	}
	
	// If no parameters were passed or an object couldn't be found, then init to NULL
	if ( jsonValue[group].isNull() ) {
		jsonValue[group] = Json::Value();
	}
}

// Helper method to set the jsonValue for a specific group
void JsonValue::setValueFromEXTfldval(EXTfldval &fVal, std::string group) {
	jsonValue[group] = Json::Value();
	
	// Get the data type of the parameter
	ffttype valType; fVal.getType(valType);
	
	// Perform appropriate initialization for each type
	if (valType == fftCharacter) {
		jsonValue[group] = getStringFromEXTFldVal(fVal);
	} else if (valType == fftInteger) {
		jsonValue[group] = static_cast<int>( fVal.getLong() );
	} else if (valType == fftNumber) {
		jsonValue[group] = getDoubleFromEXTFldVal(fVal);
	} else if (valType == fftBoolean) {
		jsonValue[group] = getBoolFromQBool(fVal.getBool());
	}
	
	// If no parameters were passed or an object couldn't be found, then init to NULL
	if ( jsonValue[group].isNull() ) {
		jsonValue[group] = Json::Value();
	}
}

// Helper method to initialize the jsonValue to a new value.
void JsonValue::setValueFromParameter(tThreadData *pThreadData, qshort paramNumber) {
	EXTParamInfo* param = ECOfindParamNum( pThreadData->mEci, paramNumber );
	if( param ) { 
		// Interpret the parameter we have
		EXTfldval fVal; fVal.setFldVal((qfldval)param->mData);
		
		setValueFromEXTfldval(fVal);
	}
}

// Set the contents of this value
void JsonValue::setPropertyContents(EXTfldval &fVal) {
	// Read parameters
	setValueFromEXTfldval(fVal);
}

/**************************************************************************************************
 **                                            METHODS                                           **
 **************************************************************************************************/

// Construct method
void JsonValue::methodConstruct( tThreadData* pThreadData, qshort pParamCount ) {
	// Pass through to initialize
	methodInitialize(pThreadData, pParamCount);
}

// Initialize method
void JsonValue::methodInitialize( tThreadData* pThreadData, qshort pParamCount )
{
	// Set the current method name
	if (pThreadData->mCurMethodName != "$new") {
		pThreadData->mCurMethodName = "$initialize";
	}
	
	setValueFromParameter(pThreadData, 1);
}

// Status methods
void JsonValue::methodIsNull( tThreadData* pThreadData, qshort pParamCount )
{
	EXTfldval fValReturn;
	qbool result = getQBoolFromBool(jsonValue.isNull());
	
	fValReturn.setBool(result); // Put property into return value
	ECOaddParam(pThreadData->mEci, &fValReturn); // Return to caller
}

void JsonValue::methodIsBool( tThreadData* pThreadData, qshort pParamCount )
{
	EXTfldval fValReturn;
	qbool result = getQBoolFromBool(jsonValue.isBool());
	
	fValReturn.setBool(result); // Put property into return value
	ECOaddParam(pThreadData->mEci, &fValReturn); // Return to caller
}

void JsonValue::methodIsInt( tThreadData* pThreadData, qshort pParamCount )
{
	EXTfldval fValReturn;
	qbool result = getQBoolFromBool(jsonValue.isInt());
	
	fValReturn.setBool(result); // Put property into return value
	ECOaddParam(pThreadData->mEci, &fValReturn); // Return to caller
}

void JsonValue::methodIsUInt( tThreadData* pThreadData, qshort pParamCount )
{
	EXTfldval fValReturn;
	qbool result = getQBoolFromBool(jsonValue.isUInt());
	
	fValReturn.setBool(result); // Put property into return value
	ECOaddParam(pThreadData->mEci, &fValReturn); // Return to caller
}

void JsonValue::methodIsIntegral( tThreadData* pThreadData, qshort pParamCount )
{
	EXTfldval fValReturn;
	qbool result = getQBoolFromBool(jsonValue.isIntegral());
	
	fValReturn.setBool(result); // Put property into return value
	ECOaddParam(pThreadData->mEci, &fValReturn); // Return to caller
}

void JsonValue::methodIsDouble( tThreadData* pThreadData, qshort pParamCount )
{
	EXTfldval fValReturn;
	qbool result = getQBoolFromBool(jsonValue.isDouble());
	
	fValReturn.setBool(result); // Put property into return value
	ECOaddParam(pThreadData->mEci, &fValReturn); // Return to caller
}

void JsonValue::methodIsNumeric( tThreadData* pThreadData, qshort pParamCount )
{
	EXTfldval fValReturn;
	qbool result = getQBoolFromBool(jsonValue.isNumeric());
	
	fValReturn.setBool(result); // Put property into return value
	ECOaddParam(pThreadData->mEci, &fValReturn); // Return to caller
}

void JsonValue::methodIsString( tThreadData* pThreadData, qshort pParamCount )
{
	EXTfldval fValReturn;
	qbool result = getQBoolFromBool(jsonValue.isString());
	
	fValReturn.setBool(result); // Put property into return value
	ECOaddParam(pThreadData->mEci, &fValReturn); // Return to caller
}

void JsonValue::methodIsArray( tThreadData* pThreadData, qshort pParamCount )
{
	EXTfldval fValReturn;
	qbool result = getQBoolFromBool(jsonValue.isArray());
	
	fValReturn.setBool(result); // Put property into return value
	ECOaddParam(pThreadData->mEci, &fValReturn); // Return to caller
}

void JsonValue::methodIsObject( tThreadData* pThreadData, qshort pParamCount )
{
	EXTfldval fValReturn;
	qbool result = getQBoolFromBool(jsonValue.isObject());
	
	fValReturn.setBool(result); // Put property into return value
	ECOaddParam(pThreadData->mEci, &fValReturn); // Return to caller
}

// JsonValue retrieval.  This implements the operator[] and $get() methods for Json::Value since there are no operators in Omnis.
// Call with $get('Settings') for settings key in object or $get(0) for an array
void JsonValue::methodGet( tThreadData* pThreadData, qshort pParamCount )
{
	EXTfldval keyVal, defVal, retVal;
	
	if ( getParamVar(pThreadData, 1, keyVal) == qfalse)
		return;
	
	JsonValue *omnisObj;
	Json::Value returnValue = Json::Value(), 
	            resultValue = Json::Value(); // Set result to Null.  If something is found it will override, otherwise this will be returned.;
	
	// Optional, if parameter passed then assign default value
	if ( getParamVar(pThreadData, 2, defVal) == qtrue) {
		omnisObj = getObjForEXTfldval<JsonValue>(pThreadData, defVal);
		if (omnisObj) {
			resultValue = omnisObj->getJsonValue(); 
		}
	} 

	std::string keyValue;
	int indexValue;
	ffttype valType; keyVal.getType(valType);
	
	if ( valType == fftCharacter || valType == fftInteger) {
		if ( jsonValue.isObject() ) {
			keyValue = getStringFromEXTFldVal( keyVal );
			returnValue = jsonValue[keyValue];
		} else if ( jsonValue.isArray() && valType == fftInteger ) {
			indexValue = static_cast<int>( keyVal.getLong() );
			returnValue = jsonValue[indexValue];
		}
	}
	
	if ( !(returnValue.isNull()) ) {
		resultValue = returnValue;
	}
	
	// Create return value object
	JsonValue *newObj = createNVObj<JsonValue>(pThreadData);
	newObj->setJsonValue(resultValue);
	
	retVal.setObjInst( newObj->mObjInst, qtrue ); 
	ECOaddParam( pThreadData->mEci, &retVal );
}

// Return size
void JsonValue::methodSize( tThreadData* pThreadData, qshort pParamCount ) {
	// Get size from value object
	int size = jsonValue.size();
	qlong qsize = static_cast<qlong>(size);
	
	// Return to Omnis
	EXTfldval fValReturn;
	fValReturn.setLong(qsize); 
	ECOaddParam(pThreadData->mEci, &fValReturn);
}

// Returns true if Value object empty
void JsonValue::methodEmpty( tThreadData* pThreadData, qshort pParamCount ) {
	// Get size from value object
	qbool status = getQBoolFromBool( jsonValue.empty() );
	
	// Return to Omnis
	EXTfldval fValReturn;
	fValReturn.setBool(status); 
	ECOaddParam(pThreadData->mEci, &fValReturn);
}

// Clears the contents of the Value object
void JsonValue::methodClear( tThreadData* pThreadData, qshort pParamCount ) {
	jsonValue.clear();
}

// Returns true if the index is in the array
void JsonValue::methodIsValidIndex( tThreadData* pThreadData, qshort pParamCount ) {
	EXTfldval indexVal, retVal;
	
	if ( getParamVar(pThreadData, 1, indexVal) == qfalse)
		return;
	
	int index = indexVal.getLong();
	
	// Send key to Value object
	qbool status = getQBoolFromBool( jsonValue.isValidIndex(index) );
	
	// Return to Omnis
	EXTfldval fValReturn;
	fValReturn.setBool(status); 
	ECOaddParam(pThreadData->mEci, &fValReturn);
}

// Return true if key (param 1) is in the object
void JsonValue::methodIsMember( tThreadData* pThreadData, qshort pParamCount ) {
	
	EXTfldval keyVal, retVal;
	
	if ( getParamVar(pThreadData, 1, keyVal) == qfalse)
		return;
	
	std::string key = getStringFromEXTFldVal(keyVal);
	
	// Send key to Value object
	qbool status = getQBoolFromBool( jsonValue.isMember(key) );
	
	// Return to Omnis
	EXTfldval fValReturn;
	fValReturn.setBool(status); 
	ECOaddParam(pThreadData->mEci, &fValReturn);
}

// Return list of values in object
void JsonValue::methodGetMemberNames( tThreadData* pThreadData, qshort pParamCount ) {
	// Create return value
	EXTfldval fValReturn, fValMember, fValColName;
	
	// Create single column list
	EXTqlist *retList = new EXTqlist( listVlen );  
	
	getEXTFldValFromString(fValColName, "Member");
	str255 colName; fValColName.getChar(colName, qtrue);
	retList->addCol(fftCharacter, dpFcharacter, 10000000, &colName);
	
	// Ensure that the Json::Value is an Object (since those are the only one's with member names)
	if ( !(jsonValue.isObject()) ) {
		fValReturn.setList(retList, qtrue, qfalse); 
		ECOaddParam(pThreadData->mEci, &fValReturn);
		return;
	}
	
	Json::Value::Members memberNames = jsonValue.getMemberNames();
	
	// Loop the member names to create the list
	qlong memberLength;
	std::string rowMember;
	str255 textForRow;
	for (qlong lineNumber = 0; lineNumber < memberNames.size(); lineNumber++) {
		rowMember = memberNames[lineNumber];
		
		getEXTFldValFromString(fValMember, rowMember);
		fValMember.getChar(textForRow, qtrue);
		retList->insertRow(0);
		retList->setCurRow( lineNumber+1 );
		//, &textForRow, lineNumber );
	}
	
	fValReturn.setList(retList, qtrue, qfalse); 
	ECOaddParam(pThreadData->mEci, &fValReturn);
	return;
}

// Method for setting data in the Value.
//
// Should work like:
// Do Value.$set('encoding','UTF-8')
// Do Value.$set('indent','length',5)
// Do Value.$set('indent','use_space', kTrue)

void JsonValue::methodSet( tThreadData* pThreadData, qshort pParamCount) {
	qshort paramNumber = 1;
	EXTfldval *paramVal = new EXTfldval[pParamCount];
	std::string stringVal, stringAssign;
	int intVal, intAssign;
	double doubleAssign;
	bool boolAssign;
	ffttype valType;
	
	
	// Loop through all parameters and assign their value to the array
	while ( getParamVar(pThreadData, paramNumber, paramVal[paramNumber-1]) == qtrue ) {
		paramNumber++;
	}
	
	// If parameters don't match then quit
	if (paramNumber != pParamCount) {
		delete [] paramVal; // Clean-up
		return;
	}
	
	// Assign values into the jsonValue
	if (pParamCount == 2) {
		// Simple assignment for a single value
		intVal = -1;
		stringVal = "";
		
		paramVal[0].getType(valType);
		
		if ( valType == fftInteger )
			intVal = static_cast<int>(paramVal[1].getLong());
		else if ( valType == fftCharacter )
			stringVal = getStringFromEXTFldVal(paramVal[0]);
		else
			return;

		
		paramVal[1].getType(valType);
		
		switch (valType) {
			case fftCharacter:
				stringAssign = getStringFromEXTFldVal(paramVal[1]);
				if (intVal >= 0 )
					jsonValue[intVal] = stringAssign;
				else
					jsonValue[stringVal] = stringAssign;
				
				break;
			case fftInteger:
				intAssign = static_cast<int>(paramVal[1].getLong());
				if (intVal >= 0 )
					jsonValue[intVal] = intAssign;
				else
					jsonValue[stringVal] = intAssign;
				
				break;
			case fftNumber:
				doubleAssign = getDoubleFromEXTFldVal(paramVal[1]);
				if (intVal >= 0 )
					jsonValue[intVal] = doubleAssign;
				else
					jsonValue[stringVal] = doubleAssign;
				
				break;
			case fftBoolean:
				boolAssign = paramVal[1].getBool();
				if (intVal >= 0 )
					jsonValue[intVal] = boolAssign;
				else
					jsonValue[stringVal] = boolAssign;
				
				break;
			case fftDate:
				stringAssign = getISO8601DateStringFromEXTFldVal(paramVal[1]);
				if (intVal >= 0 )
					jsonValue[intVal] = stringAssign;
				else
					jsonValue[stringVal] = stringAssign;
				
				break;
			default:
				break;
		}
		
		
	} else {
		// Assign values into the Value
		Json::Value mainValue;
		for (qshort i = 1; i <= paramNumber; ++i) {
		
		
		
		}
		
	}
	
	delete [] paramVal; // Clean-up
}

