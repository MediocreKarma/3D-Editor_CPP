#ifndef MYITERATORS_H
#define MYITERATORS_H

#include <iterator>

template<typename T>
class MyRandomAccessIterator {
    public:
        using iterator = MyRandomAccessIterator;
        using iterator_category = std::random_access_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using reference = T&;
        using pointer = T*;

        MyRandomAccessIterator(pointer ptr) noexcept :
            m_ptr(ptr) {}

        reference operator * () {
            return *m_ptr;
        }

        pointer operator -> () noexcept {
            return m_ptr;
        }

        iterator& operator ++ () noexcept {
            ++m_ptr;
            return *this;
        }

        iterator operator ++ (int) noexcept {
            iterator tmp(*this);
            ++m_ptr;
            return tmp;
        }

        iterator& operator -- () noexcept {
            --m_ptr;
            return *this;
        }

        iterator operator -- (int) noexcept {
            iterator tmp(*this);
            --m_ptr;
            return tmp;
        }

        iterator operator + (const int offset) const noexcept {
            return iterator(m_ptr + offset);
        }

        iterator& operator += (const int offset) noexcept {
            m_ptr += offset;
            return *this;
        }

        iterator operator - (const int offset) const noexcept {
            return iterator(m_ptr - offset);
        }

        iterator& operator -= (const int offset) noexcept {
            m_ptr -= offset;
            return *this;
        }

        reference operator [] (const int index) {
            return *(m_ptr + index);
        }

        iterator operator = (const iterator& other) noexcept {
            m_ptr = other.m_ptr;
            return *this;
        }

        friend int operator - (const iterator& a, const iterator& b) noexcept {
            return a.m_ptr - b.m_ptr;
        }

        friend bool operator == (const iterator& a, const iterator& b) noexcept {
            return a.m_ptr == b.m_ptr;
        }

        friend bool operator != (const iterator& a, const iterator& b) noexcept {
            return a.m_ptr != b.m_ptr;
        }

        friend bool operator < (const iterator& a, const iterator& b) noexcept  {
            return a.m_ptr < b.m_ptr;
        }

        friend bool operator <= (const iterator& a, const iterator& b) noexcept {
            return a.m_ptr <= b.m_ptr;
        }

        friend bool operator > (const iterator& a, const iterator& b) noexcept {
            return a.m_ptr > b.m_ptr;
        }

        friend bool operator >= (const iterator& a, const iterator& b) noexcept {
            return a.m_ptr >= b.m_ptr;
        }

        operator MyRandomAccessIterator<const T>() const noexcept {
            return MyRandomAccessIterator<const T>(m_ptr);
        }

    private:
        pointer m_ptr;
};

#endif // MYITERATORS_H
