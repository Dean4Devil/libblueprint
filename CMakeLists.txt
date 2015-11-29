cmake_minimum_required(VERSION 2.8.9)

project("libblueprint")

SET(SRCS
    src/blueprint.c
    src/parse.c

    src/parson.c

    src/bstrlib/bstraux.c
    src/bstrlib/bstrlib.c
    src/bstrlib/buniutil.c
    src/bstrlib/utf8util.c
)

SET(HDR_PUB
    include/blueprint.h
    include/bstrlib.h
)

SET(HDR_PRV
    src/resources.h
    src/parson.h

    src/bstrlib/bstraux.h
    src/bstrlib/buniutil.h
    src/bstrlib/utf8util.h
)

INCLUDE_DIRECTORIES(
    include
)

ADD_LIBRARY(blueprint SHARED
    ${SRCS} ${HDR_PUB} ${HDR_PRV}
)
