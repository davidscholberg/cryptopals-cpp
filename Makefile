CC=g++
CFLAGS=-Wall -std=c++14 -I .
OBJECTS=\
		cryptopals.o \
		wecrypt/aes.o \
		wecrypt/cipher-mode.o \
		wecrypt/info.o \
		wecrypt/padding.o \
		wecrypt/string-conversion.o \
		wecrypt/xor.o \
		utils/utils.o \
		challenges/s01c01-hex-to-base64.o \
		challenges/s01c02-fixed-xor.o \
		challenges/s01c03-single-byte-xor.o \
		challenges/s01c04-detect-single-byte-xor.o \
		challenges/s01c05-repeating-key-xor.o \
		challenges/s01c06-break-repeating-key-xor.o \
		challenges/s01c07-aes-ecb.o \
		challenges/s01c08-detect-aes-ecb.o \
		challenges/s02c09-pkcs7-padding.o \
		challenges/s02c10-aes-cbc.o
CHALLENGE_HEADERS=\
		challenges/s01c01-hex-to-base64.hpp \
		challenges/s01c02-fixed-xor.hpp \
		challenges/s01c03-single-byte-xor.hpp \
		challenges/s01c04-detect-single-byte-xor.hpp \
		challenges/s01c05-repeating-key-xor.hpp \
		challenges/s01c06-break-repeating-key-xor.hpp \
		challenges/s01c07-aes-ecb.hpp \
		challenges/s01c08-detect-aes-ecb.hpp \
		challenges/s02c09-pkcs7-padding.hpp \
		challenges/s02c10-aes-cbc.hpp

all: cryptopals

cryptopals: $(OBJECTS)
	$(CC) $^ -o $@

cryptopals.o: cryptopals.cpp $(CHALLENGE_HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

utils/utils.o: utils/utils.cpp utils/utils.hpp
	$(CC) $(CFLAGS) -c $< -o $@

wecrypt/cipher-mode.o: wecrypt/cipher-mode.cpp wecrypt/cipher-mode.hpp wecrypt/aes.hpp wecrypt/padding.hpp wecrypt/xor.hpp
	$(CC) $(CFLAGS) -c $< -o $@

wecrypt/xor.o: wecrypt/xor.cpp wecrypt/xor.hpp wecrypt/info.hpp
	$(CC) $(CFLAGS) -c $< -o $@

wecrypt/%.o: wecrypt/%.cpp wecrypt/%.hpp
	$(CC) $(CFLAGS) -c $< -o $@

wecrypt/wecrypt.hpp: wecrypt/aes.hpp wecrypt/cipher-mode.hpp wecrypt/info.hpp wecrypt/padding.hpp wecrypt/string-conversion.hpp wecrypt/xor.hpp
	touch $@

%.o: %.cpp %.hpp wecrypt/wecrypt.hpp utils/utils.hpp
	$(CC) $(CFLAGS) -c $< -o $@

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

s01c06: all
	./cryptopals $@ resources/s01c06-cipher-text.txt

s01c07: all
	./cryptopals $@ resources/s01c07-cipher-text.txt resources/s01c07-key.txt

s01c08: all
	./cryptopals $@ resources/s01c08-hex-strings.txt

s02c09: all
	./cryptopals $@ "YELLOW SUBMARINE" 20

s02c10: all
	./cryptopals $@ resources/s02c10-cipher-text.txt resources/s02c10-key.txt resources/s02c10-iv.txt

clean:
	find . -name '*.o' -exec rm '{}' \;
	rm -f cryptopals

retab:
	find . -name '*.[ch]pp' -type f -exec bash -c 'expand -i -t 4 "{}" > ./not_a_filename && mv ./not_a_filename "{}"' \;
