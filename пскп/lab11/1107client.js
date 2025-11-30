const WebSocket = require('ws');
const fs = require('fs');
const path = require('path');


const ws = new WebSocket('ws://localhost:4000');

ws.on('open',()=>
{
    console.log('connected');
});
process.stdin.setEncoding('utf-8');
process.stdin.on('data',(data)=>
{
    let event = data.toString().toUpperCase().trim();
    if(event === 'A' || event === 'B' || event === 'C')
    {
        console.log('event ');
        ws.send(JSON.stringify({event}))
    }
})