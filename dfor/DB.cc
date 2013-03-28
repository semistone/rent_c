#include "DB.h"
using namespace dfor;


log4cpp::Category* DB::logger=&log4cpp::Category::getInstance(std::string("dfor.db"));
static std::string CREATESQL = "CREATE TABLE HOST("
                               "ID INTEGER PRIMARY KEY,"
                               "NAME VARCHAR(64),"
                               "IP   VARCHAR(16),"
                               "STATUS INTEGER,"
                               "WEIGHT INTEGER,"
                               "MODIFIED INTEGER)";
static std::string CREATEINDEX = "create unique index UNI_IDX_HOST_NAME_IP on HOST(NAME,IP)";


HostRecord::HostRecord(const std::string& name, const std::string& ip, int status, long modified){
    this->name = name;
    this->ip = ip;
    this->status = status;
    this->modified = modified;
};


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
    int ret2 = sqlite3_exec(db, CREATEINDEX.c_str(), 0, 0, &errMsg);
    return ret == SQLITE_OK && ret2 == SQLITE_OK;
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
    sqlite3_bind_text(stmt, 1, record.name.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, record.ip.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 3, record.status);
    sqlite3_bind_int(stmt, 4, (int)record.modified);
    if(sqlite3_step(stmt) == SQLITE_DONE){
        sqlite3_finalize(stmt);
        return 1;
    } else {
        sqlite3_finalize(stmt);
        return 0;
    }
}

/**
 *
 *
 */
std::string
DB::query(const std::string& name, const QueryMode mode){
    switch(mode){
        case FAILOVER: 
            return failOver(name);
            break;
    }
}

/**
 * select by weight
 *
 */
std::string
DB::failOver(const std::string& name){
    sqlite3_stmt *stmt;
    if (sqlite3_prepare(db, 
                        "select IP from HOST where STATUS='1' and NAME=? order by WEIGHT desc limit 1",  // stmt
                        -1, // If than zero, then stmt is read up to the first nul terminator
                        &stmt,
                        0  // Pointer to unused portion of stmt
                       ) != SQLITE_OK){
        sqlite3_finalize(stmt);
        throw std::runtime_error("query ip fail");
    }
    sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_STATIC);
    int isNext = sqlite3_step(stmt);
    if(isNext == SQLITE_DONE){
        return "";
    }
    if (isNext != SQLITE_ROW) {
        sqlite3_finalize(stmt);
        throw std::runtime_error("query fail");
    }
    std::string ip = std::string((char*)sqlite3_column_text(stmt, 0));
    sqlite3_finalize(stmt);
    return ip;
}


/**
 * close sqlite
 */
DB::~DB(){
    this->logger->info("close database");
    sqlite3_close(this->db);
}
