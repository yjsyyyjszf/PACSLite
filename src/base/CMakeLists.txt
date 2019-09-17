file(GLOB SRCS *.cpp *.h)

set(TARGET_NAME base)

add_library(${TARGET_NAME} ${SRCS})

set(LIBS
    jsoncpp
    )

target_link_libraries(${TARGET_NAME} ${LIBS})
