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
		ANIMATED_RENDER,

		//Other Components.
		PLAYER_INPUT,
	};

	virtual Type get_type() { return Type::UNSET; }

	friend std::ostream& operator<<(std::ostream& os, const ComponentDef* def);

protected:
	virtual std::ostream& output(std::ostream& os) const = 0;
};

