const http = require('http');
const fs = require('fs');
const url = require('url');
const staticHandler = require('./m0701');
const host = "127.0.0.1";
const port = 5000;

const staticDirectory = __dirname+ '/static';


const server = http.createServer((req,res)=>
{
    staticHandler(staticDirectory,req,res);

});
server.listen(port,host,()=>
{
    console.log("server started ");
})