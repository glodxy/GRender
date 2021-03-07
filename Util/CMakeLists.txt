set(MATH_SRCS
        Math/Matrix.h
        Math/Vec.h)

set(TIMER_SRCS
        Timer/GTimer.h
        Timer/WindowsTimer.h
        Timer/WindowsTimer.cpp)
add_library(Util SHARED ${MATH_SRCS} ${TIMER_SRCS})
get_property(PR_PATH GLOBAL PROPERTY "PROJECT_PATH")

set(LIBRARY_OUTPUT_PATH ${PR_PATH}/lib)