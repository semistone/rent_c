Name resolver load balance library
==================================
This library provide one dfor.so to hook
gethostbyname and getaddrinfo function. 
and dford.js to monitor each server's status 
and update status into sqlite.


Description
------------------------------------
It's just a simple prototype to prove it work.
The code is still simple, but it work properly on my server.
It use node.js to implement the server monitor daemon.
It current support  socket mode which only check connection alive.
and http mode which will trigger http request and check status code as OK

### Install file list
install in .so in
/usr/local/lib/dfor.so

config file in 
/usr/local/etc/dfor/config.yaml
/usr/local/etc/dfor/log4cpp.property

dfor daemon 
/usr/loca/bin/dford.js

runtime sqlite cache in
/var/run/dfor/cache.db

log in
/var/log/dfor/dfor.log

### Config example:
    example.com:
       mode: weight 
       check_method: socket
       check_port: 3306
       ttl: 3000
       hosts:
           127.0.0.1:
               weight: 2
           192.168.0.2:
               weight: 3
    example2.com:
       mode: round-robin
       check_method: http 
       check_path: /ok
       check_port: 80 
       ttl: 3000
       hosts:
           - 127.0.0.1
           - 192.168.0.2
### How it build and install
Require library: log4cpp, sqlite3
Require packages: node.js
#
# compile and install library
#
make 
make install
#
# install dford
#
cd dford
npm install

### How it work
export LD_PRELOAD=/usr/local/lib/dfor.so

### Sqlite schema
    ubuntu@ip-10-142-55-159:~/rent_c/dfor$ sqlite3 /var/run/dfor/cache.db
    SQLite version 3.7.9 2011-11-01 00:52:41
    Enter ".help" for instructions
    Enter SQL statements terminated with a ";"
    sqlite> .schema
    CREATE TABLE HOST(ID INTEGER PRIMARY KEY,
                      NAME VARCHAR(64),
                      IP   VARCHAR(16),
                      STATUS INTEGER DEFAULT 0,
                      WEIGHT INTEGER,
                      COUNT INTEGER DEFAULT 0,
                      LAST_READ_TIME INTEGER DEFAULT 0,
                      MODIFIED INTEGER);
    CREATE UNIQUE INDEX IDX_HOST on HOST(NAME,IP);
    CREATE INDEX IDX_HOST_NAME on HOST(NAME);


### How to verify 
    dford start
    export LD_PRELOAD=/usr/local/lib/dfor.so
    curl http://example.com
    
### What's different mode  
If failover mode 
   Weight = 0 
   It will always return first record which status is '1'

If round robin mode
   Weight = 1
   It will return each record equally.

If weight mode
   Set weigt value
   It will return each record base on weight number.
