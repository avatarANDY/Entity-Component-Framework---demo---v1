
#ifndef __AV_ECF_ENTITY_H
#define __AV_ECF_ENTITY_H

#include "AV_Component.h"

//==========================================================================================================================================================
struct DataComponent
{
	UINT       ID;
	Component *pComponent;
};

#define ECF_EntityList     TDoublyList< Entity >
#define ECF_ComponentList  TDoublyList< DataComponent >
#define ECF_Node           TDNode

#define ENTITY_LIST_NODE_MAX     32
#define COMPONENT_LIST_NODE_MAX  32

class ActorModel;

//==========================================================================================================================================================
class DLL_EXPORT_DEFINE Entity
{
public :
	//------------------------------------------------------------
	UINT        Type;
	UINT        ID;
	ActorModel *pActorModelLink;
	Entity     *pRootEntityNodeLink;
	bool        bEnabled;
	//------------------------------
	ECF_EntityList    EntityList;
	ECF_ComponentList ComponentList;
	//------------------------------------------------------------
	Entity*      AddEntity( ActorModel *actor_model, UINT entity_type, UINT entity_id, Entity *root_entity_node = NULL );
	bool         DelEntity( UINT entity_id );
	Entity*      GetEntity( UINT entity_id );
	//------------------------------------------------------------
	template < class CLASS_TYPE >
	CLASS_TYPE*  AddNewComponent( UINT component_id )
	{
		CLASS_TYPE *component = NULL;
		MY_NEW( component, CLASS_TYPE );
		AddComponent( component, component_id );
		return component;
	}
	bool         AddComponent( Component *component, UINT component_id );
	bool         DelComponent( UINT component_id );
	Component*   GetComponent( UINT component_id );
	//------------------------------------------------------------
	bool         SendMessage( Entity *entity, void *source, UINT componend_id, UINT message_type, void *data );
	bool         SendMessage( void *source, UINT componend_id, UINT message_type, void *data );
	//------------------------------------------------------------
	void         Constructor( ActorModel *actor_model, UINT entity_type, UINT entity_id, Entity *parent_entity_node );
	void         Destructor();
	//------------------------------
	void         Create();
	void         Release();
	//------------------------------
	void         Update();
	//------------------------------------------------------------
};

#endif

// eof
