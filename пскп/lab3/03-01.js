const { KeyObject } = require('crypto');
const http = require('http');
const prompt = require("prompt")

const host = '127.0.0.1';
const port = 5000;
const appMode = ["norm","idle","stop","test"];

let currentMode = "norm";

prompt.start();
const handleUserInput = function()
{
   prompt.get(['mode'],function(error,result)
{
    if(error)
    {
        console.log(`handleUserInput: ${error}`);
        return handleUserInput();
    }
    let userinput = result.mode;
    if(userinput === 'exit')
    {
        server.close();
        process.exit(0);
    }
    if(!appMode.includes(userinput))
    {
       console.log(userinput);
       return handleUserInput();
    }
    
    console.log("switch: "+ currentMode + "-->" + userinput);
    currentMode = userinput;
    handleUserInput();
}); 
}

const server = http.createServer((request,response)=>
{
    
    response.statusCode = 200;
    response.setHeader("Content-Type",'text/html')
    response.end(`<h1> ${currentMode}`);
    
})

server.listen(port,host,()=>
{
    console.log(`server listening at ${host}:${port}`);
    handleUserInput();
});