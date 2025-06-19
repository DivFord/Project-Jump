#pragma once

#include "VitalComponent.h"

/// <summary>
/// Base class for components that add a visual representation to an Entity.
/// Every entity gets one of these.
/// If it creates a node, it should attach it to the parent node from the 
/// Physics Component.
/// </summary>
class RenderComponent : public VitalComponent
{
public:
	RenderComponent() : VitalComponent() {};
};