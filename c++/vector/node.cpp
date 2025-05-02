#include "node.hpp"

using namespace dyux;

dyux::node(T data, dyux::node *prev, dyux::node *next) {
            this->data = data;
            this->prev = prev;
            this->next = next;
}