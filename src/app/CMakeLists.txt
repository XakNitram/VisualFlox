target_sources(
    ${PROJECT_NAME} PRIVATE
    Application.cpp

    # **** Core ****
    Core/Window/Details.cpp
    Core/Window/Window.hpp
    Core/Window/Window.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(${PROJECT_NAME} PRIVATE glad)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)
target_link_libraries(${PROJECT_NAME} PRIVATE glm)
target_precompile_headers(${PROJECT_NAME} PRIVATE pch.hpp pch.cpp)
