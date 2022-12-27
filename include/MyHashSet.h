#ifndef MYHASHSET_H
#define MYHASHSET_H

#include <iterator>
#include <initializer_list>

template<typename T>
class MyHashSetIterator {
    private:
        typename MyList<T>::const_iterator m_listIt;
        size_t m_bucket;
        MyVector<MyList<T>>& m_hashmap;
        size_t m_lastUsedBucket;

    public:
        using iterator = MyHashSetIterator;
        using iterator_category = std::forward_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = const T;
        using reference = const T&;
        using pointer = const T*;

        MyHashSetIterator(typename MyList<T>::iterator it, size_t bucket, MyVector<MyList<T>>& hashmap, size_t lastUsedBucket) :
            m_listIt(it), m_bucket(bucket), m_hashmap(hashmap), m_lastUsedBucket(lastUsedBucket) {}

        reference operator * () {
            return *m_listIt;
        }

        typename MyList<T>::iterator operator -> () {
            return m_listIt;
        }

        iterator& operator = (const iterator& other) {
            m_listIt = other.m_listIt;
            m_bucket = other.m_bucket;
            return *this;
        }

        iterator& operator ++ () {
            ++m_listIt;
            if (m_listIt == m_hashmap[m_bucket].end() && m_bucket != m_lastUsedBucket) {
                do {
                    ++m_bucket;
                } while (m_bucket != m_lastUsedBucket && m_hashmap[m_bucket].empty());
                m_listIt = m_hashmap[m_bucket].begin();
            }
            return *this;
        }

        iterator operator ++ (int) {
            iterator tmp(*this);
            ++(*this);
            return tmp;
        }

        friend bool operator == (const iterator& a, const iterator& b) {
            return a.m_listIt == b.m_listIt;
        }

        friend bool operator != (const iterator& a, const iterator& b) {
            return a.m_listIt != b.m_listIt;
        }
};

template<typename T>
class MyHashSet {
    private:
        using HashSet = MyHashSet<T>;
    public:
        friend class MyHashSetIterator<T>;
        using iterator = MyHashSetIterator<T>;
        using const_iterator = iterator;

        MyHashSet() noexcept :
            m_hmap(2), m_capacity(2), m_size(0), m_firstUsedBucket(2), m_lastUsedBucket(2) {}

        MyHashSet(const HashSet& other) noexcept :
            m_hmap(other.m_hmap), m_capacity(other.m_capacity), m_size(other.m_size), m_firstUsedBucket(other.m_firstUsedBucket), m_lastUsedBucket(other.m_lastUsedBucket) {}

        MyHashSet(HashSet&& other) noexcept : MyHashSet() {
            other.swap(*this);
        }

        MyHashSet(std::initializer_list<T> l) noexcept : MyHashSet() {
            m_capacity = power2Ceil(l.size() * OneDivideOverCapacityMultiplier);
            m_hmap.resize(m_capacity);
            m_size = l.size();
            for (const T& data : l) {
                noResizeInsert(data);
            }
        }

        HashSet& operator = (const HashSet& other) noexcept {
            m_hmap = other.m_hmap;
            m_size = other.m_size;
            m_capacity = other.m_capacity;
            m_firstUsedBucket = other.m_firstUsedBucket;
            m_lastUsedBucket = other.m_lastUsedBucket;
            return *this;
        }

        HashSet& operator = (HashSet&& other) noexcept {
            other.swap(*this);
            return *this;
        }

        HashSet& operator = (std::initializer_list<T> l) noexcept {
            m_capacity = power2Ceil(l.size() * OneDivideOverCapacityMultiplier);
            m_hmap.resize(m_capacity);
            m_size = l.size();
            for (const T& x : l) {
                noResizeInsert(x);
            }
            return *this;
        }

        iterator begin() noexcept {
            return iterator(m_hmap[m_firstUsedBucket].begin(), m_firstUsedBucket, m_hmap, m_lastUsedBucket);
        }

        iterator end() noexcept {
            return iterator(m_hmap[m_lastUsedBucket].end(), m_lastUsedBucket, m_hmap, m_lastUsedBucket);
        }

        const_iterator begin() const noexcept {
            return cbegin();
        }

        const_iterator end() const noexcept {
            return cend();
        }

        const_iterator cbegin() const noexcept {
            return const_iterator(m_hmap[m_firstUsedBucket].cbegin(), m_firstUsedBucket, m_hmap, m_lastUsedBucket);
        }

        const_iterator cend() const noexcept {
            return const_iterator(m_hmap[m_lastUsedBucket].cend(), m_lastUsedBucket, m_hmap, m_lastUsedBucket);
        }

        size_t size() const noexcept {
            return m_size;
        }

        bool empty() const noexcept {
            return !m_size;
        }

        void insert(const T& key) noexcept {
            noResizeInsert(key);
            ++m_size;
            if (m_size * OverCapacityMultiplier > m_capacity) {
                rehash(m_capacity * ResizeMultiplier);
            }
        }

        void erase(const T& key) noexcept {
            size_t hashKey = hash(key);
            for (typename MyList<T>::iterator it = m_hmap[hashKey].begin(); it != m_hmap[hashKey].end(); ++it) {
                if (*it == key) {
                    m_hmap[hashKey].erase(it);
                    --m_size;
                    incrementFirstOrLastBucket(hashKey);
                    if (m_size != 2 && m_size * UnderCapacityMultiplier < m_capacity) {
                        rehash(m_capacity / ResizeMultiplier);
                    }
                    break;
                }
            }
        }

        void erase(iterator it) noexcept {
            m_hmap[it.m_bucket].erase(it.m_listIt);
            --m_size;
            incrementFirstOrLastBucket(it.m_bucket);
            if (m_size != 2 && m_size * UnderCapacityMultiplier < m_capacity) {
                rehash(m_capacity / ResizeMultiplier);
            }
        }

        bool contains(const T& key) const noexcept {
            for (T& x : m_hmap[hash(key)]) {
                if (x == key) {
                    return true;
                }
            }
            return false;
        }

        friend void swap(HashSet& a, HashSet& b) noexcept {
            a.swap(b);
        }

        void swap(HashSet& rhs) noexcept {
            std::swap(m_hmap, rhs.m_hmap);
            std::swap(m_size, rhs.m_size);
            std::swap(m_capacity, rhs.m_capacity);
            std::swap(m_firstUsedBucket, rhs.m_firstUsedBucket);
            std::swap(m_lastUsedBucket, rhs.m_lastUsedBucket);
        }

    private:
        MyVector<MyList<T>> m_hmap;
        size_t m_capacity;
        size_t m_size;
        size_t m_firstUsedBucket;
        size_t m_lastUsedBucket;
        static constexpr double OverCapacityMultiplier = 0.75;
        static constexpr double OneDivideOverCapacityMultiplier = 1.34;
        static constexpr double UnderCapacityMultiplier = 0.375;
        static constexpr size_t ResizeMultiplier = 2;
        static constexpr std::hash<T> m_hasher = std::hash<T>();

        size_t hash(const T& key) const noexcept {
            return m_hasher(key) & (m_capacity - 1);
        }

        void rehash(const size_t& maxCapacity) {
            m_capacity = maxCapacity;
            MyVector<MyList<T>> new_hmap(m_capacity);
            m_firstUsedBucket = m_capacity;
            m_lastUsedBucket = 0;
            for (MyList<T>& bucket : m_hmap) {
                for (T& x : bucket) {
                    size_t hashKey = hash(x);
                    new_hmap[hashKey].push_back(x);
                    updateFirstOrLast(hashKey);
                }
            }
            m_hmap.swap(new_hmap);
        }

        size_t power2Ceil(size_t value) const noexcept {
            if (value <= 2) {
                return 2;
            }
            static constexpr size_t BITS = sizeof(size_t) * CHAR_BIT;
            --value;
            for(size_t i = 1; i < BITS; i *= 2)
                value |= value >> i;
            return value+1;
        }

        void noResizeInsert(const T& key) noexcept {
            size_t hashKey = hash(key);
            for (T& x : m_hmap[hashKey]) {
                if (x == key) {
                    return;
                }
            }
            m_hmap[hashKey].push_back(key);
            updateFirstOrLast(hashKey);
        }

        void updateFirstOrLast(const size_t& hashKey) noexcept {
            if (m_size == 1) {
                m_lastUsedBucket = m_firstUsedBucket = hashKey;
                return;
            }
            if (hashKey > m_lastUsedBucket) {
                m_lastUsedBucket = hashKey;
            }
            else if (hashKey < m_firstUsedBucket) {
                m_firstUsedBucket = hashKey;
            }
        }

        void incrementFirstOrLastBucket(const size_t& hashKey) noexcept {
            if (!m_hmap[hashKey].empty()) {
                return;
            }
            if (m_firstUsedBucket == m_lastUsedBucket) {
                m_firstUsedBucket = m_capacity;
                m_lastUsedBucket = m_capacity;
                return;
            }
            if (hashKey == m_firstUsedBucket) {
                for (size_t i = hashKey + 1; i < m_hmap.size(); ++i) {
                    if (!m_hmap[i].empty()) {
                        m_firstUsedBucket = i;
                        return;
                    }
                }
            }
            else if (hashKey == m_lastUsedBucket) {
                for (int i = hashKey - 1; i >= 0; --i) {
                    if (!m_hmap[i].empty()) {
                        m_lastUsedBucket = i;
                        return;
                    }
                }
            }
        }
};
#endif // MYHASHSET_H
