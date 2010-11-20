/* NON-VISUAL OBJECT BASE (IMPLEMENTATION)
 * 
 * Base class for all Non-Visual objects based Omnis objects
 *
 * March 30, 2010 David McKeone (Created)
 */

#include "NVObjBase.he"

// Constructor for building from an existing object instance
NVObjBase::NVObjBase( qobjinst objinst) 
{
	mObjInst = objinst;	// we remember the objects instance
}

// Generic destruction
NVObjBase::~NVObjBase()
{
	// Insert any memory deletion code or general cleanup code
}

// Duplicate an object into a new object
NVObjBase* NVObjBase::dup( qlong propID, qobjinst objinst, tThreadData *pThreadData )
{
	NVObjBase* copy = (NVObjBase*)createObject(propID, objinst, pThreadData);  // Defined in jsoncpp
	return copy;
}

// Copy one objects contents into another objects contents.
void NVObjBase::copy( NVObjBase* pObj )
{
	qobjinst inst = mObjInst;
	*this = *pObj;
	
	mObjInst = inst;
}

// Methods Available and Method Call Handling (These should be overriden by a sub-class)
qlong NVObjBase::returnMethods( EXTCompInfo* pEci ) { return 1L; }
qlong NVObjBase::methodCall( tThreadData* pThreadData ) { return 1L; }

// Properties and Property Call Handling (These should be overriden by a sub-class)
qlong NVObjBase::returnProperties( EXTCompInfo* pEci ) { return qtrue; }
qlong NVObjBase::getProperty( EXTCompInfo* pEci ) { return qtrue; }
qlong NVObjBase::setProperty( EXTCompInfo* pEci ) { return qtrue; }
qlong NVObjBase::canAssignProperty( EXTCompInfo* pEci, qlong propID ) { return qfalse; }
