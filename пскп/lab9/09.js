const http = require ("http");
const url = require ("url");
const path = require ("path");
const { XMLParser, XMLBuilder } = require('fast-xml-parser');
const fs = require ("fs");

const filesDir = path.join(__dirname, 'files');
let host = "127.0.0.1";
let port = 5000;

const parser = new XMLParser(
    {
        ignoreAttributes: false,
        attributeNamePrefix: '',
    }
);
const builder = new XMLBuilder(
    {
        ignoreAttributes:false,
        attributeNamePrefix:'',
        format:true,
        suppressEmptyNode:true,
    }
);



const server = http.createServer(async (req,res)=>
{
    let data ='';
    req.on('data',(chunk)=>
    {
        data+=chunk;
    })
    const parsedUrl = url.parse(req.url, true);
    res.statusCode = 200;
    switch(true)
    {
        case req.method === 'GET' && req.url === '/task1':
            {
                res.setHeader('Content-Type','text/plain');
                res.end('task1 body response');
                break;
            }
        case req.method === 'GET' && parsedUrl.pathname === '/task2':
            {
                if('x' in parsedUrl.query && 'y' in parsedUrl.query)
                {
                    res.setHeader('Content-Type','text/plain');
                    res.end('x+y= '+ Number(parsedUrl.query.x + parsedUrl.query.y));
                }
                break;
               
            }
        case req.method === 'POST' && parsedUrl.pathname === '/task3':
            {
                if('x' in parsedUrl.query && 'y' in parsedUrl.query && 's' in parsedUrl.query)
                {
                    res.setHeader('Content-Type','text/plain');
                    res.end(`x: ${parsedUrl.query.x}\ny:${parsedUrl.query.y}\ns:${parsedUrl.query.s} `);
                }
                break;
            }
        case req.method === 'POST' && req.url === '/task4':
            {
                req.on('end',()=>
                {
                    try
                {
                    let parsedData = JSON.parse(data);

                    let message = parsedData?.__comment.split('.')[1];
                    let x = Number(parsedData?.x);
                    let y = Number(parsedData?.y);
                    let s = parsedData?.s.toString();

                    let m = parsedData?.m;
                    let o = parsedData?.o;

                    if(message !=='' && !isNaN(x) && !isNaN(y) && Array.isArray(m) && o !==null)
                    {
                        let xplusy = x+y;
                        let mLength = m.length;
                        let objectValues = Object.values(o).join(', ');
                        let responseMessage = s+ ':'+objectValues; 

                        let jsonResponse = {
                            __comment: `Ответ. ${message}`,
                            xplusy,
                            responseMessage,
                            mLength
                        }
                        
                        res.statusCode = 200;
                        res.setHeader('Content-Type','application/json')
                        res.end(JSON.stringify(jsonResponse,null,1));
                    }
                }
                catch(exception)
                {
                    console.log(exception);
                    res.statusCode = 400;
                    res.setHeader('Content-Type','application/json')
                    res.end(JSON.stringify(exception));

                }
                })
                
                break;
            } 
        case req.method === 'POST' && req.url === '/task5':
            {
                req.on('end',()=>
                {
                     try
                {
                let parsedBody = parser.parse(data);
                if( !parsedBody.request )
                {
                res.statusCode = 400;
                res.setHeader('Content-Type','text/plain');
                res.end('xml error: no request field');
                }
                let requestId = Number(parsedBody.request.id);

                let arrayX = parsedBody.request.x;
                let arrayY = parsedBody.request.m;
                if( !arrayX )
                {
                arrayX = [];
                }
                if( !arrayY )
                {
                arrayY = [];
                }
                let sumX = arrayX.reduce((accum,element)=>
                {
                let value = Number(element.value);
                return accum + value;
                },0);
                
                let sumY = arrayY.map(element=>element.value).join('');

                const response = {
                response:
                {
                    id:(requestId + 5).toString(),
                    request:requestId,
                    sum:
                    {
                    element:"X",
                    result: sumX.toString(),
                    },
                    concat:
                    {
                    element: 'M',
                    result:sumY,
                    },
                },
                };

                const xmlresponse = builder.build(response);
                res.statusCode = 200;
                res.setHeader('Content-Type','application/xml');
                res.end(xmlresponse);
                }
                catch(exception)
                {
                    res.statusCode = 400;
                    res.setHeader('Content-Type','text/plain')
                    res.end('xml error: ' + exception.message);
                }
                })
               
                break;
            }
        case req.method === 'POST' && req.url ==='/task6':
            {
               async function writefile(){ let filename = `task9.txt`;
                let filepath = path.join(filesDir,filename);
                
                let fileStream = fs.createWriteStream(filepath);

                req.pipe(fileStream);

                fileStream.on('error',()=>
                {
                    res.statusCode = 500;
                    res.end('filestream failed');
                });
                fileStream.on('finish',()=>
                {
                    res.setHeader('Content-Type','text/plain');
                    res.end('file saved');
                });}
                await writefile();
                break;
            }
        case req.method === 'POST' && req.url === '/task7':
            {
                let filename = `task9.png`;
                let filepath = path.join(filesDir,filename);
                
                let fileStream = fs.createWriteStream(filepath);

                res.setHeader('Content-Type','text/plain');
                req.pipe(fileStream);

                fileStream.on('error',()=>
                {
                    res.statusCode = 500;
                    res.end('filestream failed');
                });
                fileStream.on('finish',()=>
                {
                    res.end('png saved');
                });
                break;
            }
        case req.method === 'GET' && req.url === '/task8':
            {
                let filename = 'LibLabs3D.cpp';
                let filepath = path.join(filesDir,filename);

                res.setHeader('Content-Type','text/plain');
                res.setHeader('Content-Disposition', `attachment; filename=${filename}`);
                let filestream = fs.createReadStream(filepath).pipe(res);
                break;
            }
            default:
                {
                    res.statusCode = 404;
                    res.end('not found');
                }
    }
});
server.listen(port,host,()=>
{
    console.log("server started, port: " + port);
})