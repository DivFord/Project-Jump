#pragma once

#include "PlatformerControllerNode.h"

/// <summary>
/// New implementations of PandaNode need to be initialised with Panda's type registry.
/// This should be called at the start of the program.
/// </summary>
static void InitNewTypes()
{
	PlatformerControllerNode::init_type();
};