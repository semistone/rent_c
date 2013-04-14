var port = 9090, host='192.168.0.1';
var net = require('net');
var socket = net.createConnection(port, host,function(){
    console.log('Socket create ....');        
});
socket.setTimeout(3000);
socket.on("error", function (err) {
     console.log("Socket error: " + err);
}).on('connect', function() {
    console.log('Connected');
}).on('timeout', function(){
    console.log('Timeout');
    socket.destroy();
});
console.log('gogogo');
