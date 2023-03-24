#pragma once

#include "pch.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// We might be able to use the MVC pattern here.
// Doom 3 is actually implementing some of the concepts from our chat with GPT.
// Instead of implementing the observer pattern,
//   the window is our only view and we implement a controller or controllers that maintain an interface
//   that the window recognizes. When an event happens, the view passes the data to the method on the controller.
// This does not preclude us from implementing an event system or the observer pattern in the rest of the program.

namespace core {
    using Offset = glm::vec<2, double, glm::defaultp>;
    using Position = glm::vec<2, double, glm::defaultp>;
    using Size = glm::vec<2, int, glm::defaultp>;

    struct KeyboardEvent { int scancode; std::int16_t key, mods; };
    struct TextEvent { unsigned int codepoint; };
    struct MouseMotionEvent { Position pos; };
    struct MouseButtonEvent { int button, mods; };
    struct ScrollEvent { Offset offset; };

    // I don't want to split up Press and Release events into separate functions.
    // We use this on both keys and mouse buttons. Just ignore Repeat for mouse buttons.
    enum class Action { // Lawsuit
        Press, Release, Repeat
    };


    class Controller {
    public:
        virtual bool handle_key_event(KeyboardEvent event, Action action);
        virtual bool handle_text_event(TextEvent event);
        virtual bool handle_mouse_motion_event(MouseMotionEvent event);
        virtual bool handle_mouse_button_event(MouseButtonEvent event, Action action);
        virtual bool handle_scroll_event(ScrollEvent event);
    };


    class Hints {
    public:
        Hints(int width, int height, int samples = 0);
        void vsync(bool value);
        void transparent(bool value);
        void decorated(bool value);

        [[nodiscard]] bool vsync() const;
        [[nodiscard]] bool transparent() const;
        [[nodiscard]] bool decorated() const;

        [[nodiscard]] int width() const;
        [[nodiscard]] int height() const;
        [[nodiscard]] int samples() const;

    private:
        Size m_window_size;
        uint8_t m_sample_count;
        std::bitset<3> m_flags {};
    };


    class glfw_exception : public std::exception {
    public:
        explicit glfw_exception(const char *message);
    };


    class glfw_initialization_failed : public glfw_exception {
    public:
        explicit glfw_initialization_failed(const char *message);
    };


    //class glfw_opengl_failed : public glfw_exception {
    //public:
    //    explicit glfw_opengl_failed(const char *message);
    //};


    class GLFWLibrary {
    public:
        static GLFWLibrary &get();
    private:
        static void glfw_init();
        GLFWLibrary();
    public:
        ~GLFWLibrary();
        [[nodiscard]] static bool initialized();
    };


    enum class WindowStatus {
        Success, GLFWUninitialized, WindowCreationFailed, OpenGLContextCreationFailed
    };


    // GLFW will make a window fullscreen on its own sometimes, we need to handle that.
    // We don't need multiple windows.
    // Window is inherently a view.
    class Window {
        void destroy();
        static Window *get_state(GLFWwindow *state);

    public:
        Window() = default;
        ~Window();

        WindowStatus create(const char *title, Controller *controller, const Hints &hints);
        [[nodiscard]] bool created() const;

        [[nodiscard]] bool should_close() const;
        void should_close(bool value);
        void swap_buffers();
        void update();
        void clear() const;

        Size size();

    private:
        GLFWwindow *m_window_ptr {nullptr};
        Controller *m_controller {nullptr};
    };
}
