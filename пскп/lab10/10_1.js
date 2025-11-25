const http = require ("http");
const url = require ("url");
const path = require ("path");
const fs = require ("fs");
const ws = require('ws');

let httpPort = 3000;
let wsPort = 4000;

const httpServer = http.createServer((req,res)=>
{
    if(req.url === '/start' && req.method === 'GET')
    {
        res.setHeader('Content-Type','text/html');
        fs.createReadStream('index.html').pipe(res);
    }
    else
    {
        res.statusCode = 400;
        res.end('wrong url');
    }
});
httpServer.listen(httpPort,()=>
{
    console.log('http server started, port: '+ httpPort);
});

const wsServer = new ws.Server({port:wsPort});

wsServer.on('connection',(ws)=>
{

    let serverIterator = 0;
    let clientNumber;
    console.log('connection event');
    ws.on('message',(message)=>
    {
        console.log('message: ' + message);
        clientNumber = message.toString().split(':')[1].trim();
    });

    setInterval(()=>
    {
        ws.send(`10-01-server: ${clientNumber}->${serverIterator++}`);
    },5000);
});
