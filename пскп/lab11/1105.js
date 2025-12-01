const RPC = require('rpc-websockets').Server;

const rpc = new RPC({
    port: 4000,
    host: 'localhost',
    auth: true 
});

rpc.setAuth(({login, password}) => {
    console.log(`auth: ${login}:${password}`);
    return login == 'user' && password == 'pass';
});

rpc.register('square', (params) => {
    if (params.length === 1) {
        const radius = params[0];
        if (radius < 0) return { err: 'radius should be > than 0' };
        return radius * radius * Math.PI;
    } else if (params.length === 2) {
        const [a, b] = params;
        if (a < 0 || b < 0) return { err: 'a and b should be > than 0' };
        return a * b;
    } else {
        return { err: 'square expects 1 or 2 params' };
    }
}).public();

rpc.register('sum', (params) => {
    return params.reduce((acc, value) => acc + value, 0);
}).public();

rpc.register('mul', (params) => {
    return params.reduce((acc, value) => acc * value, 1);
}).public();

rpc.register('fib', (params) => {
    if (params.length !== 1 || params[0] < 0) {
        return { err: 'fib param should be a positive number' };
    }
    
    const n = params[0];
    if (n <= 0) return [];
    if (n === 1) return [0];
    
    const fibs = [0, 1];
    while (fibs.length < n) {
        fibs.push(fibs[fibs.length - 1] + fibs[fibs.length - 2]);
    }
    return fibs.slice(0, n);
}).protected();

rpc.register('fact', (params) => {
    if (params.length !== 1 || params[0] < 0) {
        return { err: 'fact param should be a positive number' };
    }
    
    const n = params[0];
    if (n < 0) return null;
    if (n === 0) return 1;
    
    let f = 1;
    for (let i = 1; i <= n; i++) f *= i;
    return f;
}).protected();


rpc.on('connection', () => {
    console.log('client connected');
});

rpc.on('disconnection', () => {
    console.log('client disconnected');
});

