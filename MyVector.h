#ifndef MYVECTOR_H
#define MYVECTOR_H
#include <stdlib.h>
#include <cstddef>
#include<iostream>
#include<initializer_list>

template<class T>
class MyVector
{
public:
    MyVector() {
        m_vec = (T*)malloc(2 * sizeof(T));
        m_size = 0;
        m_capacity = 2;
    }

    void reserve(const size_t& reqSize) {
        //nu mai face mereu reserve, doar cand e efectiv nevoie. 
        if (m_capacity < reqSize) {
            T* vecNou;
            vecNou = (T*)realloc(m_vec, m_size * sizeof(T));
            if (vecNou != NULL) {
                this->m_vec = vecNou;
            }
            else {
                throw std::bad_alloc();
            }
            m_capacity = reqSize;
        }
    }

    size_t size() const {
        return m_size;
    }

    void push_back(const T& element) {
        m_size++;
        if (m_size > m_capacity) {
            reserve(m_size * sizeof(T));
        }
        *(m_vec + m_size - 1) = element;
    }
    void pop_back() {
        if (m_size > 0) {
            m_size--;
            this->m_vec[m_size] = T();
        }
        else {
            //eroare
        }
    }

    T& operator [] (const size_t& index) {
        if (index < m_size) {
            return this->m_vec[index];
        }
        else {
            throw std::invalid_argument("received bad value");
        }
    }
    const T& operator [] (const size_t& index) const {
        return this->m_vec[index];
    }
    T& front() {
        return this->m_vec[0];
    }
    T& back() {

        if (m_size > 0) {
            return this->m_vec[m_size - 1];
        }
        else {
            throw std::invalid_argument("received bad value");
        }
    }
    //initializare cu alt vector; constructor overload
    MyVector(const MyVector<T>& other) {
        for (size_t i = 0; i < other.size(); ++i) {
            this->push_back(other[i]); //e mai eficient, vezi reserve
        }
    }
    //initializare prin override egal
    MyVector<T>& operator = (const MyVector<T>& other) {
        if (this == &other) {
            return *this;
        }
        for (size_t i = 0; i < other.size(); ++i) {
            if (i > m_capacity) {

            }
            else {
                this->m_vec[i] = other[i];
            }
        }
        return *this;
    }
    //init acolade
    MyVector<T>& operator = (const std::initializer_list<T>& acol) {
        reserve(acol.size());
        const T* el = acol.begin();
        for (size_t i = 0; i < acol.size(); i++) {
            this->m_vec[i] = *(el++);
        }
        for (size_t i = acol.size(); i < size(); i++) {
            this->m_vec[i] = T();
        }
    }

private:
    T* m_vec;
    size_t m_size, m_capacity;
};

#endif // MYVECTOR_H

