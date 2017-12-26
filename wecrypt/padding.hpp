#ifndef PADDING_HPP_
#define PADDING_HPP_

#include <memory>
#include <vector>

namespace wecrypt {
    std::shared_ptr<std::vector<unsigned char>> pkcs7_pad(
            const std::vector<unsigned char> &buffer,
            const unsigned int block_size);

    // returns false if padding is not valid
    bool pkcs7_unpad(
            std::vector<unsigned char> &buffer,
            const unsigned int block_size);
}

#endif // PADDING_HPP_
