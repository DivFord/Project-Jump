#pragma once

#include "../Tokeniser.h"
#include "../VariableLoader.h"
#include "../DataLoadingException.h"

struct ComponentDef
{
	enum class Type
	{
		UNSET,
		
		//Physics Components.
		FIXED_SHAPE_PHYS,
		PLAYER_PHYS,

		//Render Components.
		MESH_RENDER,

		//Other Components.
	};

	virtual Type get_type() { return Type::UNSET; }
};

