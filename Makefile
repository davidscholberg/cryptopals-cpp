CPP=g++
CPPFLAGS=-Wall -std=c++14 -I .
OBJECTS=\
		cryptopals.o \
		cryptotools/cryptotools.o \
		challenges/s01c01-hex-to-base64.o
CHALLENGE_HEADERS=\
		challenges/s01c01-hex-to-base64.hpp

all: cryptopals

cryptopals: $(OBJECTS)
	$(CPP) $(CPPFLAGS) $^ -o $@

cryptopals.o: cryptopals.cpp $(CHALLENGE_HEADERS)
	$(CPP) $(CPPFLAGS) -c $< -o $@

cryptotools/cryptotools.o: cryptotools/cryptotools.cpp cryptotools/cryptotools.hpp
	$(CPP) $(CPPFLAGS) -c $< -o $@

%.o: %.cpp %.hpp cryptotools/cryptotools.hpp
	$(CPP) $(CPPFLAGS) -c $< -o $@

clean:
	find . -name '*.o' -exec rm '{}' \;
	rm cryptopals

retab:
	find . -name '*.[ch]pp' -type f -exec bash -c 'expand -i -t 4 "{}" > ./not_a_filename && mv ./not_a_filename "{}"' \;
