var net = require('net');
var server = net.createServer(function(c) { //'connection' listener
    console.log('server connected');
    c.on('data', function(data) {
        console.log('data is ' + data);
        c.write('hello\r\n');
    });
    c.on('close', function() {
        console.log('server disconnected');
    });
    c.on('error', function(e) {
        console.log('server error');
    });
});
server.listen('/tmp/echo.sock', function() { //'listening' listener
    console.log('server bound');
});
