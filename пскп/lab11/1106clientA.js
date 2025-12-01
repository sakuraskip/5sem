const RPC = require('rpc-websockets').Client;

const ws = new RPC('ws://localhost:4000');
const WebSocket = require('ws');


ws.on('open', () => {
  console.log('connected');
  ws.subscribe('A');
  
});

ws.on('A', () => {
    console.log('event A');
});

ws.on('close', () => {
  console.log('disconnected');
});

