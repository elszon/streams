#include "OddBuf.hpp"


std::streambuf::int_type OddBuf::underflow() {
    std::streamsize read = sbuf->sgetn(internalBuf, BUF_SIZE);
    if (!read) return traits_type::eof();

    // Do vertical XOR decoding
    for (int i = 0; i < BUF_SIZE/2; ++i ) {
        if ( internalBuf[i] != '\n' && i%2 ){
            internalBuf[i] = internalBuf[i*2];
        }
    }

    setg(internalBuf, internalBuf, internalBuf + read);
    return traits_type::to_int_type(*gptr());
}
