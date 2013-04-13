var port = 9090, host='127.0.0.1';
var net = require('net');
var socket = net.createConnection(port, host,function(){
    console.log('Socket create ....');        
});
socket.on("error", function (err) {
     console.log("Socket error: " + err);
}).on('connect', function() {
    console.log('Connected');
});
