#include "DB.h"
using namespace dfor;


log4cpp::Category* DB::logger=&log4cpp::Category::getInstance(std::string("dfor.db"));
static std::string CREATESQL = "CREATE TABLE HOST("
                               "ID INTEGER PRIMARY KEY,"
                               "NAME VARCHAR(64),"
                               "IP   VARCHAR(16),"
                               "STATUS INTEGER,"
                               "MODIFIED INTEGER)";
/**
 * init sqlite object
 */
DB::DB(const std::string& dbfile){
    this->logger->debugStream()<< "init dbfile file" << dbfile;
    if (sqlite3_open_v2(dbfile.c_str(), &(this->db), SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL)) {
        return;
    }
}

bool
DB::create(){
    this->logger->debug("create table");
    //
    // create
    //
    char *errMsg = NULL;
    int ret = sqlite3_exec(db, CREATESQL.c_str(), 0, 0, &errMsg);
    return ret == SQLITE_OK;
}
/**
 * close sqlite
 */
DB::~DB(){
    sqlite3_close(this->db);
}
