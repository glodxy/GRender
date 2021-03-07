FILE(GLOB MODEL_SRCS
        "Texture/*.H"
        "obj/*.H"
        "Texture/*.cpp"
        "obj/*.cpp"
        "./*.h"
        "./*.cpp")

add_library(Model STATIC ${MODEL_SRCS})