const WebSocket = require('ws');
const fs = require('fs');
const path = require('path');


const wss = new WebSocket.Server({port:4000});
let n = 0;
wss.on('connection',(ws)=>
{
    ws.on('message',(msg)=>
    {
        let text = JSON.parse(msg);
        console.log('client msg: ' + JSON.stringify(text,null,1));
        const {client, timestamp } = text;
        if(!client || !timestamp)
        {
            ws.send('json error, no fields');
            return;
        }

        let response = {
            server: n++,
            client:client,
            timestamp: timestamp
        };
        ws.send(JSON.stringify(response));
    });
    ws.on('close',()=>
    {
        console.log('client disconnected');
    });
})