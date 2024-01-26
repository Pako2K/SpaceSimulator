#ifndef SQLITEDB_H
#define SQLITEDB_H

#include "sqlite3.h"

#include <sstream>


namespace sqlitedb
{

  class SQLiteDB
  {
  public:

    class SQLStatement
    {
      friend class SQLiteDB;

    public:
      // Move constructor is public
      SQLStatement(SQLStatement&& obj);

      ~SQLStatement();

      SQLStatement& bind(int position, std::string value);

      SQLStatement& bind(int position, int value);

      SQLStatement& bind(int position, int64_t value);

      SQLStatement& bind(int position, double value);

      SQLStatement& bind(int position);

      // returns: True if there are (more) records to be fetched
      bool execute();

      void reset(bool clear_bindings = false);

      template <typename T>
      T fetchValue(size_t col) { static_assert(false, "Type not supported"); }

      template <>
      int fetchValue<int>(size_t col) {
        if (sqlite3_column_type(_stmt_ptr, int(col)) == SQLITE_INTEGER)
          return sqlite3_column_int(_stmt_ptr, int(col));
        else if (sqlite3_column_type(_stmt_ptr, int(col)) == SQLITE_NULL)
          return 0;
        else
          throw std::runtime_error("SQLException: column type is not an int");
      }
      
      template <>
      int64_t fetchValue<int64_t>(size_t col) {
        if (sqlite3_column_type(_stmt_ptr, int(col)) == SQLITE_INTEGER)
          return sqlite3_column_int64(_stmt_ptr, int(col));
        else if (sqlite3_column_type(_stmt_ptr, int(col)) == SQLITE_NULL)
          return 0;
        else
          throw std::runtime_error("SQLException: column type is not an int");
      }

      template <>
      std::string fetchValue<std::string>(size_t col) {
        if (sqlite3_column_type(_stmt_ptr, int(col)) == SQLITE_TEXT)
          return std::string((const char*)sqlite3_column_text(_stmt_ptr, int(col)));
        else if (sqlite3_column_type(_stmt_ptr, int(col)) == SQLITE_NULL)
          return "";
        else
          throw std::runtime_error("SQLException: column type is not a string");
      }

      template <>
      double fetchValue<double>(size_t col) {
        if (sqlite3_column_type(_stmt_ptr, int(col)) == SQLITE_FLOAT)
          return sqlite3_column_double(_stmt_ptr, int(col));
        else if (sqlite3_column_type(_stmt_ptr, int(col)) == SQLITE_NULL)
          return 0.0;
        else
          throw std::runtime_error("SQLException: column type is not a double");
      }


    protected:
      // Constructor can only be used by SQLiteDB
      SQLStatement(SQLiteDB& lite_db, std::string sql_str);
      SQLStatement(const SQLStatement&) = delete;
      SQLStatement& operator=(const SQLStatement&) = delete;
      SQLStatement& operator=(SQLStatement&&) = delete;


    private:
      SQLiteDB&     _lite_db;
      sqlite3_stmt* _stmt_ptr{ nullptr };
      bool          _moved{ false };
    };


    SQLiteDB(const std::string dbfile);

    SQLiteDB(const SQLiteDB&) = delete;
    SQLiteDB(SQLiteDB&&) = delete;
    SQLiteDB& operator=(const SQLiteDB&) = delete;
    SQLiteDB& operator=(SQLiteDB&&) = delete;

    ~SQLiteDB();

    SQLStatement createSQL(std::string sql_str);


  private:
    sqlite3* _db{ nullptr };
  };

}

#endif  // SQLITEDB