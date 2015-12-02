CC=clang
CFLAGS=-g -Wall -std=c11 -fPIC
INCLUDE=../include
LINK=-lblueprint_static -lm

# Build directory used for cmake build
LIBBDIR=../build

# Where binaries will be put
# Clean will remove everything inside, don't use '.'
OUTDIR=build

all: setup printinfo parse-serialize

setup:
	@mkdir -p ${OUTDIR}

printinfo: setup
	${CC} ${CFLAGS} printinfo.c -I${INCLUDE} -L${LIBBDIR} ${LINK} -o ${OUTDIR}/printinfo

parse-serialize: setup
	${CC} ${CFLAGS} parse-serialize.c -I${INCLUDE} -L${LIBBDIR} ${LINK} -o ${OUTDIR}/parse-serialize

clean:
	@rm -rf ${OUTDIR}
