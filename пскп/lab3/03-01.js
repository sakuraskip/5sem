const http = require('http');


const host = '127.0.0.1';
const port = 5000;
const appMode = ["norm","idle","stop","test"];

let currentMode = "norm";

const handleUserInput = function()
{
    process.stdout.write(`current mode: ${currentMode}\nInput new mode: `);
}
3
const server = http.createServer((request,response)=>
{
    
    response.statusCode = 200;
    response.setHeader("Content-Type",'text/html')
    response.end(`<h1> ${currentMode}</h1>`);
    
})

server.listen(port,host,()=>
{
    console.log(`server running at ${host}:${port}`);
    handleUserInput();
});
process.stdin.on('data',function(input){
        const userInput = input.toString().trim();
        if(userInput === 'exit')
        {
            process.stdout.write("exiting\n");
            server.close();
            process.exit(0);
        }
        if(!appMode.includes(userInput))
        {
            process.stderr.write(userInput+"\n");
            handleUserInput();
            return;
        }
        
        process.stdout.write(`switch: ${currentMode} --> ${userInput}\n`);
        currentMode = userInput;
        handleUserInput();
});
