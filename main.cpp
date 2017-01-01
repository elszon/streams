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
public:

    const std::string& getName() const { return name; }
protected:
    void setName(const std::string& n){ name = n; }
private:
    std::string name;
};

template <class StreamBuf>
class Istream : public IstreamName {
public:
    Istream() = delete;
    Istream(std::unique_ptr<std::istream>&& s, const std::string& name)
        : iStream(std::move(s))
        , buf(iStream->rdbuf()){
        setName(newName(iStream.get(), name));
        rdbuf(&buf);
    }

    Istream(std::unique_ptr<std::istream>&& s)
        : iStream(std::move(s))
        , buf(iStream->rdbuf()){
        setName(newName(iStream.get()));
        rdbuf(&buf);
    }

    static std::string newName(const std::istream* is, const std::string& name = ""){
        std::string newName{name};
        const IstreamName* stream = dynamic_cast<const IstreamName*>(is);
        if ( stream ){
            newName = stream->getName();
        }
        newName.append("|>>|");
        newName.append(StreamBuf::name());
        return newName;
    }

private:
    std::unique_ptr<std::istream> iStream;
    StreamBuf buf;
};


template <class StreamBuf>
std::unique_ptr<Istream<OddBuf>> openIstream(const std::string& filename){
    return std::make_unique<Istream<OddBuf>>(std::make_unique<std::ifstream>(filename), filename);
}

template <class StreamBuf>
std::unique_ptr<Istream<StreamBuf>> makeIstream(std::unique_ptr<std::istream>&& s){
    return std::make_unique<Istream<StreamBuf>>(std::move(s));
}


void foo(){
    auto is = openIstream<OddBuf>("file.txt");

    std::cout << "foo stream name: " << is->getName() << std::endl;

    std::string str;
    while ( std::getline(*is, str) ){
        std::cout << str << std::endl;
    }
}



void foo2(){
    Istream<SwapBuf> is(openIstream<OddBuf>("file.txt"));

    std::cout << "foo2 stream name: " << is.getName() << std::endl;

    std::string str;
    while ( std::getline(is, str) ){
        std::cout << str << std::endl;
    }
}

void foo21(){
    Istream<SwapBuf> is1(openIstream<OddBuf>("file.txt"));

    auto is2 = makeIstream<SwapBuf>(openIstream<OddBuf>("file.txt"));

    if (is1.getName() != is2->getName()){
        std::cout << "wrong conversion" << std::endl;
    } else {
        std::cout << "foo21: " << is1.getName() << ", " << is2->getName() << std::endl;
    }

}

void foo22(){
    auto is = openIstream<OddBuf>("file.txt");

    std::cout << "foo22 stream name: " << is->getName() << std::endl;

    std::string str;
    while ( std::getline(*is, str) ){
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

    std::cout << "-------------" << std::endl;

    foo();

    std::cout << "-------------" << std::endl;

    foo2();

    std::cout << "-------------" << std::endl;

    foo21();

    std::cout << "-------------" << std::endl;

    foo22();


    return 0;
}
