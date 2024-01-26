#include <sqlitedb/sqlitedb.h>


#define BIND_ERROR_CHECK    if ( result ) { \
                              std::stringstream txt; \
                              txt << "SQLException - ERROR " << result << ": " << sqlite3_errmsg(_lite_db._db) << "\nPointer: " << _stmt_ptr; \
                              throw std::runtime_error(txt.str()); \
                            }


using namespace sqlitedb;


SQLiteDB::SQLStatement::SQLStatement(SQLiteDB& lite_db, std::string sql_str) : _lite_db{ lite_db } {
  if (sqlite3_prepare_v2(_lite_db._db, sql_str.c_str(), int(sql_str.size()), &_stmt_ptr, nullptr)) {
    std::stringstream txt;
    txt << "SQLException - ERROR: " << sqlite3_errmsg(_lite_db._db);
    throw std::runtime_error(txt.str());
  }
}


SQLiteDB::SQLStatement::SQLStatement(SQLStatement&& obj) : _lite_db{ obj._lite_db }, _stmt_ptr{ obj._stmt_ptr } {
  obj._moved = true;
}


SQLiteDB::SQLStatement::~SQLStatement() {
  if (!_moved)
    sqlite3_finalize(_stmt_ptr);
}


SQLiteDB::SQLStatement& SQLiteDB::SQLStatement::bind(int position, std::string value) {
  auto result = sqlite3_bind_text(_stmt_ptr, position, value.c_str(), int(value.size()), SQLITE_TRANSIENT);
  BIND_ERROR_CHECK;
  return *this;
}


SQLiteDB::SQLStatement& SQLiteDB::SQLStatement::bind(int position, int value) {
  auto result = sqlite3_bind_int(_stmt_ptr, position, value);
  BIND_ERROR_CHECK;
  return *this;
}


SQLiteDB::SQLStatement& SQLiteDB::SQLStatement::bind(int position, int64_t value) {
  auto result = sqlite3_bind_int64(_stmt_ptr, position, value);
  BIND_ERROR_CHECK;
  return *this;
}


SQLiteDB::SQLStatement& SQLiteDB::SQLStatement::bind(int position, double value) {
  auto result = sqlite3_bind_double(_stmt_ptr, position, value);
  BIND_ERROR_CHECK;
  return *this;
}


SQLiteDB::SQLStatement& SQLiteDB::SQLStatement::bind(int position) {
  auto result = sqlite3_bind_null(_stmt_ptr, position);
  BIND_ERROR_CHECK;
  return *this;
}


bool SQLiteDB::SQLStatement::execute() {
  auto res = sqlite3_step(_stmt_ptr);
  if (res == SQLITE_DONE) {
    return false;
  }
  if (res != SQLITE_ROW) {
    std::stringstream txt;
    txt << "SQLException: " << sqlite3_errmsg(_lite_db._db) << "\nSQL executed:" << sqlite3_expanded_sql(_stmt_ptr);
    throw std::runtime_error(txt.str());
  }
  return true;
}


void SQLiteDB::SQLStatement::reset(bool clear_bindings) {
  sqlite3_reset(_stmt_ptr);
  if (clear_bindings)
    sqlite3_clear_bindings(_stmt_ptr);
}


SQLiteDB::SQLiteDB(const std::string dbfile) {
  auto result = sqlite3_open_v2(dbfile.c_str(), &_db, SQLITE_OPEN_READWRITE, nullptr);
  if (result) {
    std::stringstream txt;
    txt << "SQLITE DB cannot be opened: " << sqlite3_errmsg(_db);
    sqlite3_close(_db);
    throw std::runtime_error(txt.str());
  }
}


SQLiteDB::~SQLiteDB() {
  sqlite3_close(_db);
}


SQLiteDB::SQLStatement SQLiteDB::createSQL(std::string sql_str) {
  SQLStatement stmt(*this, sql_str);
  return stmt;
}


#undef ERROR_CHECK