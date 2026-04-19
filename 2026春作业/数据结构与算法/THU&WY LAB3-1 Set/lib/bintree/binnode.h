
#pragma once

/******************************************************************************************
 * BinNode状态与性质的判断
 ******************************************************************************************/
#define IsLChild(x) ( ((x)->parent) && ((x) == (x)->parent->lc) )
#define IsLeaf(x) ( (!(x)->lc) && (!(x)->rc) )

/******************************************************************************************
 * 与BinNode具有特定关系的节点及指针
 ******************************************************************************************/
#define sibling( p ) ( ((p) == (p)->parent->lc) ? (p)->parent->rc : (p)->parent->lc ) /*兄弟*/
#define uncle( x ) ( sibling( (x)->parent ) ) /*叔叔*/
#define FromParentTo( x ) /*来自父亲的引用*/ \
   ( (x)->parent ? ( ((x) == (x)->parent->lc) ? (x)->parent->lc : (x)->parent->rc ) : _root )
#define max(x, y) ((x) < (y) ? (y) : (x))

using Rank = unsigned int;

#define stature( p ) ( (int)( ( p ) ? ( p )->height : -1 ) ) //外部节点高度为-1，以上递推

typedef enum { RB_RED, RB_BLACK} RBColor; //节点颜色

template <typename T> struct BinNode;
template <typename T> using BinNodePosi = BinNode<T>*; //节点位置

template <typename T> struct BinNode { //二叉树节点模板类
// 成员（为简化描述起见统一开放，读者可根据需要进一步封装）
   T data; //数值
   BinNodePosi<T> parent, lc, rc; //父节点及左、右孩子
   Rank height; //高度（通用）
   Rank npl; //Null Path Length（左式堆，也可直接用height代替）
   RBColor color; //颜色（红黑树）
// 构造方法
   BinNode() : parent( NULL ), lc( NULL ), rc( NULL ), height( 0 ), npl( 1 ), color( RB_RED ) {}
   BinNode( T e, BinNodePosi<T> p = NULL, BinNodePosi<T> lc = NULL,
            BinNodePosi<T> rc = NULL, int h = 0, int l = 1, RBColor c = RB_RED )
      : data( e ), parent( p ), lc( lc ), rc( rc ), height( h ), npl( l ), color( c )
      { if (lc) lc->parent = this; if (rc) rc->parent = this; }
// 操作接口
   Rank size(); //统计当前节点后代总数，亦即以其为根的子树的规模
   Rank updateHeight(); //更新当前节点高度
   void updateHeightAbove(); //更新当前节点及其祖先的高度
   BinNodePosi<T> insertLc( T const& ); //插入左孩子
   BinNodePosi<T> insertRc( T const& ); //插入右孩子
   void attachLc( BinNodePosi<T> ); //接入左子树
   void attachRc( BinNodePosi<T> ); //接入右子树
   BinNodePosi<T> succ(); //取当前节点的直接后继
// 比较器、判等器（各列其一，其余自行补充）
   bool operator< ( BinNode const& bn ) { return data < bn.data; } //小于
   bool operator== ( BinNode const& bn ) { return data == bn.data; } //等于
};

template <typename T> Rank BinNode<T>::updateHeight() //更新当前节点高度
   { return height = 1 + max( stature( lc ), stature( rc ) ); } //具体规则，因树而异

template <typename T> void BinNode<T>::updateHeightAbove() //更新当前节点及其祖先的高度
   { for ( BinNodePosi<T> x = this; x; x = x->parent ) x->updateHeight(); } //可优化

template <typename T> BinNodePosi<T> BinNode<T>::succ() { //定位节点v的直接后继
   BinNodePosi<T> s = this; //记录后继的临时变量
   if ( rc ) { //若有右孩子，则直接后继必在右子树中，具体地就是
      s = rc; //右子树中
      while ( s->lc ) s = s->lc; //最靠左（最小）的节点
   } else { //否则，直接后继应是“将当前节点包含于其左子树中的最低祖先”，具体地就是
      while ( (s->parent) && (s == s->parent->rc) ) s = s->parent; //逆向地沿右向分支，不断朝左上方移动
      s = s->parent; //最后再朝右上方移动一步，即抵达直接后继（如果存在）
   }
   return s;
}

template <typename T> Rank BinNode<T>::size() { //统计当前节点后代总数，即以其为根的子树规模
   Rank s = 1; //计入本身
   if ( lc ) s += lc->size(); //递归计入左子树规模
   if ( rc ) s += rc->size(); //递归计入右子树规模
   return s;
}

template <typename T> BinNodePosi<T> BinNode<T>::insertLc( T const& e )
   { return lc = new BinNode<T>( e, this ); } //将e作为当前节点的左孩子插入二叉树

template <typename T> BinNodePosi<T> BinNode<T>::insertRc( T const& e )
   { return rc = new BinNode<T>( e, this ); } //将e作为当前节点的右孩子插入二叉树

template <typename T> void BinNode<T>::attachLc( BinNodePosi<T> x )
   { lc = x; if ( x ) x->parent = this; } //接入左子树

template <typename T> void BinNode<T>::attachRc( BinNodePosi<T> x )
   { rc = x; if ( x ) x->parent = this ; } //接入右子树
