#include "Game.h"

#include <pandaFramework.h>

#include <ambientLight.h>
#include <directionalLight.h>

#include "PhysicsComponents/FixedShapePhysicsComponent.h"
#include "RenderComponents/MeshRenderComponent.h"

Game::Game(PandaFramework& pandaFramework, WindowFramework& windowFramework)
{
	physicsManager = new GamePhysicsManager();
	entityManager = new EntityManager();

	PT(AsyncTaskManager) task_mgr = AsyncTaskManager::get_global_ptr();
	task_mgr->add(physicsManager);
	task_mgr->add(entityManager);

	//TEMP CODE BELOW:

	physicsManager->create_debug(windowFramework.get_render());

	//Make a test platform.
	LVector3f pos(0, 0, -1);
	auto physics = new FixedShapePhysicsComponent("Platform", windowFramework.get_render(), physicsManager, ShapeDef(ShapeDef::Type::CYLINDER, 2, 2, 0, pos));
	auto render = new MeshRenderComponent(physics->get_node_path(), windowFramework, "Assets/Terrain/Platform_Circle.bam");
	auto testPlatform = std::make_shared<Entity>(physics, render);
	entityManager->add_entity(testPlatform);

	//Position the camera.
	NodePath camera = windowFramework.get_camera_group();
	camera.set_pos(0, -20, 20);
	camera.set_p(-45);

	//Set up lighting.
	PT(AmbientLight) ambientLight = new AmbientLight("ambient light");
	ambientLight->set_color(LColor(0.2, 0.2, 0.2, 1));
	NodePath ambientLightPath = windowFramework.get_render().attach_new_node(ambientLight);
	windowFramework.get_render().set_light(ambientLightPath);

	PT(DirectionalLight) mainLight = new DirectionalLight("main light");
	NodePath mainLightPath = windowFramework.get_render().attach_new_node(mainLight);
	mainLightPath.set_hpr(45, -45, 0);
	windowFramework.get_render().set_light(mainLightPath);

	//Set shader generator.
	windowFramework.get_render().set_shader_auto();
}