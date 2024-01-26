/**
  *  \brief  Iterator class specialized for trees
  *          It allows to iterate through all the nodes of a tree, sorted by key
  *          This iterator provides access to the values of the nodes through the indirection operator or the next() method
  */
TREE_TEMPLATE
class TREE_CLS::Iterator
{
public:
 
  /**
      *  \brief  Creates a Tree Iterator based on the _nodes_index Iterator (map iterator)
      *  @param  a_map  The map which the iterator is based on
      */
  Iterator(typename decltype(MTree::_nodes_index)& a_map) : _iter{ a_map.begin() }, _begin{ a_map.begin() }, _end{ a_map.end() } {}

  /**
    *  \brief  Moves the iterator to the next node
    *  @return  The value of the node pointed by the iterator before moving forward
    *  @throw  out_of_range  If the iterator is already pointing to the last node
    */
  OBJ_T& next() {
    if (hasNext())
      return (_iter++)->second->value();
    else
      throw std::out_of_range("No more nodes");
  }

  /**
    *  \brief  Check if there are nodes left in the tree
    *  @return  True if there are nodes left in the tree. Otherwise false
    */
  bool hasNext() { return _iter != _end; }
  
  /**
  *  \brief  Gets the value of the node pointed by the iterator
  *  @return  A constant reference to the node value
  */
  OBJ_T& operator*() { return _iter->second->value(); }
  
  /**
    *  \brief  Prefix unary operator ++.  Moves the pointer forward one position
    *  @return  A new iterator pointing one position forward
    */
  Iterator operator++() {
    ++_iter;
    return *this;
  }

  /**
    *  \brief  Postfix unary operator ++.  Moves the pointer forward one position
    *  @return  An iterator still pointing to the same position
    */
  Iterator operator++(int) {
    Iterator old_iter(*this);
    ++_iter;
    return old_iter;
  }
   
  /**
   *  \brief  Moves the iterator to the begininng
   */
  void rewind() { _iter = _begin; }


protected:

  /**
    *  \brief  The tree iterator, based on the index of nodes map
    */
  typename decltype(MTree::_nodes_index)::iterator _iter;

  /**
    *  \brief  Iterator pointing at the beginnig of the map
    */
  const typename decltype(MTree::_nodes_index)::iterator _begin;

  /**
    *  \brief  Iterator pointing at the end of the map
    */
  const typename decltype(MTree::_nodes_index)::iterator _end;
};




/**
  *  \brief  Constant Iterator class specialized for trees
  *          It allows to iterate through all the nodes of a tree, sorted by key
  *          This iterator provides access to the values of the nodes through the indirection operator or the next() method
  */
TREE_TEMPLATE
class TREE_CLS::ConstIterator
{
public:

  /**
      *  \brief  Creates a Tree Iterator based on the _nodes_index Iterator (map iterator)
      *  @param  a_map  The map which the iterator is based on
      */
  ConstIterator(const typename decltype(MTree::_nodes_index)& a_map) : _iter{ a_map.begin() }, _begin{ a_map.begin() }, _end{ a_map.end() } {}

  /**
    *  \brief  Moves the iterator to the next node
    *  @return  The value of the node pointed by the iterator before moving forward
    *  @throw  out_of_range  If the iterator is already pointing to the last node
    */
  const OBJ_T& next() {
    if (hasNext())
      return (_iter++)->second->value();
    else
      throw std::out_of_range("No more nodes");
  }

  /**
    *  \brief  Check if there are nodes left in the tree
    *  @return  True if there are nodes left in the tree. Otherwise false
    */
  bool hasNext() { return _iter != _end; }

  /**
  *  \brief  Gets the value of the node pointed by the iterator
  *  @return  A constant reference to the node value
  */
  const OBJ_T& operator*() { return _iter->second->value(); }

  /**
    *  \brief  Prefix unary operator ++.  Moves the pointer forward one position
    *  @return  A new iterator pointing one position forward
    */
  ConstIterator operator++() {
    ++_iter;
    return *this;
  }

  /**
    *  \brief  Postfix unary operator ++.  Moves the pointer forward one position
    *  @return  An iterator still pointing to the same position
    */
  ConstIterator operator++(int) {
    ConstIterator old_iter(*this);
    ++_iter;
    return old_iter;
  }

  /**
   *  \brief  Moves the iterator to the begininng
   */
  void rewind() { _iter = _begin; }


protected:

  /**
    *  \brief  The tree iterator, based on the index of nodes map
    */
  typename decltype(MTree::_nodes_index)::const_iterator _iter;

  /**
    *  \brief  Iterator pointing at the beginnig of the map
    */
  const typename decltype(MTree::_nodes_index)::const_iterator _begin;

  /**
    *  \brief  Iterator pointing at the end of the map
    */
  const typename decltype(MTree::_nodes_index)::const_iterator _end;
};




