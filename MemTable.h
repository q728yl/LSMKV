//
// Created by lenovo on 2023/4/6.
//

#ifndef LSMKV_MEMTABLE_H
#define LSMKV_MEMTABLE_H

#include "SkipList.h"
#include "MemTable.h"
#include "MurmurHash3.h"
#include <cstring>

#define DebugCap 36

class MemTable {
private:
    SkipList skip_list_;
    bool *bloom_filter_;
    int capacity_;

public:
    MemTable() {
        capacity_ = 2086868;
        bloom_filter_ = new bool[10240]();

#ifdef DEBUG
        capacity = DebugCap;
#endif
    }
    ~MemTable(){}
    bool addEntry(uint64_t key, const std::string &value) {
        std::string *s;
        if ((s = skip_list_.get(key)) != nullptr) {
            if ((capacity_ = capacity_ + (int) (s->length()) - (int) value.length()) < 0) return false;
        } else {
            if ((capacity_ = capacity_ - (int) value.length() - 12) < 0) return false;
            uint32_t hash[4];
            MurmurHash3_x64_128(&key, sizeof(key), 1, hash);
            for (unsigned int i : hash) {
                bloom_filter_[i % 10240] = true;
            }
        }
        skip_list_.put(key, value);
        return true;
    }

    bool *getBloom() {
        return bloom_filter_;
    }
    uint64_t getMax() {
        return skip_list_.getMax();
    }
    uint64_t getMin() {
        return skip_list_.getMin();
    }
//    uint32_t getMin() {
//        return skip_list_.getMin();
//    }
    std::string *search(uint64_t key) {
        std::string *result = skip_list_.get(key);
        return result;
    }
    connecter **getWhole() {
        return skip_list_.getWhole();
    }


    uint32_t getSize() {
        return skip_list_.size();
    }
    void reset() {
        skip_list_.clear();
        bloom_filter_ = new bool[10240]();
        capacity_ = 2086868;
#ifdef DEBUG
        capacity = DebugCap;
#endif
    }
    bool empty() {
        return skip_list_.empty();
    }
};

#endif //LSMKV_MEMTABLE_H
