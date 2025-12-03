const http = require('http');
const fs = require('fs');
const path = require('path');
const url = require('url');
const WebSocket = require('ws');
const RPC = require('rpc-websockets').Server;


const rpc = new RPC({
    port: 3000,
    host: 'localhost'
});
rpc.event('notification');

const backupfolder = "backup";
const jsonPath = "StudentList.json"


function sendNotification(msg) {
    let notif = {
        id: Math.random()*10,
        msg,
        timestamp: Date.now()
    };
    console.log('notif send:', msg);
    rpc.emit('notification', notif);
}

rpc.on('connection', () => {
    console.log('client connected');
});

rpc.on('disconnection', () => {
    console.log('client disconnected');
});

const server = http.createServer(async (req, res) => {
    let parsedUrl = url.parse(req.url, true);
    let pathname = parsedUrl.pathname;
    let method = req.method;

    let body = '';
    req.on('data', (chunk) => {
        body += chunk.toString();
    });

    req.on('end', async () => {
        console.log("body:", body);
        res.setHeader('Content-Type', 'application/json');

        try {
            switch(true) {
                case method === 'DELETE' && pathname.startsWith('/backup/'):
                    {
                        let dateparm = pathname.split('/backup/')[1];
                        if(!dateparm || dateparm.length !== 8) {
                            res.statusCode = 400;
                            res.end(JSON.stringify({err: `invalid date`}));
                            break;
                        }

    
                        let allFiles = await fs.promises.readdir(backupfolder);
                        let deletedCount = 0;

                        for (let file of allFiles) {
                            let fileDate = file.split('_')[0].substring(0, 8);
                            if(fileDate < dateparm) {
                                await fs.promises.unlink(path.join(backupfolder, file));
                                deletedCount++;
                            }
                        }

                        res.statusCode = 200;
                        sendNotification('backups deleted after date: ' + dateparm + ', count: ' + deletedCount);
                        res.end(JSON.stringify({msg: 'backups deleted', count: deletedCount}));
                    }
                    break;
                    case method === 'GET' && pathname === '/backup':
                    {
                        if (!fs.existsSync(backupfolder)) {
                            res.statusCode = 200;
                            res.end('[]');
                            break;
                        }

                        let files = await fs.promises.readdir(backupfolder);
                        let parsedFiles = files.map(f => ({
                            name: f,
                            date: f.split('_')[0]
                        }));
                        res.statusCode = 200;
                        res.end(JSON.stringify(parsedFiles));
                    }
                    break;
                    case method === 'POST' && pathname === '/backup':
                    {
                        if (!fs.existsSync(jsonPath)) {
                            res.statusCode = 404;
                            res.end(JSON.stringify({err: 'Student list file not found'}));
                            break;
                        }

                       

                        setTimeout(async () => {
                            try {
                                let now = new Date();
                                let y = now.getFullYear();
                                let M = String(now.getMonth() + 1).padStart(2, '0');
                                let d = String(now.getDate()).padStart(2, '0');
                                let H = String(now.getHours()).padStart(2, '0');
                                let m = String(now.getMinutes()).padStart(2, '0');
                                let s = String(now.getSeconds()).padStart(2, '0');

                                const backupName = `${y}${M}${d}${H}${m}${s}_${jsonPath}`;

                                let filetext = await fs.promises.readFile(jsonPath);
                                await fs.promises.writeFile(path.join(backupfolder, backupName), filetext);

                                res.statusCode = 201;
                                sendNotification('backup created: ' + backupName);
                                res.end(JSON.stringify({msg: 'backup created'}));
                            } catch (error) {
                                res.statusCode = 500;
                                res.end(JSON.stringify({err: 'backup creation failed'}));
                            }
                        }, 2000);
                    }
                    break;

                case method === 'GET' && pathname === '/':
                    {
                        let filetext = await fs.promises.readFile(jsonPath);
                        res.statusCode = 200;
                        res.end(filetext);
                    }
                    break;

                case method === 'GET' && pathname !== '/' && pathname.startsWith('/'):
                    {
                        let id = pathname.slice(1);
                        let filetext = await fs.promises.readFile(jsonPath);
                        let students = JSON.parse(filetext);

                        let student = students.find(st => st.id == id); 

                        if(!student) {
                            res.statusCode = 404;
                            res.end(JSON.stringify({err: `student with id ${id} not found`}));
                            break;
                        }
                        res.statusCode = 200;
                        res.end(JSON.stringify(student));
                    }
                    break;

                case method === 'POST' && pathname === '/':
                    {
                        if (!body) {
                            res.statusCode = 400;
                            res.end(JSON.stringify({err: `empty body`}));
                            break;
                        }

                        let student;
                        try {
                            student = JSON.parse(body);
                        } catch (e) {
                            res.statusCode = 400;
                            res.end(JSON.stringify({err: `invalid JSON: ${e.message}`}));
                            break;
                        }

                        if(!student.id || !student.name || !student.bday || !student.speciality) {
                            res.statusCode = 400;
                            res.end(JSON.stringify({err: `wrong student structure`}));
                            break;
                        }

                        let students = [];
                        if (fs.existsSync(jsonPath)) {
                            let filetext = await fs.promises.readFile(jsonPath);
                            students = JSON.parse(filetext);
                        }

                        let duplicate = students.find(st => st.id == student.id); 
                        if(duplicate) {
                            res.statusCode = 400;
                            res.end(JSON.stringify({err: `student with id: ${student.id} already exists`}));
                            break;
                        }

                        students.push(student);
                        await fs.promises.writeFile(jsonPath, JSON.stringify(students, null, 2));
                        res.statusCode = 201;
                        sendNotification('student created id: ' + student.id);
                        res.end(JSON.stringify(student));
                    }
                    break;

                case method === 'PUT' && pathname === '/':
                    {
                        if (!body) {
                            res.statusCode = 400;
                            res.end(JSON.stringify({err: `empty body`}));
                            break;
                        }

                        let updatedstudent;
                        try {
                            updatedstudent = JSON.parse(body);
                        } catch (e) {
                            res.statusCode = 400;
                            res.end(JSON.stringify({err: `invalid JSON: ${e.message}`}));
                            break;
                        }

                        if(!updatedstudent.id || !updatedstudent.name || !updatedstudent.bday || !updatedstudent.speciality) {
                            res.statusCode = 400;
                            res.end(JSON.stringify({err: `wrong student structure`}));
                            break;
                        }

                        if (!fs.existsSync(jsonPath)) {
                            res.statusCode = 404;
                            res.end(JSON.stringify({err: `no students file found`}));
                            break;
                        }

                        let filetext = await fs.promises.readFile(jsonPath);
                        let students = JSON.parse(filetext);

                        let oldstIndex = students.findIndex(st => st.id == updatedstudent.id); 
                        if(oldstIndex === -1) {
                            res.statusCode = 404;
                            res.end(JSON.stringify({err: `no student with id ${updatedstudent.id} found`}));
                            break;
                        }

                        students[oldstIndex] = updatedstudent;
                        await fs.promises.writeFile(jsonPath, JSON.stringify(students, null, 2));
                        sendNotification('student updated id: ' + updatedstudent.id);
                        res.statusCode = 200;
                        res.end(JSON.stringify(updatedstudent));
                    }
                    break;

                case method === 'DELETE' && pathname !== '/' && pathname.startsWith('/'):
                    {
                        let id = pathname.slice(1);

                        if (!fs.existsSync(jsonPath)) {
                            res.statusCode = 404;
                            res.end(JSON.stringify({err: `student with id ${id} not found`}));
                            break;
                        }

                        let filetext = await fs.promises.readFile(jsonPath);
                        let students = JSON.parse(filetext);

                        let oldstIndex = students.findIndex(st => st.id == id); 

                        if(oldstIndex === -1) {
                            res.statusCode = 404;
                            res.end(JSON.stringify({err: `student with id ${id} not found`}));
                            break;
                        }

                        let deletedStudent = students.splice(oldstIndex, 1)[0];
                        await fs.promises.writeFile(jsonPath, JSON.stringify(students, null, 2));
                        sendNotification('student deleted id: ' + deletedStudent.id);
                        res.statusCode = 200;
                        res.end(JSON.stringify(deletedStudent));
                    }
                    break;

                
                default:
                    {
                        res.statusCode = 404;
                        res.end(JSON.stringify({err: 'not found, wrong url or method'}));
                    }
            }
        } catch (error) {
            console.error('Server error:', error);
            res.statusCode = 500;
            res.end(JSON.stringify({err: 'internal server error'}));
        }
    });
});

server.listen(5000, () => {
    console.log('server at port 5000');
});