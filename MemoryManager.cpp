#include "MemoryManager.hpp"
#include "MemoryChunk.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <queue>
#include <list>
#include <algorithm>
#include <iterator>

MemoryManager::MemoryManager(std::string filename){
    
    std::queue<std::string> store; // temp queue to hold all the words
    std::ifstream read_file(filename);
    std::string input;
    
    // FILE I/0
    if (!(read_file.is_open())){  // If file cannot open then throw and error.
        std::cerr << "Could not open file" << std::endl;
        exit(EXIT_FAILURE);
    }
    
    std::string line;
    while (getline (read_file, line)) { // Reading the line
        
        std::istringstream stream(line);
        std::string word;
        
        while (stream >> word){ // Reading each word in the line
            store.push(word); // push each word into the queue
        }
    }
    
    // The name of the algorithm
    algo_name = store.front();
    store.pop();
    
    // Total Amount of Memory in the system
    total_memory = stoi(store.front());
    store.pop();
    
    /* Populating the processes queue. We create an instance of our struct and insert into the queue */
    while(!store.empty()){
        
        std::string action;
        std::string name;
        int size;
        
        if(store.front() == "load"){
            
            action = store.front();
            store.pop();
            
            name = store.front();
            store.pop();
            
            size = stoi(store.front());
            store.pop();
            
            read_type  r = {action,name,size};
            processes.push(r);
        }
        
        else if (store.front() == "unload"){
            
            action = store.front();
            store.pop();
            
            name = store.front();
            store.pop();
            
            size = 0; // For an unload , then use 0 as a place holder for size in the queue
            
            read_type  r = {action,name,size};
            processes.push(r);
            
        }
    }
    
  
}

// Runs the algorithm indicated in the file
void MemoryManager::run(){
    
    if(algo_name == "firstFit"){
        
        FirstFit();
    }
    
        else if(algo_name == "bestFit"){
    
            BestFit();
        }
    
        else if(algo_name == "worstFit"){
    
            WorstFit();
        }
    
    else{
        
        std::cerr << "Should be one of firstFit, bestFit or worstFit" << std::endl;
        return;
    }
}



void MemoryManager::print_list(std::list<MemoryChunk> l){
    
    for (MemoryChunk &i : l){
        
        std::cout << i.get_name() << " : start " << i.get_start() << " , size " << i.get_space() << std::endl;
        
    }
}

void MemoryManager::FirstFit(){
    
    bool cant_load = true;
    int last_hole_size = total_memory;
    int start = 0;
    std::list<MemoryChunk>::iterator process_index;
    memory_list.push_back({"hole",start,total_memory});
    
    std::cout << memory_list.begin()->get_name() << " : start " << memory_list.begin()->get_start() << " , size " << memory_list.begin()->get_space() << std::endl;
    
    while(!processes.empty()){
        
        std::cout << std::endl;
        
        if(processes.front().action == "load"){
            
            std::cout << processes.front().action << " " << processes.front().name << " " << processes.front().size << std::endl;
            
            for (std::list<MemoryChunk>::iterator i = memory_list.begin(); i != memory_list.end(); i++){
                
                process_index = std::next(i);
                
                if(i->get_name() == "hole" && i->get_space() >= processes.front().size){
                    
                    last_hole_size = i->get_space();
                    start = i->get_start();
                    cant_load = false;
                    i->set_process_name(processes.front().name);
                    i->set_space(processes.front().size);
                    
                    
                    start = start + processes.front().size;
                    last_hole_size = last_hole_size - processes.front().size;
                    
                    if(last_hole_size != 0){
                        
                        memory_list.insert(process_index,{"hole",start,last_hole_size});
                        
                    }
                    
                    processes.pop();
                    break;
                }
                
                cant_load = true;
                
            }
            
            if(cant_load){
                std::cout << "Unable to load process " << processes.front().name << std::endl;
                processes.pop();
            }
            
        }
        
        // UNLOAD
        else if(processes.front().action == "unload"){
            
            std::cout << processes.front().action << " " << processes.front().name << std::endl;
            
            for ( std::list<MemoryChunk>::iterator i = memory_list.begin(); i != memory_list.end(); i++){
                
                if(i->get_name() == processes.front().name){ // If we are at that process
                     
                    i->set_process_name("hole"); // we unload and indicate that it is a hole
                    
                    if(i != memory_list.begin()){ // we aren't at the begining
                       
                        // Check if the previous element was a hole
                        auto prev = std::prev(i, 1);
                        
                        // if it is , then we combine the two
                        if(prev->get_name() == "hole"){
                            
                            i->set_space(prev->get_space() + i->get_space());
                            i->set_starting_address(prev->get_start());
                            memory_list.erase(prev);
                        }
                        
                    }
                    
                    if(i != std::prev(memory_list.end())){ // If e aren't at the end
                           
                        // Check if the next element is a hole
                        auto next = std::next(i, 1);
                       
                         // if it is , then we combine the two
                        if(next->get_name() == "hole"){

                            i->set_space(next->get_space() + i->get_space());
                            memory_list.erase(next);
                        }

                    }
                    
                    /* After we unload the process and combine adjacent holes , we exit the loop */
                   processes.pop();
                    break;
                }
            }
            
        }
       
        
        print_list(memory_list);
    }
}


void MemoryManager::BestFit(){
    
    bool cant_load = true;
    int last_hole_size = total_memory;
    int start = 0;
    
    std::list<MemoryChunk>::iterator insert_index;
    
    memory_list.push_back({"hole",start,total_memory});
    
    std::cout << memory_list.begin()->get_name() << " : start " << memory_list.begin()->get_start() << " , size " << memory_list.begin()->get_space() << std::endl;
    
    while(!processes.empty()){
        
        std::cout << std::endl;
        int best_size = total_memory;
        
        if(processes.front().action == "load"){
            
            std::cout << processes.front().action << " " << processes.front().name << " " << processes.front().size << std::endl;
            
            // Checks for the best fit hole by looking
            for (std::list<MemoryChunk>::iterator i = memory_list.begin(); i != memory_list.end(); i++){
                // This is a potential hole
                if(i->get_name() == "hole" && i->get_space() >= processes.front().size){
                    
                    if(i->get_space() < best_size){
                        
                        best_size = i->get_space();
                        cant_load = false;
                        
                    }
                }
            }
            
            // Now we insert into the hole we found above
            for (std::list<MemoryChunk>::iterator i = memory_list.begin(); i != memory_list.end(); i++){
                
                if(i->get_name() == "hole" && i->get_space() == best_size){
                    
                    insert_index = std::next(i);
                    last_hole_size = i->get_space();
                    start = i->get_start();
                    cant_load = false;
                    i->set_process_name(processes.front().name);
                    i->set_space(processes.front().size);
                    
                    start = start + processes.front().size;
                    last_hole_size = last_hole_size - processes.front().size;
                    
                    if(last_hole_size != 0){
                        
                        memory_list.insert(insert_index,{"hole",start,last_hole_size});
                        
                    }
                    
                    processes.pop();
                    break;
                }
                
                cant_load = true;
            }
            
            if(cant_load == true) {
                std::cout << "Unable to load process " << processes.front().name << std::endl;
                processes.pop();
            }
        }
        
        // UNLOAD
        else if(processes.front().action == "unload"){
            
            std::cout << processes.front().action << " " << processes.front().name << std::endl;
            
            for ( std::list<MemoryChunk>::iterator i = memory_list.begin(); i != memory_list.end(); i++){
                
                if(i->get_name() == processes.front().name){ // If we are at that process
                     
                    i->set_process_name("hole"); // we unload and indicate that it is a hole
                    
                    if(i != memory_list.begin()){ // we aren't at the begining
                       
                        // Check if the previous element was a hole
                        auto prev = std::prev(i, 1);
                        
                        // if it is , then we combine the two
                        if(prev->get_name() == "hole"){
                            
                            i->set_space(prev->get_space() + i->get_space());
                            i->set_starting_address(prev->get_start());
                            memory_list.erase(prev);
                        }
                        
                    }
                    
                    if(i != std::prev(memory_list.end())){ // If e aren't at the end
                           
                        // Check if the next element is a hole
                        auto next = std::next(i, 1);
                       
                         // if it is , then we combine the two
                        if(next->get_name() == "hole"){

                            i->set_space(next->get_space() + i->get_space());
                            memory_list.erase(next);
                        }

                    }
                    
                    /* After we unload the process and combine adjacent holes , we exit the loop */
                   processes.pop();
                    break;
                }
            }
            
        }
        print_list(memory_list);
    }
}
    
    void MemoryManager::WorstFit(){
    
        bool cant_load = true;
        int last_hole_size = total_memory;
        int start = 0;
    
        std::list<MemoryChunk>::iterator insert_index;
    
        memory_list.push_back({"hole",start,total_memory});
    
        std::cout << memory_list.begin()->get_name() << " : start " << memory_list.begin()->get_start() << " , size " << memory_list.begin()->get_space() << std::endl;
    
        while(!processes.empty()){
    
            std::cout << std::endl;
            int max_size = 0;
    
            if(processes.front().action == "load"){
    
                std::cout << processes.front().action << " " << processes.front().name << " " << processes.front().size << std::endl;
    
                // Checks for the largest hole
                for (std::list<MemoryChunk>::iterator i = memory_list.begin(); i != memory_list.end(); i++){
    
                    // This is a potential hole
    
                    if(i->get_name() == "hole" && i->get_space() >= processes.front().size){
    
                        if(i->get_space() > max_size){
    
                            max_size = i->get_space();
                            // std::cout << "Max Size " << max_size << std::endl;
                            cant_load = false;
    
                        }
                    }
                }
    
                // Now we insert into the hole we found above
                for (std::list<MemoryChunk>::iterator i = memory_list.begin(); i != memory_list.end(); i++){
    
                    if(i->get_name() == "hole" && i->get_space() == max_size){
    
                        insert_index = std::next(i);
                        last_hole_size = i->get_space();
                        start = i->get_start();
                        cant_load = false;
                        i->set_process_name(processes.front().name);
                        i->set_space(processes.front().size);
    
                        start = start + processes.front().size;
                        last_hole_size = last_hole_size - processes.front().size;
    
                        if(last_hole_size != 0){
    
                            memory_list.insert(insert_index,{"hole",start,last_hole_size});
    
                        }
                        // start = start + processes.front().size;
                        processes.pop();
                        break;
    
                    }
                    cant_load = true;
                }
    
                if(cant_load == true) {
                    std::cout << "Unable to load process " << processes.front().name << std::endl;
                    processes.pop();
                }
            }
    
            // UNLOAD
            else if(processes.front().action == "unload"){
                
                std::cout << processes.front().action << " " << processes.front().name << std::endl;
                
                for ( std::list<MemoryChunk>::iterator i = memory_list.begin(); i != memory_list.end(); i++){
                    
                    if(i->get_name() == processes.front().name){ // If we are at that process
                         
                        i->set_process_name("hole"); // we unload and indicate that it is a hole
                        
                        if(i != memory_list.begin()){ // we aren't at the begining
                           
                            // Check if the previous element was a hole
                            auto prev = std::prev(i, 1);
                            
                            // if it is , then we combine the two
                            if(prev->get_name() == "hole"){
                                
                                i->set_space(prev->get_space() + i->get_space());
                                i->set_starting_address(prev->get_start());
                                memory_list.erase(prev);
                            }
                            
                        }
                        
                        if(i != std::prev(memory_list.end())){ // If e aren't at the end
                               
                            // Check if the next element is a hole
                            auto next = std::next(i, 1);
                           
                             // if it is , then we combine the two
                            if(next->get_name() == "hole"){

                                i->set_space(next->get_space() + i->get_space());
                                memory_list.erase(next);
                            }

                        }
                        
                        /* After we unload the process and combine adjacent holes , we exit the loop */
                       processes.pop();
                        break;
                    }
                }
                
            }
            print_list(memory_list);
        }
    }

