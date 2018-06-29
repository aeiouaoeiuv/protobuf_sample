
CROSS_COMPILE =
CPP = ${CROSS_COMPILE}g++
AR = ${CROSS_COMPILE}ar

BIN_NAME = main

PROTO_PATH = ./proto/
PROTO_GENERATE_PATH = ./proto_generated/
PROTO_FILES = $(foreach n, ${PROTO_PATH}, $(wildcard ${n}*.proto))
PROTO_ALL_CPPS = $(patsubst ${PROTO_PATH}%.proto, ${PROTO_GENERATE_PATH}%.pb.cpp, ${PROTO_FILES})
PROTO_ALL_OBJS = $(patsubst %.cpp, %.o, ${PROTO_ALL_CPPS})

INCS += -I/usr/local/include/
INCS += -I${PROTO_GENERATE_PATH}

LIB_PATH = ./lib/
LIB_MYPROTO = libmyproto.a

LIBS += -lmyproto
LIBS += -lprotobuf
LIBS += -pthread

CPPFLAGS = -Wall -O2 -std=c++11
CPPFLAGS += ${INCS}

LDFLAGS += -L /usr/local/lib/
LDFLAGS += -L ${LIB_PATH}

###############################################################

ALL: GENERATE_PROTO_FIRST ${LIB_MYPROTO} ${BIN_NAME}

${BIN_NAME}: main.o
	${CPP} ${CPPFLAGS} $^ ${LDFLAGS} ${LIBS} -o $@

${LIB_MYPROTO} : ${PROTO_ALL_OBJS}
	@echo "compile as static library"
	@if [ ! -d ${LIB_PATH} ]; then \
		mkdir -p ${LIB_PATH}; \
	fi
	${AR} crs ${LIB_PATH}$@ $^

GENERATE_PROTO_FIRST:
	@echo "generating cpps for proto"
	@if [ ! -d ${PROTO_GENERATE_PATH} ]; then \
		mkdir -p ${PROTO_GENERATE_PATH}; \
	fi
	@protoc -I=${PROTO_PATH} --cpp_out=${PROTO_GENERATE_PATH} ${PROTO_FILES}; \

%.o: %.cpp
	${CPP} ${CPPFLAGS} ${LDFLAGS} ${LIBS} -c $< -o $@

clean:
	-rm -f *.a *.o ${PROTO_ALL_OBJS} ${LIB_PATH}*.a ${BIN_NAME}

distclean: clean
	-rm -rf ${PROTO_GENERATE_PATH} ${LIB_PATH}

