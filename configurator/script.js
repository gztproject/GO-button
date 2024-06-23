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
  document.getElementById("save-preset-btn").addEventListener("click", savePreset);
  document.getElementById("save-default-btn").addEventListener("click", setDefault);  
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
      document.getElementById("loader-container").classList.remove("dn");
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
      document.getElementById("loader-percent").innerHTML = "0%";
      getVersion();   
      setTimeout(tryReadDevice,100);
      return true;
    case 1:
      document.getElementById("loader-percent").innerHTML = "5%";
      getDefaultPreset(); 
      setTimeout(tryReadDevice,100);
      return true;     
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
      document.getElementById("loader-percent").innerHTML = (10+(16*(state-10)))+"%";
      getPreset(state-10); 
      setTimeout(tryReadDevice,100);
      return true;     
    case 99: //wait for reply
      setTimeout(tryReadDevice,100);
      return true;     
    default:      
      break;    
  }
  document.getElementById("loader-container").classList.add("dn");
  document.getElementById("button-row-container").classList.remove("dn");
  document.getElementById("content-container").classList.remove("dn");
  
  CurrentPreset = Presets[DefaultPreset];
  CurrentPreset.Print();   
  enableDisableButtons(); 
  return true;
}

function nextPreset()
{  
  if(CurrentPreset.GetId() < 4){
    if(CurrentPreset.IsChanged()){
      if(!confirm("Current preset is not saved, continue?"))
        return;
    }
    CurrentPreset = Presets[CurrentPreset.GetId() + 1];
    CurrentPreset.Print();
  }
  enableDisableButtons();
}

function prevPreset()
{
  if(CurrentPreset.GetId() > 0){
    if(CurrentPreset.IsChanged()){
      if(!confirm("Current preset is not saved, continue?"))
        return;
    }
    CurrentPreset = Presets[CurrentPreset.GetId() - 1];
    CurrentPreset.Print();
  }
  enableDisableButtons();
}

function enableDisableButtons()
{
  //Navigation buttons
  if(CurrentPreset.GetId() >= 4)
    document.getElementById("btn-next").disabled = true;
  else
    document.getElementById("btn-next").disabled = false;

  if(CurrentPreset.GetId() <= 0)
    document.getElementById("btn-prev").disabled = true;
  else
    document.getElementById("btn-prev").disabled = false;

  //Register all the events
  document.getElementById("preset-col-p").addEventListener("input", function() {
    CurrentPreset.SetColor(this.value);
  });
  for(let i=0; i<5;i++)
  {
    document.getElementById("key-"+i+"-col-p").addEventListener("input", function() {
      CurrentPreset.SetKeyBaseColor(i, this.value);
    });
    document.getElementById("key-"+i+"-col-a").addEventListener("input", function() {
      CurrentPreset.SetKeyAccentColor(i, this.value);
    });
    document.getElementById("key-"+i+"-val").addEventListener("input", function() {
      CurrentPreset.SetKeyFunction(i, this.value);
    });
  }
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

function savePreset()
{
  
  if(state != 99 && state != (10+CurrentPreset.GetId())){
    webserial.on("data", parsePreset);
    console.log("Sending SavePreset cmd.");    
    var data = [0x31, CurrentPreset.GetId()];
    data = data.concat(CurrentPreset.Serialize());
    webserial.sendHex(data);      
    state = 99;
  }
  if(state == 99){
    setTimeout(setDefault,100);
    return;
  }  
}

function setDefault()
{    
  if(state != 99 && state != 10){
    webserial.on("data", parseDefaultPreset);
    console.log("Sending SetDefaultPreset cmd.");
    const data = [0x21, CurrentPreset.GetId()];   
    webserial.sendHex(data);      
    state = 99;
  }
  if(state == 99){
    setTimeout(setDefault,100);
    return;
  }
  if(DefaultPreset == CurrentPreset.GetId())
    window.alert("Success!");
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
  if(event.detail.data.length != 3)
    {
      console.log("Wrong length for version, re-reading.");
      state = 0;
      return;
    }
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
  webserial.sendHex(data);      
  state = 99;
}

function parseDefaultPreset(event) {
  console.log("Parsing the DefaultPreset.");
  if(event.detail.data.length != 1)
    {
      console.log("Wrong length for a default, re-reading.");
      state = 1;
      return;
    }
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
    if(event.detail.data.length != 72)
    {
      console.log("Wrong length for a preset, re-reading preset" + Presets.length);
      state = 10 + Presets.length;
      return;
    }
    p = new Preset(event.detail.data);    
    console.log(p);    
    Presets[p.GetId()] = p;
    CurrentPreset = p;
    
    webserial.off("data", parsePreset);
    state = 10 + p.GetId() + 1;
}

function sleep(ms) {
  return new Promise(resolve => setTimeout(resolve, ms));
}

// run the setup function when all the page is loaded:
document.addEventListener("DOMContentLoaded", setup);