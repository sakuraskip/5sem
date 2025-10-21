const http = require('http');
const url = require('url')
const host = '127.0.0.1';
const port = 8000;

const CalculateFactorial = function(num,callback)
{
    if(num < 0 || isNaN(num))
    {
        return setImmediate(()=>callback(new Error("num is NaN or less than 0")))
    }
    const CalculateFactorialHelper = function(num,accumulator)
    {
        if(num ===0 || num === 1)
        {
            return setImmediate(()=>callback(null,accumulator))
        }
        setImmediate(()=>CalculateFactorialHelper(num-1,accumulator*num));
    }
    CalculateFactorialHelper(num,1);
}

const server = http.createServer((request,response)=>
{
    response.statusCode = 200;
    response.setHeader("Content-Type",'application/json')
    let parsedUrl = url.parse(request.url,true);
    if(parsedUrl.pathname === '/fact')
    {
        let k = Number(parsedUrl.query.k);
        CalculateFactorial(k,(error,fact)=>
        {
            if(error)
            {
                console.error(error);
            }
            else
            {
                let resultString = {k,fact};

                const jsonResultString = JSON.stringify(resultString);
                response.end(jsonResultString);
            }
        });
    }
    
})


server.listen(port,host,()=>
{
    console.log(`server at ${host}:${port}`);
});