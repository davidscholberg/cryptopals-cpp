#include <fstream>
#include <iostream>
#include <iomanip>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "utils/utils.hpp"

namespace utils {
    std::shared_ptr<std::string> get_file_contents(std::shared_ptr<std::string> filename) {
        std::ifstream in(*filename, std::ios::in | std::ios::binary);

        if (!in) {
            return nullptr;
        }

        auto contents = std::make_shared<std::string>();
        std::string& contents_ref = *contents;
        in.seekg(0, std::ios::end);
        contents_ref.resize(in.tellg());
        in.seekg(0, std::ios::beg);
        in.read(&contents_ref[0], contents_ref.size());
        in.close();
        return contents;
    }

    std::shared_ptr<std::vector<std::string>> get_lines(std::shared_ptr<std::string> filename) {
        std::ifstream in(*filename, std::ios::in | std::ios::binary);

        if (!in) {
            return nullptr;
        }

        auto lines = std::make_shared<std::vector<std::string>>();

        while (in.good()) {
            lines->push_back(std::string());
            std::getline(in, lines->back());
            if (lines->back().empty()) {
                lines->pop_back();
            }
        }

        if (!in.eof()) {
            return nullptr;
        }

        return lines;
    }

    std::shared_ptr<std::string> hexdump(std::shared_ptr<std::vector<unsigned char>> bytes) {
        auto hexdump_str = std::make_shared<std::string>();
        unsigned int offset = 0;
        while (offset < bytes->size()) {
            std::stringstream address;
            std::stringstream hex;
            std::stringstream ascii;

            address << std::hex << std::setfill('0') << std::setw(8) << offset;
            for (int i = 0; i < 16; i++) {
                if (i % 8 == 0) {
                    hex << " ";
                }
                if (offset >= bytes->size()) {
                    hex << "   ";
                    ascii << " ";
                    continue;
                }
                hex << " ";
                hex << std::hex << std::setfill('0') << std::setw(2) << (unsigned int)(*bytes)[offset];

                if((*bytes)[offset] < 32) {
                    ascii << '.';
                } else {
                    ascii << (*bytes)[offset];
                }
                offset++;
            }
            *hexdump_str += address.str() + hex.str() + "  |" + ascii.str() + "|\n";
        }
        return hexdump_str;
    }
}
