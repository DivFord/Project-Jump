#include "Game.h"

#include <pandaFramework.h>

#include <ambientLight.h>
#include <directionalLight.h>

#include "PhysicsComponents/FixedShapePhysicsComponent.h"
#include "RenderComponents/MeshRenderComponent.h"
#include "PhysicsComponents/PlayerPhysicsComponent.h"
#include "RenderComponents/AnimatedRenderComponent.h"
#include "OtherComponents/PlayerInputComponent.h"

Game::Game(PandaFramework& pandaFramework, WindowFramework& windowFramework)
{
	physicsManager = new GamePhysicsManager();
	entityManager = new EntityManager();

	PT(AsyncTaskManager) task_mgr = AsyncTaskManager::get_global_ptr();
	task_mgr->add(physicsManager);
	task_mgr->add(entityManager);

	//TEMP CODE BELOW:

	//physicsManager->create_debug(windowFramework.get_render());

	//Make a test platform.
	LVector3f pos(0, 0, -1);
	auto physics = new FixedShapePhysicsComponent("Platform", windowFramework.get_render(), physicsManager, ShapeDef(ShapeDef::Type::CYLINDER, 2, 2, 0, pos));
	auto render = new MeshRenderComponent(physics->get_node_path(), windowFramework, "Assets/Terrain/Platform_Circle.bam");
	auto testPlatform = std::make_shared<Entity>(physics, render);
	entityManager->add_entity(testPlatform);

	//Add the player character.
	PlayerPhysicsDef def;
	def.capsuleHeight = 0.5f * 1.7f;
	auto pcPhysics = new PlayerPhysicsComponent("Player", windowFramework.get_render(), physicsManager, def);
	auto pcRender = new AnimatedRenderComponent(pcPhysics->get_node_path(), windowFramework, "Assets/Player/LilPiggy.bam", LVector3f(0, 0, -0.5f * 1.7f));
	auto testPC = std::make_shared<Entity>(pcPhysics, pcRender);
	auto pcInput = new PlayerInputComponent(pandaFramework, windowFramework);
	pcInput->attach(testPC);
	entityManager->add_entity(testPC);
	testPC->set_pos(0, 0, 2);
	//pcRender->play_anim("Run", true);
	//pcRender->set_anim_speed("Run", 2.0);
	//pcRender->set_anim_relative_time("Jump", 0.5);
	pcRender->play_anim("Idle", true);
	pcRender->play_anim("Run", true);

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