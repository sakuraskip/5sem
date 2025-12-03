const rpcws = require("rpc-websockets").Client;
const ws = new rpcws("ws://localhost:4000/");

process.stdin.setEncoding("utf8");
process.stdin.on("data", (chunk) => {

    let eventLetter = chunk.trim().toUpperCase();
    if (eventLetter === "A" || eventLetter === "B" || eventLetter === "C") {
      console.log(`event ${eventLetter} sent`);
      ws.notify(eventLetter);
    }
  
});