
#include "MemoryChunk.hpp"

MemoryChunk::MemoryChunk(std::string process_name, int starting_address , int space){
    
    this->process_name = process_name;
    this->starting_address = starting_address;
    this->space = space;
    
}
   
std::string MemoryChunk::get_name(){
    
   return process_name;
    
}

int MemoryChunk::get_start(){
    
    return starting_address;
    
}

int MemoryChunk::get_space(){
    
    return space;
}


void MemoryChunk::set_process_name(std::string n){
    
    process_name = n;
    
}

void MemoryChunk::set_starting_address(int s){
    
    starting_address = s;
}

void MemoryChunk::set_space(int s){
    
    space = s;
}


