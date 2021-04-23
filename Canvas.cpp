#include "Canvas.h"
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

constexpr int WINDOW_WIDTH = 800;
constexpr int WINDOW_HEIGHT = 800;

static void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}


Canvas& Canvas::GetInstance() {
    static Canvas canvas(WINDOW_WIDTH, WINDOW_HEIGHT);
    return canvas;
}

Canvas::~Canvas() {
    _renderer.reset();
    _lightningTree.reset();
    glfwDestroyWindow(_window);
    glfwTerminate();
}

void Canvas::Run() {
    while (!glfwWindowShouldClose(_window)) {
        update();
        draw();

        glfwSwapBuffers(_window);
        glfwPollEvents();
    }
}

Canvas::Canvas(int width, int height) : _width(width), _height(height) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
    
    _window = glfwCreateWindow(width, height, "Lightning Generator", nullptr, nullptr);
    if (!_window) {
        fprintf(stderr, "Failed to create window\n");
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(_window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        fprintf(stderr, "Failed to load glad!\n");
        glfwTerminate();
        return;
    }
    glfwSetFramebufferSizeCallback(_window, framebuffer_size_callback);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

    init();
}

void Canvas::update() {
}

void Canvas::draw() {
    auto projection = glm::ortho(0.f, static_cast<float>(WINDOW_WIDTH), 0.f, static_cast<float>(WINDOW_HEIGHT));
    auto view = glm::identity<glm::mat4>();

    auto projView = projection * view;

    // Render to texture module
    _renderer->SwitchFrameBuffer();
    _renderer->ClearCurrentFrame();
    _renderer->Begin(projView);
    {
        _lightningTree->Draw(_renderer);
    }
    _renderer->End();


    // Render to screen
    _renderer->ResetFrameBuffer();
    _renderer->ClearCurrentFrame();
    _renderer->Begin(projView);
    {
        _renderer->DrawLightningToScreen();
    }
    _renderer->End();
}

void Canvas::init() {
    _renderer = std::make_shared<Renderer>(_width, _height);
    _renderer->CreateNewFrameBuffer(_width, _height);


    // A straight vertical path from top to bottom
    LightningNode top = { glm::vec2(400, 800), 0, 10 };
    LightningNode bot = { glm::vec2(400, 0), 0, 8 };
    std::vector<LightningNode> keyNodes{ top, bot };

    _lightningTree = std::make_shared<LightningTree>(keyNodes, glm::vec2(1.14, 5.14), 0.1f, 0.1f);
    
    for (int i = 0; i < 12; i++) {
        _lightningTree->RunOneStep(_renderer);
    }
}
