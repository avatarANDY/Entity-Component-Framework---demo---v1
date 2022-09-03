
#ifndef __AV_ECF_ENTITY_COMPONENT_FRAMEWORK_H
#define __AV_ECF_ENTITY_COMPONENT_FRAMEWORK_H

#include "AV_Entity.h"

//==========================================================================================================================================================
class DLL_EXPORT_DEFINE EntityComponentFramework : public Entity
{
public :
	//------------------------------------------------------------
	void ECF_Constructor( ActorModel *actor_model );
	//------------------------------------------------------------
	void ECF_Create();
	void ECF_Release();
	void ECF_Update();
	//------------------------------------------------------------
};

#endif

// eof
