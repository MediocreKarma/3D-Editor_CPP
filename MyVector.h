#ifndef MYVECTOR_H
#define MYVECTOR_H

#include <stdlib.h>
#include <cstddef>
using namespace std;

template<class T>
class MyVector
{
public:
    MyVector() {
        v_size = 0;
        capacity = 0;
        begin_index = 0;
        end_index = 0; //defapt e cu 1 mai mult. decat sa lucrez in int, las in size_t
    }

    void reserve(const size_t& reqSize) {
        if (v_size == 0) {
            T* vecNou = (T*) malloc(v_size * sizeof(T));
            if (vecNou != NULL) {
                this->vec = vecNou;
            }
            else {
                //teoretic aici ar putea fi o eroare? daca gen nu merge malloc parca
                //si gen returneaza null parca
                //dar deocamdata vom pretinde ca n are cum
            }
        }
        else if (capacity < reqSize) {
            T* vecNou;
            vecNou = (T*)realloc(this->vec, v_size * sizeof(T));
            if (vecNou != NULL) {
                this->vec = vecNou;
            }
            else {
                //si aici ar putea fi o eroare
            }
            capacity = reqSize;
        }
    }
    
    size_t size() const {
        return v_size;
    }

    void resize(const size_t& size) {
        T* vecNou;
        if (size > v_size) {
            vecNou = (T*)realloc(this->vec, 2 * v_size * sizeof(T));
            capacity = 2 * v_size;
        }
        else {
            vecNou = (T*)realloc(this->vec, v_size * sizeof(T));
            capacity = size;
        }
        if (vecNou != NULL) {
            this->vec = vecNou;
        }
        else {
            //sfanta eroare
        }
    }

    void push_back(const T& element) {
        this->v_size ++;
        
        reserve(v_size);
        *(this->vec + this->end_index) = element;
        end_index++;
    }

    void pop_back() {
        if (v_size > 0) {
            this->v_size--;
            end_index--;
            resize(v_size);
        }
    }

    T& operator [] (const size_t& index) {
        return this->vec[index];
    }

    const T& operator [] (const size_t& index) const {
        return this->vec[index];
    }

    T& front() {
        return this->vec[begin_index];
    }
    T& back() {
        
        if (end_index > 0) {
            return this->vec[end_index - 1];
        }
        else {
            //nush ce returnez daca i invalid apelu
            //vedem
        }
    }

    //initializare cu alt vector
    MyVector(const MyVector<T>& other) {
        for (size_t i = 0; i < other.size(); ++i) {
            this->push_back(other[i]);
        }
    }

    //initializare egal
    MyVector<T>& operator = (const MyVector<T>& other) {
        if (this == &other) {
            return *this;
        }
        for (size_t i = 0; i < other.size(); ++i) {
            this->push_back(other[i]);
        }
        return *this;
    }

private:
    T* vec;
    size_t v_size, capacity;
    size_t begin_index, end_index;
};

#endif // MYVECTOR_H

