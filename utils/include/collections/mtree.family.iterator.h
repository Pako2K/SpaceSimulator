/**
*  \brief  Iterator class specialized for tree families
*          It allows to iterate through all the nodes of a tree family, sorted by sorting key
*          This iterator provides access to the values of the nodes through the indirection operator or the next() method
*/
TREE_TEMPLATE
class TREE_CLS::Family::Iterator
{
public:

  /**
      *  \brief  Creates a Family Iterator based on the _sorted_nodes Iterator (vector iterator)
      *  @param  a_vec  The vector which the iterator is based on
      */
  Iterator(typename decltype(Family::_sorted_nodes)& a_vec) : _iter{ a_vec.begin() }, _begin{ a_vec.begin() }, _end{ a_vec.end() } {}

  /**
    *  \brief  Moves the iterator to the next node
    *  @return  The value of the node pointed by the iterator before moving forward
    *  @throw  out_of_range  If the iterator is already pointing to the last node
    */
  OBJ_T& next() {
    if (hasNext())
      return (*_iter++)->value();
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
  OBJ_T& operator*() { return (*_iter)->value(); }

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
    *  \brief  The family iterator, based on the sorted nodes
    */
  typename decltype(Family::_sorted_nodes)::iterator _iter;

  /**
    *  \brief  Iterator pointing at the beginnig of the map
    */
  const typename decltype(Family::_sorted_nodes)::iterator _begin;

  /**
    *  \brief  Iterator pointing at the end of the map
    */
  const typename decltype(Family::_sorted_nodes)::iterator _end;
};



/**
*  \brief  Constant Iterator class specialized for tree families
*          It allows to iterate through all the nodes of a tree family, sorted by sorting key
*          This iterator provides access to the values of the nodes through the indirection operator or the next() method
*/
TREE_TEMPLATE
class TREE_CLS::Family::ConstIterator
{
public:

  /**
      *  \brief  Creates a Family Iterator based on the _sorted_nodes Iterator (vector iterator)
      *  @param  a_vec  The vector which the iterator is based on
      */
  ConstIterator(const typename decltype(Family::_sorted_nodes)& a_vec) : _iter{ a_vec.begin() }, _begin{ a_vec.begin() }, _end{ a_vec.end() } {}

  /**
    *  \brief  Moves the iterator to the next node
    *  @return  The value of the node pointed by the iterator before moving forward
    *  @throw  out_of_range  If the iterator is already pointing to the last node
    */
  const OBJ_T& next() {
    if (hasNext())
      return (*_iter++)->value();
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
  const OBJ_T& operator*() { return (*_iter)->value(); }

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
    *  \brief  The family iterator, based on the sorted nodes
    */
  typename decltype(Family::_sorted_nodes)::const_iterator _iter;

  /**
    *  \brief  Iterator pointing at the beginnig of the map
    */
  const typename decltype(Family::_sorted_nodes)::const_iterator _begin;

  /**
    *  \brief  Iterator pointing at the end of the map
    */
  const typename decltype(Family::_sorted_nodes)::const_iterator _end;
};
