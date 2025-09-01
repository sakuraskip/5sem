const http = require('http');
const fs = require('fs');

const port = 5000;
const server = http.createServer((request,response)=>
{
    response.statusCode = 200;
    
    console.log(request.url);
    if(request.url === '/html')
    {
      response.setHeader('Content-Type','text/html');
      fs.createReadStream(__dirname+ "\\index.html").pipe(response);
    }
    if(request.url === '/png')
    {
      response.setHeader('Content-Type','image/png');
      fs.createReadStream(__dirname + "\\image.png").pipe(response);
    }

})

server.listen((port));