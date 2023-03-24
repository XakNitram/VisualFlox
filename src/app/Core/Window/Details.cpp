#include "pch.hpp"
#include "Window.hpp"

// Extra code that doesn't directly deal with GLFW


namespace core {
    bool Controller::handle_key_event(KeyboardEvent event, Action action) { return false; }
    bool Controller::handle_text_event(TextEvent event) { return false; }
    bool Controller::handle_mouse_motion_event(MouseMotionEvent event) { return false; }
    bool Controller::handle_mouse_button_event(MouseButtonEvent event, Action action) { return false; }
    bool Controller::handle_scroll_event(ScrollEvent event) { return false; }


    Hints::Hints(int width, int height, int samples):
        m_window_size(width, height), m_sample_count(samples)
    {
        m_flags.set(0, true);
        m_flags.set(1, true);
        m_flags.set(2, false);
    }

    void Hints::vsync(bool value) { m_flags.set(0, value); }
    void Hints::decorated(bool value) { m_flags.set(1, value); }
    void Hints::transparent(bool value) { m_flags.set(2, value); }
    bool Hints::vsync() const { return m_flags.test(0); }
    bool Hints::decorated() const { return m_flags.test(1); }
    bool Hints::transparent() const { return m_flags.test(2); }
    int Hints::width() const { return m_window_size.x; }
    int Hints::height() const { return m_window_size.y; }
    int Hints::samples() const { return m_sample_count; }


    glfw_exception::glfw_exception(const char *message)
    // FIXME: Hack around exceptions
#ifdef _WIN32
        : std::exception(message) {}
#else
    : std::exception() {
        std::cerr << message << '\n';
    }
#endif

    glfw_initialization_failed::glfw_initialization_failed(const char *message) : glfw_exception(message) {}
    //glfw_opengl_failed::glfw_opengl_failed(const char *message) : glfw_exception(message) {}
}
