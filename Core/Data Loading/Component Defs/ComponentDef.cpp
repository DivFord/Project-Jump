#include "ComponentDef.h"

float ComponentDef::get_float(FloatID id) const
{
	std::cout << "The get_float method is not implemented on " << get_type_str() << "!";
	return 0;
};

int ComponentDef::shape_count() const
{
	return 0;
};

ShapeDef ComponentDef::get_shape(int index) const
{
	std::cout << "The get_shape method is not implemented on " << get_type_str() << "!";
	return ShapeDef();
};

std::string ComponentDef::get_file_name() const
{
	std::cout << "The get_file_path method is not implemented on " << get_type_str() << "!";
	return "";
};

LVector3f ComponentDef::get_vector3(Vector3ID id) const
{
	std::cout << "The get_vector3 method is not implemented on " << get_type_str() << "!";
	return LVector3f(0);
}

AnimDef* ComponentDef::get_anim_def() const
{
	std::cout << "The get_anim_def method is not implemented on " << get_type_str() << "!";
	return nullptr;
}

int ComponentDef::weight_binding_count() const
{
	return 0;
};

WeightBindingDef ComponentDef::get_weight_binding(int index) const
{
	std::cout << "The get_weight_binding method is not implemented on " << get_type_str() << "!";
	return WeightBindingDef();
};

std::ostream& operator<<(std::ostream& os, const ComponentDef* def)
{
	return def->output(os);
}