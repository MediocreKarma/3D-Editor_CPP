#ifndef MYARRAY_H
#define MYARRAY_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include "MyIterators.h"

template<class T, size_t m_size>
class MyArray
{
    public:

        using iterator = MyRandomAcessIterator<T>;
        using const_iterator = MyRandomAcessIterator<const T>;

        MyArray() = default;

        MyArray(const MyArray<T, m_size>& other) {
            for (size_t i = 0; i < other.size(); ++i) {
                m_data[i] = other[i];
            }
        }

        MyArray(const std::initializer_list<T>& rhs) {
            if (rhs.size() > size()) {
                throw std::invalid_argument("Too many initializers");
            }
            const T* ptr = rhs.begin();
            for (size_t i = 0; i < rhs.size(); ++i) {
                m_data[i] = *(ptr++);
            }
            for (size_t i = rhs.size(); i < size(); ++i) {
                m_data[i] = T();
            }
        }

        MyArray<T, m_size>& operator = (const MyArray<T, m_size>& rhs) {
            if (this == &rhs) {
                    return *this; // handle self assignment
            }
            for (size_t i = 0; i < rhs.size(); ++i) {
                m_data[i] = rhs[i];
            }
            return *this;
        }

        size_t size() const {
            return m_size;
        }

        iterator begin() {
            return iterator(&m_data[0]);
        }

        iterator end() {
            return iterator(&m_data[m_size]);
        }

        const_iterator cbegin() const {
            return const_iterator(&m_data[0]);
        }

        const_iterator cend() const {
            return const_iterator(&m_data[m_size]);
        }

        T* data() {
            return m_data;
        }

        void fill(const T& fillValue) {
            for (size_t i = 0; i < m_size; ++i) {
                m_data[i] = fillValue;
            }
        }

        MyArray<T, m_size>& operator = (const std::initializer_list<T>& listInit) {
            if (listInit.size() > size()) {
                throw std::invalid_argument("Too many initializers");
            }
            const T* ptr = listInit.begin();
            for (size_t i = 0; i < listInit.size(); ++i) {
                m_data[i] = *(ptr++);
            }
            for (size_t i = listInit.size(); i < size(); ++i) {
                m_data[i] = T();
            }
            return *this;
        }

        T& operator [] (const size_t& index) {
            return m_data[index];
        }

        const T& operator [] (const size_t& index) const {
            return m_data[index];
        }

    private:
        T m_data[m_size];
};

template<size_t m_size>
class MyArray<char, m_size> {
    public:
        MyArray() = default;

        MyArray(const MyArray<char, m_size>& other) {
            for (size_t i = 0; i < other.size(); ++i) {
                m_data[i] = other[i];
            }
        }

        MyArray(const std::initializer_list<int>& rhs) {
            if (rhs.size() > size()) {
                throw std::invalid_argument("Too many initializers");
            }
            const int* ptr = rhs.begin();
            for (size_t i = 0; i < rhs.size(); ++i) {
                m_data[i] = *(ptr++);
            }
            for (size_t i = rhs.size(); i < size(); ++i) {
                m_data[i] = char();
            }
        }

        MyArray(const char* p) {
            fill(0);
            if (!p) {
                m_data[0] = 0;
                return;
            }
            size_t i = 0;
            for (i = 0; p[i]; ++i) {
                m_data[i] = p[i];
            }
            m_data[i] = 0;
        }

        MyArray<char, m_size>& operator = (const MyArray<char, m_size>& rhs) {
            if (this == &rhs) {
                    return *this; // handle self assignment
            }
            for (size_t i = 0; i < rhs.size(); ++i) {
                m_data[i] = rhs[i];
            }
            return *this;
        }

        size_t size() const {
            return m_size;
        }

        void fill(const char& fillValue) {
            for (size_t i = 0; i < m_size; ++i) {
                m_data[i] = fillValue;
            }
        }

        char* data() {
            return m_data;
        }

        MyArray<char, m_size>& operator = (const char* p) {
            fill(0);
            if (!p) {
                return *this;
            }
            size_t i = 0;
            for (i = 0; p[i]; ++i) {
                m_data[i] = p[i];
            }
            m_data[i] = 0;
            return *this;
        }

        char& operator [] (const size_t& index) {
            return m_data[index];
        }

        const char& operator [] (const size_t& index) const {
            return m_data[index];
        }

    private:
        char m_data[m_size];
};

#endif // MYARRAY_H
