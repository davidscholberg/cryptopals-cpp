CXXFLAGS = -Wall -std=c++14 -I .

SRCDIRS := \
	challenges \
	utils \
	wecrypt

SRCS := $(wildcard *.cpp $(patsubst %,%/*.cpp,$(SRCDIRS)))
OBJS := $(patsubst %.cpp,%.o,$(SRCS))

DEPDIR := .d
DEPS := $(patsubst %.cpp,$(DEPDIR)/%.d,$(SRCS))
$(shell for i in $(SRCDIRS); do mkdir -p $(DEPDIR)/$$i; done >/dev/null)
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$*.Td

COMPILE.cpp = $(CXX) $(DEPFLAGS) $(CXXFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c
POSTCOMPILE = @mv -f $(DEPDIR)/$*.Td $(DEPDIR)/$*.d && touch $@

all: cryptopals

cryptopals: $(OBJS)
	$(LINK.cpp) $^ $(LOADLIBES) $(LDLIBS) $(OUTPUT_OPTION)

$(OBJS): %.o: %.cpp $(DEPDIR)/%.d
	$(COMPILE.cpp) $(OUTPUT_OPTION) $<
	$(POSTCOMPILE)

$(DEPS): ;
.PRECIOUS: $(DEPS)

include $(DEPS)

clean:
	rm -f cryptopals $(OBJS)
	rm -rf $(DEPDIR)

retab:
	find . -name '*.[ch]pp' -type f -exec bash -c 'expand -i -t 4 "{}" > ./not_a_filename && mv ./not_a_filename "{}"' \;

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

s02c11: all
	./cryptopals $@

s02c12: all
	./cryptopals $@

s02c13: all
	./cryptopals $@
