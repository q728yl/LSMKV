//
// Created by lenovo on 2023/4/6.
//

#ifndef LSMKV_SSTABLE_H
#define LSMKV_SSTABLE_H
#include <string>
#include "cache.h"
#include "MurmurHash3.h"


class SSTable {
public:
    Cache *cache_;
    char *value_;
    explicit SSTable(Cache *cache = nullptr,
                     char *value = nullptr)
            : cache_(cache), value_(value) {}
    ~SSTable() { delete value_; }

    std::string getValue(uint64_t index){
        uint64_t size = cache_size();
        uint64_t begin = (cache_->offset_array())[index] - (32 + 10240 + 12 * (size + 1));
        uint64_t end = (cache_->offset_array())[index + 1] - (32 + 10240 + 12 * (size + 1));
        return std::string(value_ + begin, end - begin);
    }
    Cache *cache() { return cache_; }
    uint64_t cache_size() { return cache_->getSize(); }
    uint64_t getKey(int index) { return (cache_->key_array())[index]; }
    uint64_t getTime() { return (cache_->header()).time_flag_; }
    uint64_t getMin() { return cache_->getMin(); }
    uint64_t getMax() { return cache_->getMax(); }
    char *getValueArray() { return value_; }
};

#endif //LSMKV_SSTABLE_H
