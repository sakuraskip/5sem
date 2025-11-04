const http = require("http");
const fs = require("fs");
const url = require("url");
const nodemailer = require("nodemailer");
const host = "127.0.0.1";
const port = 8000;

const server = http.createServer((request, response) => {
  let body = "";
  if (request.url === "/") {
    response.setHeader("Content-Type", "text/html");
    fs.createReadStream(__dirname + "\\index.html").pipe(response);
  }
  if (request.method === "POST") {
    response.setHeader("Content-Type", "text/html");
    request.on("data", (chunk) => {
      body += chunk;
    });//esmtp

    request.on("end", () => {
      let parsedBody = new URLSearchParams(body);
      let from = parsedBody.get("from");
      let passwd = parsedBody.get("password");
      let to = parsedBody.get("to");
      let message = parsedBody.get("message");

      if ( !from || !to || !message || !passwd ) {
        response.statusCode = 400;
        response.end("<h1> all fields are required </h1>");
      }
      console.log(`from: ${from} to: ${to} message: ${message}`);

      const transporter = nodemailer.createTransport({
        service: "gmail",
        auth: {
          user: from,
          pass: passwd,
        },
        secure: true,
      });

      const mail = {
        from,
        to,
        subject: "lab6 msg",
        html: `<p>${message}</p>`,
      };
      
      transporter.sendMail(mail, (err, info) => {
        if (err) {
          console.log(err);
          response.statusCode = 500;
          response.end(`<h1>mail send error: ${err}</h1>`);
        }
        if (info) {
          console.log("mail sent");
          response.statusCode = 200;
          response.end(`${info}`);
        }
      });

      response.statusCode = 200;
    });
  }
});

server.listen(port, host, () => {
  console.log(`server started at ${host}:${port}`);
});
