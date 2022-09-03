
#include "AV_Entity.h"
#include "AV_EntityComponentFramework.h"

//==========================================================================================================================================================
void Entity::Constructor( ActorModel *actor_model, UINT entity_type, UINT entity_id, Entity *parent_entity_node )
{
	Type = entity_type;
	ID = entity_id;
	pActorModelLink = actor_model;
	pRootEntityNodeLink = parent_entity_node;
	bEnabled = true;

	EntityList.Create();// ENTITY_LIST_NODE_MAX );
	ComponentList.Create();// COMPONENT_LIST_NODE_MAX );
}

void Entity::Destructor()
{
	EntityList.Release();
	ComponentList.Release();
}

//==========================================================================================================================================================
Entity* Entity::AddEntity( ActorModel *actor_model, UINT entity_type, UINT entity_id, Entity *root_entity_node )
{
	ECF_Node< Entity > *entity_node = (ECF_Node< Entity >*)EntityList.GetNode();
	if( entity_node )
	{
		if( root_entity_node == NULL )
			entity_node->Constructor( actor_model, entity_type, entity_id, pRootEntityNodeLink );
		else
			entity_node->Constructor( actor_model, entity_type, entity_id, root_entity_node );
		if( EntityList.Link( entity_node ) )
			return entity_node;
	}

	return NULL;
}

bool Entity::DelEntity( UINT entity_id )
{
	ECF_Node< Entity > *entity_node = EntityList.pHeadNode;

	while( entity_node )
	{
		if( entity_node->ID == entity_id )
		{
			if( EntityList.Unlink( entity_node ) )
			{
				entity_node->Destructor();
				EntityList.ReleaseNode( entity_node );
				return true;
			}
			return false;
		}
		entity_node = entity_node->pNext;
	}

	return false;
}

//==========================================================================================================================================================
static Entity* gRecursiveFindEntityWithID( ECF_EntityList *entity_list, UINT entity_id )
{
	// 遞迴遍歷以entity_list為起點開始的所有entity節點：搜尋指定entity_id

	ECF_Node< Entity > *entity_node = entity_list->pHeadNode;
	Entity *return_entity = NULL;

	while( entity_node )
	{
		if( entity_node->ID == entity_id )
			return entity_node;

		if( entity_node->EntityList.pHeadNode )
		{
			return_entity = gRecursiveFindEntityWithID( &entity_node->EntityList, entity_id );
			if( return_entity )
				return return_entity;
		}

		entity_node = entity_node->pNext;
	}

	return NULL;
}

Entity* Entity::GetEntity( UINT entity_id )
{
	if( entity_id == ID )
		return this;
	return gRecursiveFindEntityWithID( &EntityList, entity_id );
}

//==========================================================================================================================================================
bool Entity::AddComponent( Component *component, UINT component_id )
{
	if( component == NULL )
		return false;

	ECF_Node< DataComponent > *component_node = (ECF_Node< DataComponent >*)ComponentList.GetNode();
	if( component_node )
	{
		component_node->ID = component_id;
		component_node->pComponent = component;
		component->ID = component_id;
		component->pParentEntityNodeLink = this;
		if( ComponentList.Link( component_node ) )
			return true;
	}

	return false;
}

bool Entity::DelComponent( UINT component_id )
{
	ECF_Node< DataComponent > *component_node = ComponentList.pHeadNode;

	while( component_node )
	{
		if( component_node->ID == component_id )
		{
			if( ComponentList.Unlink( component_node ) )
			{
				component_node->pComponent->vOnRelease();
				ComponentList.ReleaseNode( component_node );
				return true;
			}
		}
		component_node = component_node->pNext;
	}

	return false;
}

//==========================================================================================================================================================
static Component* gFindComponent( ECF_ComponentList *component_list, UINT component_id )
{
	ECF_Node< DataComponent > *component_node = component_list->pHeadNode;

	while( component_node )
	{
		if( component_node->ID == component_id )
			return component_node->pComponent;
		component_node = component_node->pNext;
	}

	return NULL;
}

static Component* gRecursiveFindComponentWithID( ECF_EntityList *entity_list, UINT component_id )
{
	// 遞迴遍歷以entity_list為起點開始的所有component節點：搜尋指定component_id

	ECF_Node< Entity > *entity_node = entity_list->pHeadNode;
	Component *component = NULL;
	Component *return_component = NULL;

	while( entity_node )
	{
		component = gFindComponent( &entity_node->ComponentList, component_id );
		if( component )
			return component;

		if( entity_node->EntityList.pHeadNode )
		{
			return_component = gRecursiveFindComponentWithID( &entity_node->EntityList, component_id );
			if( return_component )
				return return_component;
		}

		entity_node = entity_node->pNext;
	}

	return NULL;
}

Component* Entity::GetComponent( UINT component_id )
{
	Component *component = gFindComponent( &ComponentList, component_id );
	if( component )
		return component;
	return gRecursiveFindComponentWithID( &EntityList, component_id );
}

//==========================================================================================================================================================
bool Entity::SendMessage( Entity *entity, void *source, UINT componend_id, UINT message_type, void *data )
{
	Component *component = gFindComponent( &entity->ComponentList, componend_id );
	if( component )
	{
		// get it.
	}
	else
	{
		component = gRecursiveFindComponentWithID( &pRootEntityNodeLink->EntityList, componend_id );
	}

	if( component )
	{
		component->vOnMessage( source, message_type, data );
		return true;
	}
	return false;
}

bool Entity::SendMessage( void *source, UINT componend_id, UINT message_type, void *data )
{
	Component *component = gFindComponent( &pRootEntityNodeLink->ComponentList, componend_id );
	if( component )
	{
		// get it.
	}
	else
	{
		component = gRecursiveFindComponentWithID( &pRootEntityNodeLink->EntityList, componend_id );
	}

	if( component )
	{
		component->vOnMessage( source, message_type, data );
		return true;
	}
	return false;
}

//==========================================================================================================================================================
static void gComponentCreate( ECF_ComponentList *component_list )
{
	ECF_Node< DataComponent > *component_node = component_list->pHeadNode;
	Component *component = NULL;

	while( component_node )
	{
		component = component_node->pComponent;
		if( component->bEnabled )
			component->vOnCreate();
		component_node = component_node->pNext;
	}
}

static void gRecursiveCreate( ECF_EntityList *entity_list )
{
	// 遞迴遍歷所有節點：呼叫vOnCreate()事件函式

	ECF_Node< Entity > *entity_node = entity_list->pHeadNode;

	while( entity_node )
	{
		gComponentCreate( &entity_node->ComponentList );

		if( entity_node->EntityList.pHeadNode )
			gRecursiveCreate( &entity_node->EntityList );

		entity_node = entity_node->pNext;
	}
}

void Entity::Create()
{
	if( bEnabled )
		gComponentCreate( &ComponentList );
	gRecursiveCreate( &EntityList );
}

//==========================================================================================================================================================
static void gComponentRelease( ECF_ComponentList *component_list )
{
	ECF_Node< DataComponent > *component_node = component_list->pHeadNode, *del_component_node = NULL;

	while( component_node )
	{
		component_node->pComponent->vOnRelease();
		MY_DELETE( component_node->pComponent );

		del_component_node = component_node;
		component_node = component_node->pNext;
		if( component_list->Unlink( del_component_node ) )
			component_list->ReleaseNode( del_component_node );
	}
}

static void gRecursiveRelease( ECF_EntityList *entity_list )
{
	// 遞迴遍歷所有節點：呼叫Release事件函式

	ECF_Node< Entity > *entity_node = entity_list->pHeadNode, *del_entity_node = NULL;

	while( entity_node )
	{
		gComponentRelease( &entity_node->ComponentList );

		if( entity_node->EntityList.pHeadNode )
			gRecursiveRelease( &entity_node->EntityList );

		del_entity_node = entity_node;
		entity_node = entity_node->pNext;
		if( entity_list->Unlink( del_entity_node ) )
		{
			del_entity_node->Destructor();
			entity_list->ReleaseNode( del_entity_node );
		}
	}
}

void Entity::Release()
{
	Destructor();

	gComponentRelease( &ComponentList );
	gRecursiveRelease( &EntityList );
}

//==========================================================================================================================================================
static void gComponentUpdate( ECF_ComponentList *component_list )
{
	ECF_Node< DataComponent > *component_node = component_list->pHeadNode;
	Component *component = NULL;

	while( component_node )
	{
		component = component_node->pComponent;
		if( component->bEnabled )
			component->vOnUpdate();
		component_node = component_node->pNext;
	}
}

static void gRecursiveUpdate( ECF_EntityList *entity_list )
{
	// 遞迴遍歷所有節點：呼叫vOnUpdate()事件函式

	ECF_Node< Entity > *entity_node = entity_list->pHeadNode;

	while( entity_node )
	{
		gComponentUpdate( &entity_node->ComponentList );

		if( entity_node->EntityList.pHeadNode )
			gRecursiveUpdate( &entity_node->EntityList );

		entity_node = entity_node->pNext;
	}
}

void Entity::Update()
{
	if( bEnabled )
		gComponentUpdate( &ComponentList );
	gRecursiveUpdate( &EntityList );
}

//==========================================================================================================================================================

// eof