const net = require('net');

let host = '127.0.0.1';
let port = 40000;

let client = new net.Socket();
let buf = new Buffer.alloc(4);
let timer = null;
client.connect(port,host,()=>
    {
        console.log('connected');
        let k = 0;
        timer = setInterval(() => {
            client.write((buf.writeInt32LE(k++,0),buf));
        }, 1000);
    });
client.on('data',(data)=>
{
    console.log('client data: ', data.readInt32LE());
    
});
client.on('close',()=>  
{
    console.log('client closed');
    if(timer) clearInterval(timer);
});
client.on('error',()=>
{
    console.log('client closed');
    client.destroy();
});
