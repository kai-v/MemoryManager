
#ifndef MemoryManager_hpp
#define MemoryManager_hpp

#include <stdio.h>
#include <iostream>
#include "MemoryChunk.hpp"
#include <list>
#include <queue>

class MemoryManager{
    
public:
    MemoryManager(std::string filename);
    void run();
    
private:
    
        // A struct to hold the values we read from the file
    struct read_type{
       
        std::string action;
        std::string name;
        int size;
    };
    
    std::string algo_name;
    int total_memory;
    void FirstFit();
       void BestFit();
       void WorstFit();
    std::list<MemoryChunk> memory_list; // Our list that holds instances of Memory Chunk
    void print_list(std::list<MemoryChunk> l);
    
    

    
    
    // A queue that holds instances of our struct , its an intermediate to hold the data we read from the file
    std::queue<read_type> processes;
};

#endif /* MemoryManager_hpp */
