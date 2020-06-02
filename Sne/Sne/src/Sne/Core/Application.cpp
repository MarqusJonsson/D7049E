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
#include "../ECS//Components/HealthComponent.h"
#include "../ECS/Systems/InputSystem.h"
#include "../ECS/Systems/CombatSystem.h"
#include "../ECS/Components/PositionComponent.h"
#include "../ECS/Components/CubeComponent.h"
#include "../ECS/ManagerManager.h"

// Event System
#include "../EventSystem/EventBus.h"
#include "../EventSystem/Events/MouseClickEvent.h"
#include "../EventSystem/Events/KeyClickEvent.h"
#include "../EventSystem/Events/DamageEvent.h"
#include "../EventSystem/MemberFunctionHandler.h"


static bool s_showStats = false;
// This should be moved into Application.h
GLFWwindow* window;
// Set view 0 to the same dimensions as the window and to clear the color buffer.
const bgfx::ViewId kClearView = 0;

Physics::Simulator physicsSimulator;
eastl::list<Entity> renderables;

//Managers
eastl::shared_ptr<ManagerManager> managerManager = eastl::make_shared<ManagerManager>();

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
    if (key == GLFW_KEY_F1 && action == GLFW_RELEASE)
        s_showStats = !s_showStats;
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
    init.resolution.reset = BGFX_RESET_VSYNC;
    init.type = bgfx::RendererType::Direct3D11;
    if (!bgfx::init(init))
        return;
    bgfx::setViewClear(kClearView, BGFX_CLEAR_COLOR);
    bgfx::setViewRect(kClearView, 0, 0, bgfx::BackbufferRatio::Equal);
}

void Sne::Application::initExample()
{


    //Events
    EventBus* eventBus = new EventBus();
    glfwSetWindowUserPointer(window, eventBus);

    //Entities
    Entity player = managerManager->CreateEntity();
    Entity cube1 = managerManager->CreateEntity();
    Entity cube2 = managerManager->CreateEntity();

    //Components
    CubeComponent cube1Component = CubeComponent();
    cube1Component.Init(-14.0, 6, 0, 1, 2, 0);
    CubeComponent cube2Component = CubeComponent();
    cube2Component.Init(-14.0, -4, 0, 1, 2, 0);
    managerManager->RegisterComponent<CubeComponent>();

    HealthComponent playerHealthComponent = HealthComponent();
    playerHealthComponent.Init(100, 100);
    managerManager->RegisterComponent<HealthComponent>();
    PositionComponent playerPositionComponent = PositionComponent();
    managerManager->RegisterComponent<PositionComponent>();
    playerPositionComponent.Init(1.0f, 1.0f, 1.0f);

    managerManager->AddComponent(player, playerHealthComponent);
    managerManager->AddComponent(player, playerPositionComponent);
    managerManager->AddComponent(cube1, cube1Component);
    managerManager->AddComponent(cube2, cube2Component);
    renderables.push_back(cube1);
    renderables.push_back(cube2);
    //Systems
    Signature combatSystemSignature;
    combatSystemSignature.set(managerManager->GetComponentType<HealthComponent>());
    eastl::shared_ptr mouseSystem = managerManager->RegisterSystem<InputSystem>();
    eastl::shared_ptr combatSystem = managerManager->RegisterSystem<CombatSystem>();
    combatSystem->setManagerManager(managerManager);
    managerManager->SetSystemSignature<CombatSystem>(combatSystemSignature);
    mouseSystem->EventSubscribe(eventBus);
    combatSystem->EventSubscribe(eventBus);

    printf("%i player hp before \n", managerManager->GetComponent<HealthComponent>(player).health);
    eventBus->publish(new DamageEvent(player));
    printf("%i player hp after \n", managerManager->GetComponent<HealthComponent>(player).health);

    // Physics
    physicsSimulator = Physics::Simulator();
    // 0 mass gives an static object, mass > 0 gives dynamic
    physicsSimulator.createCuboid(SneMath::vec3(1.0f), SneMath::vec3(0.0f), 0.0f);
    physicsSimulator.createCuboid(SneMath::vec3(1.0f), SneMath::vec3(0.0f, 5.0f, 0.0f), 1.0f);
    physicsSimulator.createCylinder(SneMath::vec3(1.0f), SneMath::vec3(0.0f, 10.0f, 0.0f), 1.0f);
    physicsSimulator.createCone(1.0f, 1.0f, SneMath::vec3(0.0f, 15.0f, 0.0f), 1.0f);
    physicsSimulator.createCapsule(1.0f, 1.0f, SneMath::vec3(0.0f, 20.0f, 0.0f), 1.0f);
    physicsSimulator.createShpere(1.0f, SneMath::vec3(0.0f, 25.0f, 0.0f), 0.0f);
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
            bgfx::reset((uint32_t)width, (uint32_t)height, BGFX_RESET_VSYNC);
            bgfx::setViewRect(kClearView, 0, 0, bgfx::BackbufferRatio::Equal);
        }

        for (Entity e : renderables) {
            managerManager->GetComponent<CubeComponent>(e).render();
        }
        // DeltaTime
        currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // This dummy draw call is here to make sure that view 0 is cleared if no other draw calls are submitted to view 0.
        bgfx::touch(kClearView);

        // Physics, should probably not be done every frame
        physicsSimulator.update(deltaTime);

        // Use debug font to print information about this example.
       /* bgfx::dbgTextClear();
        bgfx::dbgTextImage(bx::max<uint16_t>(uint16_t(width / 2 / 8), 20) - 20, bx::max<uint16_t>(uint16_t(height / 2 / 16), 6) - 6, 40, 12, s_logo, 160);
        bgfx::dbgTextPrintf(0, 0, 0x0f, "Press F1 to toggle stats.");
        
        // Temp pos & rot debug values
        for (int i = 0; i < physicsSimulator.positions.size(); i++) {
            SneMath::vec3 pos = physicsSimulator.positions[i];
            SneMath::vec3 ypr = physicsSimulator.rotations[i];
            bgfx::dbgTextPrintf(0, i + 3, 0x0f, "\x1b[Index: %i Pos: (\x1b[9;mx%f\x1b[, \x1b[10;y%f\x1b[, \x1b[12;z%f \x1b[), Rot: (\x1b[9;mx%f\x1b[, \x1b[10;y%f\x1b[, \x1b[12;z%f\x1b[)",
                i, pos.x, pos.y, pos.z, ypr.x, ypr.y, ypr.z);
        }

        bgfx::dbgTextPrintf(0, 1, 0x0f, "Color can be changed with ANSI \x1b[9;me\x1b[10;ms\x1b[11;mc\x1b[12;ma\x1b[13;mp\x1b[14;me\x1b[0m code too.");
        bgfx::dbgTextPrintf(80, 1, 0x0f, "\x1b[;0m    \x1b[;1m    \x1b[; 2m    \x1b[; 3m    \x1b[; 4m    \x1b[; 5m    \x1b[; 6m    \x1b[; 7m    \x1b[0m");
        bgfx::dbgTextPrintf(80, 2, 0x0f, "\x1b[;8m    \x1b[;9m    \x1b[;10m    \x1b[;11m    \x1b[;12m    \x1b[;13m    \x1b[;14m    \x1b[;15m    \x1b[0m");
        const bgfx::Stats* stats = bgfx::getStats();
        bgfx::dbgTextPrintf(0, 2, 0x0f, "Backbuffer %dW x %dH in pixels, debug text %dW x %dH in characters.", stats->width, stats->height, stats->textWidth, stats->textHeight);
        */
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
