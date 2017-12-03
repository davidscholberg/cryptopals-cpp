PROJECT_ROOT = .

all: build

build:
	g++ \
		-std=c++14 \
		-I ${PROJECT_ROOT} \
		s01c01.cpp \
		${PROJECT_ROOT}/cryptotools/*.cpp \
		-o s01c01

retab:
	find ${PROJECT_ROOT} -name '*.[ch]pp' -type f -exec bash -c 'expand -i -t 4 "{}" > ./not_a_filename && mv ./not_a_filename "{}"' \;
