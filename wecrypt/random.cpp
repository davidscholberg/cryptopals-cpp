#include <memory>
#include <random>
#include <vector>

#include "wecrypt/random.hpp"

namespace wecrypt {
    rng_ints::rng_ints(int a, int b) : generator(seed()), distribution(a, b) {
    }

    int rng_ints::operator()() {
        return distribution(generator);
    }

    std::shared_ptr<std::vector<unsigned char>> get_random_bytes(
            const unsigned int size) {
        auto buffer = std::make_shared<std::vector<unsigned char>>(size);

        rng_ints rng(0,255);

        for (unsigned int i = 0; i < buffer->size(); i++) {
            (*buffer)[i] = rng();
        }

        return buffer;
    }
}
