//
// Created by lenovo on 2023/4/6.
//

#ifndef LSMKV_QUADLIST_H
#define LSMKV_QUADLIST_H

#include "QuadListNode.h"
#include "QuadList.h"
class QuadList {
    //四连表
private:
    int size_;
    QListNodePosi header_, trailer_;//规模、头哨兵、尾哨兵
protected:
    //QuadList创建时的初始化
    void init(){
        //QuadList初始化，创建QuadList对象时统一调用
            header_ = new QuadListNode; //创建头哨兵节点
            trailer_ = new QuadListNode;  //创建尾哨兵节点
            header_->succ = trailer_;
            header_->pred = nullptr;  //沿横向联接哨兵
            trailer_->pred = header_;
            trailer_->succ = nullptr;                  //沿横向联接哨兵
            header_->above = trailer_->above = nullptr;//纵向的后继置空
            header_->below = trailer_->below = nullptr;//纵向的前驱置空
            size_ = 0;                                 //记录规模
        } //如此构造的四联表，不含任何实质的节点，且暂时与其它四联表相互独立

    //清除所有节点
    int clear() {
        int oldSize = size_;
        while (0 < size_) remove(header_->succ);//逐个删除所有节点
        return oldSize;
    }
public:
    // 构造函数
    QuadList() { init(); }//默认

    ~QuadList() {// 析构函数
        clear();  //删除所有节点，释放哨兵
        delete header_;
        delete trailer_;
    }
    // 只读访问接口
    int size() const { return size_; }                  //规模
    bool empty() const { return size_ <= 0; }           //判空
    QListNodePosi first() const { return header_->succ; }//首节点位置
    QListNodePosi last() const { return trailer_->pred; }//末节点位置
    bool valid(QListNodePosi p)                         //判断位置p是否对外合法
    { return p && (trailer_ != p) && (header_ != p); }
    // 可写访问接口
    connecter remove(QListNodePosi p) {
        p->pred->succ = p->succ;
        p->succ->pred = p->pred;
        size_--;
        connecter p_val = p->entry;
        delete p;
        return p_val;
    }                                                            //删除（合法）位置p处的节点，返回被删除节点的数值
    //将*e作为p的后继、b的上邻插入
    QListNodePosi insertAfterAbove(const connecter &e, QListNodePosi p, QListNodePosi b = nullptr) {
        size_++;
        return p->insertAsSuccAbove(e, b);
    }
    connecter **getWhole() {
        connecter **content = new connecter *[size_];
        QListNodePosi p = header_->succ;
        for (int i = 0; p != trailer_; i++, p = p->succ) {
            content[i] = &(p->entry);
        }
        return content;
    }
};//QuadList

#endif //LSMKV_QUADLIST_H
