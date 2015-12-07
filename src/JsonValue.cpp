/* JSON VALUE (IMPLEMENTATION)
 * 
 * This object wraps the functionality of the Json::Value class
 *
 * March 30, 2010 David McKeone (Created)
 */

#include "jsoncpp.he"
#include "JsonValue.he"

#include <string>
#include <map>

// Format of strings 
#include <boost/format.hpp>

using boost::shared_ptr;
using boost::format;

// Resource #'s for Value Type constants
const static qshort kConstValueTypeStart = 23001,
                    kConstValueTypeEnd   = 23008;


using namespace OmnisTools;

/**************************************************************************************************
 **                       CONSTRUCTORS / DESTRUCTORS                                             **
 **************************************************************************************************/

JsonValue::JsonValue(qobjinst objinst) : NVObjBase(objinst), document(new Json::Value())
{ 
	jsonValue = &(*document);
}

JsonValue::JsonValue(qobjinst objinst, tThreadData *pThreadData) : NVObjBase(objinst), document(new Json::Value())
{ 
	jsonValue = &(*document);
	
	// Constructor for $new(), interpret parameters (if any)
	pThreadData->mCurMethodName = "$new";
	qshort paramCount = ECOgetParamCount(pThreadData->mEci);
		
	methodInitialize(pThreadData, paramCount);
}

JsonValue::~JsonValue()
{ 
	
}

void JsonValue::copy( NVObjBase* pObj ) {
	// Copy in super class (This does *this = *pObj)
	NVObjBase::copy(pObj);
	
	// Copy the Json::Value and top pointer into the new object
	jsonValue = dynamic_cast<JsonValue*>(pObj)->jsonValue;
	document = dynamic_cast<JsonValue*>(pObj)->document;
}

// Get the internal Json::Value
Json::Value& JsonValue::getJsonValue() {
	return *jsonValue;
}

// Set the internal Json::Value
void JsonValue::setJsonValue(shared_ptr<Json::Value> v) {
	// Set the new value
	document = v;
	// Set the current data pointer
	jsonValue = &(*document);
}

void JsonValue::setJsonValue(boost::shared_ptr<Json::Value> v, Json::Value* pos) {
	// Set the new value
	document = v;
	// Set the current data pointer
	jsonValue = pos;
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
const static qshort cMethodConstruct      = 2100,
                    cMethodInitialize     = 2101,
                    cMethodIsNull         = 2102,
                    cMethodIsBool         = 2103,
                    cMethodIsInt          = 2104,
                    cMethodIsUInt         = 2105,
                    cMethodIsIntegral     = 2106,
                    cMethodIsDouble       = 2107,
                    cMethodIsNumeric      = 2108,
                    cMethodIsString       = 2109,
                    cMethodIsArray        = 2110,
                    cMethodIsObject       = 2111,
                    cMethodGet            = 2112,
                    cMethodSize           = 2113,
                    cMethodEmpty          = 2114,
                    cMethodClear          = 2115,
                    cMethodIsValidIndex   = 2116,
                    cMethodIsMember       = 2117,
                    cMethodGetMemberNames = 2118,
					cMethodCopy           = 2119,
					cMethodRoot           = 2120,
					cMethodSet            = 2121,
					cMethodValueToList    = 2122,
					cMethodListToValue    = 2123,
                    cMethodDelete         = 2124;

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
		case cMethodCopy:
			pThreadData->mCurMethodName = "$copy";
			methodCopy(pThreadData, paramCount);
			break;
		case cMethodRoot:
			pThreadData->mCurMethodName = "$root";
			methodRoot(pThreadData, paramCount);
			break;
		case cMethodSet:
			pThreadData->mCurMethodName = "$set";
			methodSet(pThreadData, paramCount);
			break;
        case cMethodValueToList:
			pThreadData->mCurMethodName = "$valueToList";
			methodValueToList(pThreadData, paramCount);
			break;
        case cMethodListToValue:
			pThreadData->mCurMethodName = "$listToValue";
			methodListToValue(pThreadData, paramCount);
			break;
        case cMethodDelete:
            pThreadData->mCurMethodName = "$delete";
            methodDelete(pThreadData, paramCount);
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
	4004, fftInteger,   0, 0,
	4005, fftCharacter, 0, 0,
    // $listToValue
    4006, fftList,      0, 0,
    // $delete
    4007, fftCharacter, 0, 0
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
	cMethodConstruct,      cMethodConstruct,      fftCharacter, 1, &cJsonValueMethodsParamsTable[0], 0, 0,
	cMethodInitialize,     cMethodInitialize,     fftCharacter, 1, &cJsonValueMethodsParamsTable[1], 0, 0,
	cMethodIsNull,         cMethodIsNull,         fftNone,      0, 0, 0, 0,
	cMethodIsBool,         cMethodIsBool,         fftNone,      0, 0, 0, 0,
	cMethodIsInt,          cMethodIsInt,          fftNone,      0, 0, 0, 0,
	cMethodIsUInt,         cMethodIsUInt,         fftNone,      0, 0, 0, 0,
	cMethodIsIntegral,     cMethodIsIntegral,     fftNone,      0, 0, 0, 0,
	cMethodIsDouble,       cMethodIsDouble,       fftNone,      0, 0, 0, 0,
	cMethodIsNumeric,      cMethodIsNumeric,      fftNone,      0, 0, 0, 0,
	cMethodIsString,       cMethodIsString,       fftNone,      0, 0, 0, 0,
	cMethodIsArray,        cMethodIsArray,        fftNone,      0, 0, 0, 0,
	cMethodIsObject,       cMethodIsObject,       fftNone,      0, 0, 0, 0,
	cMethodGet,            cMethodGet,            fftObject,    2, &cJsonValueMethodsParamsTable[2], 0, 0,
	cMethodSize,           cMethodSize,           fftInteger,   0, 0, 0, 0,
	cMethodEmpty,          cMethodEmpty,          fftBoolean,   0, 0, 0, 0,
	cMethodClear,          cMethodClear,          fftNone,      0, 0, 0, 0,
	cMethodIsValidIndex,   cMethodIsValidIndex,   fftBoolean,   1, &cJsonValueMethodsParamsTable[4], 0, 0,
	cMethodIsMember,       cMethodIsMember,       fftBoolean,   1, &cJsonValueMethodsParamsTable[5], 0, 0,
	cMethodGetMemberNames, cMethodGetMemberNames, fftList,      0, 0, 0, 0,
	cMethodCopy,           cMethodCopy,           fftObject,    0, 0, 0, 0,
	cMethodRoot,           cMethodRoot,           fftObject,    0, 0, 0, 0,
	cMethodSet,            cMethodSet,            fftNone,      0, 0, 0, 0,
    cMethodValueToList,    cMethodValueToList,    fftList,      0, 0, 0, 0,
    cMethodListToValue,    cMethodListToValue,    fftNone,      1, &cJsonValueMethodsParamsTable[6], 0, 0,
    cMethodDelete,         cMethodDelete,         fftObject,    1, &cJsonValueMethodsParamsTable[7], 0, 0
};

// List of methods in Simple
qlong JsonValue::returnMethods(tThreadData* pThreadData)
{
	const qshort cMethodCount = sizeof(cJsonValueMethodsTable) / sizeof(ECOmethodEvent);
	
	return ECOreturnMethods( gInstLib, pThreadData->mEci, &cJsonValueMethodsTable[0], cMethodCount );
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
	cPropertyValueType,     cPropertyValueType,     fftInteger,   EXTD_FLAG_PROPCUSTOM|EXTD_FLAG_EXTCONSTANT, 0, kConstValueTypeStart, kConstValueTypeEnd, /* Shows under Custom category */
	cPropertyValueTypeDesc, cPropertyValueTypeDesc, fftCharacter, EXTD_FLAG_PROPCUSTOM, 0, 0, 0, /* Shows under Custom category */
	cPropertyContents,      cPropertyContents,      fftCharacter, EXTD_FLAG_PROPCUSTOM, 0, 0, 0  /* Shows under Custom category */
};

// List of properties in this component
qlong JsonValue::returnProperties( tThreadData* pThreadData )
{
	const qshort propertyCount = sizeof(cJsonValuePropertyTable) / sizeof(ECOproperty);

	return ECOreturnProperties( gInstLib, pThreadData->mEci, &cJsonValuePropertyTable[0], propertyCount );
}

// Assignability of properties
qlong JsonValue::canAssignProperty( tThreadData* pThreadData, qlong propID ) {
	switch (propID) {
		case cPropertyValueType:
			return qfalse;
		case cPropertyValueTypeDesc:
			return qfalse;
		case cPropertyContents:
			return qfalse;
		default:
			return qfalse;
	}
}

// Method to retrieve a property of the object
qlong JsonValue::getProperty( tThreadData* pThreadData ) 
{
	EXTfldval fValReturn;
	
	qlong propID = ECOgetId( pThreadData->mEci );
	switch( propID ) {
		case cPropertyValueType:
			propertyValueType(fValReturn); // Put property into return value
			break;
		case cPropertyValueTypeDesc:
			propertyValueTypeDesc(fValReturn); // Put property into return value
			break;
		case cPropertyContents:
			getPropertyContents(fValReturn, pThreadData); // Put property into return value
			break;		    
	}
	
	ECOaddParam(pThreadData->mEci, &fValReturn); // Return to caller
	
	return 1L;
}

// Method to set a property of the object
qlong JsonValue::setProperty( tThreadData* pThreadData )
{	
	// Retrieve value to set for property, always in first parameter
	EXTfldval fVal;
	if( getParamVar( pThreadData->mEci, 1, fVal) == qfalse ) 
		return qfalse;
	
	// Assign to the appropriate property
	qlong propID = ECOgetId( pThreadData->mEci );
	switch( propID ) {
		case cPropertyValueType:
			// Value type is not an assignable property
			return qfalse;
		case cPropertyValueTypeDesc:
			// Value type description is not an assignable property
			return qfalse;
		case cPropertyContents:
			// Value type description is not an assignable property
			return qfalse;
		default:
			return qfalse;
	}
}

/**************************************************************************************************
 **                                         PROPERTIES                                           **
 **************************************************************************************************/

void JsonValue::propertyValueType(EXTfldval &retVal) {
	switch (jsonValue->type()) {
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
            // String can be a string or a date
            switch( isISO8601Date(jsonValue ->asString())) {
                case kISODateTime:
                    retVal.setConstant(kConstValueTypeStart, kConstValueTypeEnd, 8);
                    break;
                case kISODate:
                    retVal.setConstant(kConstValueTypeStart, kConstValueTypeEnd, 9);
                    break;
                case kISOTime:
                    retVal.setConstant(kConstValueTypeStart, kConstValueTypeEnd, 10);
                    break;
                default:
                    retVal.setConstant(kConstValueTypeStart, kConstValueTypeEnd, 4);
                    break;
            }
        
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
	switch (jsonValue->type()) {
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
            // String can be a string or a date
            switch( isISO8601Date(jsonValue->asString())) {
                case kISODateTime:
                    getEXTFldValFromString(retVal,"kJSONDateTimeValueType");
                    break;
                case kISODate:
                    getEXTFldValFromString(retVal,"kJSONDateValueType");
                    break;
                case kISOTime:
                    getEXTFldValFromString(retVal,"kJSONTimeValueType");
                    break;
                default:
                    getEXTFldValFromString(retVal,"kJSONStringValueType");
                    break;
            }
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
void JsonValue::getPropertyContents(EXTfldval &retVal, tThreadData* pThreadData) {
	switch (jsonValue->type()) {
		case Json::nullValue:
			retVal.setNull(fftCharacter, dpFcharacter);
			break;
		case Json::intValue:
            getEXTFldValFromInt(retVal, jsonValue->asInt());
			break;
		case Json::uintValue:
            getEXTFldValFromInt(retVal, jsonValue->asUInt());
			break;
		case Json::realValue:
            getEXTFldValFromDouble(retVal, jsonValue->asDouble());
			break;
		case Json::stringValue:
            // String can be a string or a date
            if (isISO8601Date(jsonValue ->asString())) {
                getEXTfldvalFromISO8601DateString(retVal, jsonValue->asString());
            } else {
                getEXTFldValFromString(retVal,jsonValue->asString());
            }
			break;
		case Json::booleanValue:
            getEXTFldValFromBool(retVal, jsonValue->asBool());
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

void JsonValue::getEXTfldvalForValue(tThreadData* pThreadData, EXTfldval &retVal, UInt indexValue) {
	Json::Value* posPointer = &((*jsonValue)[indexValue]);
	getEXTfldvalForValue(pThreadData, retVal, posPointer);
}

void JsonValue::getEXTfldvalForValue(tThreadData* pThreadData, EXTfldval &retVal, std::string keyValue) {
	Json::Value* posPointer = &((*jsonValue)[keyValue]);
	getEXTfldvalForValue(pThreadData, retVal, posPointer);
}

void JsonValue::getEXTfldvalForValue(tThreadData* pThreadData, EXTfldval &retVal, Json::Value* posPointer) {
	// Create return value object
	JsonValue *newObj = createNVObj<JsonValue>(pThreadData);
	if ( posPointer != 0 ) {
		// Return pointer to found position
		newObj->setJsonValue(document, posPointer); // Set value and pointer to source
	} else {
		// Create and return new null object
		shared_ptr<Json::Value> nullObj(new Json::Value());
		newObj->setJsonValue(nullObj);
	}
	
	retVal.setObjInst( newObj->getInstance(), qtrue );
}

// Helper method to initialize the jsonValue to a new value.
void JsonValue::setValueFromEXTfldval(tThreadData* pThreadData, EXTfldval &fVal) {
	// Get the data type of the parameter
	ffttype valType; fVal.getType(valType);
	
	// Perform appropriate initialization for each type
	if (valType == fftCharacter) {
		*jsonValue = getStringFromEXTFldVal(fVal);
	} else if (valType == fftInteger) {
		*jsonValue = getIntFromEXTFldVal(fVal);
	} else if (valType == fftNumber) {
		*jsonValue = getDoubleFromEXTFldVal(fVal);
	} else if (valType == fftBoolean) {
		*jsonValue = getBoolFromEXTFldVal(fVal);
    } else if (valType == fftDate) {
        *jsonValue = getISO8601DateStringFromEXTFldVal(fVal);
	} else if (valType == fftObject) {
		JsonValue* assignObj = getObjForEXTfldval<JsonValue>(pThreadData, fVal);
		if (assignObj) {
			*jsonValue = *(assignObj->jsonValue);
		}
	}
}

// Helper method to set the jsonValue for a specific array position
void JsonValue::setValueFromEXTfldval(tThreadData* pThreadData, EXTfldval &fVal, int group) {
	// Get the data type of the parameter
	ffttype valType; fVal.getType(valType);
	
	// Perform appropriate initialization for each type
	if (valType == fftCharacter) {
		(*jsonValue)[group] = getStringFromEXTFldVal(fVal);
	} else if (valType == fftInteger) {
		(*jsonValue)[group] = getIntFromEXTFldVal(fVal);
	} else if (valType == fftNumber) {
		(*jsonValue)[group] = getDoubleFromEXTFldVal(fVal);
	} else if (valType == fftBoolean) {
		(*jsonValue)[group] = getBoolFromEXTFldVal(fVal);
	} else if (valType == fftDate) {
        (*jsonValue)[group] = getISO8601DateStringFromEXTFldVal(fVal);
    }
}

// Helper method to set the jsonValue for a specific group
void JsonValue::setValueFromEXTfldval(tThreadData* pThreadData, EXTfldval &fVal, std::string group) {
	(*jsonValue)[group] = Json::Value();
	
	// Get the data type of the parameter
	ffttype valType; fVal.getType(valType);
	
	// Perform appropriate initialization for each type
	if (valType == fftCharacter) {
		(*jsonValue)[group] = getStringFromEXTFldVal(fVal);
	} else if (valType == fftInteger) {
		(*jsonValue)[group] = getIntFromEXTFldVal(fVal);
	} else if (valType == fftNumber) {
		(*jsonValue)[group] = getDoubleFromEXTFldVal(fVal);
	} else if (valType == fftBoolean) {
		(*jsonValue)[group] = getBoolFromEXTFldVal(fVal);
	} else if (valType == fftDate) {
        (*jsonValue)[group] = getISO8601DateStringFromEXTFldVal(fVal);
    }
	
	// If no parameters were passed or an object couldn't be found, then init to NULL
	if ( (*jsonValue)[group].isNull() ) {
		(*jsonValue)[group] = Json::Value();
	}
}

// Helper method to initialize the jsonValue to a new value.
void JsonValue::setValueFromParameter(tThreadData *pThreadData, qshort paramNumber) {
	EXTParamInfo* param = ECOfindParamNum( pThreadData->mEci, paramNumber );
	if( param ) { 
		// Interpret the parameter we have
		EXTfldval fVal; 
        fVal.setFldVal((qfldval)param->mData);
		
		setValueFromEXTfldval(pThreadData, fVal);
	}
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
	getEXTFldValFromBool(fValReturn, jsonValue->isNull());
	ECOaddParam(pThreadData->mEci, &fValReturn); // Return to caller
}

void JsonValue::methodIsBool( tThreadData* pThreadData, qshort pParamCount )
{
	EXTfldval fValReturn;
	getEXTFldValFromBool(fValReturn, jsonValue->isBool());
	ECOaddParam(pThreadData->mEci, &fValReturn); // Return to caller
}

void JsonValue::methodIsInt( tThreadData* pThreadData, qshort pParamCount )
{
	EXTfldval fValReturn;
	getEXTFldValFromBool(fValReturn, jsonValue->isInt());
	ECOaddParam(pThreadData->mEci, &fValReturn); // Return to caller
}

void JsonValue::methodIsUInt( tThreadData* pThreadData, qshort pParamCount )
{
	EXTfldval fValReturn;
	getEXTFldValFromBool(fValReturn, jsonValue->isUInt());
	ECOaddParam(pThreadData->mEci, &fValReturn); // Return to caller
}

void JsonValue::methodIsIntegral( tThreadData* pThreadData, qshort pParamCount )
{
	EXTfldval fValReturn;
	getEXTFldValFromBool(fValReturn, jsonValue->isIntegral());
	ECOaddParam(pThreadData->mEci, &fValReturn); // Return to caller
}

void JsonValue::methodIsDouble( tThreadData* pThreadData, qshort pParamCount )
{
	EXTfldval fValReturn;
	getEXTFldValFromBool(fValReturn, jsonValue->isDouble());
	ECOaddParam(pThreadData->mEci, &fValReturn); // Return to caller
}

void JsonValue::methodIsNumeric( tThreadData* pThreadData, qshort pParamCount )
{
	EXTfldval fValReturn;
	getEXTFldValFromBool(fValReturn, jsonValue->isNumeric());
	ECOaddParam(pThreadData->mEci, &fValReturn); // Return to caller
}

void JsonValue::methodIsString( tThreadData* pThreadData, qshort pParamCount )
{
	EXTfldval fValReturn;
	getEXTFldValFromBool(fValReturn, jsonValue->isString());
	ECOaddParam(pThreadData->mEci, &fValReturn); // Return to caller
}

void JsonValue::methodIsArray( tThreadData* pThreadData, qshort pParamCount )
{
	EXTfldval fValReturn;
	getEXTFldValFromBool(fValReturn, jsonValue->isArray());
	ECOaddParam(pThreadData->mEci, &fValReturn); // Return to caller
}

void JsonValue::methodIsObject( tThreadData* pThreadData, qshort pParamCount )
{
	EXTfldval fValReturn;
    getEXTFldValFromBool(fValReturn, jsonValue->isObject());
	ECOaddParam(pThreadData->mEci, &fValReturn); // Return to caller
}

// JsonValue retrieval.  This implements the operator[] and $get() methods for Json::Value since there are no operators in Omnis.
// Call with $get('Settings') for settings key in object or $get(0) for an array
void JsonValue::methodGet( tThreadData* pThreadData, qshort pParamCount )
{
	EXTfldval keyVal, defVal, retVal;
	
	if ( getParamVar(pThreadData, 1, keyVal) == qfalse)
		return;
	
	std::string keyValue;
	UInt indexValue;
	ffttype valType; keyVal.getType(valType);
	
	if (valType == fftInteger) {
		if (jsonValue->isArray()) {
			indexValue = static_cast<UInt>( keyVal.getLong() );
			getEXTfldvalForValue(pThreadData, retVal, indexValue);
		}
	} else if (valType == fftCharacter) {
		if (jsonValue->isObject()) {
			keyValue = getStringFromEXTFldVal( keyVal );
			getEXTfldvalForValue(pThreadData, retVal, keyValue);
			
		}
	}
	
	ECOaddParam( pThreadData->mEci, &retVal );
}

// Create a copy of the pointer at the current position.  This copied pointer is trimmed above the current position.
void JsonValue::methodCopy( tThreadData* pThreadData, qshort pParamCount ) {
	shared_ptr<Json::Value> copyValue(new Json::Value(*jsonValue));
	
	JsonValue *newObj = createNVObj<JsonValue>(pThreadData);
	newObj->setJsonValue(copyValue);
	
	EXTfldval retVal;
	retVal.setObjInst( newObj->getInstance(), qtrue ); 
	ECOaddParam( pThreadData->mEci, &retVal );
}

// Get a pointer to the root of the current object
void JsonValue::methodRoot( tThreadData* pThreadData, qshort pParamCount ) {
	JsonValue *newObj = createNVObj<JsonValue>(pThreadData);
	newObj->setJsonValue(document);
	
	EXTfldval retVal;
	retVal.setObjInst( newObj->getInstance(), qtrue ); 
	ECOaddParam( pThreadData->mEci, &retVal );
}

// Return size
void JsonValue::methodSize( tThreadData* pThreadData, qshort pParamCount ) {
	EXTfldval fValReturn;
    getEXTFldValFromInt(fValReturn, jsonValue->size());
	ECOaddParam(pThreadData->mEci, &fValReturn);
}

// Returns true if Value object empty
void JsonValue::methodEmpty( tThreadData* pThreadData, qshort pParamCount ) {
	EXTfldval fValReturn;
    getEXTFldValFromBool(fValReturn, jsonValue->empty());
	ECOaddParam(pThreadData->mEci, &fValReturn);
}

// Clears the contents of the Value object
void JsonValue::methodClear( tThreadData* pThreadData, qshort pParamCount ) {
	jsonValue->clear();
}

// Returns true if the index is in the array
void JsonValue::methodIsValidIndex( tThreadData* pThreadData, qshort pParamCount ) {
	EXTfldval indexVal, retVal;
	
	if ( getParamVar(pThreadData, 1, indexVal) == qfalse)
		return;
	
	int index = getIntFromEXTFldVal(indexVal);
	
	// Return to Omnis
	EXTfldval fValReturn;
    getEXTFldValFromBool(fValReturn, jsonValue->isValidIndex(index));
	ECOaddParam(pThreadData->mEci, &fValReturn);
}

// Return true if key (param 1) is in the object
void JsonValue::methodIsMember( tThreadData* pThreadData, qshort pParamCount ) {
	
	EXTfldval keyVal, retVal;
	
	if ( getParamVar(pThreadData, 1, keyVal) == qfalse)
		return;
	
	std::string key = getStringFromEXTFldVal(keyVal);
	
	// Return to Omnis
	EXTfldval fValReturn;
    getEXTFldValFromBool(fValReturn, jsonValue->isMember(key));
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
	if ( !(jsonValue->isObject()) ) {
		fValReturn.setList(retList, qtrue, qfalse); 
		ECOaddParam(pThreadData->mEci, &fValReturn);
		return;
	}
	
	Json::Value::Members memberNames = jsonValue->getMemberNames();
	
	// Loop the member names to create the list
	std::string rowMember;
	for (qlong lineNumber = 0; lineNumber < static_cast<qlong>(memberNames.size()); lineNumber++) {
		
		retList->insertRow(0);
		//retList->setCurRow( lineNumber+1 );
		retList->getColValRef(lineNumber+1, 1, fValMember, qtrue);
		
		// Retrieve member name and place it in the list
		rowMember = memberNames[lineNumber];
		getEXTFldValFromString(fValMember, rowMember);
	}
	
	fValReturn.setList(retList, qtrue, qfalse); 
	ECOaddParam(pThreadData->mEci, &fValReturn);
	return;
}

// This method sets the value for the current position to the first parameter
void JsonValue::methodSet( tThreadData* pThreadData, qshort pParamCount) {
	EXTfldval fVal;
	
	if ( getParamVar(pThreadData, 1, fVal) == qfalse)
		return;
	
	setValueFromEXTfldval(pThreadData, fVal);
}

// This method removes the key given in the first parameter
void JsonValue::methodDelete( tThreadData* pThreadData, qshort pParamCount) {
    EXTfldval fVal, retVal;
    
    if ( getParamVar(pThreadData, 1, fVal) == qfalse)
        return;
    
    if (!jsonValue->isObject()) 
        return;
    
    Json::Value old = jsonValue->removeMember(getStringFromEXTFldVal(fVal));
    if (!old.isNull()) {
        getEXTfldvalForValue(pThreadData, retVal, new Json::Value(old));
    }
    
    ECOaddParam( pThreadData->mEci, &retVal );
}

// Convert the current value object into a list
void JsonValue::methodValueToList( tThreadData* pThreadData, qshort pParamCount ) {
    
    if (!(jsonValue->isObject() || jsonValue->isArray())) {
        pThreadData->mExtraErrorText = "Value is not an object or an array.  Can only convert single objects or arrays of objects to a list."; 
    }
    
    EXTfldval retVal;
    EXTqlist* retList = new EXTqlist(listVlen);
    
    str255 colName;
    EXTfldval colVal, colNameVal;
    
    // Write container object directly to list
    if ( writeValueToList(pThreadData, retList, 0, jsonValue, 0,0) ) {
        // Return list
        retVal.setList(retList, qtrue);
        ECOaddParam(pThreadData->mEci, &retVal);

    } else {
        delete retList;
        // DEV NOTE: writeValueToList will write extra error text
    }    
}

// Convert a list into the current value object
void JsonValue::methodListToValue( tThreadData* pThreadData, qshort pParamCount ) {
    
    EXTqlist inList;
    if (getParamList(pThreadData, 1, inList) == qfalse) {
        pThreadData->mExtraErrorText = "Unrecognized parameter 1.  Expected list.";
        return;
    }
    
    EXTfldval colVal, colValName;
    str255 colName;
    Json::Value obj;
    
    // Resize array to correct number of elements (since we know what it is)
    jsonValue->resize(Json::UInt(inList.rowCnt()));
    
    // Loop all rows and all columns and create a Json::Value.  Each row is 1 line in an array and each column is part of an object.
    for (qshort row = 1; row <= inList.rowCnt(); ++row) {
        // Create new object for each row
        obj = Json::Value();  
        
        for (qshort col = 1; col <= inList.colCnt(); ++col) {
            inList.getColValRef(row, col, colVal, qfalse);
            
            inList.getCol(col, colName);
            colValName.setChar(colName);
            
            // Must ignore binary and picture data since it is not support by JSON
            if ( !(getType(colVal).valType == fftBinary || getType(colVal).valType == fftPicture)) {
                
                // Also ignore lists for the time being
                if (getType(colVal).valType != fftList) {
                    getValueFromEXTFldVal(pThreadData, colVal, obj, getStringFromEXTFldVal(colValName));
                }
            }
        }
        
        (*jsonValue)[Json::UInt(row-1)] = obj;
    }
}

// Add a column to a list based on the Json::Value type
qshort JsonValue::addColForValue(EXTqlist* list, Json::Value* val, str255& colName) {
    
    qshort colNum = 0;
    
    // Perform appropriate initialization for each type
    switch (val->type()) {
        case Json::nullValue:
            colNum = list->addCol(fftCharacter, dpFcharacter, 10000000, &colName);
            break;
        case Json::intValue:
            colNum = list->addCol(fftInteger, 0, 10000000, &colName);
            
            break;
        case Json::uintValue:
            colNum = list->addCol(fftInteger, 0, 10000000, &colName);
            break;
        case Json::realValue:
            colNum = list->addCol(fftNumber, dpFloat, 0, &colName);
            break;
        case Json::stringValue:
            // String can also be a date, check the type
            switch( isISO8601Date(val->asString()) ) {
                case kISODateTime:
                    colNum = list->addCol(fftDate, dpFdtimeC, 0, &colName);
                    break;
                case kISODate:
                    colNum = list->addCol(fftDate, dpFdate2000, 0, &colName);
                    break;
                case kISOTime:
                    colNum = list->addCol(fftDate, dpFtime, 0, &colName);
                    break;
                default:
                    colNum = list->addCol(fftCharacter, dpFcharacter, 10000000, &colName);
                    break;
            }
            
            break;
        case Json::booleanValue:
            colNum = list->addCol(fftBoolean, dpDefault, 0, &colName);
            break;
        case Json::arrayValue:
        case Json::objectValue:
            colNum = list->addCol(fftList, dpDefault, 0, &colName);
            break;
        default:
            break;
    }
    
    return colNum;
}

// Get an EXTFldVal based on a Json::Value
void JsonValue::getValueFromEXTFldVal(tThreadData* pThreadData, EXTfldval& fVal, Json::Value& val, std::string label) {
    // Perform appropriate initialization for each type
    
    FieldValType theType = getType(fVal);
    
    if (theType.valType == fftCharacter) {
        val[label] = getStringFromEXTFldVal(fVal);
    } else if (theType.valType == fftInteger) {
        val[label] = getIntFromEXTFldVal(fVal);
    } else if (theType.valType == fftNumber) {
        val[label] = getDoubleFromEXTFldVal(fVal);
    } else if (theType.valType == fftDate) {
        val[label] = getISO8601DateStringFromEXTFldVal(fVal);
    } else if (theType.valType == fftBoolean) {
        val[label] = getBoolFromEXTFldVal(fVal);
    }
}

// Get an EXTFldVal based on a Json::Value
void JsonValue::getEXTFldValFromValue(tThreadData* pThreadData, EXTfldval& fVal, Json::Value* val) {
    
    std::string stringCheck;
    
    // Perform appropriate initialization for each type
    switch (val->type()) {
        case Json::nullValue:
            fVal.setNull(fftCharacter, dpFcharacter);
            break;
        case Json::intValue:
            getEXTFldValFromInt(fVal, val->asInt());
            break;
        case Json::uintValue:
            getEXTFldValFromInt(fVal, val->asUInt());
            break;
        case Json::realValue:
            getEXTFldValFromDouble(fVal, val->asDouble());
            break;
        case Json::stringValue:
            // String could also be a date value in ISO8601 format
            stringCheck = val->asString();
                      
            // Check which kind of string it is
            if( isISO8601Date(stringCheck) ) {
                getEXTfldvalFromISO8601DateString(fVal, stringCheck);
            } else {
                // Must be a string
                getEXTFldValFromString(fVal, stringCheck);
            }
            break;
        case Json::booleanValue:
            getEXTFldValFromBool(fVal, val->asBool());
            break;
        case Json::arrayValue:
        case Json::objectValue:
            // Object and array handled above
            break;
        default:
            break;
    }
}


// Write value to passed in list
bool JsonValue::writeValueToList( tThreadData* pThreadData, EXTqlist* list, Json::Value* parent, Json::Value* val, qlong row, qshort col, const char* parentName) 
{    
    EXTqlist* innerList;
    EXTqlist* arrayList;
    str255 colName;
    EXTfldval colVal, colNameVal;
    std::string label;
    
    // Variables for jsoncpp
    Json::Value innerVal;
    Json::Value::Members memberNames;
    
    // Based on the type of the value write to a list
    if (val->isNull()) {
        // Nothing to do
    } else if (val->isObject()) {
        // Write a single row for the object with a column name as the name value pair
        
        // Get member names
        memberNames = val->getMemberNames();
        
        // Determine if we are adding to an existing (top-level) list or into a column
        if (col == 0) {
            innerList = list;
        } else {
            list->getColValRef(row, col, colVal, qtrue);
            if (getType(colVal).valType == fftList) {
                innerList = colVal.getList(qfalse);
            } else {
                innerList = new EXTqlist(listVlen);
                colVal.setList(innerList,qtrue);
            }
        }
        
        // Build set of column names in the list
        std::map<std::string, qshort> colMap;
        std::map<std::string, qshort>::iterator colMapIterator;
        qshort colCount = innerList->colCnt();
        for(qshort col = 1; col <= colCount; ++col) {
            // Get column name
            innerList->getCol(col,qfalse,colName);
            
            // Convert to string and insert in map
            colVal.setChar(colName);
            colMap[getStringFromEXTFldVal(colVal)] = col;
        }
        
        // Loop all member names and create a column for each (if applicable)
        for (qshort c = 1; c <= (qshort) val->size(); ++c) {
            // Check for existing column name and add if it's missing
            colMapIterator = colMap.find(memberNames[c-1]);
            if(colMapIterator == colMap.end()) {
                // Add column for member
                colName = initStr255(memberNames[c-1].c_str());  // jsoncpp index is off of 0
                innerVal = (*val)[memberNames[c-1]];  // jsoncpp index is off of 0
                
                // Add column and add to column map
                colMap[memberNames[c-1]] = addColForValue(innerList, &innerVal, colName);
            }             
        }
        
        // Add a row and write all the values
        qlong rowNum = innerList->insertRow();
        if (rowNum > 0) {    
            for (qshort c = 1; c <= (qshort) val->size(); ++c) {
                // Get Json::Value and write to column
                innerVal = (*val)[memberNames[c-1]];  // jsoncpp index is off of 0
                writeValueToList(pThreadData, innerList, val, &innerVal, rowNum, colMap[memberNames[c-1]], memberNames[c-1].c_str());
            }
        }
        
    } else if (val->isArray()) {        
        // Determine if we are adding to an existing (top-level) list or into a column
        if (col == 0) {
            innerList = list;
        } else {
            list->getColValRef(row, col, colVal, qtrue);
            innerList = colVal.getList(qfalse);
        }
        
        innerVal = (*val)[Json::UInt(0)];  // Get first element
        if( innerVal.isObject() ) {            
            // Array of objects; this means a list in Omnis terms
            for (qshort r = 1; r <= (qshort) val->size(); ++r) {
                // Get Json::Value and write to column
                innerVal = (*val)[r-1];  // jsoncpp index is off of 0
                writeValueToList(pThreadData, innerList, val, &innerVal, r, 0);
            }
        } else {
            // Plain array
            if (col == 0) {
                // Stand-alone, just set the list up as the array
                arrayList = innerList;
                
            } else {
                // Part of a column
                arrayList = new EXTqlist(listVlen);
                
            }
            
            // Setup name of column to be parent element (if there is one) otherwise just name it "Element"
            if(parentName) {
                colName = initStr255(parentName);
            } else {
                colName = initStr255("Element");
            }
            
            innerVal = (*val)[Json::UInt(0)]; // Get first element in array
            addColForValue(arrayList, &innerVal, colName);
            
            for (qshort r = 1; r <= (qshort) val->size(); ++r) {
                innerVal = (*val)[r-1];  // jsoncpp index is off of 0
                writeValueToList(pThreadData, arrayList, val, &innerVal, r, 0);
            }
            
            if (col != 0) {
                colVal.setList(arrayList, qtrue); // Set column in list
            }
        }
        
    } else {
        // Regular value
        if (row > 0 && col > 0) {
            // Can only write regular value if column has already been created
            EXTfldval tempVal;
            getEXTFldValFromValue(pThreadData, tempVal, val);
            list->putColVal(row, col, tempVal);
        }
    }
    
    return true;
}

