const net = require('net');

let host = '127.0.0.1';
let port = 40000;

let sum = 0;
const server  = net.createServer((sock)=>
{
    console.log('server started, port:' + port);

    sock.on('data',(data)=>
    {
        console.log(`from ${sock.remoteAddress}: `,data,sum);
        sum+=data.readInt32LE();
    });
    let buf = Buffer.alloc(4);

    setInterval(() => {
        buf.writeInt32LE(sum,0);
        sock.write(buf);
    }, 5000);
});

server.on('listening',()=>
{
    console.log('server on port ' + port);
});
server.on('error',()=>
{
    console.log('server closed');
});
server.listen(port,host);


