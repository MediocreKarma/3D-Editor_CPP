#ifndef MYARRAY_H
#define MYARRAY_H

#include <cstddef>

template<class T, size_t m_size>
class MyArray
{
    public:
        MyArray() = default;

        MyArray(const MyArray<T, m_size>& other) {
            for (size_t i = 0; i < other.size(); ++i) {
                this->m_data[i] = other[i];
            }
        }

        MyArray<T, m_size>& operator=(const MyArray<T, m_size>& rhs) {
            if (this == &rhs) {
                    return *this; // handle self assignment
            }
            for (size_t i = 0; i < rhs.size(); ++i) {
                this->m_data[i] = rhs[i];
            }
            return *this;
        }

        size_t size() const {
            return m_size;
        }

        void fill(const T& fillValue) {
            for (size_t i = 0; i < m_size; ++i) {
                this->m_data[i] = fillValue;
            }
        }

        T& operator [] (const size_t& index) {
            return this->m_data[index];
        }

        const T& operator[] (const size_t& index) const {
            return this->m_data[index];
        }

    private:
        T m_data[m_size];
};

#endif // MYARRAY_H
