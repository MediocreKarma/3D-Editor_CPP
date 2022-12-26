#ifndef MYHASHMAP_H
#define MYHASHMAP_H

#include <cstddef>
#include <iterator>
#include <initializer_list>
#include <stdexcept>
#include "MyVector.h"
#include "MyList.h"

template<typename KeyType, typename ValueType>
class MyHashMapIterator;

template<typename KeyType, typename ValueType>
class MyHashMapConstIterator;

template<typename KeyType, typename ValueType>
class MyHashMap;

template<typename KeyType, typename ValueType>
struct ___hash___node {
    KeyType key;
    ValueType value;

    ___hash___node() :
        key(), value() {}
    ___hash___node(const KeyType& key_, const ValueType & value_) :
        key(key_), value(value_) {}
};

template<typename KeyType, typename ValueType>
class MyHashMapIterator {
    private:
        friend class MyHashMap<KeyType, ValueType>;
        using HashNode = ___hash___node<const KeyType, ValueType>;
        typename MyList<HashNode>::iterator m_listIt;
        size_t m_bucket;
        MyVector<MyList<HashNode>>& m_hashmap;
        size_t m_lastUsedBucket;

    public:
        using iterator = MyHashMapIterator;
        using iterator_category = std::forward_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = HashNode;
        using reference = HashNode&;
        using pointer = HashNode*;

        MyHashMapIterator(typename MyList<HashNode>::iterator it, size_t bucket, MyVector<MyList<HashNode>>& hashmap, size_t lastUsedBucket) :
            m_listIt(it), m_bucket(bucket), m_hashmap(hashmap), m_lastUsedBucket(lastUsedBucket) {}

        reference operator * () {
            return *m_listIt;
        }

        typename MyList<HashNode>::iterator operator -> () {
            return m_listIt;
        }

        iterator& operator = (const iterator& other) {
            m_listIt = other.m_listIt;
            m_bucket = other.m_bucket;
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

        operator MyHashMapConstIterator<const KeyType, ValueType> () const noexcept {
            return MyHashMapConstIterator<const KeyType, ValueType>(m_listIt, m_bucket, m_hashmap, m_lastUsedBucket);
        }
};

template<typename KeyType, typename ValueType>
class MyHashMapConstIterator {
    private:
        friend class MyHashMap<KeyType, ValueType>;
        using HashNode = ___hash___node<const KeyType, ValueType>;
        typename MyList<HashNode>::const_iterator m_listIt;
        size_t m_bucket;
        const MyVector<MyList<HashNode>>& m_hashmap;
        size_t m_lastUsedBucket;

    public:
        using iterator = MyHashMapConstIterator;
        using iterator_category = std::forward_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = const HashNode;
        using reference = const HashNode&;
        using pointer = const HashNode*;

        MyHashMapConstIterator(typename MyList<HashNode>::const_iterator it, size_t bucket, const MyVector<MyList<HashNode>>& hashmap, size_t lastUsedBucket) noexcept :
            m_listIt(it), m_bucket(bucket), m_hashmap(hashmap), m_lastUsedBucket(lastUsedBucket) {}

        MyHashMapConstIterator(const MyHashMapConstIterator<const KeyType, ValueType>& other) noexcept :
            m_listIt(other.m_listIt), m_bucket(other.m_bucket), m_hashmap(other.m_hashmap), m_lastUsedBucket(other.m_lastUsedBucket) {}

        reference operator * () const {
            return *m_listIt;
        }

        typename MyList<HashNode>::const_iterator operator -> () const {
            return m_listIt;
        }

        iterator& operator ++ () noexcept {
            ++m_listIt;
            if (m_listIt == m_hashmap[m_bucket].end() && m_bucket != m_lastUsedBucket) {
                do {
                    ++m_bucket;
                } while (m_bucket != m_lastUsedBucket && m_hashmap[m_bucket].empty());
                m_listIt = m_hashmap[m_bucket].begin();
            }
            return *this;
        }

        iterator operator ++ (int) noexcept {
            iterator tmp(*this);
            ++(*this);
            return tmp;
        }

        friend bool operator == (const iterator& a, const iterator& b) noexcept {
            return a.m_listIt == b.m_listIt;
        }

        friend bool operator != (const iterator& a, const iterator& b) noexcept {
            return a.m_listIt != b.m_listIt;
        }
};

template<typename KeyType, typename ValueType>
class MyHashMap {
    private:
        using HashMap = MyHashMap<KeyType, ValueType>;
    public:
        using hash_node = ___hash___node<const KeyType, ValueType>;
        friend class MyHashMapIterator<const KeyType, ValueType>;
        friend class MyHashMapIterator<const KeyType, const ValueType>;
        using iterator = MyHashMapIterator<const KeyType, ValueType>;
        using const_iterator = MyHashMapConstIterator<const KeyType, ValueType>;

        MyHashMap() noexcept :
            m_hmap(2), m_capacity(2), m_size(0), m_firstUsedBucket(2), m_lastUsedBucket(2) {}

        MyHashMap(const HashMap& other) noexcept :
            m_hmap(other.m_hmap), m_capacity(other.m_capacity), m_size(other.m_size), m_firstUsedBucket(other.m_firstUsedBucket), m_lastUsedBucket(other.m_lastUsedBucket) {}

        MyHashMap(HashMap&& other) noexcept : MyHashMap() {
            other.swap(*this);
        }

        MyHashMap(std::initializer_list<hash_node> l) noexcept : MyHashMap() {
            m_capacity = power2Ceil(l.size() * OneDivideOverCapacityMultiplier);
            m_hmap.resize(m_capacity);
            m_size = l.size();
            for (const hash_node& data : l) {
                noResizeInsert(data.key, data.value);
            }
        }

        HashMap& operator = (const HashMap& other) noexcept {
            m_hmap = other.m_hmap;
            m_size = other.m_size;
            m_capacity = other.m_capacity;
            m_firstUsedBucket = other.m_firstUsedBucket;
            m_lastUsedBucket = other.m_lastUsedBucket;
            return *this;
        }

        HashMap& operator = (HashMap&& other) noexcept {
            other.swap(*this);
            return *this;
        }

        HashMap& operator = (std::initializer_list<hash_node> l) noexcept {
            m_capacity = power2Ceil(l.size() * OneDivideOverCapacityMultiplier);
            m_hmap.resize(m_capacity);
            m_size = l.size();
            for (const hash_node& data : l) {
                noResizeInsert(data.key, data.value);
            }
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

        void insert(const KeyType& key, const ValueType& val) noexcept {
            noResizeInsert(key, val);
            ++m_size;
            if (m_size * OverCapacityMultiplier > m_capacity) {
                rehash(m_capacity * ResizeMultiplier);
            }
        }

        void erase(const KeyType& key) noexcept {
            size_t hashKey = hash(key);
            for (typename MyList<hash_node>::iterator it = m_hmap[hashKey].begin(); it != m_hmap[hashKey].end(); ++it) {
                if (it->key == key) {
                    m_hmap[hashKey].erase(it);
                    --m_size;
                    incremenetFirstOrLastBucket(hashKey);
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
            incremenetFirstOrLastBucket(it.m_bucket);
            if (m_size != 2 && m_size * UnderCapacityMultiplier < m_capacity) {
                rehash(m_capacity / ResizeMultiplier);
            }
        }

        bool contains(const KeyType& key) const noexcept {
            for (hash_node& node : m_hmap[hash(key)]) {
                if (node.key == key) {
                    return true;
                }
            }
            return false;
        }

        ValueType& operator [] (const KeyType& key) noexcept {
            size_t hashKey = hash(key);
            for (hash_node& node : m_hmap[hashKey]) {
                if (key == node.key) {
                    return node.value;
                }
            }
            m_hmap[hashKey].emplace_back(key, ValueType());
            return m_hmap[hashKey].back().value;
        }

        friend void swap(HashMap& a, HashMap& b) noexcept {
            a.swap(b);
        }

        void swap(HashMap& rhs) noexcept {
            std::swap(m_hmap, rhs.m_hmap);
            std::swap(m_size, rhs.m_size);
            std::swap(m_capacity, rhs.m_capacity);
            std::swap(m_firstUsedBucket, rhs.m_firstUsedBucket);
            std::swap(m_lastUsedBucket, rhs.m_lastUsedBucket);
        }

    private:
        MyVector<MyList<hash_node>> m_hmap;
        size_t m_capacity;
        size_t m_size;
        size_t m_firstUsedBucket;
        size_t m_lastUsedBucket;
        static constexpr double OverCapacityMultiplier = 0.75;
        static constexpr double OneDivideOverCapacityMultiplier = 1.34;
        static constexpr double UnderCapacityMultiplier = 0.375;
        static constexpr size_t ResizeMultiplier = 2;
        std::hash<KeyType> m_hasher = std::hash<KeyType>();

        size_t hash(const KeyType& key) const noexcept {
            return m_hasher(key) & (m_capacity - 1);
        }

        void rehash(const size_t& maxCapacity) {
            m_capacity = maxCapacity;
            MyVector<MyList<hash_node>> new_hmap(m_capacity);
            m_firstUsedBucket = m_capacity;
            m_lastUsedBucket = 0;
            for (MyList<hash_node>& bucket : m_hmap) {
                for (hash_node& node : bucket) {
                    size_t hashKey = hash(node.key);
                    new_hmap[hashKey].push_back(node);
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

        void noResizeInsert(const KeyType& key, const ValueType& val) noexcept {
            size_t hashKey = hash(key);
            for (hash_node& node : m_hmap[hashKey]) {
                if (node.key == key) {
                    return;
                }
            }
            m_hmap[hashKey].emplace_back(key, val);
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

        void incremenetFirstOrLastBucket(const size_t& hashKey) noexcept {
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

#endif // MYHASHMAP_H
