#ifndef CLOSED_LIST_H
#define CLOSED_LIST_H

#include <cstdlib>
#include <cstring>
#include <stdexcept>
#include <ostream>

#include <logger.h>


namespace utils
{
  /**
   *  \brief  This class represents a list with a limited number of elements. 
   *          When new elements are added, if the size of the list is less than its maximum, they will be added at the end.
   *          If the list already contains the maximum number of elements, the oldest one will be removed before adding the new one.
   *          Elements can be accessed by its index from the oldest (first) added element.
   *
   *          Template parameters:
   *             - OBJ_T -> the type of values to be stored in the list
   */
  template<typename OBJ_T>
  class ClosedList
  {
  public:
    /**
     *  \brief  Maximum number of elements to be stored in the list
     */
    const size_t CAPACITY{ 0 };

    /**
     *  \brief  Constructor: creates a list with the specified capacity
     *  @param  max_size  The capacoty of the closed list
     */
    ClosedList(size_t max_size) : CAPACITY{ max_size }, _ELEMENTS{ (OBJ_T*)calloc(max_size, sizeof OBJ_T) } {
      DebugLog("ClosedList CREATED. Max Size: " << max_size);
    }

    /**
     *  \brief  Copy Constructor: 
     *  @param  a_list  List to be copied
     */
    ClosedList(const ClosedList& a_list) : CAPACITY{ a_list.CAPACITY }, _ELEMENTS{ (OBJ_T*)calloc(a_list.CAPACITY, sizeof OBJ_T) }{
      // Add all the elements from the copied list
      for (int i = 0; i < a_list._size; i++) 
        add(a_list[i]);

      DebugLog("ClosedList COPIED");
    }

    /**
     *  \brief  Assignment operator. Only if the capacities of both lists are the same
     *  @param  a_list  List to be copied
     *  @return  A reference to the list with the copied values
     */
    ClosedList& operator=(const ClosedList& a_list) {
      if (CAPACITY != a_list.CAPACITY)
        throw std::domain_error("CAPACITY OF THE LISTS MUST BE THE SAME");

      // Clean-up the contents of this List
      for (int i = 0; i < _size; i++) 
        (_ELEMENTS + i)->~OBJ_T();
      
      _size = 0;
      _ptrFnAdd = &ClosedList::addAtEnd;

      // Add all the elements from the copied list
      for (int i = 0; i < a_list._size; i++) 
        add(a_list[i]);

      DebugLog("ClosedList ASSIGNED");

      return *this;
    }


    /**
     *  \brief  Destructor: detroys all the added elements and free the allocated memory
     */
    ~ClosedList() { 
      // Destroy all the objects in the list
      for (int i = 0; i < _size; i++) 
        (_ELEMENTS + i)->~OBJ_T();
      
      // Deallocate the memory
      free(_ELEMENTS);

      DebugLog("ClosedList DESTROYED");
    }

    /**
     *  \brief  Adds a new element to the list. If the list is full it will replace the oldest element
     *  @param  new_element  The new object to be added
     */
    void add(const OBJ_T& new_element) { (this->*_ptrFnAdd)(new_element); }

    /**
     *  \brief  Retrieves the size of the list, i.e., the number of added objects
     *  @return  The number of objects in the list
     */
    size_t size() const { return _size; }
    
    /**
     *  \brief  Retrieves the object at the indicated position in the list, from oldest element (position 0)
     *          This operator does not check whther the index is valid, for performance reasons
     *  @param  indx  The position of the object to be retrieved
     *  @return  The object at the position "indx"
     */
    const OBJ_T& operator[] (size_t indx) const { return _ELEMENTS[indx];; }
    
    /**
     *  \brief  Retrieves the object at the indicated position in the list, from oldest element (position 0)
     *  @param  indx  The position of the object to be retrieved
     *  @return  The object at the position "indx"
     *  @throw  out_of_range  If the indx is bigger than the number of elements in the list
     */
    const OBJ_T& at(size_t indx) const {
      // Validate index
      if (indx >= _size)
        throw std::out_of_range("Index is bigger than the list size");

      return _ELEMENTS[indx];
    }

    /**
     *  \brief  Returns a pointer to the first element in the list
     *  @return  Pointer to the object at position 0
     */
    const OBJ_T* data() const { return _ELEMENTS; }

    
    /**
      *  \brief  Output stream operator for displaying the list
      *  @param  os  Output stream
      *  @param  a_list  The list object to be displayed
      */
    friend std::ostream& operator<< (std::ostream& os, const ClosedList& a_list) {
      os << "List Size = " << a_list.size() << std::endl;
      for (int i = 0; i < a_list.size(); i++)
        os << "  Element " << i << " = " << a_list[i] << std::endl;
      return os;
    }


  private:

    /**
      *  \brief  Array of pointers to the elements: the allocated memory will be set at the constructor and will remain untouched
      */
    OBJ_T* const _ELEMENTS;

    /**
     *  \brief  Number of elements added
     */
    size_t _size{ 0 };

    /**
     *  \brief  Pointer to add function
     */
    void (ClosedList::* _ptrFnAdd) (const OBJ_T&) { &ClosedList::addAtEnd };

    /**
     *  \brief  Reused constants in the "addAndReplace" method
     */
    const decltype(_ELEMENTS)   ELEMENTS_PLUS_1          { _ELEMENTS + 1 };
    const decltype(_ELEMENTS)   ELEMENTS_LAST            { _ELEMENTS + (CAPACITY - 1) };
    const size_t                SIZEOF_ELEMENTS_MINUS_1  { (CAPACITY - 1) * sizeof(OBJ_T) };

    /**
     *  \brief  Adds a new element always at the end of th elast position (assumes that thelist is not full)
     *          If the list is full after adding the new element, it will update the function pointer used by the public methods "add" and operator[]
     *  @param  new_element  The new object to be added
     */
    void addAtEnd(const OBJ_T& new_element) {
      ::new ((void *)(_ELEMENTS + _size++)) OBJ_T(new_element);

      // Check whether the list is full
      if (_size == CAPACITY) 
        _ptrFnAdd = &ClosedList::addAndReplace;
    }

    /**
     *  \brief  Adds a new element after the last added element, removing the oldest one at the beginning of the list
     *  @param  new_element  The new object to be added
     */
    void addAndReplace(const OBJ_T& new_element) {
      // Delete replaced obejct
      _ELEMENTS->~OBJ_T();

      // Move the elements in memory
      memcpy(_ELEMENTS, ELEMENTS_PLUS_1, SIZEOF_ELEMENTS_MINUS_1);
           
      // Create new object
      ::new ((void *)(ELEMENTS_LAST)) OBJ_T(new_element);
    }

  };
    
}

#endif // CLOSED_LIST_H
