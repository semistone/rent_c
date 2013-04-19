#!/usr/bin/node
/**
 *
 * dfor daemon to watch host status
 *
 *
 */
//
// require libs
//
var net = require('net'),
    http = require('http'),
    _ = require('underscore')._,
    yaml = require("js-yaml"),
    sqlite3 = require('sqlite3'),
    init = require('simple-daemon');

//
//  open sqlite
//
var CONF_PATH = '/usr/local/etc/dfor';
var SOCKET_TIMEOUT = 3000;
var DBFILE = '/var/run/dfor/cache.db';
var db = undefined;


function init_server(){//{{{
    var server = net.createServer(function(c) { //'connection' listener
        console.log('server connected');
        c.on('data', function(name) {
            name = new String(name);
            var index = name.indexOf('\n')
            name = name.substring(0, index);
            console.log('query ' + name);
            var ts = Math.round(Date.now() / 1000);
            db.each("select IP  from HOST where NAME = ? and STATUS='1' order by (COUNT + ADJUST ) * WEIGHT,ID asc limit 1",
                    [name],
                    function(err, row) {
                        db.run("update HOST set COUNT=COUNT+1,LAST_READ_TIME=? where NAME=? and IP=?",[ts, name, row.IP]);
                        console.log('return ' + row.IP);
                        c.write(row.IP);
                        c.write('\n');
                        c.end();
                        c.destroy();
                    },
                    function(err, rows){
                        if (rows == 0) {
                            console.log('return rows = 0 ');
                            c.write('\n');
                            c.end();
                            c.destroy();
                        }
                    });
        });
        c.on('close', function() {
            console.log('server disconnected');
        });
        c.on('error', function(e) {
            console.log('server error');
        });
    });
    server.listen('/var/run/dfor/dfor.sock', function() { //'listening' listener
        console.log('server bound');
    });
}//}}}

//
// open config file
//

var callback = {//{{{
    error: function(name, ip, last_status){
        console.log('connect error for name:' + name + ' ip:' + ip);
        var ts = Math.round(Date.now() / 1000);
        db.run('update HOST set STATUS=0, MODIFIED=? where NAME=? and  IP=?', [ts, name, ip]);
        if(last_status.status == 1) {
            last_status.status = 0;
            console.log('status change to 0, name:' + name + ' ip:' + ip);
        }
    },
    connect: function(name, ip, last_status){
        console.log('connect success for name:' + name + ' ip:' + ip);
        var ts = Math.round(Date.now() / 1000);
        db.run('update HOST set STATUS=1, MODIFIED=? where NAME=? and  IP=?', [ts, name, ip]);
        if(last_status.status == 0) {
            last_status.status = 1;
            db.run('update HOST set ADJUST = -COUNT where NAME=?', [name]);
            console.log('status change to 1, name:' + name + ' ip:' + ip);
        }
    }
};//}}}

function check_socket(name, ip , port, callback, last_status) {//{{{
    console.log('check socket name:' + name + ' ip:' + ip + ' port:' + port);
    var socket = net.createConnection(port, ip);
    socket.setTimeout(SOCKET_TIMEOUT);
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

function check_http(name, ip, port, path, callback, last_status){//{{{
    var options = {
        hostname: ip,
        port: port,
        path: path,
        method: 'GET'
    };
    console.log('check http , option is ', options);
    var req = http.request(options, function(res) {
        if (res.statusCode == 200) {
            console.log('http connected and success');
            if(callback) callback.connect(name, ip, last_status);
        } else {
            console.log('http connected and fail');
            if(callback) callback.error(name, ip, last_status);
        } 
    });
    req.setTimeout(SOCKET_TIMEOUT, function(){
        console.log('http timeout');
        if(callback) callback.error(name, ip, last_status);
    });
    req.on('error', function(err) {
        console.log('http error');
        if(callback) callback.error(name, ip, last_status);
    });
    req.end();
}//}}}

function start(name, ip, settings){//{{{
    var method = settings['check_method'], last_status = {'status' : 0};
    var ttl = settings.ttl;
    console.log(name + ' ttl is ' + ttl);
    if (ttl == undefined) ttl = 5000;
    setInterval((function() {
        var port = settings['check_port'];
        if(method == 'socket'){
            check_socket(name, ip, port, callback, last_status);
        } else if (method == 'http') {
            if (port == undefined) port = 80;
            var path = settings['check_path'];
            //console.log('check http port:' + port + ' path:' + path);
            check_http(name, ip, port, path, callback, last_status);
        }
    }), ttl);
}//}}}

function clean_extra_record(name, hosts, isarray){//{{{
    console.log('clean record for name:' + name);
    db.each("select NAME, IP  from HOST where NAME = ?", [name], function(err, row) {
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
} //}}}

function run(conffile, dbfile){//{{{
    console.log('db file in ' + dbfile);
    db = new sqlite3.Database(dbfile);
    init_server();
    console.log('config file in ' + conffile);
    var config = require(conffile);
    for(var name in config){ 
        console.log('host is ' + name);
        var hosts = config[name].hosts, mode = config[name].mode;
        var isarray = _.isArray(hosts), weight = 0;
        //
        // delete extra record
        //
        clean_extra_record(name, hosts, isarray);
    
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
                weight = 1 / hosts[ip].weight;
            }
            (function(){
               console.log('init for ip ' + ip);
               var insert_stmt = db.prepare("insert or replace into HOST (NAME,IP,COUNT, WEIGHT,MODIFIED) values (?,?,0,?,?)");
               var ts = Math.round(Date.now() / 1000);
               var _name = name, _ip = ip, _settings = config[name];
               insert_stmt.run([name, ip, weight, ts], function(){
                   start(_name, _ip, _settings);        
               });
               insert_stmt.finalize();
            })();
        }
    }
}//}}}
//console.log(JSON.stringify(config, null, "    "));
//
// init and insert  hosts into db
//
init.simple({
    pidfile : '/var/run/dfor/dford.pid',
    logfile : '/var/log/dfor/dford.log',
    command : process.argv[3],
    runSync : function () {
        var conffile = CONF_PATH + "/config.yaml";
        run(conffile, DBFILE);
    }
});
