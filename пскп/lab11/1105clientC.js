const RPC = require("rpc-websockets").Client;

const ws = new RPC("ws://localhost:4000");

ws.on("open", async () => {
  console.log("connected");

  const [
    resSquare3,
    resSquare54,
    resMul13579113,
    resFib7,
    resMul246,
  ] = await Promise.all([
    ws.call("square", [3]),
    ws.call("square", [5, 4]),
    ws.call("mul", [3, 5, 7, 9, 11, 13]),
    ws.call("fib", [7]),
    ws.call("mul", [2, 4, 6]),
  ]);

  const resSum = await ws.call("sum", [
    resSquare3,
    resSquare54,
    resMul13579113,
  ]);

  const fibSum = resFib7.reduce((acc, val) => acc + val, 0);
  const finalResult = resSum + fibSum * resMul246;

  console.log("result:", finalResult);
});

ws.on("close", () => {
  console.log("disconnected");
});
