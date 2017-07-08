#ifndef ODDBUF_HPP
#define ODDBUF_HPP

#include <streambuf>
#include <string>

class OddBuf : public std::streambuf {
public:
    static std::string name() {return "OddBuf";}
    OddBuf(std::streambuf* buf)
        : sbuf(buf){
        if (!buf){
            throw std::runtime_error("No valid std::stream buffer");
        }
    }
    std::streambuf::int_type underflow() final;

private:
    static const int BUF_SIZE = 1024;
    std::streambuf* sbuf;
    char internalBuf[BUF_SIZE];
};

#endif // ODDBUF_HPP
