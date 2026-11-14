#pragma once
#include <vector>
#include <stdexcept>
#include <utility>
#include <optional>
template <typename T>
class FreeList{
    std::vector<std::optional<T>> storage;
    std::vector<size_t> freed;
    /**
     * INTERNAL USE ONLY. Throw an error when accessing an invalid element
     * @param {size_t} index - the index that a function wants to access
     * @param {bool} needsToExist - a bool to determine if the index needs to exist(gets used in setter)
     */
    void confirmSafeAccess(size_t index, bool needsToExist = true){
        if(index >= storage.size()){
            throw std::out_of_range("Index out of range");
        }else if(needsToExist && (!storage[index].has_value())){
            throw std::runtime_error("Accessing removed entry in FreeList.");
        }
    }
    public:
        /**
         * Replaces an existing entry with a new entry
         */
        void set(T entry, size_t index){
            // check to see if the index falls within the existing storage
            confirmSafeAccess(index);
            storage[index] = std::move(entry);
        }
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
        std::vector<size_t> push_batch(std::vector<T> batch){
            // a vector of indices of where each entry exists in storage
            std::vector<size_t> placements;
            int batchSize = batch.size();
            int freedSize = freed.size();
            int totalStorageExtension = batchSize - freedSize;
            
            // extend the freelist and freed if there aren't enough entries in freed
            // to cover the batch
            if(totalStorageExtension > 0){
                int storageSize = storage.size();
                int newStorageSize = storageSize + totalStorageExtension;

                storage.resize(newStorageSize);
                // insert remainder into freed
                freed.reserve(freed.size() + totalStorageExtension);
                for(int i = storageSize; i < newStorageSize; i++){
                    freed.push_back(i);
                }
            }
            

        }
        /**
         * Checks if a given index is a valid, used index. 
         * @param {size_t} index
         * @return {bool} isValid
         */
        bool isValid(size_t index) const{
            if(index >= storage.size()) return false;
            if(!storage[index].has_value()) return false;
            return true;
        }
        T& get(size_t index){
            confirmSafeAccess(index);
            return *storage[index];
        }
        const T& get(size_t index) const{
            return &storage[index];
        }

        void remove(size_t index){
            confirmSafeAccess(index);
            storage[index].reset();
            freed.push_back(index);
        }
        size_t freedSize() const{
            return freed.size();
        }
        size_t size() const{
            return storage.size();
        }
        size_t numEntries() const{
            return size() - freedSize();
        }
        void extend(size_t slotsNeeded){
            size_t storageSize = storage.size();
            size_t newStorageSize = storageSize + slotsNeeded;
            storage.resize(newStorageSize);
            freed.reserve(freed.size() + slotsNeeded);
            for(size_t i = storageSize; i < newStorageSize; i++){
                freed.push_back(i);
            }
        }
    
};
