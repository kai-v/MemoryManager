#include <iostream>
#include "MemoryManager.hpp"

int main(int argc, const char * argv[]) {
    
    std::string filename;
    
    if(argc > 2){ //Usage Error
        std::cerr << "Usage: ./Project7 <filename>" << std::endl;
        return 1;
    }
    
    if(argc < 2){ //Usage Error
        std::cerr << "Usage: ./Project7 <filename>" << std::endl;
        return 1;
    }
    
    else{
        filename = argv[1];
    }
    
    MemoryManager m(filename);
    m.run();
    return 0;
}
