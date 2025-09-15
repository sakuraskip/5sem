const http = require('http');

const host = '127.0.0.1';
const port = 8000;
const server = http.createServer((request,response)=>
{
    response.statusCode = 200;
    response.setHeader('Content-Type','text/html');
    response.end('<h1>hello world</h1> ');
})

server.listen(port,host,()=>
{
    console.log("server started");
    
});