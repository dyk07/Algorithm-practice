
#pragma once

#include "../bst/bst.h" //基于BST实现Splay

//由BST派生的Splay树模板类，由 USE_SINGLE 模板参数决定是否使用单旋
template <typename T, bool USE_SINGLE> class Splay : public BST<T> { 
protected:
   BinNodePosi<T> splay( BinNodePosi<T> v ); //将节点v伸展至根
public:
   BinNodePosi<T>& search( const T& e ); //查找（重写）
   BinNodePosi<T> insert( const T& e ); //插入（重写）
   bool remove( const T& e ); //删除（重写）

   BinNodePosi<T> lower_bound(T);
};

template <typename T, bool USE_SINGLE> //Splay树伸展算法
BinNodePosi<T> Splay<T, USE_SINGLE>::splay( BinNodePosi<T> v ) { //v为因最新访问而需伸展的节点
   if (USE_SINGLE) {
      // 单旋实现
      BinNodePosi<T> p, g;
      while ( ( p = v->parent ) ) {
         if ( g = p->parent ) {
            if ( p == g->lc ) g->attachLc( v );
            else              g->attachRc( v );
         } else v->parent = NULL;

         if ( v == p->lc ) { p->attachLc( v->rc ); v->attachRc( p ); }
         else              { p->attachRc( v->lc ); v->attachLc( p ); }
         p->updateHeight(); v->updateHeight();
      }
      v->parent = NULL; return v;
   }
   else {
      // 双旋实现
      BinNodePosi<T> p, g; //v的父亲与祖父
      while ( ( p = v->parent ) && ( g = p->parent ) ) { //自下而上，反复对v做双层伸展
         BinNodePosi<T> gg = g->parent; //每轮之后v都以原曾祖父（great-grand parent）为父
         switch ( ((p == g->rc) << 1) | (v == p->rc) ) { //视p、v的拐向，分四种情况
            case 0b00 : //zig-zig
               g->attachLc( p->rc ); p->attachLc( v->rc );
               p->attachRc( g );     v->attachRc( p );     break;
            case 0b01 : //zig-zag
               p->attachRc( v->lc ); g->attachLc( v->rc );
               v->attachRc( g );     v->attachLc( p );     break;
            case 0b10 : //zag-zig
               p->attachLc( v->rc ); g->attachRc( v->lc );
               v->attachLc( g );     v->attachRc( p );     break;
            default : //0b11 ~ zag-zag
               g->attachRc( p->lc ); p->attachRc( v->lc );
               p->attachLc( g );     v->attachLc( p );     break;
         }
         if ( !gg ) v->parent = NULL; //若v原先的曾祖父gg不存在，则v现在应为树根
         else //否则，gg此后应该以v作为左或右孩子
            ( g == gg->lc ) ? gg->attachLc( v ) : gg->attachRc( v );
         g->updateHeight(); p->updateHeight(); v->updateHeight();
      } //双层伸展结束时，必有g == NULL，但p可能非空
      if ( p = v->parent ) { //若p果真是根，则额外再做一次单旋
         if ( v == p->lc ) { p->attachLc( v->rc ); v->attachRc( p ); }
         else              { p->attachRc( v->lc ); v->attachLc( p ); }
         p->updateHeight(); v->updateHeight();
      }
      v->parent = NULL; return v;
   }
} //调整之后新树根应为被伸展的节点，故返回该节点的位置以便上层函数更新树根


template <typename T, bool USE_SINGLE> BinNodePosi<T>& Splay<T, USE_SINGLE>::search( const T& e ) { //在伸展树中查找e
   BinNodePosi<T> p = BST<T>::search( e ); //按BST标准算法查找
   BinTree<T>::_root = p ? splay(p) : BST<T>::_hot ? splay(BST<T>::_hot) : NULL;  //无论成功、失败、树空，被访问的节点将伸展至根
   return BinTree<T>::_root;
} //与其它BST不同，无论如何，_root都指向最后被访问的节点

template <typename T, bool USE_SINGLE> BinNodePosi<T> Splay<T, USE_SINGLE>::insert( const T& e ) { //将关键码e插入伸展树中
   if ( !BinTree<T>::_root ) { BinTree<T>::_size = 1; return BinTree<T>::_root = new BinNode<T>( e ); } //原树为空
   BinNodePosi<T> t = search( e );
   if ( e == t->data ) return t; //目标节点t若存在，伸展至根
   if ( t->data < e ) //在右侧嫁接
      { t->parent = BinTree<T>::_root = new BinNode<T>( e, NULL, t, t->rc ); t->rc = NULL; }
   else //在左侧嫁接
      { t->parent = BinTree<T>::_root = new BinNode<T>( e, NULL, t->lc, t ); t->lc = NULL; }
   BinTree<T>::_size++; t->updateHeightAbove(); return BinTree<T>::_root; //更新规模及高度，报告插入成功
} //无论e是否存在于原树中，返回时总有_root->data == e


template <typename T, bool USE_SINGLE> bool Splay<T, USE_SINGLE>::remove( const T& e ) { //从伸展树中删除关键码e
   if ( !BinTree<T>::_root || ( e != search( e )->data ) ) return false; //若目标存在，则伸展至根
   BinNodePosi<T> L = BinTree<T>::_root->lc, R = BinTree<T>::_root->rc; delete BinTree<T>::_root; //记下左、右子树L、R后，释放之
   if ( !R ) { //若R空，则
      if ( L ) L->parent = NULL; BinTree<T>::_root = L; //L即是余树
   } else { //否则
      BinTree<T>::_root = R; R->parent = NULL; search( e ); //查找必败，但R的最小节点必伸展至根，且无左孩子
      BinTree<T>::_root->attachLc( L ); //故可令其以L作为左子树
   }
   if ( --BinTree<T>::_size ) BinTree<T>::_root->updateHeight(); return true; //更新规模及树高，报告删除成功
}

template <typename T, bool USE_SINGLE> 
BinNodePosi<T> Splay<T, USE_SINGLE>::lower_bound(T x) {
   BinNodePosi<T> p = BST<T>::lower_bound(x);
   if (p) {
       BinTree<T>::_root = splay(p);
   } else if (BST<T>::_hot) {
       BinTree<T>::_root = splay(BST<T>::_hot);
   }
   if (p) return p;
   return NULL;
}
