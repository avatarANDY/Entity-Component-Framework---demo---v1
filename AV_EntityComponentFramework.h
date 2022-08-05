
#ifndef __AV_ECF_ENTITY_COMPONENT_FRAMEWORK_H
#define __AV_ECF_ENTITY_COMPONENT_FRAMEWORK_H

#include "AV_Entity.h"

//==========================================================================================================================================================
class DLL_EXPORT_DEFINE EntityComponentFramework : public Entity
{
public :
	//------------------------------------------------------------
	void Constructor();
	//------------------------------------------------------------
	void Create();
	void Release();
	void Update();
	//------------------------------------------------------------
};

#endif

// eof
