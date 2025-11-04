const http = require("http");
const fs = require("fs");
const host = "127.0.0.1";
const {send} = require('./Module/m0603');
const port = 8000;

const server = http.createServer((request, response) => {
  let body = "";
  if (request.url === "/") {
    response.setHeader("Content-Type", "text/html");
    fs.createReadStream(__dirname + "\\0603.html").pipe(response);
  }
  if (request.method === "POST") {
    response.setHeader("Content-Type", "text/html");
    request.on("data", (chunk) => {
      body += chunk;
    });
    request.on("end", () => {
      let parsedBody = new URLSearchParams(body);
     
      let passwd = parsedBody.get("password");
     
      let message = parsedBody.get("message");

      if (!message || !passwd) {
        response.statusCode = 400;
        response.end("<h1> all fields are required </h1>");
      }
      console.log(`message: ${message}`);
      send(message,passwd).then(value=>
      {
        response.statusCode = 200;
        response.end(`<h1>mail sent</h1>`);
      }
      ).catch(err=>
      {
        response.statusCode = 500;
        response.end(`<h1>error: ${err}</h1>`);
      }
      )
      response.statusCode = 200;
    });
  }
});

server.listen(port, host, () => {
  console.log(`server started at ${host}:${port}`);
});
