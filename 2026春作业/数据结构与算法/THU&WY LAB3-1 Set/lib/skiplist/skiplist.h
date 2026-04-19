#pragma once

#include "../list/list.h" //引入列表
#include "../entry/entry.h" //引入词条
#include "quadlist.h" //引入Quadlist
#include "../dictionary/dictionary.h" //引入词典

template <typename K, typename V> //key、value
//符合Dictionary接口的Skiplist模板类（隐含假设元素之间可比较大小）
struct Skiplist : public Dictionary<K, V>, public List< Quadlist< Entry<K, V> >* > {
   Skiplist() { this->insertFirst( new Quadlist< Entry<K, V> > ); }; //即便为空，也有一层空列表
   QNodePosi< Entry<K, V> > search( K ); //由关键码查询词条
   QNodePosi< Entry<K, V> > lower_bound( K ); //由关键码查询词条
   Rank size() const { return this->empty() ? 0 : this->last()->data->_size; } //词条总数
   Rank height() { return List< Quadlist< Entry<K, V> >* >::size(); } //层高 == #Quadlist
   V* get( K ); //读取
   bool put(K, V); //插入（Skiplist允许词条相等，故必然成功）
   bool remove ( K ); //删除
};

template <typename K, typename V> V* Skiplist<K, V>::get( K k ) { //跳转表词条查找算法
   QNode< Entry<K, V> >* p = search(k); //无论是否命中，search()都不会返回NULL
   return (p->pred && p->entry.key == k) ? &(p->entry.value) : NULL; //故须再做判断
} //有多个命中时靠后者优先

template <typename K, typename V> bool Skiplist<K, V>::put( K k, V v ) { //跳转表词条插入算法
   Entry<K, V> e = Entry<K, V>( k, v ); //待插入的词条（将被同一塔中所有节点共用）
   QNodePosi< Entry<K, V> > p = search( k ); //查找插入位置：新塔将紧邻其右，逐层生长
   ListNodePosi< Quadlist< Entry<K, V> >* > qlist = this->last(); //首先在最底层
   QNodePosi< Entry<K, V> > b = qlist->data->insert( e, p ); //创建新塔的基座
   while ( rand() & 1 ) { //经投掷硬币，若确定新塔需要再长高，则
      while ( p->pred && !p->above ) p = p->pred; //找出不低于此高度的最近前驱
      if ( !p->pred && !p->above ) { //若该前驱是head，且已是最顶层，则
         this->insertFirst( new Quadlist< Entry<K, V> > ); //需要创建新的一层
         this->first()->data->head->below = qlist->data->head;
         qlist->data->head->above = this->first()->data->head;
      }
      p = p->above; qlist = qlist->pred; //上升一层，并在该层
      b = qlist->data->insert( e, p, b ); //将新节点插入p之后、b之上
   } //课后：调整随机参数，观察总体层高的相应变化
   return true; //Dictionary允许元素相等，插入必成功
} //体会：得益于哨兵的设置，哪些环节被简化了？

template <typename K, typename V> bool Skiplist<K, V>::remove( K k ) { //跳转表词条删除算法
   QNodePosi< Entry<K, V> > p = search ( k ); //查找目标词条
   if ( !p->pred || (k != p->entry.key) )  return false; //若不存在，直接返回
   ListNodePosi< Quadlist< Entry<K, V> >* > qlist = this->last(); //从底层Quadlist开始
   while ( p->above ) { qlist = qlist->pred; p = p->above; } //升至塔顶
   do { //逐层拆塔
      QNodePosi< Entry<K, V> > lower = p->below; //记住下一层节点，并
      qlist->data->remove( p ); //删除当前层节点，再
      p = lower; qlist = qlist->succ; //转入下一层
   } while ( qlist->succ ); //直到塔基
   while ( (1 < this->height()) && (this->first()->data->_size < 1) ) { //逐层清除
      List< Quadlist< Entry<K, V> >* >::remove( this->first() ); 
      this->first()->data->head->above = NULL;
   } //已不含词条的Quadlist（至少保留最底层空表）
   return true; //删除成功
} //体会：得益于哨兵的设置，哪些环节被简化了？

/******************************************************************************************
 * Skiplist词条查找算法（供内部调用）
 * 返回关键码不大于k的最后一个词条（所对应塔的基座）
 ******************************************************************************************/
template <typename K, typename V> QNodePosi<Entry<K, V>> Skiplist<K, V>::search( K k ) {
   for ( QNodePosi<Entry<K, V>> p = this->first()->data->head; ; ) //从顶层Quadlist的首节点p出发
      if ( ( p->succ->succ ) && ( p->succ->entry.key <= k ) ) p = p->succ; //尽可能右移
      else if ( p->below ) p = p->below; //水平越界时，下移
      else return p; //验证：此时的p符合输出约定（可能是最底层列表的head）
} //体会：得益于哨兵的设置，哪些环节被简化了？

template <typename K, typename V> QNodePosi<Entry<K, V>> Skiplist<K, V>::lower_bound( K k ) {
   QNodePosi<Entry<K, V>> p = search(k);
   if ( p->pred && p->entry.key == k) return p;
   if ( p->succ->succ ) return p->succ;
   return NULL;
}
