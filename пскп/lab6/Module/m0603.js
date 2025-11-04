const nodemailer = require("nodemailer");
const userGmail = "rainzape@gmail.com";

function send(msg, password) {
  return new Promise((res, rej) => {
    const transporter = nodemailer.createTransport({
      service: "gmail",
      auth: {
        user: userGmail,
        pass: password,
      },
      secure: true,
    });
    const mail = {
      from: userGmail,
      to: userGmail,
      subject: "m0603 msg",
      text: msg,
    };
    transporter.sendMail(mail, (err, info) => {
      if (err) {
        console.log(err);
        rej(err);
      }
      if (info) {
        console.log(info);
        res(info);
      }
    });
  });
}
module.exports = { send };
