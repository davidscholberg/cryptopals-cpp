CPP=g++
CPPFLAGS=-std=c++14 -I .

all: cryptopals

cryptopals: cryptopals.o cryptotools/cryptotools.o
	$(CPP) $(CPPFLAGS) cryptopals.o cryptotools/cryptotools.o -o cryptopals

cryptopals.o: cryptopals.cpp cryptotools/cryptotools.hpp
	$(CPP) $(CPPFLAGS) -c cryptopals.cpp -o cryptopals.o

cryptotools/cryptotools.o: cryptotools/cryptotools.cpp cryptotools/cryptotools.hpp
	$(CPP) $(CPPFLAGS) -c cryptotools/cryptotools.cpp -o cryptotools/cryptotools.o

retab:
	find ${PROJECT_ROOT} -name '*.[ch]pp' -type f -exec bash -c 'expand -i -t 4 "{}" > ./not_a_filename && mv ./not_a_filename "{}"' \;
