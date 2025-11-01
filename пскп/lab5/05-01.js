const http = require("http");
const url = require("url");
const DB = require("./DB_Module");
const fs = require("fs");
const db = new DB();

const host = "127.0.0.1";
const port = 5000;

let shutdownTimer = 0;
let commitIntervalTimer = 0;
let statisticsTimer = 0;
let isCollectingStats = false;

let reqCount = 0;
let commitsCount = 0;

let statsStartTime = '';
let statsEndTime = '';

RegisterEventHandlers = () => {
  db.on("GET", async (request, response) => {
    response.end(JSON.stringify(await db.select()));
  });

  db.on("POST", async (request, response, body) => {
    try {
      let newItem = JSON.parse(body);
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
  db.on("COMMIT", async (request, response, body) => {
    commitsCount++;
    let commitCount = await db.commit();
    response.end(commitCount);
  });
};

RegisterEventHandlers();

const server = http.createServer((request, response) => {
  if(isCollectingStats === true)
  {
    reqCount++;
  }
  response.statusCode = 200;
  response.setHeader("Content-Type", "application/json");
  let parsedUrl = url.parse(request.url, true);
  let body = "";
  request.on("data", (chunk) => {
    body += chunk.toString();
  });
  request.on("end", () => {
    if (parsedUrl.pathname === "/api/db") {

      if (request.method === "GET") {
        db.emit(request.method, request, response);
      } else if (request.method === "POST") {
        db.emit("POST", request, response, body);
      } else if (request.method === "PUT") {
        db.emit("PUT", request, response, body);
      } else if (request.method === "DELETE") {
        const id = parseInt(parsedUrl.query.id, 10);
        db.emit("DELETE", request, response, id);
      }

    }
    if (request.url === "/") {
      response.setHeader("Content-Type", "text/html");
      fs.createReadStream(__dirname + "\\index.html").pipe(response);
    }
    if(request.url === "/api/ss")
    {
      response.setHeader("content-type","application/json");
      let startTime = "";
      let endTime = "";
      if(isCollectingStats !== true)
      {
        startTime = statsStartTime;
        endTime = statsEndTime;
      }
      let statsJson = {start: startTime,finish: endTime, request: reqCount, commit: commitsCount};
      response.end(JSON.stringify(statsJson));
      
    }
  });
});

server.listen(port, host, () => {
  console.log(`server running at ${host}:${port}`);
});

process.stdin.setEncoding('utf8');

process.stdin.on("data", (data) => {
  const text = data.trim();

  let command = text.split(" ")[0];
  let parm = text.split(" ")[1];

  switch (command) {
    case "sd":
      {
        if (parm) {
          let seconds = Number.parseInt(parm);
          if (seconds === null || seconds <= 0 || isNaN(seconds)) {
            console.log("sd: parameter is invalid");
            break;
          }
          if (shutdownTimer !== 0) {
            clearTimeout(shutdownTimer);
            console.log("sd: new shutdown timer");
          }

          shutdownTimer = setTimeout(() => {
            console.log("server shutting down...");
            process.exit(0);
          }, seconds * 1000);
          console.log(`sd: shutdown timer set at ${seconds} seconds`)
        } else {
          if (shutdownTimer !== 0) {
            clearTimeout(shutdownTimer);
            shutdownTimer = 0;
          }
          console.log("shutdown cancelled");
        }
      }
      break;
    case "sc":
      {
        if (parm) {
          let seconds = Number.parseInt(parm);
          if (seconds <= 0 || seconds === null || isNaN(seconds)) {
            console.log("sc: parameter is invalid");
            break;
          }
          if (commitIntervalTimer !== 0) {
            clearInterval(commitIntervalTimer);
          }

          commitIntervalTimer = setInterval(async () => {
            commitsCount++;
            console.log("sc: commiting...");
            await db.commit();
          }, seconds * 1000);

          commitIntervalTimer.unref();
          
          console.log(`sc: commit interval set at ${seconds} seconds`);
        } else {
          if (commitIntervalTimer !== 0) {
            clearInterval(commitIntervalTimer);
            commitIntervalTimer = 0;
          }
          console.log("sc: cleared commit interval");
        }
      }
      break;
    case "ss":
      {
        if (parm) {
          let seconds = Number.parseInt(parm);
          if (seconds <= 0 || seconds === null || isNaN(seconds)) {
            console.log("ss: parameter is invalid");
            break;
          }
          if (statisticsTimer !== 0) {
            clearTimeout(statisticsTimer);
          }
          isCollectingStats = true;
          reqCount = 0;
          commitsCount = 0;

          statisticsTimer = setTimeout(() => {
            isCollectingStats = false;
            console.log(`ss: statistics collected for ${seconds} seconds
                    requests: ${reqCount} commits: ${commitsCount}`);
                    statsEndTime = new Date();
          }, seconds * 1000);
          statsStartTime = new Date();

          statisticsTimer.unref();

          console.log("ss: collecting stats...");
        } else {
          if (statisticsTimer !== 0) {
            clearTimeout(statisticsTimer);
            statisticsTimer = 0;
          }
          console.log("ss: cleared statistics timer");
        }
      }
      break;
    default: {
      console.log("command not recognized ");
    }
  }
});
