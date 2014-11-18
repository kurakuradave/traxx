/*  Nov 2014
    Controller code for Traxx.
    Hardware Setup:
    - XBee Explorer USB plugged into computer's USB port
    - Logitect Precision Gamepad plugged into USB port  
*/

var XBeeUSB = { comName : "", pnpId : "usb-FTDI" };

var serialPort = require("serialport");
serialPort.list(function (err, ports) {
  ports.forEach(function(port) {
    console.log(port.comName);
    console.log(port.pnpId);
    console.log(port.manufacturer);
    // find XBeeUSB on usb serial
    if( port.pnpId.indexOf( XBeeUSB.pnpId ) != -1 ) { 
        XBeeUSB.comName = port.comName;
        console.log( XBeeUSB  );
    }
  });

  // if XBeeUSB NOT found
  if( XBeeUSB.comName == "" ) {
    console.log( "Error: Can't Find XBeeUSB On USB - Is It Plugged In?" );
    process.exit()
  } else { // XBeeUSB FOUND, open the serialport
    var daPath = XBeeUSB.comName; 
    var SerialPort = require("serialport").SerialPort;
    console.log( "Attempting to open Serialport to XBeeUSB..." );
    serialPort = new SerialPort( daPath, {
        baudrate: 9600
    } );
    serialPort.on("open", function () {
        console.log('Serialport to XBeeUSB opened!');
        serialPort.on('data', function(data) {
            console.log('data received: ' + data);
        });
        for ( var i = 1; i < 1000; i ++ ) {
            console.log( "sending 1" );
            serialPort.write("1\n", function(err, results) {
                console.log('err ' + err);
                console.log('results ' + results);
            });
        }

    });
  }
});






// Set a deadzone of +/-3500 (out of +/-32k) and a sensitivty of 350 to reduce signal noise in joystick axis
var joystick = new (require('joystick'))(0, 3500, 350);
// indicators to detect pressing of directional buttons
var north = false;
var south = false;
var east = false;
var west = false;

// on button presses
joystick.on('button', function( daButton ) {  
    if( daButton.number == 4 && daButton.value == 1 ) { // inc speed
      serialPort.write( "10\n", function( err, results ) { console.log( "10\n" ); } );
    } else if( daButton.number == 6 && daButton.value == 1 ) { // dec speed
      serialPort.write( "11\n", function( err, results ) { console.log( "11\n" ); } );
    }
});

// directional preses
joystick.on('axis', function( daAxis ){
    if( daAxis.number == 0 ) { // HORIZONTAL AXIS
        if( daAxis.value > 1000 ) {
            east = false;
            west = true;
            sendSerial()
        }
        else if( daAxis.value < -1000 ) {
            east = true;
            west = false;
            sendSerial();
        } else if( daAxis.value < 1000 && daAxis.value > -1000 ) {
            east = false;
            west = false;
            sendSerial();
        }
    } else if( daAxis.number == 1 ) { // VERTICAL axis  
        if( daAxis.value > 1000 ) {
            north = false;
            south = true;
            sendSerial();
        } else if( daAxis.value < -1000 ) {
            north = true; 
            south = false;
            sendSerial();      
        } else if( daAxis.value < 1000 && daAxis.value > -1000 ) {
            north =false;
            south = false;
            sendSerial();
        }
    }
});


sendSerial = function() {
    var msg = "9\n";
    if( north && east ) {
        msg = "2\n";
    } else if( north && west ) {
        msg = "8\n";
    } else if( south && east ) {
        msg = "4\n";
    } else if( south && west ) {
        msg = "6\n";
    } else if( north ) {
        msg = "1\n";
    } else if( south ) {
        msg = "5\n";
    } else if( east ) {
        msg = "3\n";
    } else if( west ) {
        msg = "7\n";
    } else {
      msg = "9\n";
    }
    serialPort.write( msg, function( err, results ) {  
        console.log( msg );
        console.log( 'err' + err );
        console.log( 'results' + results );
    } );
}
