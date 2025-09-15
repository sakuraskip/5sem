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

        const headersHtml = Object.entries(request.headers)
      .map(([key, value]) => `<p>${key}: ${value}</p>`)
     .join(' ');
       

    response.end(`

        <h1>request headers</h1>
      <div>${headersHtml}</div>
      <h2> method: ${request.method}</h2>
      <h2> uri: ${request.url} </h2>
      <h3>request body: ${body}</h3>
    `);

        
    })
    
})

server.listen(port,host,()=>
{
    console.log("server started");
});