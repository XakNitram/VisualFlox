#include "pch.hpp"
#include "Core/Window/Window.hpp"


class Application : public core::Controller {
    core::Window *window;
public:
    explicit Application(core::Window *win_ptr) : window(win_ptr) {
        core::Hints window_hints {800, 450};
        core::WindowStatus result = window->create("VisualFlox", this, window_hints);
        if (result != core::WindowStatus::Success) {
            std::cerr << "Window creation failed." << std::endl;
            throw std::exception();
        }
    }

    bool handle_key_event(core::KeyboardEvent event, core::Action action) override {
        using enum core::Action;

        if (action != Release) { return false; }
        if (event.key == GLFW_KEY_ESCAPE) {
            window->should_close(true);
            return true;
        }

        return false;
    }

    int run() {
        if (!window->created()) {
            return 1;
        }

        while (!window->should_close()) {
            window->update();
            window->clear();
            window->swap_buffers();
        }

        return 0;
    }
};


int run() {
    core::Window window;
    Application application(&window);
    return application.run();
}


#ifdef WIN32
int wmain()
#else // WIN32
int main()
#endif // WIN32
{
    try {
        return run();
    } catch (const std::bad_alloc &e) {
        std::cerr << "Unable to allocate memory for program. Exiting." << std::endl;
        return 1;
    } catch (const std::exception &e) {
#ifdef WIN32
        std::cerr << e.what() << std::endl;
#endif
        return 1;
    }
}
