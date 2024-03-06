//
// Created by lenovo on 2023/4/6.
//

#ifndef LSMKV_CACHE_H
#define LSMKV_CACHE_H

#include <cstdint>
#include <string>
#include "MurmurHash3.h"

class Header {
public:
    uint64_t time_flag_;
    uint64_t size_;
    uint64_t max;
    uint64_t min;
    explicit Header(uint64_t time_flag = 0, uint64_t size = 0,
                    uint64_t max = 0, uint64_t min = 0)
            : time_flag_(time_flag), size_(size), max(max), min(min) {}
};


#define DebugCap 36

class Cache {
public:
    Cache()
            : bloom_filter_(new bool[10240]), key_array_(nullptr),
              offset_array_(nullptr), file_name_(-1) {
        capacity_ = 2086868;
#ifdef DEBUG
        capacity = DebugCap;
#endif
    }

    Cache(Header headerTmp, bool *bloomTmp, uint64_t *KeyTmp, uint32_t *offTmp)
            : header_(headerTmp),
              bloom_filter_(bloomTmp),
              key_array_(KeyTmp),
              offset_array_(offTmp),
              file_name_(-1) {}

    ~Cache() {
        delete key_array_;
        delete offset_array_;
        delete []bloom_filter_;
    }

    void set_header(Header headTmp) { header_ = headTmp; }
    Header header() { return header_; }

    void setBloom(bool *bloomTmp) { bloom_filter_ = bloomTmp; }
    bool *bloom_filter() { return bloom_filter_; }

    void set_key_array(uint64_t *keyArrayTmp) { key_array_ = keyArrayTmp; }
    uint64_t *key_array() { return key_array_; }

    void set_offset_array(uint32_t *offsetArrayTmp) { offset_array_ = offsetArrayTmp; }
    uint32_t *offset_array() { return offset_array_; }

    bool ifExist(uint64_t key){
        if (key < header_.min || key > header_.max) return false;

        uint32_t hash[4];
        MurmurHash3_x64_128(&key, sizeof(key), 1, hash);
        for (unsigned int i : hash) {
            if (!bloom_filter_[i % 10240]) return false;
        }
        return true;
    }
    uint64_t binSearch(uint64_t key,uint32_t &length){
        uint64_t left = 0, right = header_.size_;
        while (left < right) {
            uint64_t mid = left + (right - left) / 2;
            if (key_array_[mid] == key) {
                length = offset_array_[mid + 1] - offset_array_[mid];
                return offset_array_[mid];
            } else if (key_array_[mid] > key) {
                right = mid;
            } else {
                left = mid + 1;
            }
        }
        length = 0;
        return 0;
    } // return 0 if not find length=string.length()
    uint64_t getTime() const { return header_.time_flag_; }
    uint64_t getMax() const { return header_.max; }
    uint64_t getMin() const { return header_.min; }
    uint64_t getSize() const { return header_.size_; }

    void set_file_name(int filename) { file_name_ = filename; }
    int file_name() const { return file_name_; }
    bool addEntry(uint64_t key, const std::string &value) {
        if ((capacity_ = capacity_ - (int) value.size() - 12) <= 0) return false;
        uint32_t hash[4];
        MurmurHash3_x64_128(&key, sizeof(key), 1, hash);
        for (unsigned int i: hash) {
            bloom_filter_[i % 10240] = true;
        }
        return true;
    }
private:
    /* Header */
    Header header_;
    /* bloom */
    bool *bloom_filter_;
    /* key array */
    uint64_t *key_array_;
    /* offset array */
    uint32_t *offset_array_;

    int file_name_;
    int capacity_{};
};
#endif //LSMKV_CACHE_H
