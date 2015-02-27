/* OMNIS TOOLS (IMPLEMENTATION)
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
#include <iostream>
#include <map>
#include <string>

#ifdef USE_BOOST
#include <boost/lexical_cast.hpp>
#include <boost/regex.hpp>

using boost::lexical_cast;
using boost::bad_lexical_cast;
#endif

#if ! defined(MARKUP_SIZEOFWCHAR)
#if __SIZEOF_WCHAR_T__ == 4 || __WCHAR_MAX__ > 0x10000
#define MARKUP_SIZEOFWCHAR 4
#else
#define MARKUP_SIZEOFWCHAR 2
#endif
#endif

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
	
	pOutVar.setFldVal(reinterpret_cast<qfldval>(param->mData));
	pOutVar.setReadOnly( qfalse );
	
	return qtrue;
}

qbool OmnisTools::getParamList( tThreadData* pThreadData, qshort pParamNum, EXTqlist& pOutList, qbool pCanBeRow) {
	
	EXTParamInfo* param = ECOfindParamNum( pThreadData->mEci, pParamNum );
	if ( !param ) 
		return qfalse;
	
	EXTfldval fval(reinterpret_cast<qfldval>(param->mData));
	if ( isList(fval,pCanBeRow) != qtrue)
		return qfalse;
	
	fval.getList(&pOutList,qfalse);
	
	return qtrue;
}

qbool OmnisTools::isList( EXTfldval& pFVal, qbool pCanBeRow ) {
	ffttype fft; pFVal.getType(fft);
	return ( (fft == fftList || (pCanBeRow && fft == fftRow)) ? qtrue : qfalse );
}

qbool OmnisTools::getParamBool( tThreadData* pThreadData, qshort pParamNum, qbool& pOutBool ) {
	EXTParamInfo* param = ECOfindParamNum( pThreadData->mEci, pParamNum );
	if ( !param )
		return qfalse;
	
	EXTfldval fval(reinterpret_cast<qfldval>(param->mData));
	
	ffttype fft; fval.getType(fft);
	if ( fft != fftInteger && fft != fftBoolean && fft != fftConstant )
		return qfalse;
	
	qlong x = fval.getLong();
	if ( x != 0 && x != 1 )
		return qfalse;
	
	pOutBool = qbool(x != 0);
	return qtrue;
}

qbool OmnisTools::getParamShort( tThreadData* pThreadData, qshort pParamNum, qshort& pOutShort ) {
	
	qlong longVal;
	if( getParamLong(pThreadData, pParamNum, longVal) != qtrue )
		return qfalse;
	
	if (longVal < 0 || longVal > 255)
		return qfalse;
	
	pOutShort = static_cast<qshort>(longVal);
	return qtrue;	
}

qbool OmnisTools::getParamLong( tThreadData* pThreadData, qshort pParamNum, qlong& pOutInteger ) {
	EXTParamInfo* param = ECOfindParamNum( pThreadData->mEci, pParamNum );
	if ( !param )
		return qfalse;
	
	EXTfldval fval(reinterpret_cast<qfldval>(param->mData));
	
	ffttype fft; fval.getType(fft);
	if ( fft == fftCharacter )
	{
		strxxx& s_num = fval.getChar();
		if ( stringToQlong( s_num, pOutInteger ) )
			return qtrue;
	}
	else if ( fft == fftInteger || fft == fftBoolean || fft == fftConstant || fft == fftNumber )
	{
		pOutInteger = fval.getLong();
		return qtrue;
	}
	
	return qfalse;
}

// Convenience method to return the type of an EXTfldval
OmnisTools::FieldValType OmnisTools::getType( EXTfldval& fVal ) {
	
	OmnisTools::FieldValType retType;
	fVal.getType(retType.valType, &retType.valSubType);
	
	return retType;
}

// Get a qbool from a C++ boolean
qbool OmnisTools::getQBoolFromBool(bool b) {
	if (b == true) {
		return qtrue;
	} else if (b == false) {
		return qfalse;
	} else {
		return 0;
	}
}

// Get a C++ boolean from a qbool
bool OmnisTools::getBoolFromQBool(qbool qb) {
	if (qb == qtrue) {
		return true;
	} else if (qb == qfalse) {
		return false;
	} else {
		return NULL;
	}
}

// Get a std::wstring from an EXTfldval object
std::wstring OmnisTools::getWStringFromEXTFldVal(EXTfldval& fVal) {
	std::wstring retString;
	
	// Get a qchar* string
	qlong maxLength = fVal.getBinLen()+1; // Use binary length as approximation of maximum size
	qlong length = 0, stringLength = 0;
	qchar* omnisString = new qchar[maxLength];
	fVal.getChar(maxLength, omnisString, length);
	
	wchar_t* cString;
#if MARKUP_SIZEOFWCHAR == 2
	// For 2-Byte UTF16 wchar_t* (Typically Windows)
	// Convert from UTF8 to UTF16 and set new stringLength
	
	// Translate qchar* string into UTF8 binary
	qbyte* utf8data = reinterpret_cast<qbyte*>(omnisString);
	stringLength = CHRunicode::charToUtf8(omnisString, length, utf8data);
	
	// Translate UTF8 to UTF16
	CHRconvToUtf16 utf16conv(utf8data, stringLength);
	UChar* utf16data = utf16conv.dataPtr();
	stringLength = utf16conv.len();
	
	// Translate UTF16 binary into char* string
	cString = reinterpret_cast<wchar_t*>(utf16data);
#else
	// For 4-Byte UTF32 wchar_t* (Typically Mac and Linux)
	// Convert from UTF8 to UTF32 and set new stringLength
	stringLength = length;
	CHRconvToUtf32FromChar utf32conv(omnisString, stringLength, qfalse);
	U32Char* utf32data = utf32conv.dataPtr();
	stringLength = utf32conv.len();
	
	// Translate UTF16 binary into char* string
	cString = reinterpret_cast<wchar_t*>(utf32data);
#endif
	
	// Create standard string
	retString = std::wstring(cString,stringLength);
	
	// Clean-up
	delete [] omnisString;
	
	return retString;
}

// Set an existing EXTfldval object from a std::wstring
void OmnisTools::getEXTFldValFromWString(EXTfldval& fVal, const std::wstring readString) {
	qlong length;
	qchar* omnisString = getQCharFromWString(readString, length);
	
	fVal.setChar(omnisString, length); // Set value of character field, but exclude the last character since it will be the null terminator from the C String
	
	// Clean-up
	delete [] omnisString;
}

// Set an existing EXTfldval object from a std::wstring
void OmnisTools::getEXTFldValFromWChar(EXTfldval& fVal, const wchar_t* readChar) {
    std::wstring readString;
    if (readChar == 0)
        readString = L"";
    else
        readString = readChar;
    
    getEXTFldValFromWString(fVal, readString);
}

// Get a dynamically allocated qchar* array from a std::wstring
qchar* OmnisTools::getQCharFromWString(const std::wstring readString, qlong &retLength) {
	qlong length = readString.size();
	
	// Cast-away constness of c_str() pointer 
	wchar_t* cString = const_cast<wchar_t*>(readString.c_str());
	
    // Declare pointer to new data
    qchar* omnisString;
    
#if MARKUP_SIZEOFWCHAR == 2
	// For 2-Byte UTF16 wchar_t* (Typically Windows)
	// Feed into raw byte data
	UChar* utf16data = reinterpret_cast<UChar*> (cString);
	
	// Convert to UTF-8
	CHRconvFromUtf16 utf16conv(utf16data, length);
	length = utf16conv.len();
	qbyte* utf8data = utf16conv.dataPtr();
	
	// Allocate new qchar* string
	omnisString = new qchar[length];
	
	// Convert to Omnis Character field
	retLength = CHRunicode::utf8ToChar(utf8data, length, omnisString);  // Convert characters into Omnis Char Field
#else
	// For 4-Byte UTF32 wchar_t* (Typically Mac and Linux)
	U32Char* utf32data = reinterpret_cast<U32Char*> (cString);
	
	// Convert to UTF-8
	CHRconvFromUtf32ToChar utf32conv(utf32data, length, qfalse);
	retLength = length = utf32conv.len();
	
	omnisString = new qchar[length];
	OMstrcpy(omnisString, utf32conv.dataPtr()); // Copy string so it lives past the end of this function
#endif
	
	return omnisString;
}

// Get a std::string from an EXTfldval object
std::string OmnisTools::getStringFromEXTFldVal(EXTfldval& fVal) {
	std::string retString;
	
	// Get a qchar* string
	qlong maxLength = fVal.getBinLen()+1; // Use binary length as approximation of maximum size
	qlong length = 0, stringLength = 0;
	qchar* omnisString = new qchar[maxLength];
	fVal.getChar(maxLength, omnisString, length);
	
	// Translate qchar* string into UTF8 binary
	qbyte* utf8data = reinterpret_cast<qbyte*>(omnisString);
	stringLength = CHRunicode::charToUtf8(omnisString, length, utf8data);
	
	// Translate UTF8 binary into char* string
	char* cString = reinterpret_cast<char*> (utf8data);
	
	// Create standard string
	retString = std::string(cString,stringLength);
	
	// Clean-up
	delete [] omnisString;
	
	return retString;
}

// Set an existing EXTfldval object from a std::string
void OmnisTools::getEXTFldValFromString(EXTfldval& fVal, const std::string readString) {
	qlong length;
	qchar* omnisString = getQCharFromString(readString, length);
	
	fVal.setChar(omnisString, length); // Set value of character field, but exclude the last character since it will be the null terminator from the C String
	
	// Clean-up
	delete [] omnisString;
}

// Set an existing EXTfldval object from a std::wstring
void OmnisTools::getEXTFldValFromChar(EXTfldval& fVal, const char* readChar) {
    std::string readString;
    if (readChar)
        readString = readChar;
    else
        readString = "";
    
    getEXTFldValFromString(fVal, readString);
}

// Get a dynamically allocated qchar* array from a std::string
qchar* OmnisTools::getQCharFromString(const std::string readString, qlong &retLength) {
	qlong length = readString.size();
	
	// Cast-away constness of c_str() pointer 
	char* cString = const_cast<char*>(readString.c_str());
	
	// Feed into raw byte data
	qbyte* utf8data = reinterpret_cast<qbyte*> (cString);
	
	// Allocate new qchar* string
	qchar* omnisString = new qchar[length];
	
	// Convert to Omnis Character field
	retLength = CHRunicode::utf8ToChar(utf8data, length, omnisString);  // Convert characters into Omnis Char Field
	
	return omnisString;
}

// Get a str15 object for a character constant (No string if it doesn't correspond to the conditions)
str15 OmnisTools::initStr15(const char* in){
    str15 theString;
    qshort length = strlen(in);
    if (length > 0 && length <= 15) {
        theString.setUtf8((qbyte*) in, strlen(in));
    }
    
    return theString;
} 

// Get a str31 object for a character constant (No string if it doesn't correspond to the conditions)
str31 OmnisTools::initStr31(const char* in){
    str31 theString;
    qshort length = strlen(in);
    if (length > 0 && length <= 31) {
        theString.setUtf8((qbyte*) in, strlen(in));
    }
    
    return theString;
} 

// Get a str80 object for a character constant (No string if it doesn't correspond to the conditions)
str80 OmnisTools::initStr80(const char* in){
    str80 theString;
    qshort length = strlen(in);
    if (length > 0 && length <= 80) {
        theString.setUtf8((qbyte*) in, strlen(in));
    }
    
    return theString;
} 

// Get a str255 object for a character constant (No string if it doesn't correspond to the conditions)
str255 OmnisTools::initStr255(const char* in) {
    str255 theString;
    qshort length = strlen(in);
    if (length > 0 && length <= 255) {
        theString.setUtf8((qbyte*) in, strlen(in));
    }
    
    return theString;
} 

// Return a C++ bool from an EXTfldval
bool OmnisTools::getBoolFromEXTFldVal(EXTfldval& fVal) {
	qshort omnBool;
	omnBool = fVal.getBool();
	switch (omnBool) {
		case 2:
			return true;
		case 1:
			return false;
		default:
			return false;
	}
}

// Get an EXTfldval for a C++ bool
void OmnisTools::getEXTFldValFromBool(EXTfldval& fVal, bool b) {
	qshort omBool;
	if (b==true) 
        omBool = 2;
	else if (b==false)
        omBool = 1;
    
	fVal.setBool(omBool);
}

// Return a C++ int from an EXTfldval
int OmnisTools::getIntFromEXTFldVal(EXTfldval& fVal) {
	qlong omnInt = fVal.getLong();
	
	if (omnInt < INT_MIN || omnInt > INT_MAX) {
        omnInt = 0; // zero out any numbers that exceed
	}
	
	return static_cast<int>( omnInt );
}

// Get an EXTfldval for a C++ int
void OmnisTools::getEXTFldValFromInt(EXTfldval& fVal, int i) {
	fVal.setLong(static_cast<qlong>(i));
}

// Return a C++ long from an EXTfldval
long OmnisTools::getLongFromEXTFldVal(EXTfldval& fVal) {
	long retLong = static_cast<long>(fVal.getLong());
	return retLong;
}

// Get an EXTfldval for a C++ long
void OmnisTools::getEXTFldValFromLong(EXTfldval& fVal, long l) {
	fVal.setLong(static_cast<qlong>(l));
}

// Return a C++ long from an EXTfldval
float OmnisTools::getFloatFromEXTFldVal(EXTfldval& fVal) {
	qreal omnReal; qshort dp;
	fVal.getNum(omnReal, dp);
	
	float retFloat = static_cast<float>(omnReal);
	
	return retFloat;
}

// Get an EXTfldval for a C++ double
void OmnisTools::getEXTFldValFromFloat(EXTfldval& fVal, float d) {
	qreal omnReal = static_cast<qreal>(d);
	qshort dp = dpFmask;
	fVal.setNum(omnReal, dp);
}

// Return a C++ long from an EXTfldval
double OmnisTools::getDoubleFromEXTFldVal(EXTfldval& fVal) {
	qreal omnReal; qshort dp;
	fVal.getNum(omnReal, dp);
	
	double retDbl = static_cast<double>(omnReal);
	
	return retDbl;
}

// Get an EXTfldval for a C++ double
void OmnisTools::getEXTFldValFromDouble(EXTfldval& fVal, double d) {
	qreal omnReal = static_cast<qreal>(d);
	qshort dp = dpFmask;
	fVal.setNum(omnReal, dp);
}

// Get an EXTfldval for a C time
void OmnisTools::getEXTFldValFromTime(EXTfldval& fVal, struct tm* cTime) {
    
    datestamptype convDate;
    
    // Date
    convDate.mYear = static_cast<qshort>(cTime->tm_year+1900);  // Years since 1900
    convDate.mMonth = static_cast<char>(cTime->tm_mon+1); // 0 = January, 11 = December
    convDate.mDay = static_cast<char>(cTime ->tm_mday);
    convDate.mDateOk = static_cast<char>(qtrue);
    
    // Time
    convDate.mHour = static_cast<char>(cTime->tm_hour);
    convDate.mMin = static_cast<char>(cTime->tm_min);
    convDate.mSec = static_cast<char>(cTime->tm_sec);
    convDate.mTimeOk = static_cast<char>(qtrue);
    
    convDate.mHunOk = static_cast<char>(qfalse);
    
    fVal.setDate(convDate, dpFdtimeC);
}

// Get an EXTfldval for a defined constant
void OmnisTools::getEXTFldValFromConstant(EXTfldval& fVal, qlong constID, qlong prefixID) {
    
	// Check for prefix
	str80 prefixRead;
	str255 constantValue;
	if (prefixID > 0) {
		// Read string from resource, and assign it to return parameter
		RESloadString(gInstLib,prefixID,prefixRead);
		constantValue.concat(prefixRead);
	}
    
	// Read complete resource string
	str255 resourceValue;
	RESloadString(gInstLib,constID,resourceValue);
	
	// Translate into std::wstring for easy substring
	EXTfldval convVar;
	convVar.setChar(resourceValue, dpDefault);
	std::wstring resourceString = getWStringFromEXTFldVal(convVar);
	
	// Get substring between tilde (sometimes used for categories) and first colon.
	int tildePos = resourceString.find(L"~") + 1;
	int colonPos = resourceString.find(L":");
	
	std::wstring constantString = resourceString.substr(tildePos,colonPos-tildePos);
	getEXTFldValFromWString(convVar, constantString);
	
	// Add constant to EXTfldval
	constantValue.concat(convVar.getChar());
	fVal.setConstant(constantValue);
}

// Get an integer for an EXTfldval where the EXTfldval contains a constant
#ifdef USE_BOOST
static std::map<std::string,int> constCache;
int OmnisTools::getIntFromEXTFldVal(EXTfldval& fVal, qlong firstID, qlong lastID) {
	
	if (getType(fVal).valType == fftInteger) {
		return getIntFromEXTFldVal(fVal);
	} else if (getType(fVal).valType == fftConstant) {
		return getIntFromEXTFldVal(fVal);
	}
	
	int retNum = -1;
	// Get string that needs to be matched
	std::string matchString = getStringFromEXTFldVal(fVal);
	
	// Get map iterator for searching
	std::map<std::string,int>::iterator it;
	it = constCache.find(matchString);
	if (it != constCache.end()) {
		retNum = it->second;
	} else {
		// Unable to find match, must loop constants in range and look for it
		int tildePos, colonPos, numPos, constNum;
		EXTfldval convVar;
		str255 resourceValue;
		std::string resourceMatch, resourceString;
		std::string numString = "";
		
		for( int i = firstID; i <= lastID; ++i) {
			// Load resource and put into std::wstring for easy substr
			RESloadString(gInstLib,i,resourceValue);
			convVar.setChar(resourceValue, dpDefault);
			resourceString = getStringFromEXTFldVal(convVar);
			tildePos = resourceString.find("~") + 1;
			colonPos = resourceString.find(":");
			if (colonPos != -1) { // All constants should have colons.  If it doesn't then don't interpret the line
				resourceMatch = resourceString.substr(tildePos, colonPos-tildePos);
				
				// While looping add items to the const cache
				numPos = colonPos + 1;
				numString.clear();
				while (resourceString[numPos] != ':' && numPos < static_cast<int>(resourceString.length())) {
					numString += resourceString[numPos++];
				}
				try {
					constNum = lexical_cast<int>(numString);
				}
				catch(bad_lexical_cast &) {
					constNum = -1;
				}
				
				constCache[resourceMatch] = constNum;  // Add constant to cache
			}
		}
		
		// Locate constant now that all the constants have been cached
		it = constCache.find(matchString);
		if (it != constCache.end()) {
			retNum = it->second;
		}
	}
	return retNum;
}
#endif // USE_BOOST

// Get an ISO 8601 Formatted Date String from EXTFldVal
std::string OmnisTools::getISO8601DateStringFromEXTFldVal(EXTfldval& fVal) {
	datestamptype theDate;
	std::string retString;
	std::stringstream sin;
	
	fVal.getDate(theDate, dpFdtimeC);
	FieldValType theType = getType(fVal);
    
	if (theType.valType != fftDate)
		return "";
    
	// Set date part of string
    if (theDate.mDateOk == qtrue
        && !(theType.valSubType == dpFdtime1900
             || theType.valSubType == dpFdtime1980
             || theType.valSubType == dpFdtime2000
             || theType.valSubType == dpFdtimeC
             || theType.valSubType == dpFtime))
    {	
        sin << int(theDate.mYear);
        if (theDate.mMonth < 10) {
            sin << "-0" << int(theDate.mMonth);
        } else {
            sin << "-" << int(theDate.mMonth);
        } 
        if (theDate.mDay < 10) {
            sin << "-0" << int(theDate.mDay);
        } else {
            sin << "-" << int(theDate.mDay);
        } 
	}
	
	// Set time part of string
    if (theDate.mTimeOk == qtrue
        && !(theType.valSubType == dpFdate1900
             || theType.valSubType == dpFdate1900
             || theType.valSubType == dpFdate1980
             || theType.valSubType == dpFdate2000
             || theType.valSubType == dpFdtime1900
             || theType.valSubType == dpFdtime1980
             || theType.valSubType == dpFdtime2000
             || theType.valSubType == dpFdtimeC))
    {
        sin << "T";
        if (theDate.mHour < 10) {
            sin << "0" << int(theDate.mHour);
        } else {
            sin << int(theDate.mHour);
        }
        if (theDate.mMin < 10) {
            sin << ":0" << int(theDate.mMin);
        } else {
            sin << ":" << int(theDate.mMin);
        }
        if (theDate.mSecOk) {
            if (theDate.mSec < 10) {
                sin << ":0" << int(theDate.mSec);
            } else {
                sin << ":" << int(theDate.mSec);
            }
        }
        // NOTE: Even though Omnis contains hundredths data, it is not part of
        //       the ISO8601 format
    }
    
    // Place string into return value
    retString = sin.str();
	
	return retString;
}

#ifdef USE_BOOST
// RegExs used for date-checking
static boost::regex dateCheck = boost::regex("^(\\d+)-(\\d{1,2})-(\\d{1,2})$");  // Regex: ^\d+-\d{1,2}-\d{1,2}$ -- Looks like 2011-09-02 or 2011-9-2
static boost::regex timeCheck = boost::regex("^T(\\d{1,2}):(\\d{1,2}):?(\\d{0,2})\\.?(\\d*)Z?$");  // Regex: ^T\d{1,2}:\d{1,2}:\d{1,2}\.?\d*Z?$ -- Looks like T20:09:02.34398Z 
static boost::regex datetimeCheck = boost::regex("^(\\d+)-(\\d{1,2})-(\\d{1,2})T(\\d{1,2}):(\\d{1,2}):?(\\d{0,2})\\.?(\\d*)Z?$");  // Combination of date and time check.

// This method determines if a string is an ISO8601 Date
OmnisTools::ISO8601DateType OmnisTools::isISO8601Date(std::string dateString) {

    if( boost::regex_match(dateString,datetimeCheck) ) { 
        return kISODateTime;
        
    } else if ( boost::regex_match(dateString,dateCheck) ) {
        return kISODate;
        
    } else if ( boost::regex_match(dateString,timeCheck) ) {
        return kISOTime;
    }
    
    return kISODateNone;
}
#endif

#ifdef USE_BOOST
// This method sets up the passed in EXTfldval with a date representing the ISO8601 Date passed in
void OmnisTools::getEXTfldvalFromISO8601DateString(EXTfldval& fVal, std::string dateString) {

    boost::smatch theMatch;
    datestamptype theDate;
    
    if( boost::regex_match(dateString,theMatch,datetimeCheck) ) { 
        // Match date/time
        fVal.setEmpty(fftDate, dpFdtimeC);
        fVal.getDate(theDate);
        
        theDate.mDateOk = qtrue;
        theDate.mYear = lexical_cast<qshort>(theMatch[1]);  // First match is year
        theDate.mMonth = (char) lexical_cast<qshort>(theMatch[2]);  // Second match is month
        theDate.mDay = (char) lexical_cast<qshort>(theMatch[3]);  // Third match is day
        
        theDate.mTimeOk = qtrue;
        theDate.mHour = (char) lexical_cast<qshort>(theMatch[4]);  // Fourth match is hour
        theDate.mMin = (char) lexical_cast<qshort>(theMatch[5]);  // Fifth match is minute
        
        theDate.mSecOk = qfalse;
        theDate.mHunOk = qfalse;
        
        // Optional matches
        if (theMatch[6].matched && theMatch[6].length() > 0) {
            // Appears to have the seconds portion
            theDate.mSecOk = qtrue;
            theDate.mSec = lexical_cast<int>(theMatch[6]);  // Sixth match is seconds
        }
        
        fVal.setDate(theDate, dpFdtimeC);
        
    } else if ( boost::regex_match(dateString,theMatch,dateCheck) ) {
        // Match date
        fVal.setEmpty(fftDate, dpFdate2000);
        fVal.getDate(theDate);
        
        theDate.mDateOk = qtrue;
        theDate.mYear = lexical_cast<qshort>(theMatch[1]);  // First match is year
        theDate.mMonth = (char) lexical_cast<qshort>(theMatch[2]);  // Second match is month
        theDate.mDay = (char) lexical_cast<qshort>(theMatch[3]);  // Third match is day
        
        theDate.mTimeOk = qfalse;
        theDate.mSecOk = qfalse;
        theDate.mHunOk = qfalse;
        
        fVal.setDate(theDate, dpFdate2000);
        
    } else if ( boost::regex_match(dateString,theMatch,timeCheck) ) {
        // Match time
        fVal.setEmpty(fftDate, dpFtime);
        fVal.getDate(theDate);
        
        theDate.mDateOk = qfalse;
        theDate.mTimeOk = qtrue;
        theDate.mHour = (char) lexical_cast<qshort>(theMatch[1]);  // First match is hour
        theDate.mMin = (char) lexical_cast<qshort>(theMatch[2]);  // Second match is minute
        
        theDate.mSecOk = qfalse;
        theDate.mHunOk = qfalse;
        
        // Optional matches
        if (theMatch[3].matched && theMatch[3].length() > 0) {
            // Appears to have the seconds portion
            theDate.mSecOk = qtrue;
            theDate.mSec = (char) lexical_cast<qshort>(theMatch[3]);  // Third match is seconds
        }
        
        fVal.setDate(theDate);
    }
}
#endif

qbool OmnisTools::ensurePosixPath(EXTfldval& pathVal) {
#ifdef ismac
	str255 posixCheck, posixPath;
	qshort def = dpFcharacter;
	posixCheck = pathVal.getChar().cString();
	qlong err;
	std::wstring path = getWStringFromEXTFldVal(pathVal);
	
	if (path[0] != L'/') {
		err = ECOconvertHFSToPosix(posixCheck, posixPath);
		if (err != 0) {
			return qfalse;
		}
		pathVal.setChar(posixPath, def);
	}
#endif
	
	return qtrue;
}




