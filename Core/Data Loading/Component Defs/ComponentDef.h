#pragma once

#include "../Tokeniser.h"
#include "../VariableLoader.h"
#include "../DataLoadingException.h"
#include "../Other Defs/ShapeDef.h"
#include "../Other Defs/AnimDefs.h"

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

	enum class FloatID
	{
		SPEED,
		STEP_HEIGHT,
		ACCEL,
		DECCEL,
	};

	enum class Vector3ID
	{
		POS,
	};

	virtual Type get_type() const = 0;
	virtual std::string get_type_str() const = 0;

	virtual float get_float(FloatID id = FloatID::SPEED) const;

	virtual int shape_count() const;
	virtual ShapeDef get_shape(int index = 0) const;

	virtual std::string get_file_name() const;

	virtual LVector3f get_vector3(Vector3ID id = Vector3ID::POS) const;

	virtual AnimDef* get_anim_def() const;

protected:
	virtual std::ostream& output(std::ostream& os) const = 0;

public:
	friend std::ostream& operator<<(std::ostream& os, const ComponentDef* def);
};

