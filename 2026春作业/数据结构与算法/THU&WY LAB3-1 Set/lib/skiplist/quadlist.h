
#pragma once

#include "quadlistnode.h" //引入Quadlist节点类

template <typename T> struct Quadlist { //四联列表
   Rank _size; //规模
   QNodePosi<T> head, tail; //头哨兵、尾哨兵
   void init(); //初始化
   int clear(); //清除所有节点
   Quadlist() { this->init(); } //构造
   ~Quadlist() { clear(); delete head; delete tail; } //析构
   T remove( QNodePosi<T> p ); //删除（合法）位置p处的节点，返回被删除节点的数值
   QNodePosi<T> insert( T const& e, QNodePosi<T> p, QNodePosi<T> b = NULL ); //将e作为p的后继、b的上邻插入
   void traverse( void (* ) ( T& ) ); //遍历各节点，依次实施指定操作（函数指针，只读或局部修改）
   template <typename VST> void traverse ( VST& ); //遍历
}; //Quadlist

template <typename T> void Quadlist<T>::init() { //Quadlist初始化，创建Quadlist对象时统一调用
   head = new QNode<T>; //创建头哨兵节点
   tail = new QNode<T>; //创建尾哨兵节点
   head->succ = tail; head->pred = NULL; //沿横向联接哨兵
   tail->pred = head; tail->succ = NULL; //沿横向联接哨兵
   head->above = tail->above = NULL; //纵向的后继置空
   head->below = tail->below = NULL; //纵向的前驱置空
   _size = 0; //记录规模
} //如此构造的四联表，不含任何实质的节点，且暂时与其它四联表相互独立

template <typename T> QNodePosi<T> //将e作为p的后继、b的上邻插入Quadlist
Quadlist<T>::insert( T const& e, QNodePosi<T> p, QNodePosi<T> b )
{ _size++; return p->insert( e, b ); } //返回新节点位置（below = NULL）

template <typename T> //删除Quadlist内位置p处的节点，返回其中存放的词条
T Quadlist<T>::remove( QNodePosi<T> p ) { // assert: p为Quadlist中的合法位置
   p->pred->succ = p->succ; p->succ->pred = p->pred; _size--;//摘除节点
   T e = p->entry; delete p; //备份词条，释放节点
   return e; //返回词条
}

template <typename T> int Quadlist<T>::clear() { //清空Quadlist
   int oldSize = _size;
   while ( 0 < _size ) remove( head->succ ); //逐个删除所有节点
   return oldSize;
}

template <typename T> //遍历Quadlist，对各节点依次实施visit操作
void Quadlist<T>::traverse( void ( *visit )( T& ) ) { //利用函数指针机制，只读或局部性修改
   QNodePosi<T> p = head;
   while ( ( p = p->succ ) != tail ) visit( p->data );
}

template <typename T> template <typename VST> //遍历Quadlist，对各节点依次实施visit操作
void Quadlist<T>::traverse( VST& visit ) { //利用函数对象机制，可全局性修改
   QNodePosi<T> p = head;
   while ( ( p = p->succ ) != tail ) visit( p->data );
}
