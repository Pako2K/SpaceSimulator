#define TREE_TEMPLATE template <typename TT, class SortingBy, class MatchingBy>
#define TREE_CLS tree::Tree<TT, SortingBy, MatchingBy>
/// *************************************************************************************************************************************************************** ///
/// *** CLASS Tree::Iterator ************************************************************************************************************************************** ///
/**
  *  \brief  Iterator class specilized for trees
  *          It allows to iterate through all the sibling nodes of a family
  */
TREE_TEMPLATE
class TREE_CLS::Iterator
{
public:
  /**
    *  \brief  Creates a Tree Iterator based on the Family Iterator
    *  @param  family_iter  The Family iterator
    *  @param  family_iter_end  Iterator to the end of the Family
    */
  Iterator(typename Family::iterator family_iter, typename Family::iterator family_iter_end) : _family_iter_begin{ family_iter }, _family_iter { family_iter }, _family_iter_end{ family_iter_end } {}

  /**
    *  \brief  Check if there are nodes left in the family
    *  @return  True if there are nodes left in the family. Otherwise false
    */
  void begin() { _family_iter = _family_iter_begin; }

  /**
    *  \brief  Check if there are nodes left in the family
    *  @return  True if there are nodes left in the family. Otherwise false
    */
  bool hasNext() const { return (_family_iter != _family_iter_end) ? true : false; }

  /**
    *  \brief  Moves the iterator to the next node
    *  @return  The value of the node pointed by the iterator before moving forward
    *  @throw  out_of_range  If the iterator is already pointing to the last node
    */
  const TT& next() {
    if (hasNext())
      return _family_iter++->object;
    else
      throw std::out_of_range("No more sibling elements");
  }

protected:
  /**
    *  \brief  The Family iterator
    */
  typename const Family::iterator _family_iter_begin;

  /**
    *  \brief  The Family iterator
    */
  typename Family::iterator _family_iter;

  /**
    *  \brief  Iterator to the end of the Family
    */
  typename const Family::iterator _family_iter_end;
};
/// *** CLASS Tree::Iterator --> END ****************************************************************************************************************************** ///
/// *************************************************************************************************************************************************************** ///


/// *************************************************************************************************************************************************************** ///
/// *** CLASS Tree::Node ****************************************************************************************************************************************** ///
/**
  *  \brief  Node Class. Represents a node in a tree
  *          The node has a value, a pointer to the parent Node and the index of the descendant family in the tree deque of families
  *          Since only the tree has access to this class (struct) all the elemnts are public
  */
TREE_TEMPLATE
class TREE_CLS::Node 
{
public:
  /**
    *  \brief  The value of the node. Is a reference to an existing object
    */
  const TT& object;

  /**
    *  \brief  Pointer to the parent Node
    */
  const Node* parent_node{ nullptr };

  /**
    *  \brief  Index of the descendant family in the tree deque of families
    */
  size_t desc_family_id{ 0 };


  /**
    *  \brief  Default constructor (required since the value of the node is areference to an existing object)
    *  @param  obj  The value of the node. Is a reference to an existing object
    */
  Node(const TT& obj) : object{ obj } {
  DebugLog("NODE CONSTRUCTED");
  }

  /**
    *  \brief  Copy constructor. Defined just for debugging purposes
    *  @param  node  The node to be copied
    */
  Node(const Node& node) : object{ node.object } {
    parent_node = node.parent_node;
    desc_family_id = node.desc_family_id;
    DebugLog("NODE COPIED");
  }


  /**
    *  \brief  Default destructor
    */
  ~Node() {
    DebugLog("NODE DESTRUCTED");
  }
};
/// *** CLASS Tree::Node --> END ********************************************************************************************************************************** ///
/// *************************************************************************************************************************************************************** ///



/// *************************************************************************************************************************************************************** ///
/// *** CLASS Tree::SortingNode *********************************************************************************************************************************** ///
/**
  *  \brief  Sorting Functor used to sort nodes according to the provided tree template functor (which operates on the Node object)
  */
TREE_TEMPLATE
class TREE_CLS::SortingNode
{
public:
  /**
    *  \brief  Function operator encapsulating the SortingBy Functor
    *  @return  The result of the SortingBy functor execution: true or false
    */
  bool operator() (const Node& nl, const Node& nr) const {
    return _sorter(nl.object, nr.object);
  }
protected:
  /**
    *  \brief  SortingBy Functor instance
    */
  SortingBy _sorter;
};
/// *** CLASS Tree::SortingNode --> END *************************************************************************************************************************** ///
/// *************************************************************************************************************************************************************** ///



/// *************************************************************************************************************************************************************** ///
/// *** CLASS Tree --> DEFINITIONS ******************************************************************************************************************************** ///
/*   Tree()   */
/**************/
TREE_TEMPLATE
TREE_CLS::Tree() : _size{ 0 } {
  // Create empty root level and family
  Family family;
  _tree.push_back(family);
  DebugLog("TREE CONSTRUCTED");
}

/*   Tree(const TT& the_root)   */
/********************************/
TREE_TEMPLATE
TREE_CLS::Tree(const TT& the_root) : Tree() {
  root(the_root);
}

/*   Tree(const Tree& the_tree)   */
/**********************************/
TREE_TEMPLATE
TREE_CLS::Tree(const Tree& the_tree) : Tree() {
  root(the_tree.root());
  the_tree.addChildren(*this, *(the_tree._tree[0].begin()));
  DebugLog("TREE COPIED");
}

/*   Tree(const Tree& a_tree, const TT& obj)  */
/**********************************************/
TREE_TEMPLATE
TREE_CLS::Tree(const Tree& a_tree, const TT& obj) : Tree() {
  for (auto& family : a_tree._tree) {
    for (auto& node : family) {
      if (match(node.object, obj)) {
        root(obj);
        a_tree.addChildren(*this, node);
        return;
      }
    }
  }
  throw std::invalid_argument("Node not found");
}


/*   ~Tree()   */
/***************/
TREE_TEMPLATE
TREE_CLS::~Tree() {
  DebugLog("TREE DESTROYED");
}

/*   Tree& operator=(const Tree& the_tree)   */
/**********************************/
TREE_TEMPLATE
TREE_CLS& TREE_CLS::operator=(const Tree& the_tree) {
  root(the_tree.root());
  the_tree.addChildren(*this, *(the_tree._tree[0].begin()));
  return *this;
}

/*   const TT& root()   */
/********************/
TREE_TEMPLATE
const TT& TREE_CLS::root() const {
  if (_size > 0)
    return _tree[0].begin()->object;

  throw std::out_of_range("Root not set");
}


/*    const TT& 
      parent(const TT& obj)   */
/******************************/
TREE_TEMPLATE
const TT& TREE_CLS::parent(const TT& obj) const {
  for (auto& family : _tree) {
    for (auto& node : family) {
      if (match(node.object, obj))
        if (node.parent_node)
          return node.parent_node->object;
        else
          throw std::invalid_argument("Node is the root");
    }
  }
  throw std::invalid_argument("Node not found");
}

/*    Iterator
      children(const TT& parent) const   */
/*****************************************/
TREE_TEMPLATE
typename TREE_CLS::Iterator TREE_CLS::children(const TT& parent) const {
  for (auto& family : _tree) {
    for (auto& node : family) {
      if (match(node.object, parent)) {
        return Iterator(_tree[node.desc_family_id].begin(), _tree[node.desc_family_id].end());
      }
    }
  }
  throw std::invalid_argument("Node not found");
}

/*    Iterator 
      siblings(const TT& obj) const   */
/**************************************/
TREE_TEMPLATE
typename TREE_CLS::Iterator TREE_CLS::siblings(const TT& obj) const {
  for (auto& family : _tree) {
    for (auto& node : family) {
      if (match(node.object, obj)) {
        return Iterator(family.begin(), family.end());
      }
    }
  }
  throw std::invalid_argument("Node not found");
}

/*    void
      root(const TT& root)   */
/*****************************/
TREE_TEMPLATE
void TREE_CLS::root(const TT& root) {
  // Check that there is no root yet
  if (_size > 0)
    return;

  Node node(root);
  node.desc_family_id = _tree.size();
  _tree[0].insert(node);
  ++_size;

  // Add descendant family
  Family family;
  _tree.push_back(family);
}

/*    void
      addNode(const TT& obj, const TT& parent)   */
/*************************************************/
TREE_TEMPLATE
void TREE_CLS::addNode(const TT& obj, const TT& parent) {
  // Validate that the new element is unique in the tree
  for (auto& family : _tree) {
    for (auto& node : family) {
      if (match(node.object, obj))
        throw std::invalid_argument("Node already exists");
    }
  }

  // Search the parent element 
  for (auto& family : _tree) {
    for (auto& node : family) {
      // When the parent is found...
      if (match(node.object, parent)) {
        // Create the new node
        Node new_node(obj);
        new_node.parent_node = &node;
        new_node.desc_family_id = _tree.size();

        // Add new node to the parent's family
        _tree[node.desc_family_id].insert(new_node);
        ++_size;

        // Create empty descendant family for the new node
        Family family;
        _tree.push_back(family);

        return;
      }
    } // End for each node in family
  } // End for each family in tree

  // If the loop is finished, no parent was found
  throw std::invalid_argument("Parent Node not found");
}

/*    void
      removeNode(const TT& obj, bool with_desc = false)   */
/**********************************************************/
TREE_TEMPLATE
void TREE_CLS::removeNode(const TT& obj, bool with_desc) {
  // First, find the node
  for (auto& family : _tree) {
    for (auto& node : family) {
      if (match(node.object, obj)) {
        if (with_desc || (!with_desc && _tree[node.desc_family_id].size() == 0)) {
          // Remove the node and all of its descendants
          removeChildren(family, node);
          return;
        }
        else {
          throw std::invalid_argument("Node has descendants");
        }
      }
    }
  }
  throw std::invalid_argument("Node not found");
}

/*    void
      printChildren(std::ostream& os, size_t fam_id, std::string indentation)   */
/********************************************************************************/
TREE_TEMPLATE
void TREE_CLS::printChildren(std::ostream& os, size_t fam_id, std::string indentation) const {
  static const std::string IND{ "   " };
  for (auto& node : _tree[fam_id]) {
    os << indentation << "[ " << node.object << " ]" << std::endl;
    if (_tree[node.desc_family_id].size() > 0)
      printChildren(os, node.desc_family_id, indentation + IND);
  }
}

/*    void
      addChildren(Tree<TT, SortingBy>& subtree, const Node& node)   */
/********************************************************************/
template <typename TT, class SortingBy, class MatchingBy>
void tree::Tree<TT, SortingBy, MatchingBy>::addChildren(Tree<TT, SortingBy, MatchingBy>& subtree, const Node& node) const {
  for (auto& child_node : _tree[node.desc_family_id]) {
    subtree.addNode(child_node.object, node.object);
    if (_tree[child_node.desc_family_id].size() > 0)
      addChildren(subtree, child_node);
  }
}

/*    void
      removeChildren(Family& family, const Node& node)   */
/*********************************************************/
TREE_TEMPLATE
void TREE_CLS::removeChildren(Family& family, const Node& node) {
  // Iterate through all the children nodes in reverse order, to avoid invalidated pointers when a node is deleted
  Family& desc_family = _tree[node.desc_family_id];
  auto iter = desc_family.rbegin();
  while (iter != desc_family.rend()) {
    removeChildren(desc_family, *iter);
  }
  // Node can be removed (Watch out: the family is not deleted but eventually left empty
  family.erase(node);
  --_size;
}
/// *** CLASS Tree --> END DEFINITIONS **************************************************************************************************************************** ///
/// *************************************************************************************************************************************************************** ///

#undef TREE_TEMPLATE
#undef TREE_CLS 
