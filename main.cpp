#include <iostream>

#include <fstream>
#include <memory>
#include <vector>


class OddBuf : public std::streambuf {
public:
    static std::string name() {return "OddBuf";}
    OddBuf(std::streambuf* buf)
        : sbuf(buf){
    }
    std::streambuf::int_type underflow() override {
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

private:
    static const int BUF_SIZE = 1024;
    std::streambuf* sbuf;
    char internalBuf[BUF_SIZE];
};

class SwapBuf : public std::streambuf {
public:
    static std::string name() {return "SwapBuf";}
    SwapBuf(std::streambuf* buf)
        : sbuf(buf){
    }
    std::streambuf::int_type underflow() override {
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

private:
    static const int BUF_SIZE = 1024;
    std::streambuf* sbuf;
    char internalBuf[BUF_SIZE];
};

class IstreamName : public std::istream {

};

template <class Buffer>
class Istream : public std::istream {
public:
    Istream() = delete;
    Istream(std::unique_ptr<std::istream>&& s, const std::string& name)
        : inputStream(std::move(s))
        , buf(inputStream->rdbuf())
        , name(newName(inputStream.get(), name)){
        rdbuf(&buf);
    }

    Istream(std::unique_ptr<std::istream>&& s)
        : inputStream(std::move(s))
        , buf(inputStream->rdbuf())
        , name(newName(inputStream.get())){
        rdbuf(&buf);
    }

    static std::string newName(const std::istream* is, const std::string& name = ""){
        std::string newName{name};
        const Istream* stream = dynamic_cast<const Istream*>(is);
        if ( stream ){
            newName = stream->getName();
        }
        newName.append("/->/");
        newName.append(Buffer::name());
        return newName;
    }

    const std::string& getName() const { return name; }

private:
    std::unique_ptr<std::istream> inputStream;
    Buffer buf;
    const std::string name;
};



void foo(){
    auto file = std::make_unique<std::ifstream>("file.txt");
    Istream<OddBuf> is(std::move(file), "file.txt");


    std::cout << "foo stream name: " << is.getName() << std::endl;

    std::string str;
    while ( std::getline(is, str) ){
        std::cout << str << std::endl;
    }
}

void foo2(){
    auto file = std::make_unique<std::ifstream>("file.txt");
    std::unique_ptr<std::istream> file2 = std::make_unique<Istream<OddBuf>>(std::move(file), "file.txt");
    Istream<SwapBuf> is(std::move(file2));


    std::cout << "foo2 stream name: " << is.getName() << std::endl;

    std::string str;
    while ( std::getline(is, str) ){
        std::cout << str << std::endl;
    }
}

int main(int argc, char *argv[])
{
    (void)(argc);
    (void)(argv);

    std::ifstream file("file.txt");
    OddBuf buf(file.rdbuf());

    std::ifstream fileEmpty;
    OddBuf bufEmpty(fileEmpty.rdbuf());

    std::istream in(&bufEmpty);
    in.rdbuf(&buf);

    std::string str;
    while ( std::getline(in, str) ){
        std::cout << str << std::endl;
    }

    std::cout << "Hello World!" << std::endl;

    foo();

    std::cout << "Hello World!" << std::endl;

    foo2();


    return 0;
}
