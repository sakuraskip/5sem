const http = require("http");
const fs = require("fs");
const url = require("url");
const path = require('path');
const querystring = require("querystring");
const formidable = require('formidable');
const {XMLParser, XMLBuilder} = require('fast-xml-parser');

const staticDirectory = path.join(__dirname,'/static');
const port = 5000;
const host = '127.0.0.1';

const extensions = ["html", "css", "js", "png", "docx", "json", "xml", "mp4"];
const mimeTypes = [
  "text/html",
  "text/css",
  "text/javascript",
  "image/png",
  "application/msword",
  "application/json",
  "application/xml",
  "video/mp4",
];

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

const server = http.createServer((req, res) => {
  const parsedUrl = url.parse(req.url, true);
  switch (true) {
    case req.method === "GET" && parsedUrl.pathname === "/connection": {
      HandleConnection(res, parsedUrl);
      break;
    }
    case req.method === "GET" && parsedUrl.pathname === "/headers": {
      HandleHeaders(req, res);
      break;
    }
    case req.method === "GET" &&
      parsedUrl.pathname === "/parameter" &&
      'x' in parsedUrl.query &&
      'y' in parsedUrl.query: {
      HandleParameterQuery(res, parsedUrl);
      break;
    }
    case req.method === "GET" && parsedUrl.pathname.startsWith("/parameter/"): {
      HandleParameter(res, parsedUrl);
      break;
    }
    case req.method === "GET" && parsedUrl.pathname === "/close": {
      HandleClose(res, parsedUrl);
      break;
    }
    case req.method === "GET" && parsedUrl.pathname === "/socket": {
      HandleSocket(res, req, parsedUrl);
      break;
    }
    case req.method === "GET" && parsedUrl.pathname === "/req-data": {
      HandleReqData(res, req, parsedUrl);
      break;
    }
    case req.method === "GET" &&
      parsedUrl.pathname === "/resp-status" &&
      'code' in parsedUrl.query &&
      'mess' in parsedUrl.query: {
      HandleRespStatus(res, req, parsedUrl);
      break;
    }

    case req.method === "GET" && parsedUrl.pathname === "/formparameter": {
      HandleFormGet(res, req, parsedUrl);
      break;
    }
    case req.method === "POST" && parsedUrl.pathname === "/formparameter": {
      HandleFormPost(res, req, parsedUrl);
      break;
    }
    case req.method === "POST" && parsedUrl.pathname === "/json": {
      HandleJson(res,req,parsedUrl);
      break;
    }
    case req.method === "POST" && parsedUrl.pathname === "/xml": {
      HandleXml(res,req,parsedUrl);
      break;
    }
    case req.method === "GET" && parsedUrl.pathname === "/files": {
      HandleFiles(res,req,parsedUrl);
      break;
    }
    case req.method === "GET" && parsedUrl.pathname.startsWith("/files/"): {
      HandleFilesWithName(res,req,parsedUrl);
      break;
    }
    case req.method === "GET" && parsedUrl.pathname === "/upload": {
      HandleUploadGet(res,req,parsedUrl);
      break;
    }
    case req.method === "POST" && parsedUrl.pathname === '/upload':
      {
        HandleUploadPost(res,req,parsedUrl);
        break;
      }
      default:
        {
          res.statusCode = 400;
          res.end("wrong method or url or anything");
        }
  }
});

server.listen(port,host,()=>
{
  console.log('server started');
});


function HandleConnection(res, parsedUrl) {
  res.setHeader("Content-Type", "text/plain");

  if (parsedUrl.query.set) {
    let keepAliveSec = parseInt(parsedUrl.query.set, 10);

    if (keepAliveSec <= 0 || isNaN(keepAliveSec)) {
      res.statusCode = 400;
      res.end("set should be > 0 (seconds)");
    } else {
      server.keepAliveTimeout = keepAliveSec * 1000;

      res.statusCode = 200;
      res.end(" новое значение параметра keepAliveTimeout: " + keepAliveSec + " ");
    }
  } else {
    res.statusCode = 200;
    res.end("KeepAliveTimeout: " + Math.floor(server.keepAliveTimeout / 1000) + " seconds");
  }

  return;
}
function HandleHeaders(req, res) {
  res.setHeader("X-lab8-Header", "lab number 8");

  let reqheaders = JSON.stringify(req.headers, null, 1);

  res.statusCode = 200;
  res.setHeader("Content-Type", "text/html");
  let resheaders = JSON.stringify(res.getHeaders(), null, 1);
  res.end(`<h1>Request headers: </h1>
        <p>${reqheaders}</p>
        <h1>Response Headers </h1>
        <p>${resheaders}</p>`);
  return;
}
function HandleParameterQuery(res, parsedUrl) {
  let x = Number(parsedUrl.query.x);
  let y = Number(parsedUrl.query.y);

  if (!isNaN(x) && !isNaN(y)) {
    res.statusCode = 200;
    res.setHeader("Content-Type", "text/html");
    res.end(`<p> x+y: ${x + y} x-y: ${x - y} x*y: ${x * y} x/y: ${x / y}</p>`);
  } else {
    res.statusCode = 400;
    res.setHeader("Content-Type", "text/plain");
    res.end("x and y should be a numbers");
  }
  return;
}
function HandleParameter(res, parsedUrl) {
  let params = parsedUrl.pathname.split("/");
  if (params.length == 4) {
    let x = Number(params[2]);
    let y = Number(params[3]);

    if (!isNaN(x) && !isNaN(y)) {
      res.statusCode = 200;
      res.setHeader("Content-Type", "text/html");
      res.end(
        `<p> x+y: ${x + y} x-y: ${x - y} x*y: ${x * y} x/y: ${x / y}</p>`
      );
    } else {
      res.statusCode = 400;
      res.setHeader("Content-Type", "text/plain");
      res.end(parsedUrl.pathname);
    }
    return;
  } else {
    res.statusCode = 404;
    res.end("wrong url");
  }
}
function HandleClose(res, parsedUrl) {
  res.statusCode = 200;
  res.setHeader("Content-Type", "text/plain");
  res.end("server will stop in 10 seconds");
  console.log("handle close initiated");

  setTimeout(() => {
    server.close(() => {
      console.log("stopping now");
      process.exit(0);
    });
  }, 10000);
  return;
}
function HandleSocket(res, req, parsedUrl) {
  let clientip = req.socket.remoteAddress;
  let clientport = req.socket.remotePort;
  let serverip = req.socket.localAddress;
  let serverport = req.socket.localPort;

  res.statusCode = 200;
  res.setHeader("Content-Type", "text/plain");
  res.end(`client ip: ${clientip} client port: ${clientport}
        server ip: ${serverip} server port: ${serverport}`);
  return;
}
function HandleReqData(res, req, parsedUrl) {
  let body = "";
  req.on("data", (chunk) => {
    body += chunk.toString();
    console.log("chunk: " + chunk.toString());
  });
  req.on("end", () => {
    res.statusCode = 200;
    res.setHeader("Content-Type", "text/plain");
    res.end(`data: ${body}`);
  });
  return;
}
function HandleRespStatus(res, req, parsedUrl) {
  let code = parseInt(parsedUrl.query.code);
  let mess = parsedUrl.query.mess;

  if (isNaN(code) || code > 599 || code < 100) {
    res.statusCode = 404;
    res.end("wrong status code");
    return;
  }
  res.statusCode = code;
  res.setHeader("Content-Type", "text/plain");
  res.end(`status: ${code} mess: ${mess}`);
  return;
}
function HandleFormGet(res, req, parsedUrl) {
  res.statusCode = 200;
  res.setHeader("Content-Type", "text/html");
  fs.createReadStream("form.html").pipe(res);
  return;
}
function HandleFormPost(res, req, parsedUrl) {
  let body = '';
  req.on('data', chunk => {
    body += chunk;
  });

  req.on('end', () => {
    const params = querystring.parse(body);
    console.log(params);
    res.statusCode = 200;
    res.setHeader('Content-Type', 'text/html');
    res.write('<h1>params:</h1><br/>');
    for (const key in params) {
      const value = params[key];
      if (Array.isArray(value)) {
        res.write(`<p>${key}: ${value.join(', ')}</p>`);
      } else {
        res.write(`<p>${key}: ${value}</p>`);
      }
    }
    res.end();
  });
}
function HandleJson(res,req,parsedUrl)
{
    let body = '';
    req.on('data',chunk=>
    {
        body+=chunk;
    });
    
    req.on('end',()=>
    {
        try
        {
            let parsedData = JSON.parse(body);

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
        
    });
    return;
}
function HandleXml(res,req,parsedUrl)
{
    let body = '';
    req.on('data',(chunk)=>
    {
      body+=chunk.toString();
    });

    req.on('end',()=>
    {
      try
      {
        let parsedBody = parser.parse(body);
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
    });
    return;
}
function HandleFiles(res,req,parsedUrl)
{

  fs.readdir(staticDirectory,(err,files)=>
  {
    if(err)
    {
      res.statusCode = 404;
      res.end('error reading staticDirectory');
      return;
    }
    res.statusCode = 200;
    res.setHeader('Content-Type','text/plain');
    res.setHeader('X-static-files-count',files.length.toString())
    res.end();

  });
  return;  
}
function HandleFilesWithName(res,req,parsedUrl)
{
  let filename = decodeURIComponent(parsedUrl.pathname.slice('/files/'.length));
  let extension = path.extname(filename).slice(1).toLowerCase();

  let mimeTypeIndex = extensions.indexOf(extension);

  
  let filepath = path.join(staticDirectory,filename);
  fs.access(filepath, fs.constants.F_OK, (accessErr) => {
        if (accessErr) {
            res.statusCode = 404;
            res.end('File not found');
            return;
        }
  try
  {
      if(mimeTypeIndex != -1)
      {
        res.setHeader('Content-Type',mimeTypes[mimeTypeIndex]);
      }
      res.setHeader('Content-Disposition',`attachment; filename="${filename}"`);
      res.statusCode = 200;
      fs.createReadStream(filepath).pipe(res);
  }
  catch(exception)
  {
    res.statusCode = 400;
    res.end('error: ', exception.message);
  }
});
return;
  
}
function HandleUploadGet(res,req,parsedUrl)
{
  res.setHeader('Content-Type','text/html');
  res.statusCode = 200;
  fs.createReadStream('upload.html').pipe(res);
  return;
}
function HandleUploadPost(res,req,parsedUrl)
{
  let form = new formidable.IncomingForm();
  form.parse(req, (err,fields,files)=>
  {
    if(err)
    {
      res.statusCode = 400;
      res.end('file upload error');
      return;
    }
    let uploadedFile = files.uploadFile[0];
    if( !uploadedFile )
    {
      res.statusCode = 400;
      res.end('file was not uploaded');
      return;
    }

    let newFilePath = path.join(staticDirectory,uploadedFile.originalFilename);
    fs.rename(uploadedFile.filepath,newFilePath,err =>
    {
      if(err)
      {
        res.statusCode = 400;
        res.end('file rename/save error');
        return;
      }
    res.setHeader('Content-Type','text/plain');
    res.statusCode = 200;
    res.end('file uploaded successfully');
    });

   

  });
  return;
}


