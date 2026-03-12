#include <cstdint>
#include <iostream>
using Rank = uint32_t;
constexpr Rank DEFAULT_SIZE = 5;

template <typename T> class Vector {
    private:
        T* _elem; Rank _capa, _size;
    protected:
        void copy(Rank r, const T* A, Rank lo, Rank hi);
        void expand(Rank d = 1);
        void shrink();
    public:
        Vector(Rank s = DEFAULT_SIZE){
            _elem = new T[_capa = s + 1]; _size = 0;
        }
        Vector(const Vector<T>& V) : Vector(V._elem, 0, V._size){}

};

template <typename T> void Vector<T>::expand(Rank d){
    if( _size + d < _capa) return;
    
}

int main(){
    

    return 0;
}