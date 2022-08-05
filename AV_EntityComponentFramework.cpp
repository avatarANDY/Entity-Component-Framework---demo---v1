
#include "AV_EntityComponentFramework.h"

//==========================================================================================================================================================
void EntityComponentFramework::Constructor()
{
	// root entity node Constructor
	Entity::Constructor( 1000000000, 1000000000, this );
}

void EntityComponentFramework::Create()
{
	Entity::Create();
}

void EntityComponentFramework::Release()
{
	Entity::Release();
}

void EntityComponentFramework::Update()
{
	if( bEnabled )
		Entity::Update();
}

// eof