const RPC = require('rpc-websockets').Server;

const rpc = new RPC({
    port: 4000,
    host: 'localhost'
});

rpc.event('A');
rpc.event('B'); 
rpc.event('C');

rpc.on('connection', () => {
    console.log('client connected');
});

process.stdin.setEncoding('utf-8');
process.stdin.on('data', (data) => {
    let event = data.toString().toUpperCase().trim();
    if (event === 'A' || event === 'B' || event === 'C') {
        console.log('emitting event:', event);
        rpc.emit(event);
    }
});

