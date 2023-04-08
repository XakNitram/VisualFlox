#include "pch.hpp"
#include "Window.hpp"


namespace core {
    GLFWLibrary &GLFWLibrary::get() {
        static GLFWLibrary instance;
        return instance;
    }

    void GLFWLibrary::glfw_init() {
        glfwSetErrorCallback(
            [](int code, const char* message) {
                std::cerr << message << '\n';
            }
        );

        /* Initialize GLFW */
        if (!glfwInit()) {
            throw glfw_initialization_failed("Failed to initialize GLFW.");
        }
    }

    GLFWLibrary::GLFWLibrary() {
        glfw_init();
    }

    GLFWLibrary::~GLFWLibrary() {
        glfwTerminate();
    }

    bool GLFWLibrary::initialized() {
        return glfwInit();
    }


    void Window::destroy() {
        if (created()) {
            glfwSetWindowUserPointer(m_window_ptr, nullptr);
            glfwDestroyWindow(m_window_ptr);
        }

        glfwPollEvents();
        m_window_ptr = nullptr;
        m_controller = nullptr;
    }

    Window *Window::get_state(GLFWwindow *state) {
        return static_cast<Window*>(glfwGetWindowUserPointer(state));
    }

    Window::~Window() {
        if (!created()) {
            std::cerr << "Window wrapper was destroyed, but a window object was never created.\n";
            return;
        }

        destroy();
    }

    WindowStatus Window::create(const char *title, Controller *controller, const Hints &hints) {
        using enum WindowStatus;
        if (!GLFWLibrary::get().initialized()) {
            return GLFWUninitialized;
        }

        m_controller = controller;

        constexpr int glfw_enable[2] {GLFW_FALSE, GLFW_TRUE};

        // Set GLFW window hints.
        // ****** Hard Constraints ******
        // Must match available capabilities exactly.
        glfwWindowHint(GLFW_STEREO, GLFW_FALSE);  // Specifies whether to use OpenGL stereoscopic rendering.

        // Specifies whether the framebuffer should be double buffered. You nearly always want to use double buffering.
        // Investigate to see why this doesn't work when false.
        glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);

        // Specifies which context creation API to use to create the context.
        glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
        //glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
        //glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        // An extension loader library that assumes it knows which API was used to create the current context ...
        //  may fail if you change this hint. This can be resolved by having it load functions via glfwGetProcAddress.
        // Maybe have to change for linux.
        glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_NATIVE_CONTEXT_API);
        //glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_OSMESA_CONTEXT_API);
        //glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_EGL_CONTEXT_API);

        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);  // Disable deprecated OpenGL operations.

        // Specifies which OpenGL profile to create the context for.
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
        //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);

        // ****** Soft Constraints ******
        // **** OpenGL ****
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

        // Specifies the robustness strategy to be used by the context.
        //glfwWindowHint(GLFW_CONTEXT_ROBUSTNESS, GLFW_NO_RESET_NOTIFICATION);
        //glfwWindowHint(GLFW_CONTEXT_ROBUSTNESS, GLFW_LOSE_CONTEXT_ON_RESET);
        //glfwWindowHint(GLFW_CONTEXT_ROBUSTNESS, GLFW_NO_ROBUSTNESS);

        // Specifies the release behavior to be used by the context.
        // https://registry.khronos.org/OpenGL/extensions/KHR/KHR_context_flush_control.txt
        glfwWindowHint(GLFW_CONTEXT_RELEASE_BEHAVIOR, GLFW_ANY_RELEASE_BEHAVIOR);
        //glfwWindowHint(GLFW_CONTEXT_RELEASE_BEHAVIOR, GLFW_RELEASE_BEHAVIOR_FLUSH);
        //glfwWindowHint(GLFW_CONTEXT_RELEASE_BEHAVIOR, GLFW_RELEASE_BEHAVIOR_NONE);

        glfwWindowHint(GLFW_CONTEXT_NO_ERROR, GLFW_FALSE);
#ifdef FLOX_SHOW_DEBUG_INFO
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif // FLOX_SHOW_DEBUG_INFO

        // **** GLFW ****
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);
        glfwWindowHint(GLFW_DECORATED, glfw_enable[hints.decorated()]);
        glfwWindowHint(GLFW_FOCUSED, GLFW_TRUE);
        glfwWindowHint(GLFW_AUTO_ICONIFY, GLFW_TRUE);
        glfwWindowHint(GLFW_FLOATING, GLFW_FALSE);
        glfwWindowHint(GLFW_MAXIMIZED, GLFW_FALSE);
        glfwWindowHint(GLFW_CENTER_CURSOR, GLFW_TRUE);
        glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, glfw_enable[hints.transparent()]);
        glfwWindowHint(GLFW_FOCUS_ON_SHOW, GLFW_TRUE);
        glfwWindowHint(GLFW_SCALE_TO_MONITOR, true);
        glfwWindowHint(GLFW_SAMPLES, hints.samples());
        glfwWindowHint(GLFW_SRGB_CAPABLE, true);
        glfwWindowHint(GLFW_REFRESH_RATE, 60);

        /* Create a GLFW window and its OpenGL context. */
        m_window_ptr = glfwCreateWindow(hints.width(), hints.height(), title, nullptr, nullptr);
        glfwMakeContextCurrent(m_window_ptr);
        if (m_window_ptr == nullptr) {
            destroy();
            std::cerr << "Failed to create GLFW window.\n";
            return WindowCreationFailed;
        }


        if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
            destroy();
            std::cerr << "Failed to initialize Glad.\n";
            return OpenGLContextCreationFailed;
        }

        if (hints.samples() > 0) {
            glEnable(GL_MULTISAMPLE);
        }

        glfwSwapInterval(hints.vsync());
        glfwSetWindowUserPointer(m_window_ptr, this);
        glfwSetKeyCallback(
            m_window_ptr, [](GLFWwindow *window, int key, int scancode, int action, int mods) {
                Action key_action;
                switch (action) {
                    case GLFW_PRESS:
                        key_action = Action::Press;
                        break;
                    case GLFW_RELEASE:
                        key_action = Action::Release;
                        break;
                    case GLFW_REPEAT:
                        key_action = Action::Repeat;
                        break;
                    default:
                        return;
                }

                Window::get_state(window)->m_controller->handle_key_event(
                    {scancode, (std::int16_t) key, (std::int16_t) mods}, key_action
                );
            }
        );

        glfwSetCursorPosCallback(m_window_ptr, [](GLFWwindow *window, double xpos, double ypos){
            Window::get_state(window)->m_controller->handle_mouse_motion_event(
                {Position{xpos, ypos}}
            );
        });

        glfwSetMouseButtonCallback(m_window_ptr, [](GLFWwindow *window, int button, int action, int mods){
            Action key_action;
            switch(action) {
                case GLFW_PRESS:
                    key_action = Action::Press;
                    break;
                case GLFW_RELEASE:
                    key_action = Action::Release;
                    break;
                default:
                    return;
            }
            Window::get_state(window)->m_controller->handle_mouse_button_event({button, mods}, key_action);
        });

        glfwSetCharCallback(m_window_ptr, [](GLFWwindow *window, unsigned int codepoint){
            Window::get_state(window)->m_controller->handle_text_event({codepoint});
        });

        glfwSetScrollCallback(m_window_ptr, [](GLFWwindow *window, double xoff, double yoff){
            Window::get_state(window)->m_controller->handle_scroll_event({Offset{xoff, yoff}});
        });

        // Output the current GLFW version.
        std::cout << "GLFW " << glfwGetVersionString() << std::endl;

        // Output the current OpenGL version.
        std::cout << "OpenGL " << glGetString(GL_VERSION) << std::endl;

        return Success;
    }

    bool Window::created() const {
        return m_window_ptr != nullptr;
    }

    bool Window::should_close() const {
        if (!created()) { return true; }
        return glfwWindowShouldClose(m_window_ptr);
    }

    void Window::should_close(bool value) {
        if (!created()) { return; }
        glfwSetWindowShouldClose(m_window_ptr, value);
    }

    void Window::swap_buffers() {
        if (!created()) { return; }
        glfwSwapBuffers(m_window_ptr);
    }

#pragma clang diagnostic push
#pragma ide diagnostic ignored "readability-convert-member-functions-to-static"
    void Window::update() {
        glfwPollEvents();
    }

    void Window::clear() const {
        if (!created()) { return; }
        // Requires OpenGL context.
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
#pragma clang diagnostic pop

    Size Window::size() {
        if (!created()) { return {0, 0}; }
        int width, height;
        glfwGetWindowSize(m_window_ptr, &width, &height);
        return {width, height};
    }
}
