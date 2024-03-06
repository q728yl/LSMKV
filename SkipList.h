//
// Created by lenovo on 2023/4/6.
//

#ifndef LSMKV_SKIPLIST_H
#define LSMKV_SKIPLIST_H

#include "QuadList.h"
#include "connecter.h"
#include "QuadListNode.h"

#include <list>
#include <cstdint>
#include <string>

//符合Dictionary接口的SkipList模板类（但隐含假设元素之间可比较大小）
class SkipList {
protected:
    bool skipSearch(std::list<QuadList *>::iterator &qlist,//从指定层qlist的
                    QuadListNode *&p,                      //首节点p出发 注意首节点和头结点的差别 首节点是 header->succ
                    uint64_t k) {                          //向右、向下查找目标关键码k
        while (true) {                                                 //在每一层
            while (p->succ && (p->entry.key <= k))                       //从前向后查找 p->succ 防止是尾结点 尾结点的的entry使用 T()初始化。
                p = p->succ;                                               //直到出现更大的key或溢出至trailer
            p = p->pred;                                                 //此时倒回一步，即可判断是否
            if (p->pred && (k == p->entry.key)) return true;             //命中
            qlist++;                                                     //否则转入下一层
            if (qlist == skip_list_.end()) return false;                 //若已到穿透底层，则意味着失败
            p = (p->pred) ? p->below : (*qlist)->first();                //否则转至当前塔的下一节点 若p是头节点 则p是没有below的因为在Qlist的构造函数中，将Qlist的头尾节点的above和below全部置为null
        }
    }

public:
    bool empty() const { return skip_list_.empty(); }
    uint32_t size() const { return skip_list_.empty() ? 0 : skip_list_.back()->size(); }//底层QuadList的规模
    uint64_t getMax() const { return skip_list_.empty() ? 0 : skip_list_.back()->last()->entry.key; }
    uint64_t getMin() const { return skip_list_.empty() ? 0 : skip_list_.back()->first()->entry.key; }
    void put(uint64_t k, const std::string &v) {
        connecter e = connecter(k, v);
        if (empty())
            skip_list_.push_back(new QuadList);

        auto qlist = skip_list_.begin();
        QListNodePosi p = (*qlist)->first();
        if (skipSearch(qlist, p, k))
        {
            //进行替换
            do {
                p->entry.value = v;
            } while (p = p->below);
        } else {
            qlist = skip_list_.end();
            qlist--;

            QListNodePosi b = (*qlist)->insertAfterAbove(e, p);
            while (rand() & 1) {
                while ((*qlist)->valid(p) && !p->above) p = p->pred;
                if (!(*qlist)->valid(p)) {
                    if (qlist == skip_list_.begin())
                    {
                        QuadList *new_quadlist = new QuadList;
                        skip_list_.push_front(new_quadlist);
                    }
                    p = (*(--qlist))->first()->pred;
                } else {
                    p = p->above;
                    qlist--;
                }
                b = (*qlist)->insertAfterAbove(e, p, b);
            }
        }
    }

    std::string *get(uint64_t k) {
        if (skip_list_.empty()) return nullptr;
        auto qlist = skip_list_.begin();
        QListNodePosi p = (*qlist)->first();
        if (skipSearch(qlist, p, k))
            return &(p->entry.value);
        else
            return nullptr;
    }
    bool remove(uint64_t k, uint32_t &length) {
        if (skip_list_.empty()) return false;
        auto qlist = skip_list_.begin();
        QListNodePosi p = (*qlist)->first();
        if (skipSearch(qlist, p, k)) {
            length = p->entry.value.length();
            do {
                QListNodePosi next_p = p->below;
                (*qlist)->remove(p);
                qlist++;
                p = next_p;
            } while (qlist != skip_list_.end());
            while (!skip_list_.empty() && skip_list_.front()->empty()) {
                skip_list_.pop_front();
            }
            return true;
        }
        length = 0;
        return false;
    }
    connecter **getWhole() {
        return skip_list_.back()->getWhole();
    }
    void clear() {
        while (!skip_list_.empty()) {
            delete skip_list_.front();
            skip_list_.pop_front();
        }
    }

private:
    std::list<QuadList *> skip_list_;//双链表
};

#endif //LSMKV_SKIPLIST_H
