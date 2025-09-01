const http = require('http');

const host = '127.0.0.1';
const port = 8000;
const server = http.createServer((request,response)=>
{
    response.statusCode = 200;
    response.setHeader('Content-Type','text/html');
    response.end(`<h1>method : ${request.method}</h1>
        <h2>uri : ${request.url}
        <h3>http version : ${request.httpVersion}`);
})

server.listen(port,host);