const WebSocket = require('ws');

const ws = new WebSocket('ws://localhost:3000');


ws.on('open', ()=> {
   console.log('connected');
});

ws.on('message', function message(data) {
    
        const notification = JSON.parse(data);
        console.log('new notification: ')
        console.log(`   id: ${notification.id}`);
        console.log(`   msg: ${notification.msg}`);
        console.log(`   time: ${new Date(notification.timestamp).toString()}`);
    
});

ws.on('close', ()=> {
    console.log('connection closed');
});


