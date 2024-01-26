
#define TREE_TEMPLATE template <typename OBJ_T>
#define TREE_CLS tree::MTree<OBJ_T>


#include <collections/mtree.node.h>
#include <collections/mtree.family.h>
#include <collections/mtree.iterator.h>
#include <collections/mtree.family.iterator.h>


/*    void root(OBJ_T&& root)   */
/*****************************/
TREE_TEMPLATE
void TREE_CLS::root(OBJ_T&& the_root) {
  // Check that there is no root yet
  if (_size > 0)
    throw std::logic_error("Tree already has a root");

  // Create empty root family
  _tree.emplace_back(Family());

  // Add node to the root family
  _tree[0].addNode(Node(std::move(the_root), 1));
  ++_size;

  // Add empty descendant family  
  _tree.emplace_back(Family());

  // Add node to the map
  _nodes_index[_tree[0][0].value().matchingKey()] = &_tree[0][0];
}


/*   void addNode(OBJ_T&& obj, const typename OBJ_T::match_key_type& parent_key)   */
/***********************************************************************************/
TREE_TEMPLATE
void TREE_CLS::addNode(OBJ_T&& obj, const typename OBJ_T::match_key_type& parent_key) {
  // Search the parent element 
  Node& parent_node = searchNode(parent_key);

  // Add new node to the descendant family of the parent
  Node& new_node = _tree[parent_node.descFamilyId()].addNode(Node(std::move(obj), parent_node, _tree.size()));
  ++_size;

  // Add empty descendant family  
  _tree.emplace_back(Family());

  // Add node to the map
  _nodes_index[new_node.value().matchingKey()] = &new_node;
}


/*    void removeNode(const typename OBJ_T::match_key_type& key, bool with_desc)   */
/***********************************************************************************/
TREE_TEMPLATE
void TREE_CLS::removeNode(const typename OBJ_T::match_key_type& key, bool with_desc) {
  // Search the node 
  auto map_it = searchNodeIt(key);
  Node& node = *map_it->second;

  // Check whether there are descendants
  Family& desc_family = _tree[node.descFamilyId()];
  if (!with_desc && desc_family.size() > 0)
    throw std::invalid_argument("Node has descendants");

  // Remove descendant nodes (recursively)
  for (size_t i = 0; i < desc_family.size(); i++)
    removeNode(desc_family[i].value().matchingKey(), true);

  // Remove this node 
  _tree[node.parentNode().descFamilyId()].removeNode(node);
  --_size; 

  // Remove node from the index map
  _nodes_index.erase(map_it);
}


/*    OBJ_T& root()    */
/***********************/
TREE_TEMPLATE
OBJ_T& TREE_CLS::root() {
  if (_size == 0)
    throw std::out_of_range("Root not set");
  
  return _tree[0][0].value();
}


/*    const OBJ_T& root() const   */
/**********************************/
TREE_TEMPLATE
const OBJ_T& TREE_CLS::root() const {
  if (_size == 0)
    throw std::out_of_range("Root not set");

  return _tree[0][0].value();
}


/*    OBJ_T& parent(const typename OBJ_T::match_key_type& key)   */
/*****************************************************************/
TREE_TEMPLATE
OBJ_T& TREE_CLS::parent(const typename OBJ_T::match_key_type& key) {
  Node& node = searchNode(key);
  if (node.parentNode().isNull())
    throw std::invalid_argument("Node is the root");
  else
    return node.parentNode().value();  
}


/*    const OBJ_T& parent(const typename OBJ_T::match_key_type& key) const    */
/******************************************************************************/
TREE_TEMPLATE
const OBJ_T& TREE_CLS::parent(const typename OBJ_T::match_key_type& key) const{
  Node& node = searchNode(key);
  if (node.parentNode().isNull())
    throw std::invalid_argument("Node is the root");
  else
    return node.parentNode().value();
}


/*    Iterator children(const typename OBJ_T::match_key_type& key)   */
/*********************************************************************/
TREE_TEMPLATE
typename TREE_CLS::Family::Iterator TREE_CLS::children(const typename OBJ_T::match_key_type& key) {
  return _tree[searchNode(key).descFamilyId()].begin();
}


/*    const Iterator children(const typename OBJ_T::match_key_type& key) const   */
/*********************************************************************************/
TREE_TEMPLATE
typename TREE_CLS::Family::ConstIterator TREE_CLS::children(const typename OBJ_T::match_key_type& key) const {
  return _tree[searchNode(key).descFamilyId()].begin();
}


/*    Iterator siblings(const typename OBJ_T::match_key_type& key)  */
/********************************************************************/
TREE_TEMPLATE
typename TREE_CLS::Family::Iterator TREE_CLS::siblings(const typename OBJ_T::match_key_type& key) {
  // This works even when parentNode() == NULL_NODE
  return _tree[searchNode(key).parentNode().descFamilyId()].begin();
}


/*    const Iterator siblings(const typename OBJ_T::match_key_type& key) const    */
/**********************************************************************************/
TREE_TEMPLATE
typename TREE_CLS::Family::ConstIterator TREE_CLS::siblings(const typename OBJ_T::match_key_type& key) const {
  // This works even when parentNode() == NULL_NODE
  return _tree[searchNode(key).parentNode().descFamilyId()].begin();
}


/*    Node& searchNode(const typename OBJ_T::match_key_type& key)     */
/**********************************************************************/
TREE_TEMPLATE
auto TREE_CLS::searchNodeIt(const typename OBJ_T::match_key_type& key) {
  // Search the node
  auto map_it = _nodes_index.find(key);
  if (map_it == _nodes_index.end())
    throw std::invalid_argument("Node not found");

  return map_it;
}


/*    Node& searchNode(const typename OBJ_T::match_key_type& key)     */
/**********************************************************************/
TREE_TEMPLATE
auto TREE_CLS::searchNodeIt(const typename OBJ_T::match_key_type& key) const {
  // Search the node
  auto map_it = _nodes_index.find(key);
  if (map_it == _nodes_index.end())
    throw std::invalid_argument("Node not found");

  return map_it;
}


/*    void printChildren(std::ostream& os, size_t fam_id, std::string indentation)   */
/*************************************************************************************/
TREE_TEMPLATE
void TREE_CLS::printChildren(std::ostream& os, size_t fam_id, std::string indentation) const {
  for (auto node_ptr : _tree[fam_id].sortedNodes()) {
    os << indentation << "[ " << node_ptr->value() << " ]" << std::endl;
    if (_tree[node_ptr->descFamilyId()].size() > 0)
      printChildren(os, node_ptr->descFamilyId(), indentation + "   ");
  }
}


#undef TREE_TEMPLATE
#undef TREE_CLS 
