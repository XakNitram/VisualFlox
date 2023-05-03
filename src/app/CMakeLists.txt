target_sources(
    ${PROJECT_NAME} PRIVATE
    Application.cpp

    # **** Boid ****
    Boid/Algorithm/Algorithm.hpp
    Boid/Algorithm/DirectLoopAlgorithm.hpp
    Boid/Algorithm/DirectLoopAlgorithm.cpp
    Boid/Boid.hpp
    Boid/Boid.cpp
    Boid/Flock.hpp
    Boid/Flock.cpp

    # **** Core ****
    Core/Window/Details.cpp
    Core/Window/Window.hpp
    Core/Window/Window.cpp

    # **** Math ****
    Math/Camera.hpp
    Math/Camera.cpp
#    Math/Fixed.hpp
    Math/Math.hpp
    Math/Math.cpp
    Math/Rectangle.hpp
    Math/Rectangle.cpp
    Math/Cube.hpp
    Math/Cube.cpp

    # **** Rendering ****
    Rendering/FlockRenderer.hpp
    Rendering/FlockRenderer.cpp
    Rendering/BoundRenderer.hpp
    Rendering/BoundRenderer.cpp

    # **** Structures ****
    Structures/DoubleBuffer.hpp
)

target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(${PROJECT_NAME} PRIVATE lwvl)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)
target_link_libraries(${PROJECT_NAME} PRIVATE glm)
target_precompile_headers(${PROJECT_NAME} PRIVATE pch.hpp pch.cpp)
