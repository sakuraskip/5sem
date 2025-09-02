const http = require('http');
const prompt = require("prompt-sync")({sigint:true})

const host = '127.0.0.1';
const port = 5000;
const appMode = ["norm","idle","stop","test"];

let currentMode = "norm";
const handleUserInput = function()
{
    let userinput = prompt();
    
    if(!appMode.includes(userinput))
    {
       console.log(userinput);
       return userinput;
    }
    console.log("switch: "+ currentMode + "-->" + userinput);
    currentMode = userinput;
    return userinput;
}

const server = http.createServer((request,response)=>
{
    let userinput = handleUserInput();
    if(userinput === "exit")
    {
        server.close();
    }
    response.statusCode = 200;
    response.end(`<h1> ${currentMode}`);
    
})

server.listen(port,host);