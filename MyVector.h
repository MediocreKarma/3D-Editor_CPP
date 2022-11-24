#ifndef MYVECTOR_H
#define MYVECTOR_H

#include <stdlib.h>
#include <cstddef>
#include<stdexcept>
using namespace std;

template<class T>
class MyVector
{
public:
    MyVector() {

        m_vec = (T*)malloc(m_size * sizeof(T));
        m_size = 0;
        m_capacity = 2;
    }

    void reserve(const size_t& reqSize) {
        if (m_capacity < reqSize) {
            T* vecNou;

            if (m_capacity * 2 >= reqSize) {
                vecNou = (T*)realloc(m_vec, 2 * m_capacity * sizeof(T));

                m_capacity *= 2;
            }
            else {
                vecNou = (T*)realloc(m_vec, reqSize * sizeof(T));

                m_capacity = reqSize;
            }

            if (vecNou != NULL) {
                m_vec = vecNou;
            }
            else {
                throw std::bad_alloc();
            }

        }
    }

    size_t size() const {
        return m_size;
    }
    size_t capacity() const {
        return m_capacity;
    }

    void resize(const size_t& size) {
        T* vecNou;
        if (size > m_capacity) {
            vecNou = (T*)realloc(m_vec, size * sizeof(T));
            for (size_t i = m_size; i < size; i++) {
                m_vec[i] = T();
            }
           
        }
        else {
            vecNou = (T*)realloc(m_vec, size * sizeof(T));
        }
        m_capacity = size;
        if (vecNou != NULL) {
            this->m_vec = vecNou;
        }
        else {
            throw std::bad_alloc();
        }
    }

    void push_back(const T& element) {
        this->m_size++;
        int k = 2;
        while (m_size > m_capacity) {
            reserve(k * m_size);
            k *= 2;
        }
        *(m_vec + m_size-1) = element;
        
    }

    void pop_back() {
        if (m_size > 0) {
            m_vec[m_size-1] = T();
            m_size--;
        }
    }

    T& operator [] (const size_t& index) {
        if (index >= 0 && index < m_size) {
            return this->m_vec[index];
        }
        else {
            throw std::invalid_argument("out of bounds");
        }
    }

    const T& operator [] (const size_t& index) const {
        if (index >= 0 && index < m_size) {
            return this->m_vec[index];
        }
        else {
            throw std::invalid_argument("out of bounds");
        }
    }

    T& front() {
        return this->m_vec[0];
    }
    T& back() {

        if (m_size > 0) {
            return this->vec[m_size - 1];
        }
        else {
            throw std::invalid_argument("out of bounds");
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

    MyVector(const std::initializer_list<T>& init) {
        reserve(init.size());
        const T* ptr = init.begin();
        for (size_t i = 0; i < init.size(); i++){
            this->push_back(*(ptr));
            ptr++;
        }
    }

    MyVector<T>& operator = (const std::initializer_list<T>& init) {
        reserve(init.size());
        const T* ptr = init.begin();
        for (size_t i = 0; i < init.size(); i++) {
            this->push_back(*(ptr));
            ptr++;
        }
    }


private:
    T* m_vec;
    size_t m_size, m_capacity;
};

#endif // MYVECTOR_H

