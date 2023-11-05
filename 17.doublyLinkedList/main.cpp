#include <iostream>

class Node {
public:
  int value;
  Node *prev;
  Node *next;

  Node(int val, Node *prevNode = nullptr, Node *nextNode = nullptr)
    : value(val), prev(prevNode), next(nextNode) {}
};

class DoublyLinkedList {
private:
  Node *head;
  Node *tail;

public:
  DoublyLinkedList() : head(nullptr), tail(nullptr) {}

  void append(int value, int prevValue, int nextValue) {
    Node *prevNode = findNodeByValue(prevValue);
    Node *nextNode = findNodeByValue(nextValue);
    Node *newNode = new Node(value, prevNode, nextNode);

    if (!head) {
      head = tail = newNode;
    } else {
      if (prevNode) {
        prevNode->next = newNode;
      } else {
        newNode->prev = nullptr;
        head = newNode;
      }

      if (nextNode) {
        nextNode->prev = newNode;
      } else {
        newNode->next = nullptr;
        tail = newNode;
      }
    }
  }

  Node *findNodeByValue(int value) {
    Node *current = head;
    while (current) {
      if (current->value == value) {
        return current;
      }
      current = current->next;
    }
    return nullptr;
  }

  void remove(Node *node) {
    if (node->prev) {
      node->prev->next = node->next;
    } else {
      head = node->next;
    }

    if (node->next) {
      node->next->prev = node->prev;
    } else {
      tail = node->prev;
    }

    delete node;
  }

  void display() {
    Node *current = head;
    while (current) {
      if (current->prev) {
        std::cout << current->prev->value << " <- ";
      } else {
        std::cout << " <- ";
      }

      if (current->next) {
        std::cout << current->value << " -> " << current->next->value << std::endl;
      } else {
        std::cout << current->value << " -> " << std::endl;
      }

      current = current->next;
    }
    std::cout << "nullptr" << std::endl;
  }

  ~DoublyLinkedList() {
    Node *current = head;
    while (current) {
      Node *next = current->next;
      delete current;
      current = next;
    }
  }
};

int main() {
  DoublyLinkedList dll;

  dll.append(1, 0, 2); // 在没有前一个元素的情况下添加第一个元素
  dll.append(2, 0, 3); // 在没有后一个元素的情况下添加第二个元素
  dll.append(3, 2, 4);
  dll.append(4, 3, 0);

  dll.display(); // 输出：1 <-> 2 <-> 3 <-> 4 <-> nullptr

  return 0;
}
