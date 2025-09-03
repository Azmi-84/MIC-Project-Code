const express = require('express');
const http = require('http');
const { SerialPort } = require('serialport');
const { ReadlineParser } = require('@serialport/parser-readline');
const { Server } = require('socket.io');
const path = require('path');

const app = express();
const server = http.createServer(app);
const io = new Server(server);

const port = new SerialPort({ path: '/dev/ttyACM0', baudRate: 115200 });
const parser = port.pipe(new ReadlineParser({ delimiter: '\n' }));

app.use(express.static(path.join(__dirname, 'public')));

port.on('open', () => {
  console.log('Serial Port Opened');
});

port.on('error', (err) => {
  console.error('Serial Port Error:', err);
});

// Listen for complete lines from the Arduino
parser.on('data', (line) => {
  const sensorValue = line.trim();
  console.log(sensorValue);
  io.emit('sensorData', sensorValue);
});

server.listen(3000, () => {
  console.log('Server is listening on port 3000');
});
