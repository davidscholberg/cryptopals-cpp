#ifndef INFO_HPP_
#define INFO_HPP_

#include <vector>

namespace wecrypt {
    float score_english(const std::vector<unsigned char> &buffer);

    int hamming_distance(
            const std::vector<unsigned char> &buffer_a,
            const std::vector<unsigned char> &buffer_b);
}

#endif // INFO_HPP_
