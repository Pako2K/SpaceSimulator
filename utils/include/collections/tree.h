#ifndef TREE_H
#define TREE_H

#include <algorithm>
#include <deque>
#include <set>
#include <stdexcept>
#include <string>
#include <iostream>

#include <logger.h>

namespace tree
{
  /**
    *  \brief  This class represents a tree of references to objects. Objects MUST BE UNIQUE within the tree.
    *          The tree has a single root parent. Each child node has a single parent, but each parent can have [0..n] children.
    *          Children with the same parent (siblings) can be sorted according to a given functor. By default the operation used is "<"
    *          Since it is check uniqueness of the node, the referenced object class must support the comparison operation "==" or provide a matching functor
    *          This tree is optimized to reduced the used storage, since it keeps only references to the objects.
    *          For that reason, objects of this class can easily "be broken" or get inconsistent when the referenced objects go out of scope or are modified.
    *          The tree is represented by a deque of families. Each Family is a set of Nodes with the same parent Node.
    *          IMPORTANT: Since the Nodes are stored in a set, the contained objects cannot be modified, and all the methods return a const reference to the Obejct(s)
    *
    *          Template parameters:
    *             - TT -> the type of values to be stored in the tree
    *             - SortingBy -> the functor used to sort objects of type TT within a family of siblings
    *             - MatchingBy -> the functor used to check uniqueness of the objects of type TT within the tree
    */
  template <typename TT, class SortingBy = std::less<TT>, class MatchingBy = std::equal_to<TT>>
  class Tree
  {
  public:
    /**
      *  \brief  Iterator class specilized for trees
      *          It allows to iterate through all the sibling nodes of a family
      */
    class Iterator;

    /**
      *  \brief  Default constructor: creates an empty tree
      */
    Tree();

    /**
      *  \brief  Constructor: creates a tree with root node
      *  @param  the_root  The value of the root element
      */
    Tree(const TT& the_root);

    /**
      *  \brief  DELETED Constructor: creates a tree with root node specified by a right value
      *  @param  the_root  The value of the root element
      */
    Tree(const TT&& the_root) = delete;

    /**
      *  \brief  Constructor: creates a new tree based on the given tree and a node value in that tree
      *  @param  the_tree  The tree to be copied or partially copied
      *  @param  obj  The node value which will be the root node
      *  @throw  invalid_argument  If the node does not exist in the provided tree
      */
    Tree(const Tree& a_tree, const TT& obj);

    /**
      *  \brief  Copy Constructor: creates a new tree based on the given tree
      *  @param  the_tree  The tree to be copied
      */
    Tree(const Tree& the_tree);

    /**
      *  \brief  Default destructor
      */
    ~Tree();

    /**
      *  \brief  Assignment operator: creates a new tree based on the given tree
      *  @param  the_tree  The tree to be copied
      */
    Tree& operator=(const Tree& the_tree);
    
    /**
      *  \brief  Returns the number of elements in the tree
      *  @return  The number of elements in the tree
      */
    size_t size() const { return _size; }
    
    /**
      *  \brief  Retrieves the root element
      *  @return  The root element
      *  @throw  out_of-range  If the root does not exist 
      */
    const TT& root() const;

    /**
      *  \brief  Get the node's parent node value
      *  @param  obj  The node value
      *  @return The element's parent node value
      *  @throw  invalid_argument  If the node does not exist or if the node is the root node
      */
    const TT& parent(const TT& obj) const;

    /**
      *  \brief  Get the node's children 
      *  @param  obj  The node value
      *  @return An iterator to all the children element values
      *  @throw  invalid_argument  If the node does not exist
      */
    Iterator children(const TT& parent) const;

    /**
      *  \brief  Get the node's siblings, including itself
      *  @param  obj  The node value
      *  @return An iterator to all the sibling element values
      *  @throw  invalid_argument  If the node does not exist
      */
    Iterator siblings(const TT& obj) const;

    /**
      *  \brief  Set the root element. If a root value already exists, no changes are done
      *  @param  root  The root element value
      */
    void root(const TT& root);

    /**
      *  \brief  DELETED MEMBER: Set the root element with a righ value
      *  @param  root  The root element value
      */
    void root(const TT&& root) = delete;

    /**
      *  \brief  Add a node to a parent element
      *  @param  obj  The new element value
      *  @param  parent  The element's parent node value
      *  @throw  invalid_argument  If the node already exists in the tree, or if the parent does not exist
      */
    void addNode(const TT& obj, const TT& parent);
    
    /**
      *  \brief  Removes a node from the tree. If the flag is true then removes all the descendant nodes. Otherwise only a final (leaf) node can be removed.
      *  @param  obj  The value of the element to be removed
      *  @param  with_desc  FLag: if true all the descendants will be removed as well.
      *  @throw  invalid_argument  If the node does not exist or if the flag is false and the node is not a "leaf"
      */
    void removeNode(const TT& obj, bool with_desc = false);


  protected:
    /**
      *  \brief  Node Class. Represents a node in a tree
      *          The node has a value, a pointer to the parent Node and the index of the descendant family in the tree deque of families
      *          Since only the tree has access to this class (struct) all the elemnts are public
      */
    class Node;

    /**
      *  \brief  Sorting Functor used to sort nodes according to the provided tree template functor
      */
    class SortingNode;

    /**
      *  \brief  Matching Functor used to check whether objects are equivalent
      */
    MatchingBy match;

    /**
      *  \brief  Family type definition: a set of nodes sorted according to the tree template functor parameter
      */
    using Family = std::set<Node, SortingNode>;

    /**
      *  \brief  The internal representation of the tree as a deque of Node families (a vector would invalidate pointers when getting extended)
      */
    std::deque<Family> _tree;

    /**
      *  \brief  The size of the tree: total number of nodes
      */
    size_t _size;
    
    /**
      *  \brief  Output stream operator for displaying the tree
      *  @param  os  Output stream
      *  @param  a_tree  The tree object to be displayed
      */
    friend std::ostream& operator<< (std::ostream& os, const Tree& a_tree) {
      os << "Tree Size = " << a_tree._size << std::endl;
      a_tree.printChildren(os, 0, "");
      return os;
    }

  private:
    /**
      *  \brief  Recursive function to print the elemnts of a given family
      *  @param  os  Output stream
      *  @param  fam_id  Index of the family which elements must be displayed
      *  @param  indentation  Indentation string used for the nodes of this family
      */
    void printChildren(std::ostream& os, size_t fam_id, std::string indentation) const;

    /**
      *  \brief  Recursive function to add children nodes to a new subtree
      *  @param  subtree  The new tree
      *  @param  node  root node
      */
    void addChildren(Tree<TT, SortingBy, MatchingBy>& subtree, const Node& node) const;

    /**
      *  \brief  Recursive function to remove nodes from a tree
      *  @param  family  the parent_node's family
      *  @param  node  parent_node
      */
    void removeChildren(Family& family, const Node& node);
        
  }; // END CLASS Tree

  #include "tree.inl.h"
  
} // END NAMESPACE tree

#endif TREE_H