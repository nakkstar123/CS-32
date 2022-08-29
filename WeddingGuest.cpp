//
//  WeddingGuest.cpp
//  wedding
//
//  Created by Nakul Khambhati on 7/10/22.
//

#include "WeddingGuest.h"

// initialize the list by setting both head and tail to null pointers
WeddingGuest::WeddingGuest(){
    head = nullptr;
    tail = nullptr;
}

// destruct all dynamically allocated nodes
WeddingGuest::~WeddingGuest(){
    Node* p = head;
    while (p!= nullptr){
        Node* temp = p->next; // temporary pointer to remember the next while current is being destructed
        delete p;
        p = temp;
    }
}

// copy constructor
WeddingGuest::WeddingGuest(const WeddingGuest& rhs){
    
    if (rhs.head == nullptr){ // check for empty list
        head = nullptr;
        return;
    }
    
    head = new Node; // copy head values
    head->value = rhs.head->value;
    head->lastName = rhs.head->lastName;
    head->firstName = rhs.head->firstName;
    
    Node* n = head;
    Node* p = rhs.head->next;
    
    while (p != nullptr){ // while head's next is not a nullptr, we can keep traversing
        n->next = new Node;
        n = n->next;
        n->value = p->value;
        n->lastName = p->lastName;
        n->firstName = p->firstName;
        
        p = p->next;
        
    }
    
    tail = n;
    tail->next = nullptr; // set the end to a nullptr
    
}

const WeddingGuest &WeddingGuest::operator=(const WeddingGuest& rhs){
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
    head->lastName = rhs.head->lastName;
    head->firstName = rhs.head->firstName;
    
    Node* n = head;
    Node* q = rhs.head->next;
    
    while (q != nullptr){
        n->next = new Node;
        n = n->next;
        n->value = q->value;
        n->lastName = q->lastName;
        n->firstName = q->firstName;
        q = q->next;
        
    }
    
    tail = n;
    tail->next = nullptr;
    
    return *this;
    
    
}

// check for empty list
bool WeddingGuest::noGuests() const{
    return (head == nullptr);
}

// count the number of items in the list
int WeddingGuest::guestCount() const{
    int count = 0;
    Node* p = head;
    while (p != nullptr){
        count ++; // will increment by 1 every time p can advance (i.e # of items)
        p = p->next;
    }
    
    return count;
    
}

bool WeddingGuest::inviteGuest(const std::string& firstName, const std::string& lastName, const GuestType& value){ // add a guest alphabetically, but if the name is already there, return false
    
    Node* p = head;
    
    Node* n = new Node; // create the node that needs to be added
    n->value = value;
    n->firstName = firstName;
    n->lastName = lastName;
    
    if (noGuests()) { // if its empty before, link head and tail to the node
        
        n->next = nullptr;
        n->prev = nullptr;
        
        head = n;
        tail = n;

        return true;
    }
    
    while (p!= nullptr){
        if (n->lastName > p->lastName) p = p->next; // traverse list till it finds position based on last name
        else if (n->lastName == p->lastName) {
            if (n->firstName > p->firstName) p = p->next; // if equal last name, traverse till it finds position of first name
            else if (n->firstName == p->firstName){
                delete n;
                return false;} // if that's equal too, return false
            else break; // once position has been found, break the loop
        }
        else break; // once position has been found, break the loop
    }
    
    // p here points to the node after its correct spot
    
    if (p == nullptr){ // if nullptr, add it to the end of the list
        tail->next = n;
        n->next = nullptr;
        n->prev = tail;
        tail = n;
    }
    
    else{ // otherwise add it between p->prev and p
        Node* after = p;
        Node* before = after->prev;
        
        if (before == nullptr) head = n; // in case its being added to the front of the list
        else before->next = n;
        
        n->prev = before;
        n->next = after;
        after->prev = n;
    }
    
    return true;
    
}

// if a name is there, alter its value
bool WeddingGuest::alterGuest(const std::string& firstName, const std::string& lastName, const GuestType & value){
    
    Node* p = head;
    
    while (p!=nullptr && (p->lastName!=lastName || p->firstName != firstName)){ // breaks if p == nullptr OR (lastName match AND firstName match)
        p = p->next;
    }
    
    if (p == nullptr) return false; // broke while loop because no match
    p->value = value;
    return true;
    
}

// if a name is there, alter it otherwise add it

bool WeddingGuest::inviteOrAlter(const std::string& firstName, const std::string& lastName, const GuestType& value){
    
    Node* p = head;
    
    while (p!=nullptr && (p->lastName!=lastName || p->firstName != firstName)){ // breaks if p == nullptr OR (lastName match AND firstName match)
        p = p->next;
    }
    
    if (p == nullptr) return inviteGuest(firstName, lastName, value); // broke while loop because no match
    p->value = value;
    return true;
}

// cross off a name if it's in the list
bool WeddingGuest::crossGuestOff(const std::string& firstName, const std::string& lastName){
   
    Node* p = head;
    
    while (p!=nullptr && (p->lastName!=lastName || p->firstName != firstName)){ // breaks if p == nullptr OR (lastName match AND firstName match)
        p = p->next;
    }
    
    if (p == nullptr) return false; // if its the nullptr, name isn't in the list so return false
    
    if (p == head){ // if its being added to the front, deletion needs to account for bad access errors
        
        head = p->next;
        
        if (head!=nullptr) head->prev = nullptr;
        
        if (guestCount() == 1) tail = nullptr; // otherwise tail will continue pointing at the item to be deleted
    }
    
    else if (p == tail){ // else if, so there's more than one otherwise p would be head
        tail = p->prev;
        tail->next = nullptr;
    }
    
    else { // neither the first, nor the last (at least 3 items)
        p->next->prev = p->prev;
        p->prev->next = p->next;
    }
    
    delete p; // finally dynamically deallocate the memory previously held
    
    return true;
}

// check if a certain guest is on the list

bool WeddingGuest::verifyGuestOnTheList(int i, std::string& firstName, std::string& lastName, GuestType & value) const{
    if (i < 0 || i >= guestCount()) return false; // if its out of range
    
    // get p at position i:
    
    Node* p = head;
    
    if (p == nullptr) return false; // if its empty a guest can't be on it
    
    for (int n = 0; n < i; n++){
        p = p->next;
    } // otherwise move p up to the i-th position
    
    firstName = p->firstName;
    lastName = p->lastName;
    value = p->value; // set values accordingly
    
    return true;
    
}

// check if a certain guest is invited to the wedding
bool WeddingGuest::invitedToTheWedding(const std::string& firstName, const std::string& lastName) const{
    
    Node* p = head;
    
    while (p!=nullptr && (p->lastName!=lastName || p->firstName != firstName)){ // breaks if p == nullptr OR (lastName match AND firstName match)
        p = p->next;
    }
    
    if (p == nullptr) return false; // if it broke the while loop because p became nullptr, then the name isn't on the list
    return true;
    
}

// if the person is there, then return their value accordingly

bool WeddingGuest::matchInvitedGuest(const std::string& firstName, const std::string& lastName, GuestType& value) const{
    
    Node* p = head;
    
    while (p!=nullptr && (p->lastName!=lastName || p->firstName != firstName)){ // breaks if p == nullptr OR (lastName match AND firstName match)
        p = p->next;
    }
    
    if (p == nullptr) return false;
    value = p->value; // if the guest is there, change their value
    return true;
    
}


void WeddingGuest::swapWeddingGuests(WeddingGuest& other){
    Node* temp;
    
    temp = head;
    head = other.head;
    other.head = temp;
    
    temp = tail;
    tail = other.tail;
    other.tail = temp;
    
    //similar to HW1 but here need to swap the tail as well
}


// combines 2 lists but duplicate names are only added if they have the same value as well
bool joinGuests(const WeddingGuest & odOne,
                const WeddingGuest & odTwo,
                WeddingGuest & odJoined){
    
    WeddingGuest temp; // to account for aliasing
    
    bool return_val = true;
    
    for (int i = 0; i < odOne.guestCount(); i++)
    {
        
        std::string first;
        std::string last;
        GuestType val_odOne;
        odOne.verifyGuestOnTheList (i, first, last, val_odOne);
        
        GuestType val_odTwo;
        
        if (odTwo.matchInvitedGuest(first, last, val_odTwo)){ // checks if names match, if they do, val_odTwo is the corresponding value
            if (val_odTwo == val_odOne){ // ok because it only enters this if names match so val_odTwo will never be garbage.
                temp.inviteGuest(first, last, val_odTwo);
            }
            
            else {return_val = false;}
        }
        else temp.inviteGuest(first, last, val_odOne); // if that name isn't there, add it to odJoined
    }
    
    
    // now do the same thing traversing odTwo
//    /////////// too inefficient but idk what else to do
    
    for (int i = 0; i < odTwo.guestCount(); i++)
    {
        
        std::string first;
        std::string last;
        GuestType val_odTwo;
        odTwo.verifyGuestOnTheList (i, first, last, val_odTwo);
//        std::cout << first << " " << last << " " << val << std::endl;
        
        GuestType val_odOne;
        
        if (odOne.matchInvitedGuest(first, last, val_odOne)){ // checks if names match, if they do, val_odTwo is the corresponding value
            if (val_odTwo == val_odOne){ // ok because it only enters this if names match so val_odTwo will never be garbage.
                temp.inviteGuest(first, last, val_odOne);
            }
            
            else {return_val = false;} // ideally should never reach this
        }
        else temp.inviteGuest(first, last, val_odTwo); // if that name isn't there, add it to odJoined
    }
    
    odJoined = temp; // to account for aliasing
    
    return return_val;
    
}

// adds to another list matches from the first
// can either specify a full name, a first name or a last name (or nothing) for a match to be considered valid

void attestGuests (const std::string& fsearch,
                   const std::string& lsearch,
                   const WeddingGuest& odOne,
                   WeddingGuest& odResult){
    
    
    WeddingGuest temp;
    
    for (int i = 0; i < odOne.guestCount(); i++)
    {
        
        std::string first;
        std::string last;
        GuestType val;
        odOne.verifyGuestOnTheList (i, first, last, val);
        
        if (fsearch == "*" && lsearch == "*") temp = odOne; // if no criteria, copy the full list
        
        else if (fsearch == "*"){ // implies lsearch != "*"
            if (last == lsearch) temp.inviteGuest(first, last, val); // if first is a wild card, only add if last matches
        }
        
        else if (lsearch == "*"){ // implies fsearch != "*"
            if (first == fsearch) temp.inviteGuest(first, last, val); // if last is a wild card, only add if first matches
        }
        
        // if neither fsearch or lsearch are *
        else if (first == fsearch && last == lsearch) temp.inviteGuest(first, last, val);
    }
    
    odResult = temp; // to account for aliasing
    
}
