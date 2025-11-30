const WebSocket = require('ws');
const fs = require('fs');
const path = require('path');


const wss = new WebSocket.Server({port:4000});

wss.on('connection',(ws)=>
{
    console.log('client connected');
});
process.stdin.setEncoding('utf-8');
process.stdin.on('data',(data)=>
{
    let event = data.toString().toUpperCase().trim();
    if(event === 'A' || event === 'B' || event === 'C')
    {
        console.log('event ');
        wss.clients.forEach(client=>
        {
            client.send(JSON.stringify({event}));
        });
    }
})