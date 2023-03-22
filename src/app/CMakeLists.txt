target_sources(
    ${CMAKE_PROJECT_NAME} PRIVATE
    Application.cpp
)

target_precompile_headers(${CMAKE_PROJECT_NAME} PRIVATE pch.hpp pch.cpp)
