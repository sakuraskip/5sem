const url = require("url");
const fs = require("fs");
const path = require("path");

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

function staticHandler(staticDirectory, req, res) 
{
  if (req.method !== "GET") {
    res.statusCode = 405;
    res.end();
  }
  const parsedUrl = url.parse(req.url);
  let pathname = decodeURIComponent(parsedUrl.pathname);
  console.log(pathname);

  let extension = path.extname(pathname).slice(1).toLowerCase();

  let fullpath = staticDirectory + pathname;

  if (extensions.includes(extension)) {

    fs.stat(fullpath, (err, stats) => {
      if (err || !stats.isFile()) {
        res.statusCode = 404;
        res.setHeader("Content-Type", "text/html");
        res.end("<h1>file not found </h1>");
        return;
      }

    });
    res.statusCode = 200;
    res.setHeader("Content-Type", mimeTypes[extensions.indexOf(extension)]);

    fs.createReadStream(fullpath).pipe(res);
    console.log(`returned ${pathname}`);
  } else {
    res.statusCode = 404;
    res.end("file extension not supported");
  }
}

module.exports = staticHandler;
