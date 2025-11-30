const WebSocket = require('ws');

let wss = new WebSocket.Server({port:4000})

wss.on('connection', (ws) => {

  console.log('client connected');

  ws.on('message', (data) => {
    const message = JSON.parse(data);
    if (message.event === 'A' || message.event === 'B' || message.event === 'C')
    {
      console.log(`event ${message.event} received`);
    }
});
});



wss.on('close', () => {
  console.log('server close');
});

