#ifndef _DFOR_DB_H
#define _DFOR_DB_H
#include <boost/shared_ptr.hpp>
#include <sqlite3.h>
#include <string>

namespace dfor{

class HostRecord{
public:
    int id;
    std::string host;
    std::string ip;
    int status;
    long modified; 
};


class DB{
public:
    DB(std::string& dbfile);
    bool create();
    int insert(HostRecord& record);
    int update(std::string& host, int status);
    int deleteRecord(int id);
    ~DB();
private:
    sqlite3 *db;
};
};// namespace c2c
#endif
