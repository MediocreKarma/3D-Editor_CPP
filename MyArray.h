#ifndef MYARRAY_H
#define MYARRAY_H

#include <cstddef>
#include <initializer_list>

template<class T, size_t m_size>
class MyArray
{
    public:
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

#endif // MYARRAY_H
