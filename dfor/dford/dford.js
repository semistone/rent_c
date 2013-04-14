/**
 *
 * dfor daemon to watch host status
 *
 *
 */
var _ = require('underscore')._;
var yaml = require("js-yaml"), confpath = process.argv[2];
//
//  open sqlite
//
var sqlite3 = require('sqlite3').verbose();
var dbfile = confpath + "/cache.db";
console.log('db file in ' + dbfile);
var db = new sqlite3.Database(dbfile);
//
// open config file
//
var conffile = confpath + "/config.yaml";
console.log('config file in ' + conffile);
var config = require(conffile);

console.log(JSON.stringify(config, null, "    "));
//
// init and insert  hosts into db
//
for(var name in config){ 
    console.log('host is ' + name);
    var hosts = config[name].hosts, mode = config[name].mode;
    var isarray = _.isArray(hosts), weight = 0;
    if (mode == 'round-robin') {
        weight = 1;
    }
    for(var index in hosts){
        var ip;
        if(isarray){
            ip = hosts[index];
        } else {
            ip = index;
        }
        if(mode == 'weight'){
            weight = hosts[ip].weight;
        }
        console.log('ip is ' + ip);
        var insert_stmt = db.prepare("insert or replace into HOST (NAME,IP,COUNT, WEIGHT,MODIFIED) values (?,?,0,?,?)");
        var ts = Math.round(Date.now() / 1000);
        insert_stmt.run([name, ip, weight, ts]);
        insert_stmt.finalize();
    }
    //
    // delete extra record
    //
    db.each("select name, ip from HOST where name = ? ", [name], function(err, row) {
        console.log("query " + row.NAME + ":" + row.IP);
        if(isarray){
            if(_.indexOf(hosts, row.IP) == -1) {
                console.log("1.name:" + row.NAME+ " ip:" + row.IP + " not exist in config");
                db.run('delete from HOST where NAME=? and  IP=?', [row.NAME, row.IP]);
            }
        } else {
            if (hosts[row.IP] == undefined) {
                console.log("2.name:" + row.NAME + " ip:" + row.IP + " not exist in config");
                db.run('delete from HOST where NAME=? and  IP=?', [row.NAME, row.IP]);
            }
        }
    });
}
