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

let fwVer;
let DefaultPreset;
let CurrentPreset;
var Presets = [];

let state = 0;

function setup() {
  // get the DOM elements and assign any listeners needed:
  readingsSpan = document.getElementById("readings");

  // span for incoming serial messages:  
  webserial = new WebSerialPort();
  if (webserial) {
    webserial.on("data", serialRead);
    // port open/close button:
    portButton = document.getElementById("portButton");
    portButton.addEventListener("click", openClosePort);
  }

  document.getElementById("btn-prev").addEventListener("click", prevPreset);
  document.getElementById("btn-next").addEventListener("click", nextPreset);
}

async function openClosePort() {  
  // if port is open, close it; if closed, open it:
  if (webserial.port) {
    await webserial.closePort();
  } else {
    await webserial.openPort();
    if(tryReadDevice())
    {
      portButton.classList.add("dn");
      document.getElementById("content-container").classList.remove("dn");
    }
    else
    {
      window.alert("Communication error!\nAre you sure you selected the right device?");
      await webserial.closePort();
    }
  }
}

function tryReadDevice()
{   
  switch(state) 
  {
    case 0:
      getVersion();   
      setTimeout(tryReadDevice,100);
      return true;
    case 1:
      getDefaultPreset(); 
      setTimeout(tryReadDevice,100);
      return;     
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
      getPreset(state-10); 
      setTimeout(tryReadDevice,100);
      return true;     
    case 99: //wait for reply
      setTimeout(tryReadDevice,100);
      return true;     
    default:      
      break;    
  }
   
  
  CurrentPreset = DefaultPreset; 
  Presets[CurrentPreset].Print();   
  enableDisableButtons(); 
  return true;
}

function nextPreset()
{
  if(CurrentPreset++ < 4)
    Presets[CurrentPreset].Print();  

  enableDisableButtons();
}

function prevPreset()
{
  if(CurrentPreset-- > 0)
    Presets[CurrentPreset].Print();  

  enableDisableButtons();
}

function enableDisableButtons()
{
  if(CurrentPreset >= 4)
    document.getElementById("btn-next").disabled = true;
  else
    document.getElementById("btn-next").disabled = false;

  if(CurrentPreset <= 0)
    document.getElementById("btn-prev").disabled = true;
  else
    document.getElementById("btn-prev").disabled = false;
}

function serialRead(event) {
  console.log("serialRead()");
  let string = "0x";
  console.log(event.detail);
  string += toHexString(event.detail.data);  
  readingsSpan.innerHTML = string;  
}

function toHexString(byteArray) {
  return Array.from(byteArray, function(byte) {
    return ('0' + (byte & 0xFF).toString(16)).slice(-2);
  }).join('')
}

function getVersion() {    
    //Set the listener  
    webserial.on("data", parseVersion); 

    console.log("Sending GetVersion cmd.");
    const data = [0x10];
    gotResponse = false;
    webserial.sendHex(data);   
    
    state = 99; //wait        
}

function parseVersion(event) {
  console.log("Parsing the version.");
  fwVer = new Version(event.detail.data);
  fwVer.Print();
  console.log(fwVer);

  webserial.off("data", parseVersion); 
  state = 1;
}

function getDefaultPreset() { 
   //Set the listener
   webserial.on("data", parseDefaultPreset);

    console.log("Sending GetDefaultPreset cmd.");
    const data = [0x20];
    webserial.sendHex(data, parseDefaultPreset);      
    state = 99;
}

function parseDefaultPreset(event) {
  console.log("Parsing the DefaultPreset.");
  DefaultPreset = event.detail.data[0];  
  console.log(DefaultPreset);

  webserial.off("data", parseDefaultPreset);
  state = 10;
}

function getPreset(numPreset) {   
  //Set the listener
  webserial.on("data", parsePreset);

  console.log("Sending GetPreset " + Number(numPreset) + " cmd.");    
  const data = [0x30, Number(numPreset)];
  webserial.sendHex(data);
        
  state = 99;
}

function parsePreset(event)
{
    p = new Preset(event.detail.data);    
    console.log(p);

    Presets.push(p);
    
    webserial.off("data", parsePreset);
    state = 10 + p.id + 1;
}

function sleep(ms) {
  return new Promise(resolve => setTimeout(resolve, ms));
}

// run the setup function when all the page is loaded:
document.addEventListener("DOMContentLoaded", setup);