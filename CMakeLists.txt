cmake_minimum_required(VERSION 2.8.9)

set(CMAKE_C_STANDARD 11)

project(libblueprint C)

set(VERSION_MAJOR 1)
set(VERSION_MINOR 0)
set(VERSION_PATCH 0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SRCS
    src/blueprint.c
    src/parse.c
    src/item_dictionary.c
    src/serialize.c

    src/parson.c

    src/bstrlib/bstraux.c
    src/bstrlib/bstrlib.c
    src/bstrlib/buniutil.c
    src/bstrlib/utf8util.c
)

set(HDR_PUB
    include/blueprint.h
    include/bstrlib.h
)

set(HDR_PRV
    src/item_dictionary.h
    src/parson.h

    src/bstrlib/bstraux.h
    src/bstrlib/buniutil.h
    src/bstrlib/utf8util.h
)

include_directories(
    include
)

add_library(blueprint SHARED
    ${SRCS} ${HDR_PUB} ${HDR_PRV}
)

add_library(blueprint_static STATIC
    ${SRCS} ${HDR_PUB} ${HDR_PRV}
)
set_target_properties(blueprint_static PROPERTIES
    POSITION_INDEPENDENT_CODE ON
)
