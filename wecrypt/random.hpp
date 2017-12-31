#ifndef RANDOM_HPP_
#define RANDOM_HPP_

#include <limits>
#include <memory>
#include <random>
#include <vector>

namespace wecrypt {
    class rng_ints {
        public:
            rng_ints(int a = 0, int b = std::numeric_limits<int>::max());
            int operator()();
        private:
            std::random_device seed;
            std::mt19937_64 generator;
            std::uniform_int_distribution<int> distribution;
    };

    std::shared_ptr<std::vector<unsigned char>> get_random_bytes(
            const unsigned int size);
}

#endif // RANDOM_HPP_
