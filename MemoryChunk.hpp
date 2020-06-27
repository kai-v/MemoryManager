

#ifndef MemoryChunk_hpp
#define MemoryChunk_hpp

#include <stdio.h>
#include <iostream>


class MemoryChunk{
    public:
    MemoryChunk(std::string process_name, int starting_address , int space);
    
    // Getters
    std::string get_name();
    int get_start();
    int get_space();
    
    // Setters
    void set_process_name(std::string n);
    void set_starting_address(int s);
    void set_space(int s);
    

private:
   
    std::string process_name;
    int starting_address;
    int space;
    
    

    
};

#endif /* MemoryChunk_hpp */
