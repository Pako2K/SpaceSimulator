#ifndef M_TREE_H
#define M_TREE_H

#include <type_traits>
#include <deque>
#include <vector>
#include <map>
#include <stdexcept>
#include <utility>
#include <ostream>
#include <string>
#include <memory>

#include <logger.h>
#include <collections/unique_sortable.h>


namespace tree
{
  /**
    *  \brief  This class represents a tree of modifiable objects. Objects MUST BE UNIQUE within the tree. Sibling nodes will be sorted according to a sorting key.
    *          In order to guarantee the uniqueness of the objects, as well as the ordering of sibling nodes, node values must derive from the type "UniqueSortable".
    *          This will also allow to be able to modify these objects wihout breaking the uniqueness and ordering of the objects in the tree.
    *          The Tree takes ownership of the objects passed as values for the nodes, and will store them in the heap. 
    *          As a consequence NO DOWNCASTING will be possible anymore, if an object derived from type OBJ_T is added to the tree.
    *          The tree has a single root parent. Each child node has a single parent, and each parent can have [0..n] children.
    *          The tree is represented by a deque of families. Each Family is a deque of Nodes with the same parent Node. 
    *          The order is stored in a vector of sorted references to the objects in the family
    *
    *          Template parameters:
    *             - OBJ_T -> the type of values to be stored in the tree (must derive from "UniqueSortable")
    */
  template <class OBJ_T> 
  class MTree
  {
    static_assert(!std::is_fundamental<OBJ_T>::value, "OBJ_T cannot be a fundamental type");
    static_assert(std::is_base_of<utils::UniqueSortable<typename OBJ_T::match_key_type, typename OBJ_T::sort_key_type, typename OBJ_T::sort_func>, OBJ_T >::value, "OBJ_T must be derived from UniqueSortable");
  protected:

    /**
      *  \brief  Node Class. Represents a node in a tree
      *          The node has a value, a pointer to the parent Node and the index of the descendant family in the tree deque of families
      *          Since only the tree has access to this class (struct) all the elemnts are public
      */
    class Node;

    /**
      *  \brief  Family type definition: list of nodes in the order of creation plus a sorted list (using the node object sorting key) of references to the nodes.
      */
    class Family;


  public:

    /**
      *  \brief  Iterator class specialized for trees
      *          It allows to iterate through all the nodes of a tree, sorted by key
      */
    class Iterator;

    /**
      *  \brief  Iterator class specialized for trees
      *          It allows to iterate through all the nodes of a tree, sorted by key
      */
    class ConstIterator;


    /**
      *  \brief  Default constructor: creates an empty tree 
      */
    MTree() : _size{ 0 } {
      DebugLog("TREE CONSTRUCTED");
    }

    /**
      *  \brief  Move Constructor: creates a tree with root node specified by a right value
      *  @param  the_root  The value of the root element
      */
    MTree(OBJ_T&& the_root) : MTree() { root(std::move(the_root)); }

    /**
      *  \brief  Constructor: creates a tree with root node specified via a pointer. The tree takes ownership of the object so the original one gets deleted
      *  @param  the_root  The value of the root element
      */
    MTree(OBJ_T* the_root) : MTree() { root(the_root); }

    /**
      *  \brief  DELETED: Copy Constructor: since the values of the nodes must be unique
      */
    MTree(const MTree&) = delete;

    /**
      *  \brief  DELETED: Move Constructor: since all the pointers should be recalculated
      */
    MTree(MTree&&) = delete;

    /**
      *  \brief  DELETED: Assignment operator:  since the values of the nodes must be unique
      */
    MTree& operator=(const MTree&) = delete;

    /**
      *  \brief  DELETED: Assignment operator (move):  since all the pointers should be recalculated
      */
    MTree& operator=(MTree&&) = delete;

    /**
      *  \brief  Default destructor
      */
    ~MTree() { 
      DebugLog("TREE DESTROYED");
    };

    /**
      *  \brief  Set the root node from a right value
      *  @param  root  The root node value
      *  @throw  logic_error  If the tree already has a root
      */
    void root(OBJ_T&& the_root);

    /**
      *  \brief  Set the root element from a pointer.
      *  @param  root  The root node value
      *  @throw  logic_error  If the tree already has a root
      */
    void root(OBJ_T* the_root) { root(std::move(*the_root)); delete the_root; }

    /**
      *  \brief  Add a child node to an existing node with the specified value's key 
      *  @param  obj  The new child node value (given as a right value)
      *  @param  parent_key  The parent node value's key
      *  @throw  invalid_argument  If the parent does not exist
      */
    void addNode(OBJ_T&& obj, const typename OBJ_T::match_key_type& parent_key);

    /**
      *  \brief  Add a child node to am existing node with the specified value's key
      *  @param  obj  The new child node value (given as a pointer)
      *  @param  parent_key  The parent node value's key
      *  @throw  invalid_argument  If the parent does not exist
      */
    void addNode(OBJ_T* obj, const typename OBJ_T::match_key_type& parent_key) { addNode(std::move(*obj), parent_key); delete obj; }

    /**
      *  \brief  Removes a node with the given value key from the tree. If the flag is true then removes all the descendant nodes. Otherwise only a final (leaf) node can be removed.
      *          If the descendant family and the node's family itself are empty, thy will not be removed, since that would lead to a cascade
      *          move of families in the tre and the nodes they contain, destroying all the node pointers
      *  @param  key  The value key of the node to be removed
      *  @param  with_desc  If true all the descendants will be removed as well.
      *  @throw  invalid_argument  If the node does not exist or if the flag is false and the node is not a "leaf"
      */
    void removeNode(const typename OBJ_T::match_key_type& key, bool with_desc = false);

    /**
      *  \brief  Returns the number of elements in the tree
      *  @return  The number of elements in the tree
      */
    size_t size() const { return _size; }


    /**
      *  \brief  Returns the iterator to all the nodes in the tree
      *  @return  An iterator
      */
    Iterator begin() { return Iterator(_nodes_index); }


    /**
      *  \brief  Returns the iterator to all the nodes in the tree
      *  @return  A constant iterator
      */
    ConstIterator begin() const { return ConstIterator(_nodes_index); }


    /**
      *  \brief  Retrieves the root node value
      *  @return  The root value
      *  @throw  out_of_range  If the root does not exist
      */
    OBJ_T& root();

    /**
      *  \brief  Retrieves the root node value
      *  @return  The root value
      *  @throw  out_of_range  If the root does not exist
      */
    const OBJ_T& root() const;

    /**
      *  \brief  Get the parent node value of a node with a specified value's key 
      *  @param  obj  The node value key
      *  @return  The node's parent node value
      *  @throw  invalid_argument  If there is no node with this value's key or if the node is the root node
      */
    OBJ_T& parent(const typename OBJ_T::match_key_type& key);

    /**
      *  \brief  Get the parent node value of a node with a specified value's key
      *  @param  obj  The node value key
      *  @return  The node's parent node value
      *  @throw  invalid_argument  If there is no node with this value's key or if the node is the root node
      */
    const OBJ_T& parent(const typename OBJ_T::match_key_type& key) const;

    /**
      *  \brief  Get the children nodes of the node specified by the key
      *  @param  key  The node value's key
      *  @return  An iterator to all the children node values
      *  @throw  invalid_argument  If the node does not exist
      */
    typename Family::Iterator children(const typename OBJ_T::match_key_type& key);

    /**
      *  \brief  Get the children nodes of the node specified by the key
      *  @param  key  The node value's key
      *  @return  An iterator to all the children node values
      *  @throw  invalid_argument  If the node does not exist
      */
    typename Family::ConstIterator children(const typename OBJ_T::match_key_type& key) const;

    /**
      *  \brief  Get the sibling nodes of the node specified by the key
      *  @param  key  The node value's key
      *  @return  An iterator to all the siblings node values
      *  @throw  invalid_argument  If the node does not exist
      */
    typename Family::Iterator siblings(const typename OBJ_T::match_key_type& key);

    /**
      *  \brief  Get the sibling nodes of the node specified by the key
      *  @param  key  The node value's key
      *  @return  An iterator to all the siblings node values
      *  @throw  invalid_argument  If the node does not exist
      */
    typename Family::ConstIterator siblings(const typename OBJ_T::match_key_type& key) const;

    /**
      *  \brief  Gets the node value of a node with a specific value key
      *  @param  key  The node value's key
      *  @return  A reference to the object value
      *  @throw  out_of_range  If the node with this value key does not exist
      */
    OBJ_T& find(const typename OBJ_T::match_key_type& key) { return _nodes_index.at(key)->value(); }

    /**
      *  \brief  Gets the node value of a node with a specific value key
      *  @param  key  The node value's key
      *  @return  A reference to the object value
      *  @throw  out_of_range  If the node with this value key does not exist
      */
    const OBJ_T& find(const typename OBJ_T::match_key_type& key) const { return _nodes_index.at(key)->value(); }

    /**
      *  \brief  Output stream operator for displaying the tree
      *  @param  os  Output stream
      *  @param  a_tree  The tree object to be displayed
      */
    friend std::ostream& operator<< (std::ostream& os, const MTree& a_tree) {
      os << "Tree Size = " << a_tree._size << std::endl;
      a_tree.printChildren(os, 0, "");
      return os;
    }


  protected:

    /**
      *  \brief  The internal representation of the tree as a deque of Node families (a vector would invalidate pointers when getting extended)
      */
    std::deque<Family> _tree;

    /**
      *  \brief  A map to help speeding up the search of nodes by the key of its stored value
      *          It also provides an iterator to all the nodes (sorted by key)
      */
    std::map<typename OBJ_T::match_key_type, Node*> _nodes_index;

    /**
      *  \brief  The size of the tree: total number of nodes
      */
    size_t _size{ 0 };


  private:
    /**
      *  \brief  Gets the iterator to the map index of nodes
      *  @param  key  The node value's key
      *  @return An iterator to the nodes index map
      *  @throw  invalid_argument  If the node does not exist
      */
    auto searchNodeIt(const typename OBJ_T::match_key_type& key);

    /**
      *  \brief  Gets the iterator to the map index of nodes
      *  @param  key  The node value's key
      *  @return A constant iterator to the nodes index map
      *  @throw  invalid_argument  If the node does not exist
      */
    auto searchNodeIt(const typename OBJ_T::match_key_type& key) const;

    /**
      *  \brief  Gets the node with a specific value key
      *  @param  key  The node value's key
      *  @return A reference to the Node
      *  @throw  invalid_argument  If the node does not exist
      */
    Node& searchNode(const typename OBJ_T::match_key_type& key) { return *searchNodeIt(key)->second; }

    /**
      *  \brief  Gets the node with a specific value key
      *  @param  key  The node value's key
      *  @return A const reference to the Node
      *  @throw  invalid_argument  If the node does not exist
      */
    const Node& searchNode(const typename OBJ_T::match_key_type& key) const { return *searchNodeIt(key)->second; }

    /**
      *  \brief  Recursive function to print the elemnts of a given family
      *  @param  os  Output stream
      *  @param  fam_id  Index of the family which elements must be displayed
      *  @param  indentation  Indentation string used for the nodes of this family
      */
    void printChildren(std::ostream& os, size_t fam_id, std::string indentation) const;
    
  };

  #include "mtree.inl.h"

} // END NAMESPACE utils

#endif // M_TREE_H
