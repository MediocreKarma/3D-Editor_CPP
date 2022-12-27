#ifndef MYVECTOR_H
#define MYVECTOR_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>

template<typename T>
class MyVector {
    public:
        using iterator = MyRandomAcessIterator<T>;
        using const_iterator = MyRandomAcessIterator<const T>;

        MyVector() noexcept : m_vec(nullptr), m_size(0), m_capacity(2) {
            setCapacity(m_capacity);
        }

        MyVector(const MyVector<T>& other) noexcept : m_vec(nullptr), m_size(0), m_capacity(2) {
            setCapacity(other.m_capacity);
            m_size = other.size();
            for (size_t i = 0; i < other.size(); ++i) {
                try {
                    m_vec[i] = other[i];
                }
                catch(...) {
                    delete[] m_vec;
                    m_size = 0;
                    m_capacity = 0;
                    return;
                }
            }
        }

        MyVector(std::initializer_list<T> initList) noexcept : m_vec(nullptr), m_size(0), m_capacity(2) {
            resize(initList.size());
            const T* el = initList.begin();
            for (size_t i = 0; i < initList.size(); ++i) {
                m_vec[i] = *(el++);
            }
        }

        MyVector(const size_t& vecSize, const T& value) noexcept : m_vec(nullptr), m_size(0), m_capacity(0) {
            resize(vecSize);
            for (size_t i = 0; i < size(); ++i) {
                m_vec[i] = value;
            }
        }

        MyVector(const size_t& vecSize) noexcept : m_vec(nullptr), m_size(0), m_capacity(0) {
            resize(vecSize);
        }

        MyVector(iterator it1, iterator it2) : m_vec(nullptr), m_size(0), m_capacity(0) {
            resize(it2 - it1);
            size_t i = 0;
            while (it1 != it2) {
                m_vec[i++] = *(it1++);
            }
        }

        MyVector(const_iterator it1, const_iterator it2) : m_vec(nullptr), m_size(0), m_capacity(0) {
            resize(it2 - it1);
            size_t i = 0;
            while (it1 != it2) {
                m_vec[i++] = *(it1++);
            }
        }

        MyVector(MyVector&& movedVec) noexcept : m_vec(nullptr), m_size(0), m_capacity(0) {
            movedVec.swap(*this);
        }

        ~MyVector() {
            delete[] m_vec;
        }

        MyVector<T>& operator = (const MyVector<T>& other) noexcept {
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

        MyVector<T>& operator = (std::initializer_list<T> initList) noexcept {
            resize(initList.size());
            const T* el = initList.begin();
            for (size_t i = 0; i < initList.size(); ++i) {
                m_vec[i] = *(el++);
            }
            return *this;
        }

        MyVector<T>& operator = (MyVector&& other) noexcept {
            other.swap(*this);
            return *this;
        }

        iterator begin() noexcept {
            return iterator(&m_vec[0]);
        }

        iterator end() noexcept {
            return iterator(&m_vec[m_size]);
        }

        const_iterator begin() const noexcept {
            return cbegin();
        }

        const_iterator end() const noexcept {
            return cend();
        }

        const_iterator cbegin() const noexcept {
            return const_iterator(&m_vec[0]);
        }

        const_iterator cend() const noexcept {
            return const_iterator(&m_vec[m_size]);
        }

        void reserve(const size_t& reservedCapacity) noexcept {
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

        void resize(const size_t& newSize) noexcept {
            setCapacity(newSize);
            m_size = newSize;
        }

        size_t size() const noexcept {
            return m_size;
        }

        bool empty() const noexcept {
            return !m_size;
        }

        void erase(iterator it) {
            if (outOfBounds(it)) {
                throw std::out_of_range("erase failed: outside vector of range");
            }
            iterator nxt = it + 1;
            while (nxt != end()) {
                *(it++) = *(nxt++);
            }
            pop_back();
        }

        void insert(iterator it, const T& val) {
            if (it == end()) {
                push_back(val);
                return;
            }
            if (outOfBounds(it)) {
                throw std::out_of_range("insert failed: outside vector range");
            }
            int pos = it - begin();
            push_back(back());
            for (int i = size() - 2; i > pos; --i) {
                m_vec[i] = m_vec[i - 1];
            }
            m_vec[pos] = val;
        }

        void push_back(const T& element) {
            if (m_size == m_capacity) {
                if (!m_capacity) {
                    setCapacity(1);
                }
                else {
                    setCapacity(m_capacity * 2);
                }
            }
            m_vec[m_size++] = element;
        }

        template<class... Args>
        void emplace_back(Args&&... args) {
            push_back(T(args...));
        }

        void pop_back() noexcept {
            if (!m_size) {
                return;
            }
            --m_size;
            if (m_capacity / 2 == m_size) {
                setCapacity(m_capacity / 2);
            }
        }

        void fill(const T& value) noexcept {
            if (!m_size) {
                return;
            }
            for (size_t i = 0; i < size(); ++i) {
                m_vec[i] = value;
            }
        }

        T& front() {
            return m_vec[0];
        }

        T& back() {
            return m_vec[size() - 1];
        }

        const T& front() const {
            return m_vec[0];
        }

        const T& back() const {
            return m_vec[size() - 1];
        }

        T& operator [] (const size_t& index) {
            return m_vec[index];
        }

        const T& operator [] (const size_t& index) const {
            return m_vec[index];
        }

        void clear() noexcept {
            setCapacity(0);
        }

        friend void swap(MyVector<T>& a, MyVector<T>& b) noexcept {
            a.swap(b);
        }

        void swap(MyVector<T>& rhs) noexcept {
            std::swap(m_vec, rhs.m_vec);
            std::swap(m_size, rhs.m_size);
            std::swap(m_capacity, rhs.m_capacity);
        }

    private:
        T* m_vec;
        size_t m_size, m_capacity;

        void setCapacity(const size_t& newCapacity) noexcept {
            if (newCapacity == 0) {
                delete[] m_vec;
                m_vec = nullptr;
                m_capacity = 0;
                return;
            }
            T* newVec = nullptr;
            try {
                newVec = new T[newCapacity];
            }
            catch(...) {
                return;
            }
            if (m_vec) {
                size_t toCopy = newCapacity > m_capacity ? m_capacity : newCapacity;
                for (size_t i = 0; i < toCopy; ++i) {
                    try {
                        newVec[i] = m_vec[i];
                    }
                    catch(...) {
                        delete[] newVec;
                        return;
                    }
                }
            }
            delete[] m_vec;
            m_capacity = newCapacity;
            m_vec = newVec;
        }

        bool outOfBounds(const size_t& index) const noexcept {
            return m_size <= index;
        }

        bool outOfBounds(const_iterator it) const noexcept {
            return it < cbegin() && cend() <= it;
        }
};

#endif // MYVECTOR_H
