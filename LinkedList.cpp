//
//  LinkedList.cpp
//  HW1
//
//  Created by Nakul Khambhati on 7/1/22.
//

#include "LinkedList.h"
#include <iostream>

LinkedList::LinkedList(){
    head = nullptr;
}

LinkedList::LinkedList(const LinkedList& rhs){
    
    if (rhs.head == nullptr){
        head = nullptr;
        return;
    }
    
    head = new Node;
    head->value = rhs.head->value;
    
    Node* n = head;
    Node* p = rhs.head->next;
    
    while (p != nullptr){
        n->next = new Node;
        n = n->next;
        n->value = p->value;
        p = p->next;
        
    }
    
    n->next = nullptr;
    
}

LinkedList::~LinkedList(){
    Node* p = head;
    while (p!=nullptr){
        Node* temp = p->next;
        delete p;
        p = temp;
    }
}

const LinkedList &LinkedList::operator=(const LinkedList& rhs){
    if (&rhs == this) return *this;
    
    Node* p = head;                     // delete original list
    while (p!=nullptr){
        Node* temp = p->next;
        delete p;
        p = temp;
    }
    
    if (rhs.head == nullptr){
        head = nullptr;
        return *this;
    }
    
    head = new Node;
    head->value = rhs.head->value;
    
    Node* n = head;
    Node* q = rhs.head->next;
    
    while (q != nullptr){
        n->next = new Node;
        n = n->next;
        n->value = q->value;
        q = q->next;
        
    }
    
    n->next = nullptr;
    
    return *this;
    
    
}

void LinkedList::insertToFront(const ItemType &val){
    
    Node* p = new Node;
    p->value = val;
    p->next = head;
    head = p;
    
}

void LinkedList::printList() const {
    Node* p = head;
    while (p!=nullptr){
        std::cout << p->value << ' ';
        p = p->next;
    }
    std::cout << std::endl;
}

bool LinkedList::get(int i, ItemType& item) const{
    
    Node* p = head;
    int pos = 0;
    while (p!=nullptr){
        if (pos == i) {
            item = p->value;
            return true;
        }
        p = p->next;
        pos ++;
        
    }
    
    return false;
}

void LinkedList::reverseList(){
    
    Node* curr = head;
    Node* prev = nullptr;
    Node* next = nullptr;
    
    while (curr != nullptr){
        next = curr->next;
        curr->next = prev;
        
        prev = curr;
        curr = next;
    }
    
    head = prev;
}

void LinkedList::printReverse() const{
    
    LinkedList reverse(*this);
    reverse.reverseList();
    reverse.printList();
    
}

void LinkedList::append(const LinkedList &other){ // doesn't work, double destruction
    
    this->reverseList();
    Node* p = other.head;
    while (p!= nullptr){
        this->insertToFront(p->value);
        p = p->next;
    }
    this->reverseList();
}

void LinkedList::swap(LinkedList &other){
    Node* temp;
    temp = head;
    head = other.head;
    other.head = temp;
}

int LinkedList::size() const{
    
    int size = 0;
    Node* p = head;
    while (p!=nullptr){
        size ++;
        p = p->next;
    }
    
    return size;
    
}
