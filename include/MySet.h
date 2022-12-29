#ifndef MYSET_H
#define MYSET_H


#include <iterator>

template<typename T, typename F = std::less<T>>
class MySet;

template<typename T, typename F>
class MySetIterator {
    private:
        friend class MySet<T, F>;
        using Node = typename MySet<T, F>::SetNode;
        Node* m_ptr;

    public:
        using iterator = MySetIterator;
        using iterator_category = std::bidirectional_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = const T;
        using reference = const T&;
        using pointer = const T*;

        MySetIterator() :
            m_ptr(nullptr) {}

        MySetIterator(Node* ptr) noexcept :
            m_ptr(ptr) {}

        MySetIterator(const MySetIterator& other) noexcept :
            m_ptr(other.m_ptr) {}

        reference operator * () noexcept {
            return m_ptr->key;
        }

        pointer operator -> () noexcept {
            return &(m_ptr->key);
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
            m_ptr = successor(m_ptr);
            return *this;
        }

        iterator operator ++ (int) noexcept {
            iterator tmp(*this);
            ++(*this);
            return tmp;
        }

        iterator& operator -- () noexcept {
            m_ptr = predecessor(m_ptr);
            return *this;
        }

        iterator operator -- (int) noexcept {
            iterator tmp(*this);
            --(*this);
            return tmp;
        }

        friend struct std::hash<MySetIterator<T, F>>;

};

namespace std {
    template<typename T, typename F> struct hash<MySetIterator<T, F>> {
        size_t operator()(const MySetIterator<T, F>& it) const noexcept {
            return std::hash<void*>{}((void*)it.m_ptr);
        }
    };
}

template<typename T, typename F>
class MySet {
    public:
        using iterator = MySetIterator<T, F>;
        using const_iterator = iterator;
        using reverse_iterator = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<iterator>;

        MySet() noexcept : m_root(nil), m_size(0) {
            m_root->parent = nil;
            m_root->left = nil;
            m_root->right = nil;
        }

        ~MySet() noexcept {
            clear();
        }

        MySet(const MySet<T, F>& other) noexcept {
            m_root = nil;
            m_root = copyTree(other.m_root);
        }

        MySet(MySet<T, F>&& other) noexcept {
            std::swap(m_root, other.m_root);
        }

        MySet(std::initializer_list<T> l) noexcept {
            for (const T& key : l) {
                insert(key);
            }
        }

        MySet<T, F>& operator = (const MySet<T, F>& other) {
            clear();
            m_root = nil;
            m_root = copyTree(other.m_root);
            return *this;
        }

        MySet<T, F>& operator = (std::initializer_list<T> l) noexcept {
            clear();
            for (const T& key : l) {
                insert(key);
            }
            return *this;
        }

        MySet<T, F>& operator = (MySet<T, F>&& rhs) noexcept {
            std::swap(m_root, rhs.m_root);
            return *this;
        }

        void insert(const T& key) {
            if (search(key) != nil) {
                return;
            }
            SetNode* x = m_root;
            SetNode* y = nil;
            SetNode* newNode = new SetNode(key);
            while (x != nil) {
                y = x;
                if (isLess(newNode->key, x->key)) {
                    x = x->left;
                }
                else {
                    x = x->right;
                }
            }
            newNode->parent = y;
            if (y == nil) {
                m_root = newNode;
            }
            else {
                if (isLess(newNode->key, y->key)) {
                    y->left = newNode;
                }
                else {
                    y->right = newNode;
                }
            }
            newNode->left = nil;
            newNode->right = nil;
            insertHelper(newNode);
        }

        void erase(const T& key) {
            erase(find(key));
        }

        void erase(iterator it) noexcept {
            SetNode* x;
            SetNode* y;
            SetNode* z = it.m_ptr;
            if (z == nil) {
                return;
            }
            bool currentColor = z->color;
            if (z->left == nil) {
                x = z->right;
                transplant(z, x);
            }
            else if (z->right == nil) {
                x = z->left;
                transplant(z, x);
            }
            else {
                y = successor(z);
                currentColor = y->color;
                x = y->right;
                if (y->parent == z) {
                    x->parent = y;
                }
                else {
                    transplant(y, x);
                    y->right = z->right;
                    y->right->parent = y;
                }
                transplant(z, y);
                y->left = z->left;
                y->left->parent = y;
                y->color = z->color;
            }
            delete z;
            if (currentColor == SetNode::Black) {
                deleteHelper(x);
            }
            --m_size;
        }

        bool contains(const T& key) const noexcept {
            return search(key) != nil;
        }

        void clear() noexcept {
            clearHelper(m_root);
        }

        bool empty() const noexcept {
            return !m_size;
        }

        size_t size() const noexcept {
            return m_size;
        }

        iterator find(const T& key) noexcept {
            return iterator(search(key));
        }

        const_iterator find(const T& key) const noexcept {
            return const_iterator(search(key));
        }

        iterator begin() noexcept {
            return iterator(leftmost(m_root));
        }

        iterator end() noexcept {
            return iterator(nil);
        }

        const_iterator begin() const noexcept {
            return cbegin();
        }

        const_iterator end() const noexcept {
            return cend();
        }

        const_iterator cbegin() const noexcept {
            return const_iterator(leftmost(m_root));
        }

        const_iterator cend() const noexcept {
            return const_iterator(nil);
        }

        reverse_iterator rbegin() noexcept {
            return std::reverse_iterator<iterator>(end());
        }

        reverse_iterator rend() noexcept {
            return std::reverse_iterator<iterator>(begin());
        }

        const_reverse_iterator rbegin() const noexcept {
            return crbegin();
        }

        const_reverse_iterator rend() const noexcept {
            return crend();
        }

        const_reverse_iterator crbegin() const noexcept {
            return std::reverse_iterator<const_iterator>(cend());
        }

        const_reverse_iterator crend() const noexcept {
            return std::reverse_iterator<const_iterator>(cbegin());
        }

    private:
        friend class MySetIterator<T, F>;

        F isLess = F();

        struct SetNode {

            static constexpr bool Red = true, Black = false;

            SetNode() :
                key(), color(Red),
                left(nullptr), right(nullptr), parent(nullptr) {}

            SetNode(bool color_) :
                key(), color(color_),
                left(nullptr), right(nullptr), parent(nullptr) {}

            SetNode(const T& key_) :
                key(key_), color(Red),
                left(nullptr), right(nullptr), parent(nullptr) {}

            SetNode(const SetNode& other) :
                key(other.key), color(other.color),
                left(other.left), right(other.right), parent(other.parent) {}

            SetNode& operator = (const SetNode& rhs) {
                key = rhs.key;
                color = rhs.color;
                left = rhs.color;
                right = rhs.right;
                parent = rhs.parent;
            }

            friend SetNode* leftmost(SetNode* node) {
                if (node == nil) {
                    return node;
                }
                while (node->left != nil) {
                    node = node->left;
                }
                return node;
            }

            friend SetNode* rightmost(SetNode* node) {
                if (node == nil) {
                    return node;
                }
                while (node->right != nil) {
                    node = node->right;
                }
                return node;
            }

            friend SetNode* predecessor(SetNode* node) {
                if (node->left != nil) {
                    return rightmost(node->left);
                }
                while (node->parent != nil && node == node->parent->left) {
                    node = node->parent;
                }
                return node->parent;
            }

            friend SetNode* successor(const SetNode* node) {
                if (node->right != nil) {
                    return leftmost(node->right);
                }
                while (node->parent != nil && node == node->parent->right) {
                    node = node->parent;
                }
                return node->parent;
            }

            const T key;
            bool color;
            SetNode* left;
            SetNode* right;
            SetNode* parent;
        };

        SetNode* m_root;
        static SetNode nilGuard;
        static SetNode * const nil;
        size_t m_size;

        void insertHelper(SetNode* z) {
            SetNode* y;
            while (z->parent->color == SetNode::Red) {
                if (z->parent == z->parent->parent->left) {
                    y = z->parent->parent->right;
                    if (y->color == SetNode::Red) {
                        z->parent->color = SetNode::Black;
                        y->color = SetNode::Black;
                        z->parent->parent->color = SetNode::Red;
                        z = z->parent->parent;
                    }
                    else {
                        if (z == z->parent->right) {
                            z = z->parent;
                            rotateLeft(z);
                        }
                        z->parent->color = SetNode::Black;
                        z->parent->parent->color = SetNode::Red;
                        rotateRight(z->parent->parent);
                    }
                }
                else {
                    y = z->parent->parent->left;
                    if (y->color == SetNode::Red) {
                            // duplicated from if above
                        z->parent->color = SetNode::Black;
                        y->color = SetNode::Black;
                        z->parent->parent->color = SetNode::Red;
                        z = z->parent->parent;
                    }
                    else {
                        if (z == z->parent->right) {
                            z = z->parent;
                            rotateRight(z);
                        }
                        z->parent->color = SetNode::Black;
                        z->parent->parent->color = SetNode::Red;
                        rotateLeft(z->parent->parent);
                    }
                }
            }
            m_root->color = SetNode::Black;
        }

        void deleteHelper(SetNode*& node) {
            while (node != m_root && node->color == SetNode::Black) {
                SetNode* helper;
                if (node == node->parent->left) {
                    helper = node->parent->right;
                    if (helper->color == SetNode::Red) {
                        helper->color = SetNode::Black;
                        node->parent->color = SetNode::Red;
                        rotateLeft(node->parent);
                        helper = node->parent->right;
                    }
                    if (helper->left->color == SetNode::Black && helper->right->color == SetNode::Black) {
                        helper->color = SetNode::Red;
                        node = node->parent;
                    }
                    else {
                        if (helper->right->color == SetNode::Black) {
                            helper->left->color = SetNode::Black;
                            helper->color = SetNode::Red;
                            rotateRight(helper);
                            helper = node->parent->right;
                        }
                        helper->color = node->parent->color;
                        node->parent->color = SetNode::Black;
                        helper->right->color = SetNode::Black;
                        rotateLeft(node->parent);
                        node = m_root;
                    }
                }
                else {
                    helper = node->parent->left;
                    if (helper->color == SetNode::Red) {
                        helper->color = SetNode::Black;
                        node->parent->color = SetNode::Red;
                        rotateRight(node->parent);
                        helper = node->parent->left;
                    }
                    if (helper->left->color == SetNode::Black && helper->right->color == SetNode::Black) {
                        helper->color = SetNode::Red;
                        node = node->parent;
                    }
                    else {
                        if (helper->left->color == SetNode::Black) {
                            helper->right->color = SetNode::Black;
                            helper->color = SetNode::Red;
                            rotateLeft(helper);
                            helper = node->parent->left;
                        }
                        helper->color = node->parent->color;
                        node->parent->color = SetNode::Black;
                        helper->left->color = SetNode::Black;
                        rotateRight(node->parent);
                        node = m_root;
                    }
                }
            }
            node->color = SetNode::Black;
        }

        void transplant(SetNode* x, SetNode* y) {
            if (x->parent == nil) {
                m_root = y;
            }
            else {
                if (x == x->parent->left) {
                    x->parent->left = y;
                }
                else {
                    x->parent->right = y;
                }
            }
            y->parent = x->parent;
        }

        void rotateLeft(SetNode* node) {
            if (node->right == nil) {
                return;
            }
            SetNode* aux = node->right;
            node->right = aux->left;
            if (aux->left != nil) {
                aux->left->parent = node;
            }
            aux->parent = node->parent;
            if (node->parent == nil) {
                m_root = aux;
            }
            else if (node == node->parent->left) {
                node->parent->left = aux;
            }
            else {
                node->parent->right = aux;
            }
            aux->left = node;
            node->parent = aux;
        }

        void rotateRight(SetNode* node) {
            if (node->left == nil) {
                return;
            }
            SetNode* aux = node->left;
            node->left = aux->right;
            if (aux->right != nil) {
                aux->right->parent = node;
            }
            aux->parent = node->parent;
            if (node->parent == nil) {
                m_root = aux;
            }
            else if (node == node->parent->right) {
                node->parent->right = aux;
            }
            else {
                node->parent->left = aux;
            }
            aux->right = node;
            node->parent = aux;
        }

        SetNode* search(const T& key) const {
            SetNode* x = m_root;
            while (x != nil) {
                if (isLess(key, x->key)) {
                    x = x->left;
                }
                else if (isLess(x->key, key)) {
                    x = x->right;
                }
                else {
                    break;
                }
            }
            return x;
        }

        void clearHelper(SetNode*& x) noexcept {
            if (x == nil) {
                return;
            }
            clearHelper(x->left);
            clearHelper(x->right);

            delete x;
            x = nil;
        }

        SetNode* copyTree(SetNode* node) {
            if (node == nil) {
                return nil;
            }
            SetNode* newNode = new SetNode(node->key, node->value, node->color);
            newNode->left = nil;
            newNode->right = nil;
            newNode->parent = nil;

            newNode->left = copyTree(node->left);
            newNode->left->parent = node;

            newNode->right = copyTree(node->right);
            newNode->right->parent = newNode;

            return newNode;
        }

};

template<typename KeyType, typename F>
typename MySet<KeyType, F>::SetNode MySet<KeyType, F>::nilGuard(MySet<KeyType, F>::SetNode::Black);

template<typename KeyType, typename F>
typename MySet<KeyType, F>::SetNode * const MySet<KeyType, F>::nil = &nilGuard;

#endif // MYSET_H
