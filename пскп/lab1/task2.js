const http = require('http');

const host = '127.0.0.1';
const port = 8000;
const server = http.createServer((request,response)=>
{
    let body = '';
    request.on('data',chunk=>
    {
        body+= chunk.toString();
    }
    );
    request.on('end',()=>
    {
        response.statusCode = 200;
        response.setHeader('Content-Type','text/html');
        response.end(`<h1>method : ${request.method}</h1>
        <h2>uri : ${request.url}
        <h3>http version : ${request.httpVersion}
        <h3> request body: ${body},</h3>`);
        
    })
    
})

server.listen(port,host);