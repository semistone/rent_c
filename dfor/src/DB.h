#ifndef _DFOR_DB_H
#define _DFOR_DB_H
#include <boost/shared_ptr.hpp>
#include <sqlite3.h>
#include <string>
#include "log4cpp/Category.hh"

namespace dfor{

class HostRecord{
public:
    int id;
    std::string name;
    std::string ip;
    int status;
    long modified; 
    HostRecord(const std::string& name, const std::string& ip, int status, long modified);
};


class DB{
public:
    DB(const std::string& dbfile);
    bool create();
    int insert(HostRecord& record);
    int update(std::string& host, int status);
    int updateCount(std::string& name, std::string& ip);
    std::string query(const std::string& name);
    ~DB();
private:
    sqlite3 *db;
    static log4cpp::Category* logger;
};
};// namespace c2c
#endif
