
#pragma once

#include "../bintree/bintree.h" //引入BinTree

template <typename T> class BST : public BinTree<T> { //由BinTree派生BST模板类
protected:
   BinNodePosi<T> _hot; //“命中”节点的父亲
   BinNodePosi<T> connect34 ( //按照“3 + 4”结构，联接3个节点及四棵子树
      BinNodePosi<T>, BinNodePosi<T>, BinNodePosi<T>,
      BinNodePosi<T>, BinNodePosi<T>, BinNodePosi<T>, BinNodePosi<T> );
   BinNodePosi<T> rotateAt ( BinNodePosi<T> x ); //对x及其父亲、祖父做统一旋转调整
public: //基本接口：以virtual修饰，强制要求所有派生类（BST变种）根据各自的规则对其重写
   virtual BinNodePosi<T> & search ( const T& e ); //查找
   virtual BinNodePosi<T> insert ( const T& e ); //插入
   virtual bool remove ( const T& e ); //删除

   BinNodePosi<T> lower_bound(T);
};

template <typename T> BinNodePosi<T> BST<T>::insert( const T& e ) { //将关键码e插入BST树中
   BinNodePosi<T>& x = search( e ); //通过查找
   if ( x ) return x; //确认目标不存在，并设置_hot
   x = new BinNode<T>( e, _hot ); //在x处创建新节点，以_hot为父
   BinTree<T>::_size++; x->updateHeightAbove(); //更新全树规模，以及历代祖先的高度
   return x; //新插入的节点，必为叶子
} //无论e是否存在于原树中，返回时总有x->data == e

/******************************************************************************************
 * BST节点删除算法：删除位置x所指的节点（全局静态模板函数，适用于AVL、Splay、RedBlack等各种BST）
 * 目标x在此前经查找定位，并确认非NULL，故必删除成功；与searchIn不同，调用之前不必将hot置空
 * 返回值指向实际被删除节点的接替者，hot指向实际被删除节点的父亲——二者均有可能是NULL
 ******************************************************************************************/
template <typename T>
static BinNodePosi<T> removeAt( BinNodePosi<T>& x, BinNodePosi<T>& hot ) {
   BinNodePosi<T> w = x; //实际被摘除的节点，初值同x
   BinNodePosi<T> succ = NULL; //实际被删除节点的接替者
   if ( !x->lc ) //若x的左子树为空，则可
      succ = x = x->rc; //直接将x替换为其右子树
   else if ( !x->rc ) //若右子树为空，则可
      succ = x = x->lc; //对称地处理——注意：此时succ != NULL
   else { //若左右子树均存在，则选择x的直接后继作为实际被摘除节点，为此需要
      w = w->succ(); //（在右子树中）找到x的直接后继w
      T t = x->data; x->data = w->data; w->data = t;
      // swap(x->data, w->data ); //交换x和w的数据元素
      BinNodePosi<T> u = w->parent;
      ( ( u == x ) ? u->rc : u->lc ) = succ = w->rc; //隔离节点w
   }
   hot = w->parent; //记录实际被删除节点的父亲
   if ( succ ) succ->parent = hot; //并将被删除节点的接替者与hot相联
   delete w; return succ; //释放被摘除节点，返回接替者
}

template <typename T> bool BST<T>::remove( const T& e ) { //从BST树中删除关键码e
   BinNodePosi<T>& x = search( e ); if ( !x ) return false; //确认目标存在（留意_hot的设置）
   removeAt( x, _hot ); //实施删除
   BinTree<T>::_size--; _hot->updateHeightAbove(); //更新全树高度，以及历代祖先的高度
   return true;
} //删除成功与否，由返回值指示

template <typename T> BinNodePosi<T>& BST<T>::search( const T& e ) { //在BST中查找关键码e
   if ( !BinTree<T>::_root || e == BinTree<T>::_root->data ) { _hot = NULL; return BinTree<T>::_root; } //空树，或恰在树根命中
   for ( _hot = BinTree<T>::_root;; ) { //否则，自顶而下
      BinNodePosi<T>& v = ( e < _hot->data ) ? _hot->lc : _hot->rc; //确定方向，深入一层
      if ( !v || e == v->data ) return v; _hot = v; //一旦命中或抵达叶子，随即返回
   } //返回目标节点位置的引用，以便后续插入、删除操作
} //无论命中或失败，_hot均指向v之父亲（v是根时，hot为NULL）

template <typename T> 
BinNodePosi<T> BST<T>::lower_bound(T x) {
   BinNodePosi<T> v = BinTree<T>::_root;
   BinNodePosi<T> candidate = NULL;
   while (v) {
      if (v->data >= x) { candidate = v; v = v->lc; }
      else v = v->rc;
   }
   return candidate;
}
