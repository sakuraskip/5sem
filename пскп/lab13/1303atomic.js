const net = require('net');

let host = '127.0.0.1';
let port = 40000;

let sharedBuffer = new SharedArrayBuffer(4);
let sumArray = new Int32Array(sharedBuffer);
const server  = net.createServer((sock)=>
{
    console.log('server started, port:' + port);

    sock.on('data',(data)=>
    {
        console.log(`from ${sock.remoteAddress}: `,data,Atomics.load(sumArray,0));
        let numfromclient =data.readInt32LE();
        Atomics.add(sumArray,0,numfromclient);
    });
    let buf = Buffer.alloc(4);

    setInterval(() => {
        let currentSum = Atomics.load(sumArray,0);
        buf.writeInt32LE(currentSum,0);
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


