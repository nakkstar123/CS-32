#include "WordTree.h"

void WordTree::addRecursive(WordType v, WordNode* &curr){
    
    if (curr == nullptr){
        WordNode* n = new WordNode(v);
        curr = n;
        curr->m_count++;
    }
    
    else if (curr->m_data == v){
        curr->m_count++;
        return;
    }
    
    else if (v < curr->m_data) addRecursive(v, curr->m_left);
    else if (v > curr->m_data) addRecursive(v, curr->m_right);
    
}

void WordTree::printHelper(WordNode* curr, std::ostream& out) const{
    if (curr == nullptr) return;
    printHelper(curr->m_left, out);
    out << curr->m_data << " " << curr->m_count << std::endl;
    printHelper(curr->m_right, out);
}

void WordTree::distinctWordsRecursive(WordNode* curr, int &count) const{
    if (curr == nullptr) return;
    
    distinctWordsRecursive(curr->m_left, count);
    count++;
    distinctWordsRecursive(curr->m_right, count);
    
    return;
    
}

void WordTree::totalWordsRecursive(WordNode* curr, int &count) const{
    if (curr == nullptr) return;
    
    totalWordsRecursive(curr->m_left, count);
    count+=curr->m_count;
    totalWordsRecursive(curr->m_right, count);
    
    return; 
}

void WordTree::freeTree(WordNode* curr){
    if (curr == nullptr) return;
    freeTree(curr->m_left);
    freeTree(curr->m_right);
    
    delete curr;
    
}

WordNode* WordTree::copyHelper(WordNode* rhs){
    
    if (rhs != nullptr)
        {
            WordNode* lhs = new WordNode(rhs->m_data);
            lhs->m_count = rhs->m_count;
            lhs->m_left = copyHelper(rhs->m_left);
            lhs->m_right = copyHelper(rhs->m_right);
            return lhs;
        }
        else
        {
            return nullptr;
        }
}

WordTree::WordTree(const WordTree& rhs){
    
    root = copyHelper(rhs.root);
    
}

const WordTree& WordTree::operator=(const WordTree& rhs){
    if (&rhs == this) return *this;
    
    freeTree(root);
    root = copyHelper(rhs.root);
    
    return *this;
    
}

void WordTree::add(WordType v){
        addRecursive(v, root);
}



int WordTree::distinctWords() const{
    
    int count = 0;
    distinctWordsRecursive(root, count);
    return count;
}

int WordTree::totalWords() const{
    int count = 0;
    totalWordsRecursive(root, count);
    return count;
}


WordTree::~WordTree(){
    freeTree(root);
}

std::ostream& operator<<(std::ostream &out, const WordTree& rhs){
    
    rhs.printHelper(rhs.root, out);
    
    return out;
}
