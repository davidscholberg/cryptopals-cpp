CPP=g++
CPPFLAGS=-Wall -std=c++14 -I .
OBJECTS=\
		cryptopals.o \
		wecrypt/wecrypt.o \
		utils/utils.o \
		challenges/s01c01-hex-to-base64.o \
		challenges/s01c02-fixed-xor.o \
		challenges/s01c03-single-byte-xor.o \
		challenges/s01c04-detect-single-byte-xor.o \
		challenges/s01c05-repeating-key-xor.o
CHALLENGE_HEADERS=\
		challenges/s01c01-hex-to-base64.hpp \
		challenges/s01c02-fixed-xor.hpp \
		challenges/s01c03-single-byte-xor.hpp \
		challenges/s01c04-detect-single-byte-xor.hpp \
		challenges/s01c05-repeating-key-xor.hpp

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

s01c01: all
	./cryptopals $@ 49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d

s01c02: all
	./cryptopals $@ 1c0111001f010100061a024b53535009181c 686974207468652062756c6c277320657965

s01c03: all
	./cryptopals $@ 1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736

s01c04: all
	./cryptopals $@ resources/s01c04-hex-strings.txt

s01c05: all
	./cryptopals $@ resources/s01c05-clear-text.txt resources/s01c05-key.txt

retab:
	find . -name '*.[ch]pp' -type f -exec bash -c 'expand -i -t 4 "{}" > ./not_a_filename && mv ./not_a_filename "{}"' \;
