const http = require("http");
const fs = require("fs/promises");
const fstream = require('fs');
const path = require('path');
const { XMLParser, XMLBuilder } = require('fast-xml-parser');
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
http.get('http://localhost:5000/task1', (res) => {
    console.log("TASK1");
    console.log('status code:', res.statusCode);
    console.log('status message:', res.statusMessage);

    const socket = res.socket;
    console.log('remote IP:', socket.remoteAddress);
    console.log('remote port:', socket.remotePort);

    let body = '';
    res.setEncoding('utf-8');
    res.on('data', (chunk) => {
        body += chunk;
    });

    res.on('end', () => {
        console.log('body: ' + body);
        console.log('------------------------------------------------------')
    });
}).on('error', (err) => {
    console.error('Error with TASK1:', err.message);
});

http.get('http://localhost:5000/task2?x=4&y=5', (res) => {
    console.log("TASK2");
    console.log('status code:', res.statusCode);

    let body = '';
    res.setEncoding('utf-8');
    res.on('data', (chunk) => {
        body += chunk;
    });

    res.on('end', () => {
        console.log('body: ' + body);
        console.log('------------------------------------------------------')
    });
}).on('error', (err) => {
    console.error('Error with TASK2:', err.message);
});

//task3
let options = {
    hostname: 'localhost',
  port: 5000,
  path: '/task3?x=ta&y=sk&s=3',
  method: 'POST',
}
let request = http.request(options, (res) => {
    console.log("TASK3");
    console.log('status code:', res.statusCode);

    let body = '';
    res.setEncoding('utf-8');
    res.on('data', (chunk) => {
        body += chunk;
    });

    res.on('end', () => {
        console.log('body: ' + body);
        console.log('------------------------------------------------------')
    });
}).on('error', (err) => {
    console.error('Error with TASK3:', err.message);
});
request.end();

//task4
async function task4() {
let filedata = await fs.readFile(path.join(__dirname,'files/data.json'));
JSON.parse(filedata)
options = {
    hostname: 'localhost',
  port: 5000,
  path: '/task4',
  method: 'POST',
  headers: {
    'Content-Type': 'application/json'
    
  }
}
request = http.request(options, (res) => {
    console.log("TASK4");
    console.log('status code:', res.statusCode);



    let body = '';
    res.setEncoding('utf-8');
    res.on('data', (chunk) => {
        body += chunk;
    });

    res.on('end', () => {
        console.log('body: ' + body);
        console.log('------------------------------------------------------')
    });
    
}).on('error', (err) => {
    console.error('Error with TASK4:', err.message);
});
request.write(filedata);
request.end();
}
task4();
//task5

async function task5() {
let filedata = await fs.readFile(path.join(__dirname,'files/data.xml'),'utf-8');
options = {
    hostname: 'localhost',
  port: 5000,
  path: '/task5',
  method: 'POST',
  headers: {
    'Content-Type': 'application/xml',
    'Content-Length': Buffer.byteLength(filedata, 'utf8'),
    
  }
}
request = http.request(options, (res) => {
    console.log("TASK5");
    console.log('status code:', res.statusCode);



    let body = '';
    res.setEncoding('utf-8');
    res.on('data', (chunk) => {
        body += chunk;
    });

    res.on('end', () => {
        console.log('body: ' + body);
        console.log('------------------------------------------------------')
    });
    
}).on('error', (err) => {
    console.error('Error with TASK5:', err.message);
});
request.write(filedata,'utf-8');
request.end();
}
task5();

//task6
options = {
    hostname: 'localhost',
  port: 5000,
  path: '/task6',
  method: 'POST',
  headers: {
    'Content-Type': 'text/plain'
  }
}
request = http.request(options, (res) => {

    console.log("TASK6");
    console.log('status code:', res.statusCode);

    let body = '';
    res.setEncoding('utf-8');
    res.on('data', (chunk) => {
        body += chunk;
    });

    res.on('end', () => {
        console.log('body: ' + body);
        console.log('------------------------------------------------------')
    });

}).on('error', (err) => {
    console.error('Error with TASK6:', err.message);
});

let stream = fstream.createReadStream(path.join(__dirname,'myfile.txt'));
stream.pipe(request);

//task7
options = {
    hostname: 'localhost',
  port: 5000,
  path: '/task7',
  method: 'POST',
  headers: {
    'Content-Type': 'image/png'
  }
}
request = http.request(options, (res) => {

    console.log("TASK7");
    console.log('status code:', res.statusCode);

    let body = '';
    res.setEncoding('utf-8');
    res.on('data', (chunk) => {
        body += chunk;
    });

    res.on('end', () => {
        console.log('body: ' + body);
        console.log('------------------------------------------------------')
    });

}).on('error', (err) => {
    console.error('Error with TASK7:', err.message);
});

stream = fstream.createReadStream(path.join(__dirname,'myfile.png'));
stream.pipe(request);

//task8
http.get('http://127.0.0.1:5000/task8',(res)=>
{
    let filepath = path.join(__dirname,"clientside","downloaded_labs3d.cpp");
    let filestream = fstream.createWriteStream(filepath);

    res.pipe(filestream);
    filestream.on('finish',()=>
    {
        console.log('file saved on client side');
    });
});