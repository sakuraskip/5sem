const net = require('net');

let host = '127.0.0.1';
let port = 40000;

net.createServer((sock)=>
{
    console.log('server started, port:' + port);

    sock.on('data',(data)=>
    {
        console.log(`from ${sock.remoteAddress}: ${data}`);
        sock.write(`ECHO ${data}`);
    });
}).listen(port,host);