#include "SwapBuf.hpp"


std::streambuf::int_type SwapBuf::underflow() {
    std::streamsize read = sbuf->sgetn(internalBuf, BUF_SIZE);
    if (!read) return traits_type::eof();

    // Do vertical XOR decoding
    for (int i = 0; i < BUF_SIZE; ++i ) {
        if ( internalBuf[i] == '1' ){
            internalBuf[i] = 's';
        }
    }

    setg(internalBuf, internalBuf, internalBuf + read);
    return traits_type::to_int_type(*gptr());
}
