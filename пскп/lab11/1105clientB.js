const RPC = require('rpc-websockets').Client;
const async = require('async');

const ws = new RPC('ws://localhost:4000');

ws.on('open', () => {
    console.log('connected');

    ws.login({login: 'user', password: 'pass'})
        .then(() => {
            async.parallel({
                square1: (cb) => {
                    ws.call('square', [3])
                        .then(result => cb(null, result))
                        .catch(err => cb(err));
                },
                square2: (cb) => {
                    ws.call('square', [5, 4])
                        .then(result => cb(null, result))
                        .catch(err => cb(err));
                },
                sum1: (cb) => {
                    ws.call('sum', [2])
                        .then(result => cb(null, result))
                        .catch(err => cb(err));
                },
                sum2: (cb) => {
                    ws.call('sum', [2, 4, 6, 8, 10])
                        .then(result => cb(null, result))
                        .catch(err => cb(err));
                },
                mul1: (cb) => {
                    ws.call('mul', [3])
                        .then(result => cb(null, result))
                        .catch(err => cb(err));
                },
                mul2: (cb) => {
                    ws.call('mul', [3, 5, 7, 9, 11, 13])
                        .then(result => cb(null, result))
                        .catch(err => cb(err));
                },
                fib1: (cb) => {
                    ws.call('fib', [1])
                        .then(result => cb(null, result))
                        .catch(err => cb(err));
                },
                fib2: (cb) => {
                    ws.call('fib', [2])
                        .then(result => cb(null, result))
                        .catch(err => cb(err));
                },
                fib3: (cb) => {
                    ws.call('fib', [7])
                        .then(result => cb(null, result))
                        .catch(err => cb(err));
                },
                fact1: (cb) => {
                    ws.call('fact', [0])
                        .then(result => cb(null, result))
                        .catch(err => cb(err));
                },
                fact2: (cb) => {
                    ws.call('fact', [5])
                        .then(result => cb(null, result))
                        .catch(err => cb(err));
                },
                fact3: (cb) => {
                    ws.call('fact', [10])
                        .then(result => cb(null, result))
                        .catch(err => cb(err));
                }
            }, (err, results) => {
                if (err) {
                    console.error('Error:', err);
                } else {
                    Object.entries(results).forEach(([key, result], index) => {
                        console.log(`call ${index + 1} (${key}):`, result);
                    });
                }
                ws.close();
            });
        })
        .catch(err => {
            console.error('Login failed:', err);
            ws.close();
        });
});