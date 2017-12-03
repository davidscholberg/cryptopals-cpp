CPP=g++
CPPFLAGS=-Wall -std=c++14 -I .

all: cryptopals

cryptopals: cryptopals.o cryptotools/cryptotools.o
	$(CPP) $(CPPFLAGS) cryptopals.o cryptotools/cryptotools.o -o cryptopals

cryptopals.o: cryptopals.cpp cryptotools/cryptotools.hpp
	$(CPP) $(CPPFLAGS) -c cryptopals.cpp -o cryptopals.o

cryptotools/cryptotools.o: cryptotools/cryptotools.cpp cryptotools/cryptotools.hpp
	$(CPP) $(CPPFLAGS) -c cryptotools/cryptotools.cpp -o cryptotools/cryptotools.o

clean:
	find . -name '*.o' -exec rm '{}' \;
	rm cryptopals

retab:
	find . -name '*.[ch]pp' -type f -exec bash -c 'expand -i -t 4 "{}" > ./not_a_filename && mv ./not_a_filename "{}"' \;
