const http = require('http');
const url = require('url');
const DB = require('./DB_Module');
const { resolve } = require('path');
const db = new DB();

const host = "127.0.0.1";
const port = 8000;

RegisterEventHandlers = () => {
    db.on("GET", async (request, response) => {
      response.end(JSON.stringify(await db.select()));
    });

    db.on("POST", async (request, response, body) => {
      try {
        let newItem = JSON.parse(body);
        console.log("post body: ", newItem);
        await db.insert(newItem);
        response.end(JSON.stringify(newItem));
      } catch (err) {
        response.statusCode = 500;
        response.end(JSON.stringify({ error: err.message }));
      }
    });

    db.on("PUT", async (request, response, body) => {
      try {
        let newItem = JSON.parse(body);
        let updItem = await db.update(newItem);
        response.end(JSON.stringify(updItem));
      } catch (err) {
        response.statusCode = 400;
        response.end(JSON.stringify({ error: err.message }));
      }
    });

    db.on("DELETE", async (request, response, id) => {
      try {
        let delItem = await db.delete(id);
        response.end(JSON.stringify(delItem));
      } catch (err) {
        response.statusCode = 400;
        response.end(JSON.stringify({ error: err.message }));
      }
    });
  };

RegisterEventHandlers();


const server = http.createServer((request, response) => {
    response.statusCode = 200;
    response.setHeader("Content-Type", 'application/json')
    let parsedUrl = url.parse(request.url, true);
    let body = '';
    request.on('data', chunk => {
        body += chunk.toString();
    }
    );
    request.on('end', () => {
        if (parsedUrl.pathname === "/api/db") {
            if (request.method === 'GET') {
                db.emit(request.method, request, response);
            }
            else if (request.method === 'POST') {
                db.emit('POST', request, response, body);
            } else if (request.method === 'PUT') {
                db.emit('PUT', request, response, body);
            } else if (request.method === 'DELETE') {
                const id = parseInt(parsedUrl.query.id, 10);
                db.emit('DELETE', request, response, id);
            }
        }
    }
    )
});

server.listen(port, host, () => {
    console.log(`server running at ${host}:${port}`);
})