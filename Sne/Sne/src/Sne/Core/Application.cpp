#include "Application.h"
#include <stdio.h>

// BGFX
#include <bx/bx.h>
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include "logo.h"
#include <bx/allocator.h>
#include <bgfx/platform.h>
#include <bx/math.h>

// Sound
#include "soloud.h"
#include "soloud_speech.h"
#include "soloud_thread.h"

// Pyhsics
#include "../Physics/Simulator.h"

// Eastl
#include <EASTL/algorithm.h>
#include "EastlOverides.h"

// Entity Component System
#include "../ECS/BaseSystem.h"
#include "../ECS/Entity.h"
#include "../ECS/IComponentArray.h"
#include "../ECS/ComponentArray.h"
#include "../ECS/Components/HealthComponent.h"
#include "../ECS/Components/TransformComponent.h"
#include "../ECS/Components/CuboidComponent.h"
#include "../ECS/Components/RigidBodyComponent.h"
#include "../ECS/Systems/InputSystem.h"
#include "../ECS/Systems/CombatSystem.h"
#include "../ECS/Systems/RenderSystem.h"
#include "../ECS/Systems/PhysicsUpdateSystem.h"
#include "../ECS/ManagerManager.h"

// Event System
#include "../EventSystem/EventBus.h"
#include "../EventSystem/Events/MouseClickEvent.h"
#include "../EventSystem/Events/KeyClickEvent.h"
#include "../EventSystem/Events/DamageEvent.h"
#include "../EventSystem/Events/RenderEvent.h"
#include "../EventSystem/MemberFunctionHandler.h"


static bool s_showStats = false;
// This should be moved into Application.h
GLFWwindow* window;
// Set view 0 to the same dimensions as the window and to clear the color buffer.
const bgfx::ViewId kClearView = 0;

EventBus eventBus;
Physics::Simulator physicsSimulator;

//Managers
eastl::shared_ptr<ManagerManager> managerManager = eastl::make_shared<ManagerManager>();
Entity player;


static void glfw_errorCallback(int error, const char* description)
{
    fprintf(stderr, "GLFW error %d: %s\n", error, description);
}

static void glfw_keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    EventBus* eventBus = static_cast<EventBus*>(glfwGetWindowUserPointer(window));
    if (action == GLFW_PRESS)
    {
        eventBus->publish(new KeyClickEvent(key));
    }
    if (key == GLFW_KEY_F1 && action == GLFW_PRESS)
    {
        s_showStats = !s_showStats;
    }

    if (key == GLFW_KEY_Q && action == GLFW_PRESS)
    {

            eventBus->publish(&DamageEvent(player));
        
    }
}

static void glfw_mouseInputCallback(GLFWwindow* window, int button, int action, int mods)
{
    EventBus* eventBus = static_cast<EventBus*>(glfwGetWindowUserPointer(window));
    if (action == GLFW_PRESS)
    {
        eventBus->publish(new MouseClickEvent(button));
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
    initWindow();
    initBGFX();
    initExample();
    mainLoop();
    cleanup();
}

void Sne::Application::initWindow()
{
    // Create a GLFW window.
    glfwSetErrorCallback(glfw_errorCallback);
    if (!glfwInit())
        return;
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    //glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    window = glfwCreateWindow(width, height, "Sne", nullptr, nullptr);
    if (!window)
        return;
    glfwSetKeyCallback(window, glfw_keyCallback);
    glfwSetMouseButtonCallback(window, glfw_mouseInputCallback);
}

void Sne::Application::initBGFX()
{
    // Call bgfx::renderFrame before bgfx::init to signal to bgfx not to create a render thread.
    // Most graphics APIs must be used on the same thread that created the window.
    bgfx::renderFrame();
    // Initialize bgfx using the native window handle and window resolution.
    bgfx::Init init;
#if BX_PLATFORM_WINDOWS
    init.platformData.nwh = glfwGetWin32Window(window);
#endif
    glfwGetWindowSize(window, &width, &height);
    init.resolution.width = (uint32_t)width;
    init.resolution.height = (uint32_t)height;
    //init.resolution.reset = BGFX_RESET_VSYNC;
    init.type = bgfx::RendererType::Direct3D11;
    if (!bgfx::init(init))
        return;
    bgfx::setViewClear(kClearView, BGFX_CLEAR_COLOR);
    bgfx::setViewRect(kClearView, 0, 0, bgfx::BackbufferRatio::Equal);
}

void Sne::Application::initExample()
{
    // Setup camera view
    unsigned int counter = 0;
    const bx::Vec3 at = { 0.0f, 0.0f, 0.0f };
    const bx::Vec3 eye = { 0.0f, 0.0f, -30.0f };
    float view[16];
    bx::mtxLookAt(view, eye, at);
    float proj[16];
    bx::mtxProj(proj, 60.0f, float(width) / float(height), 0.1f, 10000.0f, bgfx::getCaps()->homogeneousDepth);
    bgfx::setViewTransform(0, view, proj);

    // Set view 0 default viewport.
    bgfx::setViewRect(0, 0, 0, uint16_t(width), uint16_t(height));

    //Events
    eventBus = EventBus();
    glfwSetWindowUserPointer(window, &eventBus);

    // Register components
    managerManager->RegisterComponent<HealthComponent>();
    managerManager->RegisterComponent<CuboidComponent>();
    managerManager->RegisterComponent<TransformComponent>();
    managerManager->RegisterComponent<RigidBodyComponent>();

    // Resigster systems
    eastl::shared_ptr mouseSystem = managerManager->RegisterSystem<InputSystem>();
    eastl::shared_ptr combatSystem = managerManager->RegisterSystem<CombatSystem>();
    eastl::shared_ptr renderSystem = managerManager->RegisterSystem<RenderSystem>();
    eastl::shared_ptr physicsUpdateSystem = managerManager->RegisterSystem<PhysicsUpdateSystem>();

    // Create and set system signatures
    // Render system
    Signature renderSystemSignature;
    renderSystemSignature.set(managerManager->GetComponentType<CuboidComponent>());
    renderSystemSignature.set(managerManager->GetComponentType<TransformComponent>());
    // Combat system
    Signature combatSystemSignature;
    combatSystemSignature.set(managerManager->GetComponentType<HealthComponent>());
    // Physics update system
    Signature physicsUpdateSystemSignature;
    physicsUpdateSystemSignature.set(managerManager->GetComponentType<RigidBodyComponent>());
    physicsUpdateSystemSignature.set(managerManager->GetComponentType<TransformComponent>());

    // Set system signatures
    managerManager->SetSystemSignature<CombatSystem>(combatSystemSignature);
    managerManager->SetSystemSignature<RenderSystem>(renderSystemSignature);
    managerManager->SetSystemSignature<PhysicsUpdateSystem>(physicsUpdateSystemSignature);

    // Provide a reference to the managerManger for each system
    combatSystem->setManagerManager(managerManager);
    renderSystem->setManagerManager(managerManager);
    physicsUpdateSystem->setManagerManager(managerManager);

    // Player
    player = managerManager->CreateEntity();
    // Player components
    HealthComponent playerHealthComponent = HealthComponent();
    playerHealthComponent.Init(100, 100);
    // Add player components
    managerManager->AddComponent(player, playerHealthComponent);
    
    // Physics simulator
    physicsSimulator = Physics::Simulator();

    // Floor
    Entity floor = managerManager->CreateEntity();

    // cuboids
    CuboidComponent cuboidComponent = CuboidComponent();
    cuboidComponent.Init();
    
    SneMath::vec3 floorPosition = SneMath::vec3(0.0f, -10.0f, 0.0f);
    SneMath::vec3 floorRotation = SneMath::vec3(0.0f, 45.0f, 0.0f);
    SneMath::vec3 floorScale = SneMath::vec3(10.0f, 0.1f, 5.0f);
    TransformComponent floorTransformComponent = TransformComponent();
    floorTransformComponent.Init(floorPosition, floorRotation, floorScale);

    btTransform floorCollisionTransform;
    floorCollisionTransform.setIdentity();
    floorCollisionTransform.setOrigin(SneMath::vec3_to_btVector3(floorPosition));
    btQuaternion floorQuaternion = btQuaternion(btScalar(floorRotation.y), btScalar(floorRotation.x), btScalar(floorRotation.z));
    floorCollisionTransform.setRotation(floorQuaternion);
    btCollisionShape* floorCollisionShape = physicsSimulator.createCuboidCollision(floorScale);
    btRigidBody* floorRigidBody = physicsSimulator.createRigidBody(floorCollisionShape, floorCollisionTransform, 0.0f);
    RigidBodyComponent floorRigidBodyComponent = RigidBodyComponent();
    floorRigidBodyComponent.Init(floorRigidBody);

    physicsSimulator.addToSimulation(floorRigidBody);

    // Add cuboid components
    managerManager->AddComponent(floor, cuboidComponent);
    managerManager->AddComponent(floor, floorTransformComponent);
    managerManager->AddComponent(floor, floorRigidBodyComponent);
    
    const int n_cuboids = 16;
    TransformComponent transformComponents[n_cuboids];
    // Cuboid collision shape
    SneMath::vec3 scale = SneMath::vec3(1.0f, 2.5f, 1.0f);
    btCollisionShape* collisionShape = physicsSimulator.createCuboidCollision(scale);
    for (int i = 0; i < n_cuboids; i++)
    {
        // Create entity
        Entity cuboid = managerManager->CreateEntity();

        // Transform
        SneMath::vec3 position = SneMath::vec3((i % 4) * 3.0f - 4.5f, i, SneMath::floor(i / 4.0f) * 3.0f - 4.5f);
        SneMath::vec3 rotation = SneMath::vec3(45.0f, 45.0f, 0.0f);
        transformComponents[i].Init(
            position,
            rotation,
            scale);

        // Rigid body
        btTransform collisionTransform;
        collisionTransform.setIdentity();
        collisionTransform.setOrigin(SneMath::vec3_to_btVector3(position));
        btQuaternion quaternion = btQuaternion(btScalar(rotation.y), btScalar(rotation.x), btScalar(rotation.z));
        collisionTransform.setRotation(quaternion);
        btRigidBody* rigidBody = physicsSimulator.createRigidBody(collisionShape, collisionTransform, 1.0f);
        RigidBodyComponent rigidBodyComponent = RigidBodyComponent();
        rigidBodyComponent.Init(rigidBody);

        physicsSimulator.addToSimulation(rigidBody);

        // Add cuboid components
        managerManager->AddComponent(cuboid, cuboidComponent);
        managerManager->AddComponent(cuboid, transformComponents[i]);
        managerManager->AddComponent(cuboid, rigidBodyComponent);
    }

    physicsSimulator.setGravity(SneMath::vec3(0.0f, -9.0f, 0.0f));
    
    /*instantiateCuboid(&cuboid1Component,
        SneMath::vec3(-10.0f, -1.0f, 0.0f),
        SneMath::vec3(0.0f, 45.0f, 0.0f),
        SneMath::vec3(2.0f, 1.0f, 1.0f));
    instantiateCuboid(&cuboid1Component,
        SneMath::vec3(10.0f, 0.0f, 0.0f),
        SneMath::vec3(0.0f, 0.0f, 0.0f),
        SneMath::vec3(1.0f, 1.0f, 1.0f));
    instantiateCuboid(&cuboid1Component,
        SneMath::vec3(-10.0f, 0.0f, 0.0f),
        SneMath::vec3(0.0f, 0.0f, 0.0f),
        SneMath::vec3(1.0f, 1.0f, 1.0f));*/

    // Provide a reference to the eventBus for each system
    mouseSystem->EventSubscribe(&eventBus);
    combatSystem->EventSubscribe(&eventBus);
    renderSystem->EventSubscribe(&eventBus);
    physicsUpdateSystem->EventSubscribe(&eventBus);
    
    // Test publish an event
    printf("%i player hp before \n", managerManager->GetComponent<HealthComponent>(player).health);
    eventBus.publish(&DamageEvent(player));


}

void Sne::Application::mainLoop()
{
    double currentFrame = glfwGetTime();;
    double lastFrame = currentFrame;
    double deltaTime;
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // Handle window resize.
        int oldWidth = width, oldHeight = height;
        glfwGetWindowSize(window, &width, &height);
        if (width != oldWidth || height != oldHeight) {
            bgfx::reset((uint32_t)width, (uint32_t)height, BGFX_RESET_NONE);
            bgfx::setViewRect(kClearView, 0, 0, bgfx::BackbufferRatio::Equal);
        }

        const bgfx::ViewId kClearView = 0;
        bgfx::setViewClear(kClearView, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x00000000);
        bgfx::setViewRect(kClearView, 0, 0, bgfx::BackbufferRatio::Equal);

        eventBus.publish(&RenderEvent());
        
        // DeltaTime
        currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Physics, should probably not be done every frame
        eventBus.publish(&PhysicsUpdateEvent(&physicsSimulator, deltaTime));

        // This dummy draw call is here to make sure that view 0 is cleared if no other draw calls are submitted to view 0.
        bgfx::touch(kClearView);

        // Use debug font to print information about this example.
        bgfx::dbgTextClear();
        //bgfx::dbgTextImage(bx::max<uint16_t>(uint16_t(width / 2 / 8), 20) - 20, bx::max<uint16_t>(uint16_t(height / 2 / 16), 6) - 6, 40, 12, s_logo, 160);
        bgfx::dbgTextPrintf(0, 0, 0x0f, "Press F1 to toggle stats.");
        const bgfx::Stats* stats = bgfx::getStats();
        // Enable stats or debug text.
        bgfx::setDebug(s_showStats ? BGFX_DEBUG_STATS : BGFX_DEBUG_TEXT);

        // Advance to next frame. Process submitted rendering primitives.
        bgfx::frame();
    }
}

void Sne::Application::cleanup()
{
    bgfx::shutdown();
    glfwTerminate();
}
