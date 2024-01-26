/**
  *  \brief  Node Class. Represents a node in a tree
  *          A node has a value, a pointer to the parent node and the index of the descendant family of nodes.
  *          These data members are set during the initialization of the node (constructor) and cannot be changed.
  *          Nodes cannot be copied or assigned, but they can be moved.
  *          A static const node, NULL_NODE, represents the empty node (for instance the parent of a root node)
  */
TREE_TEMPLATE
class TREE_CLS::Node
{
public:
  /**
    *  \brief  Node Constructor. It will move the value to the heap memory and store the pointer, as well as the pointer to the parent node and the id of the
    *          descendant family of nodes.
    *  @param  val  The value contained by the node. Is a moved lvalue or rvalue
    *  @param  parent  Pointer to the parent node (it can be null)
    *  @param  desc_fam  Id of the descendant family of nodes
    */
  Node(OBJ_T&& val, Node& parent, size_t desc_fam) :
    _value          { new OBJ_T(std::move(val)) }, 
    _parent_node    { &parent }, 
    _desc_family_id { desc_fam } 
  {
    DebugLog("NODE CONSTRUCTED " << *_value);
  }

  /**
    *  \brief  Root Node Constructor. It calls the Node constructor with a NULL_NODE parent node
    *  @param  val  The value contained by the node. Is a moved lvalue or rvalue
    *  @param  desc_fam  Id of the descendant family of nodes
    */
  Node(OBJ_T&& val, size_t desc_fam) : Node(std::move(val), NULL_NODE, desc_fam) {}


  /**
    *  \brief  DELETED: Copy Constructor
    */
  Node(const Node&) = delete;

  /**
    *  \brief  Move Constructor. It will move the contents of the node, invalidating the moved one
    *  @param  m_node  The moved node, which will get invalidated
    */
  Node(Node&& m_node) : 
    _value          { std::move(m_node._value) }, 
    _parent_node    { m_node._parent_node }, 
    _desc_family_id { m_node._desc_family_id } 
  {
    m_node._value.reset(NULL_NODE._value.get());
    m_node._parent_node     = NULL_NODE._parent_node;
    m_node._desc_family_id  = NULL_NODE._desc_family_id;
    DebugLog("NODE MOVED " << *_value);
  }

  /**
    *  \brief  DELETED: Assignment operator
    */
  Node& operator=(const Node&) = delete;

  /**
    *  \brief  Assignment operator of a right value
    *          It will "transfer" the data members to the new node and invalidate this node
    */
  Node& operator=(Node&& m_node) {
    _value          = std::move(m_node._value);
    _parent_node    = m_node._parent_node;
    _desc_family_id = m_node._desc_family_id;

    m_node._value.reset(NULL_NODE._value.get());
    m_node._parent_node     = NULL_NODE._parent_node;
    m_node._desc_family_id  = NULL_NODE._desc_family_id;
    DebugLog("NODE MOVED (Assignment) " << *_value);

    return *this;
  }
    
  /**
    *  \brief  Destructor. Nothing to do, since the value is a smart pointer
    */
  ~Node() {}

  /**
    *  \brief  Checks whether a Node is the NULL_NODE
    */
  bool isNull() const { return _value == NULL_NODE._value; }

  /**
    *  \brief  Get value
    */
  OBJ_T& value() const { return *_value; }

  /**
    *  \brief  Get parent node
    */
  Node& parentNode() const { return (_parent_node != nullptr) ? *_parent_node : NULL_NODE; }

  /**
    *  \brief  Get descendant family id
    */
  size_t descFamilyId() const { return _desc_family_id; }


protected:
  /**
    *  \brief  An empty node. Either the parent of a root node or an invalidated node after being moved.
    */
  static Node NULL_NODE;

  /**
    *  \brief  The value of the node, which is a pointer to the actual "Object" value. The values cannot be changed (no set method)
    */
  std::unique_ptr<OBJ_T> _value{ nullptr };

  /**
    *  \brief  Pointer to the parent Node. This pointer value cannot be changed, i.e. the parent of a node cannot change (no set method)
    */
  Node* _parent_node{ nullptr };

  /**
    *  \brief  Index of the descendant family in the tree deque of families. The desendant family cannot be changed (no set method)
    */
  size_t _desc_family_id{ 0 };


  /**
    *  \brief  Default Node Constructor. It creates an empty or NULL Node. 
    *          It will be used only to provide the NULL_NODE macro, which is the parent of the root Node
    */
  Node() {}

};

TREE_TEMPLATE
typename TREE_CLS::Node TREE_CLS::Node::NULL_NODE;
