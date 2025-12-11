const net = require('net');

let host = '127.0.0.1';
let port = 40000;

let client = new net.Socket();
client.connect(port,host,()=>
    {
        console.log('connected');
        client.write('hello');
    });
client.on('data',(data)=>
{
    console.log('client data: ' + data);
    client.destroy();
});
client.on('close',()=>  
{
    console.log('client closed');
})
