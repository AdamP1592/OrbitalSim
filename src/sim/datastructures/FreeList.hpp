#pragma once
#include <vector>
#include <stdexcept>
#include <utility>
template <typename T>
class FreeList{
    std::vector<std::optional<T>> storage;
    std::vector<size_t> freed;
    /**
     * INTERNAL USE ONLY. Throw an error when accessing an invalid element.
     */
    void retrievalSafetyChecks(size_t index){
        if(index >= storage.size()){
            throw std::runtime_error("Index out of bounds.");
        }else if(!storage[index].has_value()){
            throw std::runtime_error("Accessing removed entry in FreeList.");
        }
    }

    public:
        /**
         * Pushes either to a freed index or the last position
         *  @param {T} entry - The thing you want to put in the storage
         *  @returns {size_t} index - The position it was placed at
         */
        size_t push(T entry){
            size_t index;
            // if there is an open index, use that
            if(!freed.empty()){
                index = freed.back();
                freed.pop_back();

                storage[index] = std::move(entry);
            }else{ //otherwise just push
                index = storage.size();
                storage.push_back(std::move(entry));
            }
            return index;
        }
        /**
         * Checks if a given index is a valid, used index. 
         * @param {size_t} index
         * @return {bool} isValid
         */
        bool isValid(size_t index){
            if(index >= storage.size()) return false;
            if(!storage[index].has_value()) return false;
            return true;
        }
        T& get(size_t index){
            retrievalSafetyChecks(index);
            return *storage[index];
        }
        void remove(size_t index){
            retrievalSafetyChecks(index);
            storage[index].reset();
            freed.push_back(index);
        }
        size_t freedSize(){
            return freed.size();
        }
        size_t size(){
            return storage.size();
        }
        size_t numEntries(){
            return size() - freedSize();
        }
    
};
