CPP=g++
CPPFLAGS=-Wall -std=c++14 -I .

all: cryptopals

cryptopals: cryptopals.o cryptotools/cryptotools.o
	$(CPP) $(CPPFLAGS) $^ -o $@

cryptopals.o: cryptopals.cpp cryptotools/cryptotools.hpp
	$(CPP) $(CPPFLAGS) -c $< -o $@

%.o: %.cpp %.hpp
	$(CPP) $(CPPFLAGS) -c $< -o $@

clean:
	find . -name '*.o' -exec rm '{}' \;
	rm cryptopals

retab:
	find . -name '*.[ch]pp' -type f -exec bash -c 'expand -i -t 4 "{}" > ./not_a_filename && mv ./not_a_filename "{}"' \;
