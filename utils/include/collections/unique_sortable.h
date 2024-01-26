#ifndef UNIQUE_SORTABLE_H
#define UNIQUE_SORTABLE_H

#include <set>
#include <map>
#include <string>
#include <sstream>
#include <stdexcept>
#include <functional>

#include <logger.h>


namespace utils
{
  /**
    *  \brief  Abstract template class for objects which can be instantiated only once with the same key (matching key), and can be sorted accoridung to a second key (sorting key).
    *          Both the matching key and the sorting key must support the comparison operator <.
    *          Uniqueness of the key is guaranteed by inserting it in a set.
    *          This class must be inhereted by classes which represent objects with a unique key and sortable.
    *          Each derived class must provide a unique identifier for the type of objects it represents. THe set of keys belonging to each type are stored in a map
    *          Only the derived class has access to the methods which provide the values of the keys.
    *          It is allowed to move objects of this class, but the moved object will loose the access to the keys, and the ability to be moved again,getting unusable.
    *
    *          Template parameters:
    *             - MATCH_KEY_TYPE -> the type of the matching key
    *             - SORT_KEY_TYPE -> the type of the sorting key
    *             - SORTING_FUNC -> the functor used to sort objects of this type
    */
  template <class MATCH_KEY_TYPE, class SORT_KEY_TYPE, class SORTING_FUNC = std::less<SORT_KEY_TYPE>>
  class UniqueSortable
  {
  public:

    /**
      *  \brief  Alias of the matching key type
      */
    using match_key_type = MATCH_KEY_TYPE;

    /**
      *  \brief  Alias of the sorting key type
      */
    using sort_key_type = SORT_KEY_TYPE;

    /**
      *  \brief  Alias of the sorting key type
      */
    using sort_func = SORTING_FUNC;

    /**
      *  \brief  DELETED Copy Constructor: Objects with a unique key can't be copied
      *          IMPORTANT: The derived class may write its own copy constructor or get a default one: it is recommended to explicitly delete it 
      *                     in case hard copies of the derived class members must be prevented
      */
    UniqueSortable(const UniqueSortable&) = delete;

    /**
      *  \brief  DELETED Assignment operator: Objects with a unique key can't be duplicated
      */
    UniqueSortable& operator=(const UniqueSortable&) = delete;

    /**
      *  \brief  Destructor: removes the matching key from the list of unique keys
      */
    virtual ~UniqueSortable() { 
      // If the object was moved don't delete the key from the list of keys
      if (!_moved) 
        _keys[_type_id].erase(_matching_key);

      DebugLog("DESTROYED: " << _type_id << " - key: " << _matching_key << "(moved: " << _moved << ")");
    }

    /**
      *  \brief  Comparison operator used for sorting
      *  @param  Another instance of a UniqueSortable object
      *  @return  the result of comparing whether this object is less than the provided object
      */
    bool operator<(const UniqueSortable& rv) const {
      static SORTING_FUNC sorter;
      return sorter(sortingKey(), rv.sortingKey());
    }

    /**
      *  \brief  Get the matching key.
      *  @return  The matching key
      *  @throw  bad_function_call If the object is not valid anymore (i.e. it has been moved)
      */
    const MATCH_KEY_TYPE& matchingKey() const { return (this->*matchingKeyFP)(); }

    /**
      *  \brief  Get the sorting key.
      *  @return  The sorting key
      *  @throw  bad_function_call If the object is not valid anymore (i.e. it has been moved)
      */
    const SORT_KEY_TYPE& sortingKey() const { return (this->*sortingKeyFP)(); }
       

  protected:

    /**
      *  \brief  Constructor: can only be called by derived classes. It will store the 2 keys making sure that the matching key is unique for the given type of objects.
      *          The type of objects can be shared with other derived classes
      *  @param  type_id  The identifier of the type of objects. If it already exists, the key is added to the existing set.
      *  @param  matching_key  The value of the matching key
      *  @param  sorting_key  The value of the sorting key
      *  @throw  invalid_argument  If the matching key has already being used or the type identifier is empty
      */
    UniqueSortable(const std::string& type_id, const MATCH_KEY_TYPE& matching_key, const SORT_KEY_TYPE& sorting_key) 
                   : _type_id{ type_id }, _matching_key { matching_key }, _sorting_key{ sorting_key } {

      if (_type_id == "")
        throw std::invalid_argument("TYPE CANNOT BE EMPTY");

      if (!_keys[_type_id].insert(_matching_key).second)
        throw std::invalid_argument("KEY ALREADY EXISTS");
      
      DebugLog("CREATED: " << _type_id << " - key: " << _matching_key);
    }

    /**
      *  \brief  Move Constructor: can only be called from derived classes. It will store the 2 keys from the moved object and invalidate it
      *          IMPORTANT: The derived class must write its own move constructor to handle its own class members! 
      *  @param  obj  The object to be moved
      *  @throw  invalid_argument  If the object is invalid (has already being moved)
      */
    UniqueSortable(UniqueSortable&& obj) : _type_id{ std::move(obj._type_id) }, _matching_key { std::move(obj._matching_key) }, _sorting_key{ std::move(obj._sorting_key) } {
      // Chack whether the object is valid
      if (obj._moved)
        throw std::invalid_argument("INVALID OBJECT");

      // Invalidate moved object
      obj._moved        = true; 
      obj.matchingKeyFP = &UniqueSortable::_matchingKeyInvalidated;
      obj.sortingKeyFP  = &UniqueSortable::_sortingKeyInvalidated;
      
      DebugLog("MOVED: " << _type_id << " - key: " << _matching_key);
    }
    
    /**
      *  \brief  Checks whether a matching key is already used for a specific type of objects. Only accesible to the derived classes
      *  @param  type_id  The identifier of the type of objects
      *  @param  a_key  The matching key to be checked
      *  @return  The result of the check
      */
    static bool exists(const std::string type_id, MATCH_KEY_TYPE& a_key) { return _keys[type_id].find(a_key) != _keys[_type_id].end(); }

  private:

    /**
      *  \brief  Constant: The unique identifier for the type of object
      */
    const std::string _type_id;
        
    /**
      *  \brief  Constant: The unique matching key
      */
    const MATCH_KEY_TYPE _matching_key;

    /**
      *  \brief  Constant: The sorting key
      */
    const SORT_KEY_TYPE _sorting_key;

    /**
      *  \brief  Map with the Set of unique matching keys already used for each type of object, idntified by a unique string
      */
    static std::map<std::string, std::set<MATCH_KEY_TYPE>> _keys;

    /**
      *  \brief  Flag used to indicated that an object has been moved (invalidated)
      */
    bool _moved{ false };

    /**
      *  \brief  Pointer to the function retrieving the matching key. Only accesible to the derived classes
      *          This pointer is set to an invalidated function when the object is moved
      */
    const MATCH_KEY_TYPE&(UniqueSortable::* matchingKeyFP)() const { &UniqueSortable::_matchingKey };

    /**
      *  \brief  Pointer to the function retrieving the sorting key. Only accesible to the derived classes
      *          This pointer is set to an invalidated function when the object is moved
      */
    const SORT_KEY_TYPE&(UniqueSortable::* sortingKeyFP)() const { &UniqueSortable::_sortingKey };

    /**
      *  \brief  Get function for the matching key
      *  @return  The matching key    
      */
    const MATCH_KEY_TYPE& _matchingKey() const { return _matching_key; }

    /**
      *  \brief  Get function for the matching key
      *  @return  The sorting key
      */
    const SORT_KEY_TYPE& _sortingKey() const { return _sorting_key; }

    /**
      *  \brief  Invalidated function for getting the matching key
      *  @throw  bad_function_call  If the object is not valid anymore (i.e. it has been moved)
      */
    const MATCH_KEY_TYPE& _matchingKeyInvalidated() const { throw std::bad_function_call(); }

    /**
      *  \brief  Get function for the matching key
      *  @throw  bad_function_call  If the object is not valid anymore (i.e. it has been moved)
      */
    const SORT_KEY_TYPE& _sortingKeyInvalidated() const { throw std::bad_function_call(); }

  };

  template <class MATCH_KEY_TYPE, class SORT_KEY_TYPE, class SORTING_FUNC>
  std::map<std::string, std::set<MATCH_KEY_TYPE>> utils::UniqueSortable<MATCH_KEY_TYPE, SORT_KEY_TYPE, SORTING_FUNC>::_keys;
}


#endif // UNIQUE_SORTABLE_H

