//
// Created by lenovo on 2023/4/6.
//

#ifndef LSMKV_CONNECTER_H
#define LSMKV_CONNECTER_H

#include <string>
#include <utility>

struct connecter {
    uint64_t key; std::string value; //关键码、数值
    explicit connecter (uint64_t k = 0, std::string v = std::string() ) : key (k ), value (std::move(v )) {}; //默认构造函数
    connecter (connecter const &e ) : key (e.key ), value (e.value ) {}; //基于克隆的构造函数
    bool operator< (connecter const &e ) const { return key < e.key; }  //比较器：小于
    bool operator> (connecter const &e ) const { return key > e.key; }  //比较器：大于
    bool operator== (connecter const &e ) const { return key == e.key; } //判等器：等于
    bool operator!= (connecter const &e ) const { return key != e.key; } //判等器：不等于
}; //得益于比较器和判等器，从此往后，不必严格区分词条及其对应的关键码

#endif //LSMKV_CONNECTER_H
