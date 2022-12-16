#ifndef MYLIST_H
#define MYLIST_H

#include <cstddef>
#include "MyIterators.h"

template<class T>
class MyList {
    public:
        MyList() {}

    private:
        struct Node {
            T data;

        };
};

#endif // MYLIST_H
