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

int 
DB::insert(HostRecord& record){
    sqlite3_stmt *stmt;
    if (sqlite3_prepare(db, 
                        "insert into HOST (NAME,IP,STATUS,MODIFIED)values (?, ?, 0, ?)",  // stmt
                        -1, // If than zero, then stmt is read up to the first nul terminator
                        &stmt,
                        0  // Pointer to unused portion of stmt
                       ) != SQLITE_OK){
        return 0;
    }
    return 1;
}
/**
 * close sqlite
 */
DB::~DB(){
    this->logger->info("close database");
    sqlite3_close(this->db);
}
