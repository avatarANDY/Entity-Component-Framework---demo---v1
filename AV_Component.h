
#ifndef __AV_ECF_COMPONENT_H
#define __AV_ECF_COMPONENT_H

#include "AV_MemoryPoolStructureLock.hpp"

#define DLL_EXPORT_DEFINE __declspec( dllexport )

class Entity;

//==========================================================================================================================================================
class DLL_EXPORT_DEFINE Component
{
public :
	//------------------------------------------------------------
	UINT     Type;
	UINT     ID;
	Entity  *pParentEntityNodeLink;
	bool     bEnabled;
	//------------------------------------------------------------
	Component();
	virtual ~Component(){}
	//------------------------------------------------------------
	virtual void vOnCreate(){}
	virtual void vOnRelease(){}
	//------------------------------
	virtual void OnUpdate(){}
	//------------------------------
	virtual void vOnMessage( Component *source, UINT message_type, void *data ){}
	//------------------------------------------------------------
};

#endif

// eof
