const net = require('net');

let host = '127.0.0.1';
let port = 40000;

if(process.argv.length < 3)
{
    console.log('usage: 1306.js {x}');
    return 1;
}

let x = parseInt(process.argv[2]);
if(isNaN(x))
{
    console.log('x is not a number');
    return 1;
}
let client = new net.Socket();
let buf = new Buffer.alloc(4);
let timer = null;
client.connect(port,host,()=>
    {
        console.log('connected ');
        timer = setInterval(() => {
            client.write((buf.writeInt32LE(x,0),buf));
            console.log('send ',x);
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
