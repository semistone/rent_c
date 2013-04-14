/**
 *
 * dfor daemon to watch host status
 *
 *
 */
var _ = require('underscore')._;
var net = require('net');
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
var callback = {
    error: function(name, ip, last_status){
        if(last_status.status == 1) {
            last_status.status = 0;
            var ts = Math.round(Date.now() / 1000);
            console.log('status change to 0');
            db.run('update HOST set STATUS=0, MODIFIED=? where NAME=? and  IP=?', [ts, name, ip]);
        }
        console.log('connect error for name:' + name + ' ip:' + ip);
    },
    connect: function(name, ip, last_status){
        console.log('connect error for name:' + name + ' ip:' + ip);
        if(last_status.status == 0) {
            last_status.status = 1;
            var ts = Math.round(Date.now() / 1000);
            console.log('status change to 1');
            db.run('update HOST set STATUS=1, MODIFIED=? where NAME=? and  IP=?', [ts, name, ip]);
        }
    }
};

function check_socket(name, ip , port, callback, last_status) {//{{{
    var socket = net.createConnection(port, ip);
    socket.setTimeout(3000);
    socket.on("error", function (err) {
        console.log("socket error: " + err);
        socket.end();
        socket.destroy();
        if(callback) callback.error(name, ip, last_status);
    }).on('connect', function() {
        console.log('socket connected');
        socket.end();
        socket.destroy();
        if(callback) callback.connect(name, ip, last_status);
    }).on('timeout', function(){
        console.log("socket timeout: ");
        socket.end();
        socket.destroy();
        if(callback) callback.error(name, ip, last_status);
    });
}//}}}


function start(name, ip, settings){//{{{
    var method = settings['check_method'], last_status = {'status' : 0};
    setInterval((function() {
        if(method == 'socket'){
            var port = settings['check_port'];
            console.log('check socket port:' + port);
            check_socket(name, ip, port, callback, last_status);
        }
    }), 5000);
}//}}}

//console.log(JSON.stringify(config, null, "    "));
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
        var insert_stmt = db.prepare("insert or replace into HOST (NAME,IP,COUNT, WEIGHT,MODIFIED) values (?,?,0,?,?)");
        var ts = Math.round(Date.now() / 1000);
        (function(){
           var _name = name, _ip = ip, _settings = config[name];
           insert_stmt.run([name, ip, weight, ts], function(){
               start(_name, _ip, _settings);        
           });
           insert_stmt.finalize();
        })();
    }
    //
    // delete extra record
    //
    (function(name, _hosts, isarray){
        db.each("select name, ip from HOST where name = ? ", [name], function(err, row) {
            if(isarray){
                if(_.indexOf(_hosts, row.IP) == -1) {
                    console.log("1.name:" + row.NAME+ " ip:" + row.IP + " not exist in config");
                    db.run('delete from HOST where NAME=? and  IP=?', [row.NAME, row.IP]);
                }
            } else {
                if (_hosts[row.IP] == undefined) {
                    console.log("2.name:" + row.NAME + " ip:" + row.IP + " not exist in config");
                    db.run('delete from HOST where NAME=? and  IP=?', [row.NAME, row.IP]);
                }
            }
        });
    })(name, hosts, isarray);
}
