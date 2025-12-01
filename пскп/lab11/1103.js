const WebSocket = require('ws');
const fs = require('fs');
const path = require('path');

function heartbeat() {
  this.isAlive = true;
}

const wss = new WebSocket.Server({port:4000});
let n = 0;
wss.on('connection',(ws)=>
{
    console.log('client connected');
    
    ws.on('pong',heartbeat);
    ws.on('close',()=>
    {
        console.log('client disconnected');
    });
});
setInterval(() => {
        wss.clients.forEach(client=>
        {
            if(client.readyState === WebSocket.OPEN)
            {
                client.send(`11-03-server: ${n++}`);
            }
        });
    }, 15000);
 setInterval(() => {
        let aliveClients = 0;
        wss.clients.forEach(client=>
        {
            if(client.isAlive === false)
            {
                client.terminate();
            }
            client.isAlive = false;
            client.ping();
            aliveClients++;
        });
        console.log('alive connections : ' + aliveClients);
    }, 5000);