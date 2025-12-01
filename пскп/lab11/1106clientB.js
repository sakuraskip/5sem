const RPC = require('rpc-websockets').Client;

const ws = new RPC('ws://localhost:4000');
const WebSocket = require('ws');


ws.on('open', () => {
  console.log('connected');
  ws.subscribe('B');
  
});

ws.on('B', () => {
    console.log('event B');
});

ws.on('close', () => {
  console.log('disconnected');
});

