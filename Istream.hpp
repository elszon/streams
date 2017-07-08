#ifndef ISTREAM_HPP
#define ISTREAM_HPP

#include <istream>
#include <string>

#include <memory>


class IstreamName : public std::istream {
public:
    IstreamName(const std::string& rootName, const std::string& name)
        : name(name)
        , rootName(rootName) {
    }
    const std::string& getName() const { return name; }
    const std::string& getRootName() const { return rootName; }
private:
    std::string name;
    std::string rootName;
};

template <class StreamBuf>
class Istream : public IstreamName {
public:
    Istream(std::unique_ptr<std::istream>&& s, const std::string& name)
        : IstreamName(rootName(s.get(), name),
                      newName(s.get(), name))
        , iStream(std::move(s))
        , buf(iStream->rdbuf()){
        rdbuf(&buf);
    }

    Istream(std::unique_ptr<std::istream>&& s)
        : IstreamName(rootName(s.get()),
                      newName(s.get()))
        , iStream(std::move(s))
        , buf(iStream->rdbuf()){
        rdbuf(&buf);
    }

private:
    static std::string newName(const std::istream* is, const std::string& name = ""){
        std::string newName{name};
        const IstreamName* stream = dynamic_cast<const IstreamName*>(is);
        if ( stream ){
            newName = stream->getName();
        }
        newName.append("|>>|").append(StreamBuf::name());
        return newName;
    }

    static std::string rootName(const std::istream* is, const std::string& name = ""){
        const IstreamName* stream = dynamic_cast<const IstreamName*>(is);
        if ( stream ){
            return stream->getRootName();
        }
        return name;
    }

private:
    std::unique_ptr<std::istream> iStream;
    StreamBuf buf;
};



template <class StreamBuf = std::streambuf >
std::unique_ptr<Istream<StreamBuf>> openIstream(const std::string& filename){
    return std::make_unique<Istream<StreamBuf>>(std::make_unique<std::ifstream>(filename), filename);
}

template <class StreamBuf>
std::unique_ptr<Istream<StreamBuf>> makeIstream(std::unique_ptr<std::istream>&& s){
    return std::make_unique<Istream<StreamBuf>>(std::move(s));
}


#endif // ISTREAM_HPP
