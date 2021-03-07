FILE(GLOB PLATFORM_SRCS
        "DX/*.H"
        "Win/*.H"
        "DX/*.cpp"
        "Win/*.cpp")

add_library(Platform STATIC ${PLATFORM_SRCS})