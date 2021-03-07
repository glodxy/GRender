#AUX_SOURCE_DIRECTORY(.  DIR_LIB_RENDER)

set(SRCS
        ./GApp.h
        ./GCanvas.cpp
        ./GCanvas.h
        ./GColor.h)

set(SHADER_SRCS
        Shader/BaseShader.h
        Shader/TestShader.h)

set(PIPELINE_SRCS
        Pipeline/Pipeline.h
        Pipeline/Pipeline.cpp
        Pipeline/TrivalPipeline.h
        Pipeline/TrivalPipeline.cpp)

get_property(P_PATH GLOBAL PROPERTY "PROJECT_PATH")

add_subdirectory(Graph)
add_subdirectory(Platform)
add_subdirectory(Object)
add_subdirectory(Camera)

set(LIBRARY_OUTPUT_PATH ${P_PATH}/lib)

add_library(Render SHARED ${SRCS} ${SHADER_SRCS} ${PIPELINE_SRCS})
target_link_libraries(Render Platform Graph Model Camera)

MESSAGE("build over")
MESSAGE(${P_PATH})
