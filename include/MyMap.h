#ifndef MYMAP_H
#define MYMAP_H

#include <iterator>

template<typename KeyType, typename ValueType, typename F = std::less<KeyType>>
class MyMap;

template<typename KeyType, typename ValueType, typename F, typename DRT>
class MyMapIterator {
    private:
        friend MyMap<KeyType, ValueType, F>;
        using Node = typename MyMap<KeyType, ValueType, F>::MapNode;
        Node* m_ptr;
        Node* m_root;

    public:
        using iterator = MyMapIterator;
        using iterator_category = std::bidirectional_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = DRT;
        using reference = DRT&;
        using pointer = DRT*;

        MyMapIterator() noexcept :
            m_ptr(nullptr), m_root(nullptr) {}

        MyMapIterator(Node* ptr, Node* root) noexcept :
            m_ptr(ptr), m_root(root) {}

        MyMapIterator(const MyMapIterator& other) noexcept :
            m_ptr(other.m_ptr), m_root(other.m_root) {}

        reference operator * () noexcept {
            return m_ptr->data;
        }

        pointer operator -> () noexcept {
            return &(m_ptr->data);
        }

        iterator& operator = (const iterator& other) noexcept {
            m_ptr = other.m_ptr;
            m_root = other.m_root;
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
            if (m_ptr == MyMap<KeyType, ValueType, F>::nil) {
                m_ptr = rightmost(m_root);
            }
            else {
                m_ptr = predecessor(m_ptr);
            }
            return *this;
        }

        iterator operator -- (int) noexcept {
            iterator tmp(*this);
            --(*this);
            return tmp;
        }

        operator MyMapIterator<KeyType, ValueType, F, const DRT>() const noexcept {
            return MyMapIterator<KeyType, ValueType, F, const DRT>(m_ptr);
        }

        explicit operator bool() const {
            return m_ptr;
        }
};

template<typename KeyType, typename ValueType, typename F>
class MyMap {
    public:

        struct map_node {
            const KeyType key;
            ValueType value;
        };

        using iterator = MyMapIterator<KeyType, ValueType, F, map_node>;
        using const_iterator = MyMapIterator<KeyType, ValueType, F, const map_node>;
        using reverse_iterator = std::reverse_iterator<MyMapIterator<KeyType, ValueType, F, map_node>>;
        using const_reverse_iterator = std::reverse_iterator<MyMapIterator<KeyType, ValueType, F, const map_node>>;

        MyMap() noexcept : m_root(nil), m_size(0) {
            m_root->parent = nil;
            m_root->left = nil;
            m_root->right = nil;
        }

        ~MyMap() noexcept {
            clear();
        }

        MyMap(const MyMap<KeyType, ValueType, F>& other) noexcept : MyMap() {
            m_root = nil;
            m_root = copyTree(other.m_root);
        }

        MyMap(MyMap<KeyType, ValueType, F>&& other) noexcept {
            std::swap(m_root, other.m_root);
        }

        MyMap(std::initializer_list<map_node> l) noexcept : MyMap() {
            for (const map_node& node : l) {
                insert(node.key, node.value);
            }
        }

        MyMap<KeyType, ValueType, F>& operator = (const MyMap<KeyType, ValueType, F>& other) {
            clear();
            m_root = nil;
            m_root = copyTree(other.m_root);
            return *this;
        }

        MyMap<KeyType, ValueType, F>& operator = (std::initializer_list<map_node> l) noexcept {
            clear();
            for (const map_node& node : l) {
                insert(node.key, node.value);
            }
            return *this;
        }

        MyMap<KeyType, ValueType, F>& operator = (MyMap<KeyType, ValueType, F>&& rhs) noexcept {
            std::swap(m_root, rhs.m_root);
            return *this;
        }

        void insert(const KeyType& key, const ValueType& value) {
            if (search(key) != nil) {
                return;
            }
            ++m_size;
            MapNode* x = m_root;
            MapNode* y = nil;
            MapNode* newNode = new MapNode(key, value);
            while (x != nil) {
                y = x;
                if (isLess(newNode->data.key, x->data.key)) {
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
                if (isLess(newNode->data.key, y->data.key)) {
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

        void erase(const KeyType& key) {
            erase(find(key));
        }

        void erase(iterator it) noexcept {
            MapNode* x;
            MapNode* y;
            MapNode* z = it.m_ptr;
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
            if (currentColor == MapNode::Black) {
                deleteHelper(x);
            }
            --m_size;
        }

        bool contains(const KeyType& key) const noexcept {
            return search(key) != nil;
        }

        void clear() noexcept {
            clearHelper(m_root);
            m_size = 0;
        }

        bool empty() const noexcept {
            return !m_size;
        }

        size_t size() const noexcept {
            return m_size;
        }

        iterator find(const KeyType& key) noexcept {
            return iterator(search(key), m_root);
        }

        const_iterator find(const KeyType& key) const noexcept {
            return const_iterator(search(key), m_root);
        }

        iterator begin() noexcept {
            return iterator(leftmost(m_root), m_root);
        }

        iterator end() noexcept {
            return iterator(nil, m_root);
        }

        const_iterator begin() const noexcept {
            return cbegin();
        }

        const_iterator end() const noexcept {
            return cend();
        }

        const_iterator cbegin() const noexcept {
            return const_iterator(leftmost(m_root), m_root);
        }

        const_iterator cend() const noexcept {
            return const_iterator(nil, m_root);
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

        ValueType& operator [] (const KeyType& key) noexcept {
            if (search(key) == nil) {
                insert(key, ValueType());
            }
            MapNode* node = search(key);
            return node->data.value;
        }

    private:
        friend class MyMapIterator<KeyType, ValueType, F, map_node>;
        friend class MyMapIterator<KeyType, ValueType, F, const map_node>;

        F isLess = F();

        struct MapNode {

            static constexpr bool Red = true, Black = false;

            MapNode() :
                data(), color(Red),
                left(nullptr), right(nullptr), parent(nullptr) {}

            MapNode(bool color_) :
                data({KeyType(), ValueType()}), color(color_),
                left(nullptr), right(nullptr), parent(nullptr) {}

            MapNode(const KeyType& key_, const ValueType& value_) :
                data({key_, value_}), color(Red),
                left(nullptr), right(nullptr), parent(nullptr) {}

            MapNode(const KeyType& key_, const ValueType& value_, bool color_) :
                data({key_, value_}), color(color_),
                left(nullptr), right(nullptr), parent(nullptr) {}

            MapNode(const MapNode& other) :
                data(other.data), color(other.color),
                left(other.left), right(other.right), parent(other.parent) {}

            MapNode& operator = (const MapNode& rhs) {
                data = rhs.data;
                color = rhs.color;
                left = rhs.color;
                right = rhs.right;
                parent = rhs.parent;
            }

            friend MapNode* leftmost(MapNode* node) {
                if (node == nil) {
                    return node;
                }
                while (node->left != nil) {
                    node = node->left;
                }
                return node;
            }

            friend MapNode* rightmost(MapNode* node) {
                if (node == nil) {
                    return node;
                }
                while (node->right != nil) {
                    node = node->right;
                }
                return node;
            }

            friend MapNode* predecessor(MapNode* node) {
                if (node->left != nil) {
                    return rightmost(node->left);
                }
                while (node->parent != nil && node == node->parent->left) {
                    node = node->parent;
                }
                return node->parent;
            }

            friend MapNode* successor(const MapNode* node) {
                if (node->right != nil) {
                    return leftmost(node->right);
                }
                while (node->parent != nil && node == node->parent->right) {
                    node = node->parent;
                }
                return node->parent;
            }

            map_node data;
            bool color;
            MapNode* left;
            MapNode* right;
            MapNode* parent;
        };

        MapNode* m_root;
        static MapNode nilGuard;
        static MapNode * const nil;
        size_t m_size;

        void insertHelper(MapNode* z) {
            MapNode* y;
            while (z->parent->color == MapNode::Red) {
                if (z->parent == z->parent->parent->left) {
                    y = z->parent->parent->right;
                    if (y->color == MapNode::Red) {
                        z->parent->color = MapNode::Black;
                        y->color = MapNode::Black;
                        z->parent->parent->color = MapNode::Red;
                        z = z->parent->parent;
                    }
                    else {
                        if (z == z->parent->right) {
                            z = z->parent;
                            rotateLeft(z);
                        }
                        z->parent->color = MapNode::Black;
                        z->parent->parent->color = MapNode::Red;
                        rotateRight(z->parent->parent);
                    }
                }
                else {
                    y = z->parent->parent->left;
                    if (y->color == MapNode::Red) {
                            // duplicated from if above
                        z->parent->color = MapNode::Black;
                        y->color = MapNode::Black;
                        z->parent->parent->color = MapNode::Red;
                        z = z->parent->parent;
                    }
                    else {
                        if (z == z->parent->right) {
                            z = z->parent;
                            rotateRight(z);
                        }
                        z->parent->color = MapNode::Black;
                        z->parent->parent->color = MapNode::Red;
                        rotateLeft(z->parent->parent);
                    }
                }
            }
            m_root->color = MapNode::Black;
        }

        void deleteHelper(MapNode*& node) {
            while (node != m_root && node->color == MapNode::Black) {
                MapNode* helper;
                if (node == node->parent->left) {
                    helper = node->parent->right;
                    if (helper->color == MapNode::Red) {
                        helper->color = MapNode::Black;
                        node->parent->color = MapNode::Red;
                        rotateLeft(node->parent);
                        helper = node->parent->right;
                    }
                    if (helper->left->color == MapNode::Black && helper->right->color == MapNode::Black) {
                        helper->color = MapNode::Red;
                        node = node->parent;
                    }
                    else {
                        if (helper->right->color == MapNode::Black) {
                            helper->left->color = MapNode::Black;
                            helper->color = MapNode::Red;
                            rotateRight(helper);
                            helper = node->parent->right;
                        }
                        helper->color = node->parent->color;
                        node->parent->color = MapNode::Black;
                        helper->right->color = MapNode::Black;
                        rotateLeft(node->parent);
                        node = m_root;
                    }
                }
                else {
                    helper = node->parent->left;
                    if (helper->color == MapNode::Red) {
                        helper->color = MapNode::Black;
                        node->parent->color = MapNode::Red;
                        rotateRight(node->parent);
                        helper = node->parent->left;
                    }
                    if (helper->left->color == MapNode::Black && helper->right->color == MapNode::Black) {
                        helper->color = MapNode::Red;
                        node = node->parent;
                    }
                    else {
                        if (helper->left->color == MapNode::Black) {
                            helper->right->color = MapNode::Black;
                            helper->color = MapNode::Red;
                            rotateLeft(helper);
                            helper = node->parent->left;
                        }
                        helper->color = node->parent->color;
                        node->parent->color = MapNode::Black;
                        helper->left->color = MapNode::Black;
                        rotateRight(node->parent);
                        node = m_root;
                    }
                }
            }
            node->color = MapNode::Black;
        }

        void transplant(MapNode* x, MapNode* y) {
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

        void rotateLeft(MapNode* node) {
            if (node->right == nil) {
                return;
            }
            MapNode* aux = node->right;
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

        void rotateRight(MapNode* node) {
            if (node->left == nil) {
                return;
            }
            MapNode* aux = node->left;
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

        MapNode* search(const KeyType& key) const {
            MapNode* x = m_root;
            while (x != nil) {
                if (isLess(key, x->data.key)) {
                    x = x->left;
                }
                else if (isLess(x->data.key, key)) {
                    x = x->right;
                }
                else {
                    break;
                }
            }
            return x;
        }

        void clearHelper(MapNode*& x) noexcept {
            if (x == nil) {
                return;
            }
            clearHelper(x->left);
            clearHelper(x->right);

            delete x;
            x = nil;
        }

        MapNode* copyTree(MapNode* node) {
            if (node == nil) {
                return nil;
            }
            MapNode* newNode = new MapNode(node->data.key, node->data.value, node->color);
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

template<typename KeyType, typename ValueType, typename F>
typename MyMap<KeyType, ValueType, F>::MapNode MyMap<KeyType, ValueType, F>::nilGuard(MyMap<KeyType, ValueType, F>::MapNode::Black);

template<typename KeyType, typename ValueType, typename F>
typename MyMap<KeyType, ValueType, F>::MapNode * const MyMap<KeyType, ValueType, F>::nil = &nilGuard;

#endif // MYMAP_H
