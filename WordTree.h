//
//  WordTree.hpp
//  HW4
//
//  Created by Nakul Khambhati on 8/10/22.
//

#ifndef WordTree_hpp
#define WordTree_hpp

#include <iostream>
#include <string>

typedef std::string WordType;
struct WordNode {
   WordType m_data;
   WordNode *m_left;
   WordNode *m_right;
    int m_count;
    WordNode(WordType data){
        m_data = data;
        m_count = 0;
        m_left = nullptr;
        m_right = nullptr;
    }
   // You may add additional data members and a constructor
}; // in WordNode
class WordTree {
   private:
      WordNode* root;
    void addRecursive(WordType v, WordNode* &curr);
    void printHelper(WordNode* curr, std::ostream& out) const;
    void distinctWordsRecursive(WordNode* curr, int &count) const;
    void totalWordsRecursive(WordNode* curr, int &count) const;
    void freeTree(WordNode* curr);
    WordNode* copyHelper(WordNode* rhs);
    
   public:
      // default constructor
    WordTree() : root(nullptr) { };
      // copy constructor
    WordTree(const WordTree& rhs);
      // assignment operator
    const WordTree& operator=(const WordTree& rhs);
      // Inserts v into the WordTree
    void add(WordType v);
      // Returns the number of distinct words / nodes
    int distinctWords() const;
          // Returns the total number of words inserted, including
          // duplicate values
    int totalWords() const;
          // Prints the WordTree
    friend std::ostream& operator<<(std::ostream &out, const WordTree& rhs);
          // Destroys all the dynamically allocated memory in the
          // tree
    ~WordTree();
    
};

#endif /* WordTree_hpp */
