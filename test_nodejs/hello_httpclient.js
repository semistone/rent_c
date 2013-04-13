var http = require('http');
var sys = require('sys');

function checkSite(url) {
    var site = http.createClient(80, url);
    site.on('error', function(err) {
        sys.debug('unable to connect to ' + url);
    });
    var request = site.request('GET', '/', {'host': url});
    request.end();
    request.on('response', function(res) {
        sys.debug('status code: ' + res.statusCode);
    });
}

checkSite("www.google.com");
checkSite("foo.bar.blrfl.org");
