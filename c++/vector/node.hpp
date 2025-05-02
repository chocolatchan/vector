// #ifdef __VECTOR__

namespace dyux {

    template <typename T> class node {
    friend class vector;
    private:
        typename T *data;
        dyux::node *prev;
        dyux::node *next;

        node(typename T data, dyux::node *prev, dyux::node *next);

        ~node() {}
    };
}

// #endif