#include <iostream>

#include <fstream>
#include <memory>
#include <vector>


std::string newName(std::fstream* fs, const std::string& name);

class FstreamWrapper : public std::fstream {
public:
    FstreamWrapper(std::unique_ptr<std::fstream> fs, const std::string& name)
        : name(newName(fs.get(), name)){
        swap(*fs);
    }

    const std::string& getName() const { return name; }
    std::fstream& getStream() { return *this; }
private:
    const std::string name;
    };

std::string newName(std::fstream* fs, const std::string& name){
    std::string newName{name};
    const FstreamWrapper* basedStream = dynamic_cast<FstreamWrapper*>(fs);
    if ( basedStream ){
        newName.append("/<-/");
        newName.append(basedStream->getName());
        return newName;
    }
    return newName;
}

template< class StreamBuf >
class Fstream : public FstreamWrapper {
public:
    Fstream(std::unique_ptr<std::fstream>&& fs)
        : FstreamWrapper(std::move(fs), StreamBuf::name())
        , sbuf(getStream().rdbuf())
        , istream(&sbuf){
    }
private:
    StreamBuf sbuf;
    std::istream istream;
};

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

template <class Buffer>
class Istream : public std::istream {
public:
    Istream() = delete;
    Istream(std::unique_ptr<std::istream>&& s)
        : inputStream(std::move(s))
        , buf(inputStream->rdbuf()){
        rdbuf(&buf);
    }

private:
    std::unique_ptr<std::istream> inputStream;
    Buffer buf;
};

void foo(){
    auto file = std::make_unique<std::ifstream>("file.txt");
    Istream<OddBuf> is(std::move(file));

    std::string str;
    while ( std::getline(is, str) ){
        std::cout << str << std::endl;
    }
}

void foo2(){
    auto file = std::make_unique<std::ifstream>("file.txt");
    std::unique_ptr<std::istream> file2 = std::make_unique<Istream<OddBuf>>(std::move(file));
    Istream<SwapBuf> is(std::move(file2));

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
