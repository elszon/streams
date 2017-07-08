#include <iostream>
#include <fstream>


#include "SwapBuf.hpp"
#include "OddBuf.hpp"
#include "Istream.hpp"



void foo(){
    auto is = openIstream<OddBuf>("file.txt");

    std::cout << "foo stream root name: " << is->getRootName() << std::endl;
    std::cout << "foo stream name: " << is->getName() << std::endl;

    std::string str;
    while ( std::getline(*is, str) ){
        std::cout << str << std::endl;
    }
}



void foo2(){
    Istream<SwapBuf> is(openIstream<OddBuf>("file.txt"));

    std::cout << "foo2 stream root name: " << is.getRootName() << std::endl;
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
        std::cout << "foo21: " << is1.getRootName() << ", " << is2->getRootName() << std::endl;
    }

}

void foo22(){
    auto is = openIstream<OddBuf>("file.txt");

    std::cout << "foo22 stream root name: " << is->getRootName() << std::endl;
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
