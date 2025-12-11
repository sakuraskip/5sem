const dgram = require('dgram');
const client = dgram.createSocket('udp4');

const message = 'lab10 udp happy!';

client.on('message', (msg) => {
    console.log(msg.toString());
    client.close();
});

client.bind(0,()=>
{
    client.send(message, 40000, '127.0.0.1');

});