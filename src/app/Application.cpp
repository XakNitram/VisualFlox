#include "pch.hpp"
#include "Core/Window/Window.hpp"
#include "Boid/Flock.hpp"
#include "Boid/Algorithm/DirectLoopAlgorithm.hpp"
#include "Rendering/FlockRenderer.hpp"
#include "Rendering/BoundRenderer.hpp"
#include "Math/Camera.hpp"


using namespace lwvl::debug;


// https://on-demand.gputechconf.com/siggraph/2016/presentation/sig1611-thomas-true-high-dynamic-range-rendering-displays.pdf

// Notes:
// . How do models integrate with this application style?
// . Is this class style of application going to be annoying?
// . A model is a data source. A controller manages a data source. A view displays a data source.
// ? An array of "Layer"s that handle different aspects of the program.
// . Animations have distinct types they can be sorted into. Translation, Rotation, Scale, Interpolation?, Color

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
    Flock flock {250};
    bool s_pressed = false;
    bool w_pressed = false;
    bool a_pressed = false;
    bool d_pressed = false;
    bool paused = false;
    const Vector cursor_position {0.0f, 0.0f, 0.0f};
public:
    explicit Application() {
        core::Hints window_hints {1920, 1080};
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

        if (event.key == GLFW_KEY_W) {
            if (action == Press) {
                w_pressed = true;
            } else if (action == Release) {
                w_pressed = false;
            }
        }

        if (event.key == GLFW_KEY_S) {
            if (action == Press) {
                s_pressed = true;
            } else if (action == Release) {
                s_pressed = false;
            }
        }

        if (event.key == GLFW_KEY_D) {
            if (action == Press) {
                d_pressed = true;
            } else if (action == Release) {
                d_pressed = false;
            }
        }

        if (event.key == GLFW_KEY_A) {
            if (action == Press) {
                a_pressed = true;
            } else if (action == Release) {
                a_pressed = false;
            }
        }

        if (action != Release) { return false; }
        if (event.key == GLFW_KEY_ESCAPE) {
            window.should_close(true);
            return true;
        }

        if (event.key == GLFW_KEY_SPACE) {
            paused ^= true;
        }

        return false;
    }

    bool handle_mouse_motion_event(core::MouseMotionEvent event) override {
        //const Vector cursor_previous_position {cursor_position}; {
        //    const core::Size window_size {window.size()};
        //    const auto x_pos = static_cast<float>(event.pos.x) / static_cast<float>(window_size.x);
        //    const auto y_pos = static_cast<float>(event.pos.y) / static_cast<float>(window_size.y);
        //    cursor_position = Vector {
        //        (x_pos * 2.0f - 1.0f) * bounds.x,
        //        (y_pos * 2.0f - 1.0f) * bounds.y,
        //        0.0f  // Aim sensitivity as z-depth, then apply perspective?
        //    };
        //}
        return false;
    }

    int run() {
        if (!window.created()) {
            return 1;
        }

        GLEventListener listener(this, handle_gl_error);

        const float world_bound = 50.0f;
        const int width = 1920;
        const int height = 1080;
        const float aspect = static_cast<float>(width) / static_cast<float>(height);
        const Vector bounds {world_bound};
        const Cube bounding_cube {bounds};

        const float far_plane = bounds.z * 11.0f;
        const float near_plane = 1.0f; // distance to the near clipping plane
        const float fov = 91.0f;
        Transform projection = glm::perspective(
            glm::radians(fov), aspect,
            near_plane, far_plane + near_plane
        );

        // What do we need for a camera?
        // * Position
        // * Direction pointer
        // * Has to remain upright <-- sticking point

        Transform view = glm::translate(
            Transform(1.0f), Vector(0.0f, 0.0f, -bounds.z * 2.0f)
        );

        DirectLoopAlgorithm direct_loop_algorithm {bounds};
        Algorithm *algorithm {&direct_loop_algorithm};

        //glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        FlockRenderer flock_renderer {flock.count(), projection};
        BoundRenderer bound_renderer {bounding_cube, projection};

        auto frame_start = high_resolution_clock::now();
        while (!window.should_close()) {
            const auto dt = static_cast<float>(delta(frame_start));
            const float normal_delta = dt * 60.0f;
            frame_start = high_resolution_clock::now();
            window.update();

            if (!paused) {
                flock.update(algorithm, dt);
                flock_renderer.update(flock);
            }

            auto z_direction = static_cast<float>(w_pressed - s_pressed);
            auto x_direction = static_cast<float>(a_pressed - d_pressed);

            //view = glm::translate(view, Vector(0.0f, 0.0f, z_direction * normal_delta));
            view = glm::rotate(view, glm::radians(x_direction * normal_delta * 0.75f), Vector(0.0f, 1.0f, 0.0f));
            view = glm::rotate(view, glm::radians(z_direction * normal_delta * 0.75f), Vector(1.0f, 0.0f, 0.0f));

            flock_renderer.set_view(view);
            bound_renderer.set_view(view);
            window.clear();
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Enable wireframe rendering
            flock_renderer.set_color(Triple(0.0f, 0.0f, 0.0f));
            flock_renderer.draw();
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Restore the default rendering mode
            glDisable(GL_DEPTH_TEST);
            bound_renderer.draw();
            glEnable(GL_DEPTH_TEST);
            flock_renderer.set_color(Boid::Color);
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
