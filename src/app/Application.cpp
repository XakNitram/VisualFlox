#include "pch.hpp"
#include "Core/Window/Window.hpp"
#include "Boid/Flock.hpp"
#include "Boid/Algorithm/DirectLoopAlgorithm.hpp"
#include "Rendering/FlockRenderer.hpp"


using namespace lwvl::debug;


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

static const char *debug_source(lwvl::debug::Source source);
static const char *debug_type(lwvl::debug::Type type);


class Application final : public core::Controller {
    core::Window window;
    Flock flock {100};
public:
    explicit Application() {
        core::Hints window_hints {800, 450};
        core::WindowStatus result = window.create("VisualFlox", this, window_hints);
        if (result != core::WindowStatus::Success) {
            std::cerr << "Window creation failed." << std::endl;
            throw std::exception();
        }
    }

    static void handle_gl_error(
        Source source, Type type, Severity, unsigned int, int length, const char *message, const void *
    ) {
        if (type == Type::Other) {
            return;
        }

        std::cout
            << "[OpenGL]["
            << debug_source(source) << "][" << debug_type(type) << "] "
            << std::string_view {message, static_cast<Unsigned>(length)}
            << std::endl;
    }

    bool handle_key_event(core::KeyboardEvent event, core::Action action) override {
        using
        enum core::Action;

        if (action != Release) { return false; }
        if (event.key == GLFW_KEY_ESCAPE) {
            window.should_close(true);
            return true;
        }

        return false;
    }

    int run() {
        if (!window.created()) {
            return 1;
        }

        GLEventListener listener(this, handle_gl_error);

        const float world_bound = 500.0f;
        const int width = 800;
        const int height = 450;
        const float aspect = static_cast<float>(width) / static_cast<float>(height);
        //const Vector bounds {
        //    aspect >= 1.0f ? world_bound * aspect : world_bound,
        //    aspect < 1.0f ? world_bound * aspect : world_bound,
        //    world_bound
        //};

        const Vector bounds {
            world_bound * aspect,
            world_bound,
            world_bound
        };

        DirectLoopAlgorithm direct_loop_algorithm {bounds};
        Algorithm *algorithm {&direct_loop_algorithm};

        Projection projection {
            1.0f / bounds.x, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f / bounds.y, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f / bounds.z, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        };

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        FlockRenderer flock_renderer {flock.count(), projection};

        auto frame_start = high_resolution_clock::now();
        while (!window.should_close()) {
            const auto dt = static_cast<float>(delta(frame_start));
            frame_start = high_resolution_clock::now();
            window.update();
            flock.update(algorithm, dt);
            flock_renderer.update(flock);
            window.clear();
            flock_renderer.draw();
            window.swap_buffers();

            if (delta(frame_start) <= 0.008) {
                std::this_thread::sleep_for(milliseconds(1));
            }

            std::cout.flush();
        }

        return 0;
    }
};


int run() {
    Application application;
    return application.run();
}

static const char *debug_source(lwvl::debug::Source source) {
    switch (source) {
        using
        enum lwvl::debug::Source;
        case API: return "API";
        case Application: return "Application";
        case ShaderCompiler: return "Shader";
        case ThirdParty: return "ThirdParty";
        case WindowSystem: return "WindowSystem";
        default: return "Other";
    }
}

static const char *debug_type(lwvl::debug::Type type) {
    switch (type) {
        using
        enum lwvl::debug::Type;
        case DeprecatedBehavior: return "Deprecation";
        case Error: return "Error";
        case Marker: return "Marker";
        case Performance: return "Performance";
        case Portability: return "Portability";
        case PopGroup: return "PopGroup";
        case PushGroup: return "PushGroup";
        case UndefinedBehavior: return "UB";
        default: return "Other";
    }
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
