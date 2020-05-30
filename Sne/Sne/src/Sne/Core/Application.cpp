/*
 * Copyright 2011-2019 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx#license-bsd-2-clause
 */


/*
#if defined(_MSC_VER)
 // We have little choice but to disable this warning. See the FAQ for why.
#pragma warning(disable: 4244) // conversion from '___' to '___', possible loss of data
#endif
*/

#include "Application.h"
#include <stdio.h>
#include <bx/bx.h>
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <bx/allocator.h>
#include <bgfx/platform.h>
#include <bx/math.h>

#include <GLFW/glfw3.h>
#if BX_PLATFORM_WINDOWS
#define GLFW_EXPOSE_NATIVE_WIN32
#endif
#include <GLFW/glfw3native.h>
#include "logo.h"

#include "soloud.h"
#include "soloud_speech.h"
#include "soloud_thread.h"
#include "btBulletDynamicsCommon.h"
#include <EASTL/algorithm.h>
#include "EastlOverides.h"

// Entity Component System
#include "../ECS/BaseSystem.h"
#include "../ECS/ComponentManager.h"
#include "../ECS/Entity.h"
#include "../ECS/EntityManager.h"
#include "../ECS/IComponentArray.h"
#include "../ECS/SystemManager.h"
#include "../ECS//Components/HealthComponent.h"
#include "../ECS/HealthComponentManager.h"
#include "../ECS/SystemManager.h"
#include "../ECS/Systems/MouseSystem.h"
#include "../ECS/Systems/CombatSystem.h"

// Event System
#include "../EventSystem/EventBus.h"
#include "../EventSystem/Events/MouseClickEvent.h"
#include "../EventSystem/Events/DamageEvent.h"
#include "../EventSystem/MemberFunctionHandler.h"
#include <iostream>

//CUBE TEST
bgfx::VertexBufferHandle m_vbh;
bgfx::IndexBufferHandle m_ibh;

struct PosColorVertex
{
    float x;
    float y;
    float z;
    uint32_t abgr;

	static void init()
	{
		ms_layout
			.begin()
			.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
			.add(bgfx::Attrib::Color0,   4, bgfx::AttribType::Uint8, true)
			.end();
	};

	static bgfx::VertexLayout ms_layout;
};

bgfx::VertexLayout PosColorVertex::ms_layout;

static PosColorVertex cubeVertices[] =
{
    {-1.0f,  1.0f,  1.0f, 0xff000000 },
    { 1.0f,  1.0f,  1.0f, 0xff0000ff },
    {-1.0f, -1.0f,  1.0f, 0xff00ff00 },
    { 1.0f, -1.0f,  1.0f, 0xff00ffff },
    {-1.0f,  1.0f, -1.0f, 0xffff0000 },
    { 1.0f,  1.0f, -1.0f, 0xffff00ff },
    {-1.0f, -1.0f, -1.0f, 0xffffff00 },
    { 1.0f, -1.0f, -1.0f, 0xffffffff },
};

static const uint16_t cubeTriList[] =
{
    0, 1, 2,
    1, 3, 2,
    4, 6, 5,
    5, 6, 7,
    0, 2, 4,
    4, 2, 6,
    1, 5, 3,
    5, 7, 3,
    0, 4, 1,
    4, 5, 1,
    2, 3, 6,
    6, 3, 7,
};

bgfx::ShaderHandle loadShader(const char *FILENAME)
{
   /* const char* shaderPath = "";

    switch(bgfx::getRendererType()) {
        case bgfx::RendererType::Noop:
        case bgfx::RendererType::Direct3D9:  shaderPath = "../Shader/dx9/";   break;
        case bgfx::RendererType::Direct3D11:
        case bgfx::RendererType::Direct3D12: shaderPath = "../Shader/dx11/";  break;
        case bgfx::RendererType::Gnm:        shaderPath = "../Shader/pssl/";  break;
        case bgfx::RendererType::Metal:      shaderPath = "../Shader/metal/"; break;
        case bgfx::RendererType::OpenGL:     shaderPath = "../Shader/glsl/";  break;
        case bgfx::RendererType::OpenGLES:   shaderPath = "../Shader/essl/";  break;
        case bgfx::RendererType::Vulkan:     shaderPath = "../Shader/spirv/"; break;
    }

    size_t shaderLen = strlen(shaderPath);
    size_t fileLen = strlen(FILENAME);
    char *filePath = (char *)malloc(shaderLen + fileLen + 1);
    memcpy(filePath, shaderPath, shaderLen);
    memcpy(&filePath[shaderLen], FILENAME, fileLen);*/

    FILE *file = fopen(FILENAME, "rb");
    fseek(file, 0, SEEK_END);
	    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    const bgfx::Memory *mem = bgfx::alloc(fileSize + 1);
    fread(mem->data, 1, fileSize, file);
    mem->data[mem->size - 1] = '\0';
    fclose(file);

    return bgfx::createShader(mem);
}

static bool s_showStats = false;

static void glfw_errorCallback(int error, const char* description)
{
	fprintf(stderr, "GLFW error %d: %s\n", error, description);
}

static void glfw_keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_F1 && action == GLFW_RELEASE)
		s_showStats = !s_showStats;
}

static void glfw_mouseInputCallback(GLFWwindow* window, int button, int action, int mods)
{
	if ((button == GLFW_MOUSE_BUTTON_1) && (action == GLFW_PRESS))
	{
		int once = 0;
		printf("Left mouse button \n");
		if (once == 0)
		{
			once = 1;
		}
	}
}



Sne::Application::Application()
{
}

Sne::Application::~Application()
{
}


void Sne::Application::Run()
{
	//Events
	EventBus* eventBus = new EventBus();

	EntityManager entityManager = EntityManager();
	ComponentManager componentManager =  ComponentManager();
	SystemManager systemManager = SystemManager();

	//Entities
	Entity player = entityManager.CreateEntity();
	//Components
	HealthComponent playerHealthComponent = HealthComponent();
	playerHealthComponent.Initialize(100, 100);
	componentManager.RegisterComponent<HealthComponent>();
	componentManager.AddComponent(player, playerHealthComponent);
	//Systems
	MouseSystem mouseSystem = MouseSystem();
	CombatSystem combatSystem = CombatSystem(componentManager);

	systemManager.RegisterSystem<MouseSystem>();
	//systemManager.RegisterSystem<CombatSystem>();

	mouseSystem.EventSubscribe(eventBus);
	combatSystem.EventSubscribe(eventBus);

	printf("%i player hp before \n", componentManager.GetComponent<HealthComponent>(player).health);
	eventBus->publish(new DamageEvent(player));


	/*int time = 0;
	game_initialize();
	for (int i = 0; i < 10000; i++) {
		
		game_update(time, 1);
		time++;
	}*/
	



	/*//printf("%f ===== \n",eastl::min(5.0f, 7.0f));
	///-----includes_end-----

	///-----initialization_start-----

	///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
	btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();

	///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
	btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);

	///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
	btBroadphaseInterface* overlappingPairCache = new btDbvtBroadphase();

	///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
	btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;

	btDiscreteDynamicsWorld* dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);

	dynamicsWorld->setGravity(btVector3(0, -10, 0));

	///-----initialization_end-----

	//keep track of the shapes, we release memory at exit.
	//make sure to re-use collision shapes among rigid bodies whenever possible!
	btAlignedObjectArray<btCollisionShape*> collisionShapes;

	///create a few basic rigid bodies

	//the ground is a cube of side 100 at position y = -56.
	//the sphere will hit it at y = -6, with center at -5
	{
		btCollisionShape* groundShape = new btBoxShape(btVector3(btScalar(50.), btScalar(50.), btScalar(50.)));

		collisionShapes.push_back(groundShape);

		btTransform groundTransform;
		groundTransform.setIdentity();
		groundTransform.setOrigin(btVector3(0, -56, 0));

		btScalar mass(0.);

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isDynamic = (mass != 0.f);

		btVector3 localInertia(0, 0, 0);
		if (isDynamic)
			groundShape->calculateLocalInertia(mass, localInertia);

		//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, groundShape, localInertia);
		btRigidBody* body = new btRigidBody(rbInfo);

		//add the body to the dynamics world
		dynamicsWorld->addRigidBody(body);
	}

	{
		//create a dynamic rigidbody

		//btCollisionShape* colShape = new btBoxShape(btVector3(1,1,1));
		btCollisionShape* colShape = new btSphereShape(btScalar(1.));
		collisionShapes.push_back(colShape);

		/// Create Dynamic Objects
		btTransform startTransform;
		startTransform.setIdentity();

		btScalar mass(1.f);

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isDynamic = (mass != 0.f);

		btVector3 localInertia(0, 0, 0);
		if (isDynamic)
			colShape->calculateLocalInertia(mass, localInertia);

		startTransform.setOrigin(btVector3(2, 10, 0));

		//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);
		btRigidBody* body = new btRigidBody(rbInfo);

		dynamicsWorld->addRigidBody(body);
	}

	/// Do some simulation

	///-----stepsimulation_start-----
	for (int i = 0; i < 150; i++)
	{
		dynamicsWorld->stepSimulation(1.f / 60.f, 10);

		//print positions of all objects
		for (int j = dynamicsWorld->getNumCollisionObjects() - 1; j >= 0; j--)
		{
			btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[j];
			btRigidBody* body = btRigidBody::upcast(obj);
			btTransform trans;
			if (body && body->getMotionState())
			{
				body->getMotionState()->getWorldTransform(trans);
			}
			else	
			{
				trans = obj->getWorldTransform();
			}
			printf("world pos object %d = %f,%f,%f\n", j, float(trans.getOrigin().getX()), float(trans.getOrigin().getY()), float(trans.getOrigin().getZ()));
		}
	}

	///-----stepsimulation_end-----

	//cleanup in the reverse order of creation/initialization

	///-----cleanup_start-----

	//remove the rigidbodies from the dynamics world and delete them
	for (int i = dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
	{
		btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		if (body && body->getMotionState())
		{
			delete body->getMotionState();
		}
		dynamicsWorld->removeCollisionObject(obj);
		delete obj;
	}

	//delete collision shapes
	for (int j = 0; j < collisionShapes.size(); j++)
	{
		btCollisionShape* shape = collisionShapes[j];
		collisionShapes[j] = 0;
		delete shape;
	}

	//delete dynamics world
	delete dynamicsWorld;

	//delete solver
	delete solver;

	//delete broadphase
	delete overlappingPairCache;

	//delete dispatcher
	delete dispatcher;

	delete collisionConfiguration;

	//next line is optional: it will be cleared by the destructor when the array goes out of scope
	collisionShapes.clear();

	
	// SOLOUD TEST
	// Define a couple of variables
	SoLoud::Soloud soloud;  // SoLoud engine core
	SoLoud::Speech speech;  // A sound source (speech, in this case)

	// Configure sound source
	speech.setText("7 7 7   7 7 1   Hello world. Welcome to Sne.");

	// initialize SoLoud.
	soloud.init();

	// Play the sound source (we could do this several times if we wanted)
	soloud.play(speech);

	// Wait until sounds have finished
	while (soloud.getActiveVoiceCount() > 0)
	{
		// Still going, sleep for a bit
		SoLoud::Thread::sleep(100);
	}

	// Clean up SoLoud
	soloud.deinit();
	// END OF SOLOUD TEST
	*/
	// GLFW & BGFX TEST
	// Create a GLFW window without an OpenGL context.

	glfwSetErrorCallback(glfw_errorCallback);
	if (!glfwInit())
		return;
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	GLFWwindow* window = glfwCreateWindow(1024, 768, "helloworld", nullptr, nullptr);
	if (!window)
		return;
	eventBus->publish(new MouseClickEvent(player, window));
	printf("%i player hp after \n", componentManager.GetComponent<HealthComponent>(player).health);

	glfwSetKeyCallback(window, glfw_keyCallback);
	glfwSetMouseButtonCallback(window, glfw_mouseInputCallback);
	// Call bgfx::renderFrame before bgfx::init to signal to bgfx not to create a render thread.
	// Most graphics APIs must be used on the same thread that created the window.
	bgfx::renderFrame();
	// Initialize bgfx using the native window handle and window resolution.
	bgfx::Init init;
#if BX_PLATFORM_WINDOWS
	init.platformData.nwh = glfwGetWin32Window(window);
#endif
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	init.resolution.width = (uint32_t)width;
	init.resolution.height = (uint32_t)height;
	init.resolution.reset = BGFX_RESET_VSYNC;
	if (!bgfx::init(init))
		return;
	// Set view 0 to the same dimensions as the window and to clear the color buffer.
	const bgfx::ViewId kClearView = 0;
	bgfx::setViewClear(kClearView, BGFX_CLEAR_COLOR|BGFX_CLEAR_DEPTH, 0xFFFFFFFF);
	bgfx::setViewRect(kClearView, 0, 0, bgfx::BackbufferRatio::Equal);


	bgfx::ShaderHandle vsh = loadShader("C:/Users/sandr/Documents/vie/Sne/extlibs/bgfx/examples/runtime/shaders/dx11/vs_cubes.bin");
	bgfx::ShaderHandle fsh = loadShader("C:/Users/sandr/Documents/vie/Sne/extlibs/bgfx/examples/runtime/shaders/dx11/fs_cubes.bin");
	bgfx::ProgramHandle m_program = bgfx::createProgram(vsh, fsh, true);
	PosColorVertex::init();
	// Create static vertex buffer.
		m_vbh = bgfx::createVertexBuffer(
			// Static data can be passed with bgfx::makeRef
			  bgfx::makeRef(cubeVertices, sizeof(cubeVertices) )
			, PosColorVertex::ms_layout
			);

		// Create static index buffer for triangle list rendering.
		m_ibh = bgfx::createIndexBuffer(
			// Static data can be passed with bgfx::makeRef
			bgfx::makeRef(cubeTriList, sizeof(cubeTriList) )
			);

		bgfx::IndexBufferHandle ibh = m_ibh;

    unsigned int counter = 0;

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		// Handle window resize.
		int oldWidth = width, oldHeight = height;
		glfwGetWindowSize(window, &width, &height);

			const bx::Vec3 at = { 0.0f, 0.0f,  0.0f };
			const bx::Vec3 eye = { 0.0f, 0.0f, -5.0f };
			float view[16];
			bx::mtxLookAt(view, eye, at);
			float proj[16];
			bx::mtxProj(proj, 60.0f, float(1024) / float(720), 0.1f, 100.0f, bgfx::getCaps()->homogeneousDepth);
			bgfx::setViewTransform(0, view, proj);

			float mtx[16];
			bx::mtxRotateXY(mtx, counter * 0.01f, counter * 0.01f);
			bgfx::setTransform(mtx);

			// Set vertex and index buffer.
			bgfx::setVertexBuffer(0, m_vbh);
			bgfx::setIndexBuffer(ibh);

			// Submit primitive for rendering to view 0.
			bgfx::submit(0, m_program);
		/*
		// This dummy draw call is here to make sure that view 0 is cleared if no other draw calls are submitted to view 0.
		bgfx::touch(kClearView);
		// Use debug font to print information about this example.
		bgfx::dbgTextClear();
		bgfx::dbgTextImage(bx::max<uint16_t>(uint16_t(width / 2 / 8), 20) - 20, bx::max<uint16_t>(uint16_t(height / 2 / 16), 6) - 6, 40, 12, s_logo, 160);
		bgfx::dbgTextPrintf(0, 0, 0x0f, "Press F1 to toggle stats.");
		bgfx::dbgTextPrintf(0, 1, 0x0f, "Color can be changed with ANSI \x1b[9;me\x1b[10;ms\x1b[11;mc\x1b[12;ma\x1b[13;mp\x1b[14;me\x1b[0m code too.");
		bgfx::dbgTextPrintf(80, 1, 0x0f, "\x1b[;0m    \x1b[;1m    \x1b[; 2m    \x1b[; 3m    \x1b[; 4m    \x1b[; 5m    \x1b[; 6m    \x1b[; 7m    \x1b[0m");
		bgfx::dbgTextPrintf(80, 2, 0x0f, "\x1b[;8m    \x1b[;9m    \x1b[;10m    \x1b[;11m    \x1b[;12m    \x1b[;13m    \x1b[;14m    \x1b[;15m    \x1b[0m");
		const bgfx::Stats* stats = bgfx::getStats();
		bgfx::dbgTextPrintf(0, 2, 0x0f, "Backbuffer %dW x %dH in pixels, debug text %dW x %dH in characters.", stats->width, stats->height, stats->textWidth, stats->textHeight);*/
		// Enable stats or debug text.
		bgfx::setDebug(s_showStats ? BGFX_DEBUG_STATS : BGFX_DEBUG_TEXT);
		
		// Advance to next frame. Process submitted rendering primitives.
		bgfx::frame();

	}
	bgfx::shutdown();
	glfwTerminate();
	//END OF GLFW & BGFX TEST
	

	return;
	
}


