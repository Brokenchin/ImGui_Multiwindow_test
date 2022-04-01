


set(EDITOR_SRC
        src/Editor_App.cpp
        src/Editor_App.h)

#------------------------------------------------->
#------------------ GLFW ------------------------->
#https://www.glfw.org/docs/3.3/build_guide.html
#set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
#set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
#set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(Vendor/GLFW)
#------------------------------------------------->
#------------------- OpenGL ---------------------->
find_package(OpenGL REQUIRED)

#------------------------------------------------->
#------------------- GLEW ------------------------>
set(GLEW_USE_STATIC_LIBS TRUE)

#https://github.com/nigels-com/glew/issues/161
#Setting this allows static usage.
add_definitions(-DGLEW_STATIC)
set(GLEW_VERBOSE TRUE)
set(GLEW_ROOT "C:/dev/GLEW/glew-2.1.0/GLEW/")
set(GLEW_INCLUDE_DIR "C:/dev/GLEW/glew-2.1.0/GLEW/include")
set(GLEW_LIBRARIES "C:/dev/GLEW/glew-2.1.0/GLEW/lib")
find_package(GLEW REQUIRED)


#------------------------------------------------->
#------------------- IMGUI ----------------------->
SET(IMGUI_SRC
        ./Vendor/Imgui/imgui-docking/include/ImGui/imconfig.h
        ./Vendor/Imgui/imgui-docking/include/ImGui/imgui.cpp
        Vendor/Imgui/imgui-docking/include/ImGui/imgui.h
        Vendor/Imgui/imgui-docking/include/ImGui/imgui_demo.cpp
        Vendor/Imgui/imgui-docking/include/ImGui/imgui_draw.cpp
        Vendor/Imgui/imgui-docking/include/ImGui/imgui_internal.h
        Vendor/Imgui/imgui-docking/include/ImGui/imgui_tables.cpp
        Vendor/Imgui/imgui-docking/include/ImGui/imgui_widgets.cpp
        Vendor/Imgui/imgui-docking/include/ImGui/imstb_rectpack.h
        Vendor/Imgui/imgui-docking/include/ImGui/imstb_textedit.h
        Vendor/Imgui/imgui-docking/include/ImGui/imstb_truetype.h

        Vendor/Imgui/imgui-docking/include/ImGui/backends/imgui_impl_opengl3.cpp
        Vendor/Imgui/imgui-docking/include/ImGui/backends/imgui_impl_opengl3.h
        Vendor/Imgui/imgui-docking/include/ImGui/backends/imgui_impl_glfw.cpp
        Vendor/Imgui/imgui-docking/include/ImGui/backends/imgui_impl_glfw.h)


#TODO make a script to copy some assets to build folder..

find_package(GLEW)






add_executable(Editor
        ${EDITOR_SRC}
        ${IMGUI_SRC})
target_include_directories(Editor PUBLIC
        ./src
        ./Vendor/Imgui/imgui-docking/include
        ./Vendor/GLFW/include
        C:/dev/GLEW/glew-2.1.0/GLEW/include

        )

target_link_libraries(Editor PUBLIC ${OPENGL_LIBRARY} GLEW::GLEW glfw)