const WebSocket = require('ws');

let ws = new WebSocket('ws://localhost:4000');

ws.on('open', () => {
  console.log('connected');
});

ws.on('message', (data) => {
    const message = JSON.parse(data);
    if (message.event === 'A')
    {
      console.log('event A received');
    }
});

ws.on('close', () => {
  console.log('disconnected');
});

