/* OMNIS TOOLS (HEADER)
 * 
 * This is a colleciton of tools for creating omnis components
 *
 * NOTE: All headers for external components must have the .he extension instead of the standard .h
 *       To enable code-highliting in Visual Studio 2008 go to Tools->Options-Text Editor-File Extension 
 *       and add "he" as a "Microsoft Visual C++" experience.
 *
 * October 4, 2010 David McKeone (Created)
 */

#include "OmnisTools.he"
#include <sstream>

// Get a parameter from the thread data
qbool OmnisTools::getParamVar( tThreadData* pThreadData, qshort pParamNum, EXTfldval& pOutVar )
{
	return getParamVar(pThreadData->mEci, pParamNum, pOutVar);
}

// Get a parameter from the EXTCompInfo
qbool OmnisTools::getParamVar( EXTCompInfo* pEci, qshort pParamNum, EXTfldval& pOutVar )
{
	EXTParamInfo* param = ECOfindParamNum( pEci, pParamNum );
	if( !param )
		return qfalse;
	
	pOutVar.setFldVal((qfldval)param->mData);
	pOutVar.setReadOnly( qfalse );
	
	return qtrue;
}

// Get a qbool from a C++ boolean
qbool OmnisTools::getQBoolFromBool(bool b) {
	if (b == true) {
		return 2;
	} else if (b == false) {
		return 1;
	} else {
		return 0;
	}
}

// Get a C++ boolean from a qbool
bool OmnisTools::getBoolFromQBool(qbool qb) {
	if (qb == 2) {
		return true;
	} else if (qb == 1) {
		return false;
	} else {
		return NULL;
	}
}

// Get double from EXTfldval
double OmnisTools::getDoubleFromEXTFldVal(EXTfldval& fVal) {
	// Create intermediate qreal value and decimal place constant
	qreal realValue;
	qshort dpValue = dpDefault;
	
	// Fetch qreal value
	fVal.getNum(realValue, dpValue);
	
	// Cast and return double
	return static_cast<double>(realValue);
}

// Assign EXTfldval from double
void OmnisTools::getEXTFldValFromDouble(EXTfldval& fVal, double doubleValue) {
	// Cast double into qreal
	qreal realValue = static_cast<qreal>(doubleValue);
	
	// Assign decimal place constant
	qshort dpValue = dpDefault;
	
	// Put qreal in EXTfldval
	fVal.setNum(realValue, dpValue);
}

// Get a std::string from an EXTfldval object
std::string OmnisTools::getStringFromEXTFldVal(EXTfldval& fVal) {
	std::string retString;
	
	// Get a qchar* string
	qlong maxLength = fVal.getBinLen()+1; // Use binary length as approximation of maximum size
	qlong length = 0;
	qchar* omnisString = new qchar[maxLength];
	fVal.getChar(maxLength, omnisString, length, qfalse);
	length++; // Increment for \0 string terminator
	
	// Translate qchar* string into UTF8 binary
	qbyte* utf8data = new qbyte[length];
	CHRunicode::charToUtf8(omnisString, length, utf8data);
	
	// Copy UTF8 binary into char* string
	char* cString = new char[length];
	memcpy(cString, utf8data, length);
	cString[length] = '\0'; // Set terminator
	
	// Create standard string
	retString = std::string(cString);
	
	// Clean-up
	delete [] omnisString;
	delete [] utf8data;
	delete [] cString;
	
	return retString;
}

// Set an existing EXTfldval object from a std::string
void OmnisTools::getEXTFldValFromString(EXTfldval& fVal, std::string readString) {
	qlong length;
	qchar* omnisString = getQCharFromString( readString, length);
	
	fVal.setChar(omnisString, length-1); // Set value of character field, but exclude the last character since it will be the null terminator from the C String
	
	// Clean-up
	delete [] omnisString;
}

qchar* OmnisTools::getQCharFromString( std::string readString, qlong &retLength ) {
	qlong length = readString.size()+1; // Include /0 terminator in length
	
	// Initialize C string and setup variables needed for memory copy
	char* cString = new char[length];
	strcpy(cString, readString.c_str()); //Get C String
	qbyte* utf8data = new qbyte[length];
	qchar* omnisString = new qchar[length];
	
	// Copy C string into qbyte buffer
	memcpy(utf8data, cString, length);
	
	// Convert to Omnis Character field
	CHRunicode::utf8ToChar(utf8data, length, omnisString);  // Convert characters into Omnis Char Field
	
	retLength = length;
	
	// Clean-up
	delete [] utf8data;
	delete [] cString;
	
	return omnisString;
}

// Get an ISO 8601 Formatted Date String from EXTFldVal
std::string OmnisTools::getISO8601DateStringFromEXTFldVal(EXTfldval& fVal) {
	datestamptype theDate;
	std::string retString;
	std::stringstream sin(retString);
	
	fVal.getDate(theDate, dpFdtimeC);
	ffttype valType; qshort subType; fVal.getType(valType, &subType);
	
	if (valType != fftDate)
		return "";
	
	// Set date part of string
	switch (subType) {
		case dpFdate1900:
		case dpFdate1980:
		case dpFdate2000:
		case dpFdtime1900:
		case dpFdtime1980:
		case dpFdtime2000:
	    case dpFdtimeC:
			sin << theDate.mYear << "-" << theDate.mMonth << "-" << theDate.mDay; 
			break;
		default:
			break;
	}
	
	// Set time part of string
	switch (subType) {
		case dpFdtime1900:
		case dpFdtime1980:
		case dpFdtime2000:
	    case dpFdtimeC:
		case dpFtime:
			sin << "T" << theDate.mHour << ":" << theDate.mMin << ":" << theDate.mSec;
			break;
		default:
			break;
	}
	
}



