#pragma once

#include "../entry/entry.h"
template <typename T> struct QNode;
template <typename T> using QNodePosi = QNode<T>*; //跳转表节点位置
template <typename T> struct QNode { //四联节点
   T entry; //所存词条
   QNodePosi<T> pred, succ, above, below; //前驱、后继、上邻、下邻
   QNode( T e = T(), QNodePosi<T> p = NULL, QNodePosi<T> s = NULL,
          QNodePosi<T> a = NULL, QNodePosi<T> b = NULL ) //构造器
      : entry( e ), pred( p ), succ( s ), above( a ), below( b ) {}
   QNodePosi<T> insert( T const& e, QNodePosi<T> b = NULL ); //将e作为当前节点的后继、b的上邻插入
};

template <typename T> QNodePosi<T> //将e作为当前节点的后继、b的上邻插入Quadlist
QNode<T>::insert( T const& e, QNodePosi<T> b ) {
   QNodePosi<T> x = new QNode<T>( e, this, succ, NULL, b ); //创建新节点
   succ->pred = x; succ = x; //设置水平逆向链接
   if ( b ) b->above = x; //设置垂直逆向链接
   return x; //返回新节点的位置
}
