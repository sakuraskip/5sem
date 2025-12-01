const RPC = require('rpc-websockets').Client;

const ws = new RPC('ws://localhost:4000');

ws.on('open', async () => {
    console.log('connected');

    await ws.login({login:'user', password:'pass'});

    const calls = [
        ws.call('square', [3]),
        ws.call('square', [5, 4]),
        ws.call('sum', [2]),
        ws.call('sum', [2, 4, 6, 8, 10]),
        ws.call('mul', [3]),
        ws.call('mul', [3, 5, 7, 9, 11, 13]),
        ws.call('fib', [1]),
        ws.call('fib', [2]),
        ws.call('fib', [7]),
        ws.call('fact', [0]),
        ws.call('fact', [5]),
        ws.call('fact', [10]),
    ];

    const results = await Promise.all(calls);
    results.forEach((result, index) => {
        console.log(`call ${index + 1}:`, result);
    });

    ws.close();
});

