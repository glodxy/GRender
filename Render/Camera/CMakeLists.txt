FILE(GLOB CAMERA_SRCS
        "./*.h"
        "./*.cpp")

add_library(Camera STATIC ${CAMERA_SRCS})