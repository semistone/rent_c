#include "DB.h"
using namespace dfor;

/**
 * init sqlite object
 */
DB::DB(std::string& dbfile){
   if (sqlite3_open_v2(dbfile.c_str(), &(this->db), SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL)) {
       return;
   }
}
