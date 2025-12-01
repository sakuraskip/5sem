const RPC = require('rpc-websockets').Client;

const ws = new RPC('ws://localhost:4000');
const WebSocket = require('ws');


ws.on('open', () => {
  console.log('connected');
  ws.subscribe('C');
  
});

ws.on('C', () => {
    console.log('event C');
});

ws.on('close', () => {
  console.log('disconnected');
});

