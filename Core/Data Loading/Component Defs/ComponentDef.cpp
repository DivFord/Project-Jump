#include "ComponentDef.h"

std::ostream& operator<<(std::ostream& os, const ComponentDef* def)
{
	return def->output(os);
}