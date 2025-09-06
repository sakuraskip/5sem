const { timeStamp } = require('console');
const http = require('http');

const host = '127.0.0.1';
const port = 5000;

const poop = async function(i)
{
    let fetchUrl = `http://127.0.0.1:8000/fact?k=${i}`
    const fetchResponse = await fetch(fetchUrl);
    const jsonResult = await fetchResponse.json();
    let {k,fact} = jsonResult;
    return {k,fact};
    
}

const server = http.createServer( async (request,response)=>
{
    response.statusCode = 200;
    response.setHeader("Content-Type",'text/html; charset=utf-8');

    let startTime = Date.now();
    let endTime;
    for(let i =0;i<=20;i++)
    {
        let {k,fact} = await poop(i);
        endTime = Date.now();
        response.write(`${i}. Результат: ${endTime-startTime}-${k}/${fact} <br>`);
    }
    response.end();

})


server.listen(port,host,()=>
{
    console.log(`server at ${host}:${port}`);
});