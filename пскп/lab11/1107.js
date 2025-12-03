const rpcws = require("rpc-websockets").Server;


const server = new rpcws({port:4000});


server
  .register("A", () => {
    console.log("event A");
  })
  .public();
server
  .register("B", () => {
    console.log("event B");
  })
  .public();
server
  .register("C", () => {
    console.log("event C");
  })
  .public();