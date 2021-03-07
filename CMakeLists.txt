cmake_minimum_required(VERSION 3.12)
project(GRenderer)

set(CMAKE_CXX_STANDARD 14)

add_definitions(-DDEBUG)
add_definitions(-D_WIN)

aux_source_directory(. DIR_SRCS)

set(EXECUTABLE_OUTPUT_PATH e:/project/${PROJECT_NAME})
set_property(GLOBAL PROPERTY PROJECT_PATH "${EXECUTABLE_OUTPUT_PATH}")
add_subdirectory(Util)
add_subdirectory(Render)


add_executable(GRenderer WIN32 main.cpp CanvaApp.h 3DApp.h Library/stb_image.h)
link_directories(${EXECUTABLE_OUTPUT_PATH}/lib)
target_link_libraries(GRenderer Util Render dxgi.lib dxguid.lib d3d12.lib d3dcompiler.lib)
install(FILES "${EXECUTABLE_OUTPUT_PATH}/lib/Render.dll" DESTINATION ${EXECUTABLE_OUTPUT_PATH})
install(FILES "${EXECUTABLE_OUTPUT_PATH}/lib/Util.dll" DESTINATION ${EXECUTABLE_OUTPUT_PATH})

#install(TARGETS Render RUNTIME DESTINATION ${EXECUTABLE_OUTPUT_PATH}/lib)
#install(TARGETS Util RUNTIME DESTINATION ${EXECUTABLE_OUTPUT_PATH}/lib)
#add_executable(GRenderer WIN32 main.cpp Render/GCanvas.cpp Render/GCanvas.h Render/GColor.h Device/GDevice.h Render/Platform/DX/D3DApp.cpp Render/Platform/DX/D3DApp.h Render/Platform/DX/D3DUtil.cpp Render/Platform/DX/D3DUtil.h Util/Timer/WindowsTimer.cpp Util/Timer/WindowsTimer.h Util/Timer/GTimer.h Render/GApp.h Render/Platform/Win/WinApp.cpp Render/Platform/Win/WinApp.h Util/Math/Matrix.h Render/Graph/BaseGraph.h Render/Graph/Graph.h Render/Graph/Triangle.h Render/Shader/BaseShader.h Render/Object/GRObject.cpp Render/Object/GRObject.h Render/Pipeline/Pipeline.h Render/Pipeline/TrivalPipeline.cpp Render/Pipeline/TrivalPipeline.h Render/Graph/ScreenGraph/Line.h Render/Shader/TestShader.h Render/Pipeline/Pipeline.cpp Render/Object/obj/ObjLoader.h Render/Camera/BaseCamera.h Render/Camera/RoundCamera.cpp Render/Camera/RoundCamera.h Render/Graph/ScreenGraph/Line.cpp Render/Graph/ScreenGraph/Circle.cpp Render/Graph/ScreenGraph/Circle.h Render/Graph/ScreenGraph/Polygon.cpp Render/Graph/ScreenGraph/Polygon.h CanvaApp.h 3DApp.h Render/Object/Texture/Texture.cpp Render/Object/Texture/Texture.h)
#target_link_libraries(${PROJECT_NAME} dxgi.lib dxguid.lib d3d12.lib d3dcompiler.lib)

#
#set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:CONSOLE")
#set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS_RELWITHDEBINFO "/SUBSYSTEM:WINDOWS")
#set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS")
#set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS_MINSIZEREL "/SUBSYSTEM:WINDOWS")

