#include <files/csv_file_reader.h>

#include <fstream>

using namespace utils;


/// ***********************************************************************************************************************
/// ************************************************* PUBLIC **************************************************************

/// CONSTRUCTOR()
CSVFileReader::CSVFileReader(std::string file_name, char separator) {
  // Open file
  std::ifstream file(file_name, std::ios::in);
  if (!file.is_open())
    throw std::string("File cannot be opened: " + file_name);

  // Resize the vector for records
  _records.reserve(100);

  // Read each line, discarding empty ones or starting with '#' or '!'
  std::string line {""};
  size_t num_values {0};
  std::vector<std::string> values;
  Tokenizer tokenizer;
  while ( !file.eof() ) {
    getline(file, line);

    if (line.length() == 0 || line[0] == '#' || line[0] == '!') // Not a record
      continue;

    tokenizer(values, line, separator);

    if (num_values && values.size() != num_values){
      file.close();
      _records.clear();
      throw std::string("Inconsistent CSV file");
    }

    num_values = values.size();

    _records.push_back(values);
  }

  file.close();

  _records.shrink_to_fit();
}

/// ************************************************* END PUBLIC **********************************************************
/// ***********************************************************************************************************************


/// ********************************************** TOKENIZER FUNCTOR ******************************************************

void Tokenizer::operator() (std::vector<std::string> &tokens, const std::string &str, const char sep) {
  size_t init_pos {0};
  size_t pos {0};
  tokens.clear();
  while ( (pos = str.find(sep, init_pos)) != std::string::npos ) {
    tokens.push_back(str.substr(init_pos, pos - init_pos));
    init_pos = pos + 1;
  }
  /* Add last token */
  tokens.push_back(str.substr(init_pos, str.size() - init_pos));
}
