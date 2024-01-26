//  MIT License
//
//  Copyright (c) 2018 Francisco de Lanuza
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in all
//  copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//  SOFTWARE.

#ifndef PROPERTIES_FILE_READER_H
#define PROPERTIES_FILE_READER_H

#include <string>
#include <map>
#include <vector>
#include <type_traits>
#include <stdexcept>


namespace utils
{
  /**
    *  \brief JAVA-like properties file reader class
    *           Each property key is separated from the value by a '='.
    *           Property keys can be duplicated
    *           Commented lines (starting with '#' or '!') and invalid lines are discarded.
    *           Leading and trailing spaces are removed.
  */
  class PropertiesFileReader
  {
  public:
    /**
      *  \brief Constructor
      *         Reads the file and initializes the map of key/value pairs
      *  @param file_name [in] Name of the properties file
      *  @throw  runtime_error exception containing the description of the issue
      */
    PropertiesFileReader(std::string file_name);

    /**
      *  \brief Destructor (INLINE)
      */
    ~PropertiesFileReader() {}

    /**
      *  \brief Returns the content of the file as a multimap of strings
      *  @return multimap<string,string> Refrence to the map of (key, value) pairs of type string, with repeating keys
      */
    const std::multimap<std::string, std::string>& getProperties() const { return _properties; }

    /**
      *  \brief Returns the values of the properties with the specified key, converted to type T
      *  @param key [in] key which value has to be returned
      *  @return  a vector which contains the values for the properties with the specified key
      *  @throw  out_of_range exception if there is no property with the specified key
      */
    template <class TYPE, typename = std::enable_if<std::is_arithmetic<TYPE>::value> >
    std::vector<TYPE> properties(std::string key) const;

    /**
      *  \brief Returns the values of the properties with the specified key, as a string
      *  @param key [in] key which value has to be returned
      *  @return  a vector which contains the values for the properties with the specified key
      *  @throw  out_of_range exception if there is no property with the specified key
      */
    std::vector<std::string> properties(std::string key) const;

    /**
      *  \brief Returns the first value of the properties with the specified key, converted to type T
      *  @param key [in] key which value has to be returned
      *  @return  a TYPE which contains the first value for the properties with the specified key
      *  @throw  out_of_range exception if there is no property with the specified key
      */
    template <class TYPE, typename = std::enable_if<std::is_arithmetic<TYPE>::value> >
    TYPE property(std::string key) const;

    /**
      *  \brief Returns the first value of the properties with the specified key, as a string
      *  @param key [in] key which value has to be returned
      *  @return  a string which contains the first value for the properties with the specified key
      *  @throw  out_of_range exception if there is no property with the specified key
      */
    std::string property(std::string key) const;


  protected:
    /**
      *  Multimap used to store in memory the content of the file
      */
    std::multimap<std::string, std::string> _properties;

  };


  /// propertyCast(string key)
  template <class TYPE, typename >
  std::vector<TYPE> PropertiesFileReader::properties(std::string key) const {
    auto match_iter = _properties.equal_range(key);
    if (match_iter.first == _properties.end() )
      throw std::out_of_range("Key NOT found: " + key);

    std::vector<TYPE> values;
    values.reserve(match_iter.second - match_iter.first);
    while (match_iter.first != match_iter.second ) {
      if constexpr( std::is_floating_point<TYPE>::value)
        values.push_back(TYPE(atof( (match_iter.first->second).c_str() )));
      else if constexpr ( sizeof(TYPE) > sizeof(int) )
        values.push_back(TYPE(atol( (match_iter.first->second).c_str() )));
      else
        values.push_back(TYPE(atoi( (match_iter.first->second).c_str() )));

      ++match_iter.first;
    }

    return values;
  }


  template <class TYPE, typename >
  TYPE PropertiesFileReader::property(std::string key) const {
    auto match_iter = _properties.equal_range(key);
    if (match_iter.first == _properties.end())
      throw std::out_of_range("Key NOT found: " + key);
    
    if constexpr (std::is_floating_point<TYPE>::value)
      return TYPE(atof((match_iter.first->second).c_str()));
    else if constexpr (sizeof(TYPE) > sizeof(int))
      return TYPE(atol((match_iter.first->second).c_str()));
    else
      return TYPE(atoi((match_iter.first->second).c_str()));
  }
  
}

#endif // PROPERTIES_FILE_READER_H
