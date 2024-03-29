#ifndef MYLIST_H
#define MYLIST_H

#include <cstddef>
#include <iterator>
#include <initializer_list>
#include <stdexcept>

template<typename T>
class MyList;

template<typename T, typename DRT>
class MyListIterator {
    private:
        friend class MyList<T>;
        using Node = typename MyList<T>::list_node;
        Node* m_ptr;

    public:
        using iterator = MyListIterator;
        using iterator_category = std::bidirectional_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = DRT;
        using reference = DRT&;
        using pointer = DRT*;

        MyListIterator() noexcept :
            m_ptr(nullptr) {}

        MyListIterator(Node* ptr) noexcept :
            m_ptr(ptr) {}

        MyListIterator(const MyListIterator& other) noexcept :
            m_ptr(other.m_ptr) {}

        reference operator * () const noexcept {
            return m_ptr->data;
        }

        pointer operator -> () const noexcept {
            return &(m_ptr->data);
        }

        iterator& operator = (const iterator& other) noexcept {
            m_ptr = other.m_ptr;
            return *this;
        }

        friend bool operator == (const iterator& a, const iterator& b) noexcept {
            return a.m_ptr == b.m_ptr;
        }

        friend bool operator != (const iterator& a, const iterator& b) noexcept {
            return a.m_ptr != b.m_ptr;
        }

        iterator& operator ++ () noexcept {
            m_ptr = m_ptr->next;
            return *this;
        }

        iterator operator ++ (int) noexcept {
            iterator tmp(*this);
            m_ptr = m_ptr->next;
            return tmp;
        }

        iterator& operator -- () noexcept {
            m_ptr = m_ptr->prev;
            return *this;
        }

        iterator operator -- (int) noexcept {
            iterator tmp(*this);
            m_ptr = m_ptr->prev;
            return tmp;
        }

        operator MyListIterator<T, const DRT>() const noexcept {
            return MyListIterator<T, const DRT>(m_ptr);
        }

        friend struct std::hash<MyListIterator<T, DRT>>;

        explicit operator bool() const noexcept {
            return m_ptr;
        }
};

namespace std {
    template<typename T, typename DRT> struct hash<MyListIterator<T, DRT>> {
        size_t operator()(const MyListIterator<T, DRT>& it) const noexcept {
            return std::hash<void*>{}((void*)it.m_ptr);
        }
    };
}

template<typename T>
class MyList {
    private:
        friend class MyListIterator<T, T>;
        friend class MyListIterator<T, const T>;

        struct list_node {
                T data;
                list_node* prev;
                list_node* next;

                list_node() noexcept :
                    data(), prev(nullptr), next(nullptr) {}
                list_node(const T& _data) noexcept :
                    data(_data), prev(nullptr), next(nullptr) {}
                list_node(const T& _data, list_node* _prev, list_node* _next) noexcept :
                    data(_data), prev(_prev), next(_next) {}
                list_node(const list_node& other) noexcept :
                    data(other.data), prev(other.prev), next(other.next) {}
                list_node(list_node&& other) noexcept : list_node() {
                    other.swap(*this);
                }

                list_node& operator = (const list_node& rhs) noexcept {
                    data = rhs.data;
                    prev = rhs.prev;
                    next = rhs.next;
                    return *this;
                }
                list_node& operator = (list_node&& rhs) noexcept {
                    rhs.swap(*this);
                    return *this;
                }

                friend void swap(list_node& a, list_node& b) noexcept {
                    a.swap(b);
                }

                void swap(list_node& rhs) noexcept {
                    std::swap(data, rhs.data);
                    std::swap(prev, rhs.prev);
                    std::swap(next, rhs.next);
                }
        };

        using Node = list_node;
        Node* m_first;
        Node* m_last;
        size_t m_size;

    public:
        using iterator = MyListIterator<T, T>;
        using const_iterator = MyListIterator<T, const T>;
        using reverse_iterator = std::reverse_iterator<MyListIterator<T, T>>;
        using const_reverse_iterator = std::reverse_iterator<MyListIterator<T, const T>>;

        MyList() :
            m_first(new Node()), m_last(new Node()), m_size(0) {
            m_first->next = m_last;
            m_last->prev = m_first;
        }

        MyList(const MyList<T>& other) noexcept : MyList() {
            for (const T& x : other) {
                push_back(x);
            }
        }

        MyList(const std::initializer_list<T>& initList) noexcept : MyList() {
            for (const T& x : initList) {
                push_back(x);
            }
        }

        MyList(MyList&& other) noexcept : MyList() {
            other.swap(*this);
        }

        ~MyList() noexcept {
            clear();
            delete m_first;
            delete m_last;
        }

        MyList<T>& operator = (const MyList<T>& rhs) noexcept {
            clear();
            for (const T& x : rhs) {
                push_back(x);
            }
            if (m_size != rhs.size()) {
                clear();
                return *this;
            }
            return *this;
        }

        MyList<T>& operator = (std::initializer_list<T> rhs) noexcept {
            clear();
            for (const T& x : rhs) {
                push_back(x);
            }
            if (m_size != rhs.size()) {
                clear();
                return *this;
            }
        }

        MyList<T>& operator = (MyList&& rhs) noexcept {
            rhs.swap(*this);
            return *this;
        }

        size_t size() const noexcept {
            return m_size;
        }

        bool empty() const noexcept {
            return !m_size;
        }

        void push_front(const T& newValue) noexcept {
            insert(begin(), newValue);
        }

        void push_back(const T& newValue) noexcept {
            insert(end(), newValue);
        }

        template<class... Args>
        void emplace_front(Args&&... args) noexcept {
            push_front(T(args...));
        }

        template<class... Args>
        void emplace_back(Args&&... args) noexcept {
            push_back(T(args...));
        }

        void pop_front() noexcept {
            erase(begin());
        }

        void pop_back() noexcept {
            erase(iterator(m_last->prev));
        }

        iterator begin() noexcept {
            return iterator(m_first->next);
        }

        iterator end() noexcept {
            return iterator(m_last);
        }

        const_iterator begin() const noexcept {
            return cbegin();
        }

        const_iterator end() const noexcept {
            return cend();
        }

        const_iterator cbegin() const noexcept {
            return const_iterator(m_first->next);
        }

        const_iterator cend() const noexcept {
            return const_iterator(m_last);
        }

        reverse_iterator rbegin() noexcept {
            return std::reverse_iterator<iterator>(end());
        }

        reverse_iterator rend() noexcept {
            return std::reverse_iterator<iterator>(begin());
        }

        const_reverse_iterator crbegin() const noexcept {
            return std::reverse_iterator<const_iterator>(cend());
        }

        const_reverse_iterator crend() const noexcept {
            return std::reverse_iterator<const_iterator>(cbegin());
        }

        iterator find(const T& findData) noexcept {
            Node* pos = m_first->next;
            while (pos != m_last && pos->data != findData) {
                pos = pos->next;
            }
            return iterator(pos);
        }

        void erase(iterator it) {
            Node* pos = it.m_ptr;
            pos->next->prev = pos->prev;
            pos->prev->next = pos->next;
            --m_size;
            delete pos;
        }

        void erase(iterator it1, iterator it2) {
            Node* pos1 = it1.m_ptr;
            Node* pos2 = it2.m_ptr;
            pos2 = pos2->prev;
            pos1->prev->next = pos2->next;
            pos2->next->prev = pos1->prev;
            pos2 = pos2->next;
            while (pos1 != pos2) {
                Node* del = pos1;
                pos1 = pos1->next;
                delete del;
            }
        }

        void insert(iterator it, const T& value) noexcept {
            Node* newNode, * pos = it.m_ptr;
            try {
                newNode = new Node(value, pos->prev, pos);
            }
            catch(...) {
                return;
            }
            pos->prev->next = newNode;
            pos->prev = newNode;
            ++m_size;
        }

        template<class... Args>
        void emplace(iterator it, Args&& ...args) {
            insert(it, T(args...));
        }

        void clear() noexcept {
            Node* q = m_first->next;
            while (q != m_last) {
                Node* del = q;
                q = q->next;
                delete del;
            }
            m_first->next = m_last;
            m_last->prev = m_first;
            m_size = 0;
        }

        T& front () {
            return m_first->next->data;
        }

        const T& front() const {
            return m_first->next->data;
        }

        T& back() {
            return m_last->prev->data;
        }

        const T& back() const {
            return m_last->prev->data;
        }

        friend void swap(MyList<T>& a, MyList<T>& b) {
            a.swap(b);
        }

        void swap(MyList<T>& rhs) noexcept {
            std::swap(m_first, rhs.m_first);
            std::swap(m_last, rhs.m_last);
            std::swap(m_size, rhs.m_size);
        }
};

#endif // MYLIST_H
