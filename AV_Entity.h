
#ifndef __AV_ECF_ENTITY_H
#define __AV_ECF_ENTITY_H

#include "AV_Component.h"

//==========================================================================================================================================================
struct DataComponent
{
	UINT       ID;
	Component *pComponent;
};

#define ECF_EntityList     AV_MemoryPoolDoublyLinkedListLock< Entity >
#define ECF_ComponentList  AV_MemoryPoolDoublyLinkedListLock< DataComponent >
#define ECF_Node           MPNode2

#define ENTITY_LIST_NODE_MAX     32
#define COMPONENT_LIST_NODE_MAX  32

//==========================================================================================================================================================
class DLL_EXPORT_DEFINE Entity
{
public :
	//------------------------------------------------------------
	UINT     Type;
	UINT     ID;
	Entity  *pRootEntityNodeLink;
	bool     bEnabled;
	//------------------------------
	ECF_EntityList    EntityList;
	ECF_ComponentList ComponentList;
	//------------------------------------------------------------
	Entity*      AddEntity( UINT entity_type, UINT entity_id, Entity *root_entity_node = NULL );
	bool         DelEntity( UINT entity_id );
	Entity*      FindEntity( UINT entity_id );
	//------------------------------------------------------------
	template < class CLASS_TYPE >
	CLASS_TYPE*  NewComponent()
	{
		CLASS_TYPE *component = NULL;
		MY_NEW( component, CLASS_TYPE );
		return component;
	}
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
	Component*   FindComponent( UINT component_id );
	//------------------------------------------------------------
	bool         SendMessage( Entity *entity, Component *source, UINT componend_id, UINT message_type, void *data );
	bool         SendMessage( Component *source, UINT componend_id, UINT message_type, void *data );
	//------------------------------------------------------------
	void         Constructor( UINT entity_type, UINT entity_id, Entity *parent_entity_node );
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
