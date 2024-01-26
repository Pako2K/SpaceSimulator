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

#ifndef CSV_FILE_READER_H
#define CSV_FILE_READER_H

#include <string>
#include <vector>
#include <type_traits>

namespace utils
{


  /**
    *  \brief CSV file reader class
    *           Each field is separated by a separator character.
    *           Commented lines (starting with '#' or '!') and invalid lines are discarded.
    */
  class CSVFileReader
  {
  public:
    /**
      *  \brief Constructor
      *         Reads the file and initializes the list of records, each record is a vectro of string fields
      *  @param file_name [in] Name of the properties file
      *  @throw  string exception containing the description of the issue
      */
    CSVFileReader(std::string file_name, char separator);

    /**
      *  \brief Destructor (INLINE)
      */
    inline ~CSVFileReader() {}

    /**
      *  \brief Returns the content of the file as a list of vectors (INLINE)
      *  @param
      *  @return list<vector<string>> Reference to the list of records
      */
    const std::vector<std::vector<std::string>> & getRecords() { return _records; }

  protected:
    /**
      *  Vector used to store in memory the content of the file
      */
    std::vector<std::vector<std::string>> _records;

  };



  /**
    *  \brief Tokenizer Functor
    */
  class Tokenizer
  {
  public:
    Tokenizer() {};
    /**
      *  \brief operator() --> tokenizes a string. according to the given seprater character, and returns a vector of token strings
      *  @param tokens [out] reference to the vector of string to be used to return the tokens
      *  @param str [in] string to be tokenized
      *  @param sep [in] token separator character
      *  @return void
      */
    void operator() (std::vector<std::string> &tokens, const std::string &str, const char sep);
  };

}

#endif // CSV_FILE_READER_H
