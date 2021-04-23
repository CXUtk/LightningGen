cmake_minimum_required (VERSION 3.8)

project (Lightning VERSION 1.0)

add_executable (Lightning "Lightning.h" 
        "Lightning.cpp" 
        "Canvas.h"
        "Canvas.cpp"
        "Renderer.h"
        "Renderer.cpp"
        "LightningTree.h"
        "LightningTree.cpp"
        "Shader.h"
        "ShaderLoader.h"
        "ShaderLoader.cpp"
)

set_target_properties (Lightning PROPERTIES
                      CXX_STANDARD 17
                      CXX_EXTENSIONS OFF
                      )

include_directories (${PROJECT_SOURCE_DIR}/deps)

include_directories (${PROJECT_SOURCE_DIR}/deps/GLFW/include)

if (UNIX)
    FIND_PACKAGE (glfw3)
    if (glfw3_FOUND)
        target_link_libraries(Lightning PRIVATE glfw)
    else()
        add_subdirectory("${PROJECT_SOURCE_DIR}/deps/GLFW")
        target_link_libraries(Lightning PRIVATE glfw)
    endif()
    target_link_libraries (Lightning PRIVATE dl X11 pthread)
else()
    add_subdirectory("${PROJECT_SOURCE_DIR}/deps/GLFW")
    target_link_libraries(Lightning PRIVATE glfw)
endif()

find_package(OpenGL REQUIRED)
target_link_libraries(Lightning PRIVATE OpenGL::GL)


add_subdirectory ("deps/glad")

target_link_libraries (Lightning PRIVATE glad)