const express = require('express');
const http = require('http');
const { SerialPort } = require('serialport');
const { Server } = require('socket.io');
const path = require('path');

const app = express();
const server = http.createServer(app);
const io = new Server(server);

const port = new SerialPort({ path: '/dev/ttyACM0', baudRate: 115200 });

app.use(express.static(path.join(__dirname, 'public')));

port.on('open', () => {
  console.log('Serial Port Opened');
});

port.on('error', (err) => {
  console.error('Serial Port Error:', err);
});

port.on('data', (data) => {
  const sensorValue = data.toString().trim();
  console.log('Sensor Value:', sensorValue);
  io.emit('sensorData', sensorValue);
});

server.listen(3000, () => {
  console.log('Server is listening on port 3000');
});
