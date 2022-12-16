#ifndef MYITERATORS_H
#define MYITERATORS_H

template<class T>
class MyRandomAcessIterator {
    public:
        using iterator = MyRandomAcessIterator;
        using iterator_category = std::random_access_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using reference = T&;
        using pointer = T*;

        MyRandomAcessIterator(pointer ptr) :
            m_ptr(ptr) {}

        reference operator * () {
            return *m_ptr;
        }

        pointer operator -> () {
            return m_ptr;
        }

        iterator& operator ++ () {
            ++m_ptr;
            return *this;
        }

        iterator operator ++ (int) {
            iterator tmp(*this);
            ++m_ptr;
            return tmp;
        }

        iterator& operator -- () {
            --m_ptr;
            return *this;
        }

        iterator operator -- (int) {
            iterator tmp(*this);
            --m_ptr;
            return tmp;
        }

        iterator operator + (const int& offset) const {
            return iterator(m_ptr + offset);
        }

        iterator& operator += (const int& offset) {
            m_ptr += offset;
            return *this;
        }

        iterator operator - (const int& offset) const {
            return iterator(m_ptr - offset);
        }

        iterator& operator -= (const int& offset) {
            m_ptr -= offset;
            return *this;
        }

        reference operator [] (const int& index) {
            return *(m_ptr + index);
        }

        friend int operator - (const iterator& a, const iterator& b) {
            return a.m_ptr - b.m_ptr;
        }

        friend bool operator == (const iterator& a, const iterator& b) {
            return a.m_ptr == b.m_ptr;
        }

        friend bool operator != (const iterator& a, const iterator& b) {
            return a.m_ptr != b.m_ptr;
        }

        friend bool operator < (const iterator& a, const iterator& b)  {
            return a.m_ptr < b.m_ptr;
        }

        friend bool operator <= (const iterator& a, const iterator& b) {
            return a.m_ptr <= b.m_ptr;
        }

        friend bool operator > (const iterator& a, const iterator& b) {
            return a.m_ptr > b.m_ptr;
        }

        friend bool operator >= (const iterator& a, const iterator& b) {
            return a.m_ptr >= b.m_ptr;
        }

    private:
        pointer m_ptr;
};

template<class T>
class MyForwardIterator {
    public:
        using iterator = MyForwardIterator;
        using iterator_category = std::random_access_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using reference = T&;
        using pointer = T*;

        MyForwardIterator(pointer ptr) :
            m_ptr(ptr) {}

        reference operator * () {
            return *m_ptr;
        }

        pointer operator -> () {
            return m_ptr;
        }

        iterator& operator ++ () {
            ++m_ptr;
            return *this;
        }

        iterator operator ++ (int) {
            iterator tmp(*this);
            ++m_ptr;
            return tmp;
        }

    private:
        pointer m_ptr;
};

#endif // MYITERATORS_H
