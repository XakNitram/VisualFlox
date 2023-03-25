#include "pch.hpp"
#include "Core/Window/Window.hpp"
#include "Boid/Flock.hpp"
#include "Boid/Algorithm/DirectLoopAlgorithm.hpp"


// Notes:
// . How do models integrate with this application style?
// . Is this class style of application going to be annoying?
// . A model is a data source. A controller manages a data source. A view displays a data source.
// ? An array of "Layer"s that handle different aspects of the program.

using namespace std::chrono;

template<class Clock>
static inline double delta(time_point<Clock> start) {
    return 0.000001 * static_cast<double>(duration_cast<microseconds>(
        high_resolution_clock::now() - start
    ).count());
}


class Application : public core::Controller {
    core::Window *window;
    Flock flock {128};
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
        using
        enum core::Action;

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

        DirectLoopAlgorithm direct_loop_algorithm {Vector {0.0f, 0.0f, 0.0f}};
        Algorithm *algorithm {&direct_loop_algorithm};

        auto frame_start = high_resolution_clock::now();

        while (!window->should_close()) {
            const auto dt = static_cast<float>(delta(frame_start));
            frame_start = high_resolution_clock::now();
            window->update();
            flock.update(algorithm, dt);
            window->clear();
            window->swap_buffers();

            if (delta(frame_start) <= 0.008) {
                std::this_thread::sleep_for(milliseconds(1));
            }
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
