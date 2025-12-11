const dgram = require('dgram');

const PORT = 40000;
const HOST = '127.0.0.1';

const server = dgram.createSocket('udp4');



server.on('message', (msg, rinfo) => {
    const clientMessage = msg.toString();
    
    console.log(`received from ${rinfo.port} msg: ${clientMessage}`);
    
    const response = `ECHO: ${clientMessage}`;
    
    server.send(response, rinfo.port, rinfo.address, () => {
        console.log(`to ${rinfo.port}: "${response}"`);
    });
});

server.on('listening', () => {
    const address = server.address();
    console.log('server on port ', address.port);
   
});

server.on('close', () => {
    console.log(`server closed`);
});

server.bind(PORT, HOST);

