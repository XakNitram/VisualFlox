target_sources(
    ${PROJECT_NAME} PRIVATE
    Application.cpp

    # **** Boid ****
    Boid/Algorithm/Algorithm.hpp
    Boid/Algorithm/DirectLoopAlgorithm.hpp
    Boid/Algorithm/DirectLoopAlgorithm.cpp
    Boid/Boid.hpp
    Boid/Flock.hpp
    Boid/Flock.cpp

    # **** Core ****
    Core/Window/Details.cpp
    Core/Window/Window.hpp
    Core/Window/Window.cpp

    # **** Structures ****
    Structures/DoubleBuffer.hpp
)

target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(${PROJECT_NAME} PRIVATE glad)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)
target_link_libraries(${PROJECT_NAME} PRIVATE glm)
target_precompile_headers(${PROJECT_NAME} PRIVATE pch.hpp pch.cpp)
