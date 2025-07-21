#include "Game.h"

#include <pandaFramework.h>

#include <ambientLight.h>
#include <directionalLight.h>

#include "PhysicsComponents/FixedShapePhysicsComponent.h"
#include "RenderComponents/MeshRenderComponent.h"
#include "PhysicsComponents/PlayerPhysicsComponent.h"
#include "RenderComponents/AnimatedRenderComponent.h"
#include "OtherComponents/PlayerInputComponent.h"

#include "Animation/SingleAnimNode.h"
#include "Animation/DrivenPoseAnimNode.h"
#include "Animation/BlendAnimNode.h"
#include "Animation/StateMachineAnimNode.h"

#include "Data Loading/DataLoader.h"

Game::Game(PandaFramework& pandaFramework, WindowFramework& windowFramework)
{
	physicsManager = new GamePhysicsManager();
	entityManager = new EntityManager();

	PT(AsyncTaskManager) task_mgr = AsyncTaskManager::get_global_ptr();
	task_mgr->add(physicsManager);
	task_mgr->add(entityManager);

	DataLoader dataLoader;

	//TEMP CODE BELOW:

	//physicsManager->create_debug(windowFramework.get_render());

	//Make some test platforms.
	EntityDef* platformDef = dataLoader.load_entity("Platform.txt", true);
	auto testPlatform1 = std::make_shared<Entity>(platformDef, physicsManager, pandaFramework, windowFramework);
	entityManager->add_entity(testPlatform1);
	auto testPlatform2 = std::make_shared<Entity>(platformDef, physicsManager, pandaFramework, windowFramework);
	entityManager->add_entity(testPlatform2);
	testPlatform2->set_pos(5, 0, 1);
	auto testPlatform3 = std::make_shared<Entity>(platformDef, physicsManager, pandaFramework, windowFramework);
	entityManager->add_entity(testPlatform3);
	testPlatform3->set_pos(-4, 5, 0);

	//Add the player character.
	try {
		EntityDef* playerDef = dataLoader.load_entity("Player.txt", false);
		auto player = std::make_shared<Entity>(playerDef, physicsManager, pandaFramework, windowFramework);
		entityManager->add_entity(player);
		player->set_pos(0, 0, 2);
	}
	catch (std::exception e)
	{
		std::cout << e.what() << '\n';
	}
	catch (...)
	{
		std::cout << "Unknown Error!";
	}

	/*auto pcPhysics = new PlayerPhysicsComponent("Player", windowFramework.get_render(), physicsManager, 0.5f, 0.5f * 1.7f, 0.1f, 6.0f, 10.0f, 8.0f);
	auto pcRender = new AnimatedRenderComponent(pcPhysics->get_node_path(), windowFramework, "Assets/Player/LilPiggy.bam", LVector3f(0, 0, -0.5f * 1.7f));
	auto testPC = std::make_shared<Entity>(pcPhysics, pcRender);
	auto pcInput = new PlayerInputComponent(pandaFramework, windowFramework);
	pcInput->attach(testPC.get());
	entityManager->add_entity(testPC);
	testPC->set_pos(0, 0, 2);

	auto animGraph = new AnimGraph();
	auto idleAnim = new SingleAnimNode(pcRender->get_anim_index("Idle"), true, animGraph);
	auto runAnim = new SingleAnimNode(pcRender->get_anim_index("Run"), true, animGraph);
	auto groundBlend = new BlendAnimNode(idleAnim, runAnim, AnimWeightName::MOVE_SPEED, animGraph);
	
	auto jumpAnim = new DrivenPoseAnimNode(pcRender->get_anim_index("Jump"), AnimWeightName::GROUND_DIST, animGraph);
	auto fallAnim = new DrivenPoseAnimNode(pcRender->get_anim_index("Fall"), AnimWeightName::GROUND_DIST, animGraph);
	auto airBlend = new BlendAnimNode(jumpAnim, fallAnim, AnimWeightName::VERT_SPEED, animGraph);

	auto stateMachine = new StateMachineAnimNode(groundBlend, animGraph);
	stateMachine->add_state(airBlend);
	stateMachine->add_transition(0, 1, AnimWeightName::GROUND_DIST, Comparator::GREATER_THAN, 0.05f, 0.1f);
	stateMachine->add_transition(1, 0, AnimWeightName::GROUND_DIST, Comparator::LESS_THAN, 0.03f, 0.1f);
	animGraph->add_layer(stateMachine);
	
	pcRender->set_anim_graph(animGraph);
	pcRender->add_weight_binding(Message::MessageType::MOVE_SPEED, AnimWeightName::MOVE_SPEED, InterpolationFunctions::Type::LINEAR);
	pcRender->add_weight_binding(Message::MessageType::GROUND_DIST, AnimWeightName::GROUND_DIST, InterpolationFunctions::Type::LINEAR);
	pcRender->add_weight_binding(Message::MessageType::VERT_SPEED, AnimWeightName::VERT_SPEED, InterpolationFunctions::Type::CUBIC, -1, 1);
	*/

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
};