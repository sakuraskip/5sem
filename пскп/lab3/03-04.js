const http = require('http');
const url = require('url')
const host = '127.0.0.1';
const port = 8000;

const CalculateFactorial = function(k)
{
    if(k < 0 || isNaN(k))
    {
        throw new Error("k меньше нуля")
    }
    if(k === 1 || k === 0)
    {
        return 1
    }
    else
    {
        return k * CalculateFactorial(k-1);
    }
}

const server = http.createServer((request,response)=>
{
    response.statusCode = 200;
    response.setHeader("Content-Type",'application/json')
    let parsedUrl = url.parse(request.url,true);
    if(parsedUrl.pathname === '/fact')
    {
        let k = Number(parsedUrl.query.k);
        let fact = CalculateFactorial(k);
        let resultString = {k,fact};

        const jsonResultString = JSON.stringify(resultString);
        response.end(jsonResultString);

    }
    
})


server.listen(port,host,()=>
{
    console.log(`server at ${host}:${port}`);
});