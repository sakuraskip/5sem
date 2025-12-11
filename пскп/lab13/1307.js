const net = require('net');

let host = '127.0.0.1';
let ports = [40000,50000];

const server1  = net.createServer((sock)=>
{
    console.log('server started, port:' + ports[0]);
    let number = 0;
    sock.on('data',(data)=>
    {
        number = data.readInt32LE(0);
        let response = `ECHO ` + number;
        console.log('send msg to ', sock.remotePort);
         sock.write(response);
    });

    
});

server1.on('listening',()=>
{
    console.log('server on port ' + ports[0]);
});
server1.on('error',()=>
{
    console.log('server closed');
});
server1.listen(ports[0],host);

const server2  = net.createServer((sock)=>
{
    console.log('server started, port:' + ports[1]);
    let number = 0;
    sock.on('data',(data)=>
    {
        number = data.readInt32LE(0);
        let response = `ECHO ` + number;
        console.log('send msg to ', sock.remotePort);
         sock.write(response);
    });

    
});

server2.on('listening',()=>
{
    console.log('server on port ' + ports[1]);
});
server2.on('error',()=>
{
    console.log('server closed');
});
server2.listen(ports[1],host);


