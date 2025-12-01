const RPC = require('rpc-websockets').Client;

const ws = new RPC('ws://localhost:4000');

async function callRPCMethod(method, params) {
    try {
        const result = await ws.call(method, params);
        console.log(`${method}${params}: `, result);
        return result;
    } catch (err) {
        console.log(`${method} error:`, err.message || err);
        return null;
    }
}

ws.on('open', async () => {
    console.log('connected');

  
        const authResult = await ws.login({login:'user', password:'pass'});
        
        
        
        await callRPCMethod('square', [3]);
        await callRPCMethod('square', [5, 4]);
        
        await callRPCMethod('sum', [2]);
        await callRPCMethod('sum', [2, 4, 6, 8, 10]);
        
        await callRPCMethod('mul', [3]);
        await callRPCMethod('mul', [3, 5, 7, 9, 11, 13]);
        
        await callRPCMethod('fib', [1]);
        await callRPCMethod('fib', [2]);
        await callRPCMethod('fib', [7]);
        
        await callRPCMethod('fact', [0]);
        await callRPCMethod('fact', [5]);
        await callRPCMethod('fact', [10]);
        
        
    
});

ws.on('error', (error) => {
    console.error('Connection error:', error);
});

ws.on('close', () => {
    console.log('Disconnected from server');
});