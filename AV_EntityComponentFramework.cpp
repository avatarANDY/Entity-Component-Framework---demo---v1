
#include "AV_EntityComponentFramework.h"

//==========================================================================================================================================================
void EntityComponentFramework::ECF_Constructor( ActorModel *actor_model )
{
	// root entity node Constructor
	Entity::Constructor( actor_model, 1000000000, 1000000000, this );
}

void EntityComponentFramework::ECF_Create()
{
	Entity::Create();
}

void EntityComponentFramework::ECF_Release()
{
	Entity::Release();
}

void EntityComponentFramework::ECF_Update()
{
	if( bEnabled )
		Entity::Update();
}

// eof