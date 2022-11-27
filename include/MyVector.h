#ifndef MYVECTOR_H
#define MYVECTOR_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <iostream>
#include <cstring>

template<class T>
class MyVector
{
    public:
        MyVector() : m_vec(nullptr), m_size(0), m_capacity(2) {
            setCapacity(m_capacity);
        }

        MyVector(const MyVector<T>& other) : m_vec(nullptr), m_size(0), m_capacity(2) {
            setCapacity(other.m_capacity);
            m_size = other.size();
            for (size_t i = 0; i < other.size(); ++i) {
                m_vec[i] = other[i];
            }
        }

        MyVector(const std::initializer_list<T>& initList) : m_vec(nullptr), m_size(0), m_capacity(2) {
            resize(initList.size());
            const T* el = initList.begin();
            for (size_t i = 0; i < initList.size(); i++) {
                m_vec[i] = *(el++);
            }
        }

        ~MyVector() {
            delete[] m_vec;
            m_vec = nullptr;
        }

        MyVector<T>& operator = (const MyVector<T>& other) {
            if (this == &other) {
                return *this;
            }
            setCapacity(other.m_capacity);
            m_size = other.m_size;
            for (size_t i = 0; i < other.size(); ++i) {
                m_vec[i] = other[i];
            }
            return *this;
        }

        MyVector<T>& operator = (const std::initializer_list<T>& initList) {
            resize(initList.size());
            const T* el = initList.begin();
            for (size_t i = 0; i < initList.size(); i++) {
                m_vec[i] = *(el++);
            }
        }

        void reserve(const size_t& reservedCapacity) {
            if (reservedCapacity < m_capacity) {
                return;
            }
            int mult = 2;
            if (!m_capacity) {
                m_capacity = 1;
            }
            while (reservedCapacity >= m_capacity * mult) {
                mult *= 2;
            }
            setCapacity(m_capacity * mult);
        }

        void resize(const size_t& newSize) {
            if (newSize > size()) {
                reserve(newSize);
            }
            else {
                setCapacity(newSize);
            }
            m_size = newSize;
        }

        size_t size() const {
            return m_size;
        }

        bool empty() const {
            return !m_size;
        }

        void push_back(const T& element) {
            if (m_size == m_capacity) {
                setCapacity(m_capacity * 2);
            }
            m_vec[m_size++] = element;
        }

        void pop_back() {
            if (!m_size)
                return;
            --m_size;
            if (m_capacity / 2 == m_size) {
                setCapacity(m_capacity / 2);
            }
        }

        T& front() {
            return m_vec[0];
        }

        T& back() {
            return m_vec[size() - 1];
        }

        T& operator [] (const size_t& index) {
            return m_vec[index];
        }

        const T& operator [] (const size_t& index) const {
            return m_vec[index];
        }

    private:
        T* m_vec;
        size_t m_size, m_capacity;

        void setCapacity(const size_t& newCapacity) {
            if (newCapacity == 0) {
                delete[] m_vec;
                m_vec = nullptr;
                m_capacity = 0;
                return;
            }
            T* newVec = new T[newCapacity];
            if (m_vec) {
                int toCopy = newCapacity > m_capacity ? m_capacity : newCapacity;
                memcpy(newVec, m_vec, toCopy * sizeof(T));
            }
            delete[] m_vec;
            m_capacity = newCapacity;
            m_vec = newVec;

        }
};

#endif // MYVECTOR_H
