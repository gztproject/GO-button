/*
WebSerial example
Reads from a webSerial serial port, and writes to it.
Works on Chrome and Edge and Opera browsers. 

created 28 Jan 2022
modified 15 May 2022
by Tom Igoe
*/


// the DOM elements that might be changed by various functions:
let portButton;   // the open/close port button
let webserial;

let readingsSpan; // DOM element where the incoming readings go

let getVersionBtn;
let getDefaultPresetBtn;
let getPresetBtn;
let preset;

function setup() {
  // get the DOM elements and assign any listeners needed:
  preset = document.getElementById("preset");  
  
  readingsSpan = document.getElementById("readings");

  // span for incoming serial messages:
  getVersionBtn = document.getElementById("getVersionBtn");
  getVersionBtn.addEventListener("click", getVersion);

  getDefaultPresetBtn = document.getElementById("getDefaultPresetBtn");
  getDefaultPresetBtn.addEventListener("click", getDefaultPreset);

  getPresetBtn = document.getElementById("getPresetBtn");
  getPresetBtn.addEventListener("click", getPreset);
  
  webserial = new WebSerialPort();
  if (webserial) {
    webserial.on("data", serialRead);
     // port open/close button:
     portButton = document.getElementById("portButton");
     portButton.addEventListener("click", openClosePort);
   }
}

async function openClosePort() {
  // label for the button will change depending on what you do:
  let buttonLabel = "Open port";
  // if port is open, close it; if closed, open it:
  if (webserial.port) {
    await webserial.closePort();
  } else {
    await webserial.openPort();
    buttonLabel = "Close port";
  }
  // change button label:
  portButton.innerHTML = buttonLabel;
}

function serialRead(event) {
  //readingsSpan.innerHTML = event.detail.data;
  let string = "0x";
  console.log(event.detail);
  string += toHexString(event.detail.data);  
  readingsSpan.innerHTML = string;

  if(event.detail.data.length == 72)
  {
    p = new Preset(event.detail.data);
    p.Print();
    console.log(p);
  }
}

function toHexString(byteArray) {
    return Array.from(byteArray, function(byte) {
      return ('0' + (byte & 0xFF).toString(16)).slice(-2);
    }).join('')
  }

function getVersion(event) { 
    const data = [0x10];
    webserial.sendHex(data);
}

function getDefaultPreset(event) { 
    const data = [0x20];
    webserial.sendHex(data);
}

function getPreset(event) { 
    console.log(Number(preset.value));
    const data = [0x30, Number(preset.value)];
    webserial.sendHex(data);
}

// run the setup function when all the page is loaded:
document.addEventListener("DOMContentLoaded", setup);