/**
  *  \brief  Family type definition: container of nodes (a deque) in the order of creation plus a vector of sorted references (by the node object value) to the nodes.
  *          A family is created empty. Nodes will be added or removed afterwards.
  *          When a family is added to a Tree, it must be posible to move it. Families can't be copied or assigned since that would break all the node pointers.
  */
TREE_TEMPLATE
class TREE_CLS::Family
{
public:

  /**
  *  \brief  Iterator class specialized for tree families
  *          It allows to iterate through all the nodes of a tree family, sorted by sorting key
  *          This iterator provides access to the values of the nodes through the indirection operator or the next() method
  */
  class Iterator;


  /**
*  \brief  Constant Iterator class specialized for tree families
*          It allows to iterate through all the nodes of a tree family, sorted by sorting key
*          This iterator provides access to the values of the nodes through the indirection operator or the next() method
*/
  class ConstIterator;


  /**
    *  \brief  Default Constructor. It creates an empty family object.
    */
  Family() {}


  /**
    *  \brief  DELETED: Copy Constructor
    */
  Family(const Family&) = delete;


  /**
    *  \brief  Move Constructor. Required by the deque.emplace_back() for families in the tree, which normallyis used only with empty families, anyway
    *          It moves the nodes to the destination family and resets the sorted pointers accordingly
    */
  Family(Family&& m_fam) {
    for (auto node : m_fam._sorted_nodes) {
      _nodes.emplace_back(std::move(*node));
      _sorted_nodes.push_back(&_nodes.back());
    }
    m_fam._nodes.clear();
    m_fam._sorted_nodes.clear();
  }


  /**
    *  \brief  DELETED: Copy Assignment
    */
  Family& operator=(const Family&) const = delete;


  /**
    *  \brief  DELETED: Move Assignment
    */
  Family& operator=(Family&&) = delete;


  /**
    *  \brief  Destructor: nothing to do. The nodes will be automatically destroyed
    */
  ~Family() {}


  /**
    *  \brief  Adds a node to the family, sorting it in the vector of sorted nodes
    *  @param  new_node  A new node as rvalue to be moved into the nodes of the family
    *  @return  A reference to the inserted node
    */
  Node& addNode(Node&& new_node) {
    // Insert node in the deque
    _nodes.emplace_back(std::move(new_node));

    // Add node pointer in the right order to the _sorted_nodes vector. Search starting from the end until a value is found which is less than the inserted node value
    // First, add a new empty element at the end
    _sorted_nodes.push_back(nullptr);
    size_t pos = _sorted_nodes.size() - 1;
    while (pos > 0 && _nodes.back().value() < _sorted_nodes[pos - 1]->value()) {
      // Copy existing pointer to the next position
      _sorted_nodes[pos] = _sorted_nodes[pos - 1];
      --pos;
    }
    _sorted_nodes[pos] = &_nodes.back();

    return _nodes.back();
  }


  /**
    *  \brief  Removes a node from the family, recalculating the values of the pointers in the sorted nodes
    *          It cannot check whether the node has descendants (that must be done previously before calling this method)
    *           It will not check whether the node exists: it is assumed that has already been checked
    *  @param  old_node  A reference to the node to be removed
    */
  void removeNode(Node& old_node) {
    //    First, search the node in the sorted nodes
    auto _sorted_nodes_it = _sorted_nodes.begin();
    while ((*_sorted_nodes_it)->value() < old_node.value())
      ++_sorted_nodes_it;
    //    Now check which of the following nodes really matches the one to be removed
    while ((*_sorted_nodes_it) != &old_node)
      ++_sorted_nodes_it;
    //    Found: remove it
    _sorted_nodes.erase(_sorted_nodes_it);

    //    Second search the node in the list of nodes
    auto nodes_it = _nodes.begin();
    while (nodes_it->value().matchingKey() != old_node.value().matchingKey())
      ++nodes_it;
    //      Iterator is pointing to the found node: delete it
    _nodes.erase(nodes_it);
  }


  /**
    *  \brief  Returns the number of nodes in the family
    *  @return  The number of node in the family
    */
  size_t size() const { return _nodes.size(); }


  /**
   *  \brief  Returns the iterator to all the nodes in the tree
   *  @return  An iterator
   */
  Iterator begin() { return Iterator(_sorted_nodes); }


  /**
   *  \brief  Returns the iterator to all the nodes in the tree
   *  @return  An iterator
   */
  ConstIterator begin() const { return ConstIterator(_sorted_nodes); }


  /**
    *  \brief  Returns the list of sorted pointers to the nodes
    *  @return  A reference to the vector with the sorted pointers
    */
  const std::vector<Node*>& sortedNodes() const { return _sorted_nodes; }


  /**
    *  \brief  Index operator: returns the node at the specified position in the list of nodes
    *  @param  pos  Position of the node in the list of nodes
    *  @return  A reference to the node
    *  @throw  out_of_range  If the position is higher than the size of the deque of nodes
    */
  Node& operator[](size_t pos) {
    if(pos >= _nodes.size())
      throw std::out_of_range("Invalid position");
    return _nodes[pos];
  }

  
  /**
    *  \brief  Index operator: returns the node at the specified position in the list of nodes
    *  @param  pos  Position of the node in the list of nodes
    *  @return  A reference to the node
    *  @throw  out_of_range  If the position is higher than the size of the deque of nodes
    */
  const Node& operator[](size_t pos) const {
    if (pos >= _nodes.size())
      throw std::out_of_range("Invalid position");
    return _nodes[pos];
  }


protected:

  /**
    *  \brief  List of nodes in the order of insertion, stored as a deque to avoid copying
    */
  std::deque<Node> _nodes;


  /**
    *  \brief  Sorted list of pointers to the nodes, according to the sorting key of the node values
    */
  std::vector<Node*> _sorted_nodes;
};

