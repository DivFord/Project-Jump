#include "MeshRenderComponent.h"

MeshRenderComponent::MeshRenderComponent(NodePath parentNode, WindowFramework& window, std::string modelFilepath, float scale) : RenderComponent()
{
	model = window.load_model(parentNode, modelFilepath);
	model.set_scale(scale);
};

MeshRenderComponent::MeshRenderComponent(NodePath parentNode, WindowFramework& window, ComponentDef* def)
{
	model = window.load_model(parentNode, "Assets/" + def->get_file_name() + ".bam");
};