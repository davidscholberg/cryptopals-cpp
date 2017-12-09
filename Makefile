CPP=g++
CPPFLAGS=-Wall -std=c++14 -I .
OBJECTS=\
		cryptopals.o \
		wecrypt/wecrypt.o \
		utils/utils.o \
		challenges/s01c01-hex-to-base64.o \
		challenges/s01c02-fixed-xor.o \
		challenges/s01c03-single-byte-xor.o \
		challenges/s01c04-detect-single-byte-xor.o
CHALLENGE_HEADERS=\
		challenges/s01c01-hex-to-base64.hpp \
		challenges/s01c02-fixed-xor.hpp \
		challenges/s01c03-single-byte-xor.hpp \
		challenges/s01c04-detect-single-byte-xor.hpp

all: cryptopals

cryptopals: $(OBJECTS)
	$(CPP) $(CPPFLAGS) $^ -o $@

cryptopals.o: cryptopals.cpp $(CHALLENGE_HEADERS)
	$(CPP) $(CPPFLAGS) -c $< -o $@

wecrypt/wecrypt.o: wecrypt/wecrypt.cpp wecrypt/wecrypt.hpp
	$(CPP) $(CPPFLAGS) -c $< -o $@

utils/utils.o: utils/utils.cpp utils/utils.hpp
	$(CPP) $(CPPFLAGS) -c $< -o $@

%.o: %.cpp %.hpp wecrypt/wecrypt.hpp utils/utils.hpp
	$(CPP) $(CPPFLAGS) -c $< -o $@

clean:
	find . -name '*.o' -exec rm '{}' \;
	rm cryptopals

retab:
	find . -name '*.[ch]pp' -type f -exec bash -c 'expand -i -t 4 "{}" > ./not_a_filename && mv ./not_a_filename "{}"' \;
