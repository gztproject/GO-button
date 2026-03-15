class Preset {
    
    #id;
    #name;
    #mode;
    #keys;
    #color;
    #intensity;
    #changed;

    constructor(data) 
    {   
        this.#id = data[0];
        this.#name = new TextDecoder().decode(data.slice(1,17));
        this.#mode = data[17];
        this.#keys = [new Key(data,0),new Key(data,1),new Key(data,2),new Key(data,3),new Key(data,4)];
        this.#color = new Color(data[68],data[69],data[70]);
        this.#intensity = data[71];
        this.#changed = false;
    }

    Print()
    {        
        document.getElementById("preset-val").innerHTML = this.#id + ": " + this.#name + " - " + this.#mode;
        document.getElementById("preset-col-p").value = this.#color.toHex();

        for (let i=0; i < this.#keys.length; i++) {  
            this.#keys[i].Print(this.#mode);
        }
    }

    Serialize()
    {
        var p = [this.#id];
        
        var nameArr = new TextEncoder().encode(this.#name);                
        p = p.concat(Array.from(nameArr));
        p.push(this.#mode);
        for (let i = 0; i<5; i++)
        {
            p = p.concat(this.#keys[i].Serialize());
        }
        p = p.concat(this.#color.Serialize());
        p.push(this.#intensity);
        console.log(p);
        return p;
    }

    SetColor(hexCol)
    {
        if(hexCol != this.#color.toHex())
        {
            this.#color.setFromHex(hexCol);
            this.#changed = true;
        }
    }

    SetKeyBaseColor(key, hexCol)
    {
        if(hexCol != this.#keys[key].getBaseColorHex())
        {
            this.#keys[key].SetBaseColor(hexCol);
            this.#changed = true;
        }
    }

    SetKeyAccentColor(key, hexCol)
    {
        if(hexCol != this.#keys[key].getAccentColorHex())
        {
            this.#keys[key].SetAccentColor(hexCol);
            this.#changed = true;
        }
    }

    SetKeyFunction(key, fnc)
    {
        if(fnc != this.#keys[key].getFunction())
        {
            this.#keys[key].SetFunction(fnc);
            this.#changed = true;
        }
    }

    GetId()
    {
        return this.#id;
    }

    IsChanged()
    {
        return this.#changed;
    }
}


class Key
{
    #id;
    #function;
    #baseColor;
    #accentColor;
    #baseInt;
    #accentInt;

    constructor(data, id)
    {
        let start = 18;
        let keyLength = 10;

        this.#id = id;
        this.#function = (data[start + id*keyLength + 0] << 8) + data[start + id*keyLength + 1],
        this.#baseColor = new Color(data[start + id*keyLength + 2], data[start + id*keyLength + 3], data[start + id*keyLength + 4]);
        this.#accentColor = new Color(data[start + id*keyLength + 5], data[start + id*keyLength + 6], data[start + id*keyLength + 7]);
        this.#baseInt = data[start + id*keyLength + 8];
        this.#accentInt = data[start + id*keyLength + 9];
    }

    Print(mode)
    {        
        //document.getElementById("key-"+this.#id+"-val").innerHTML = this.#function;
        document.getElementById("key-"+this.#id+"-col-p").value = this.#baseColor.toHex();
        document.getElementById("key-"+this.#id+"-col-a").value = this.#accentColor.toHex();

        var select = document.getElementById("key-"+this.#id+"-val");
        
        var i, L = select.options.length - 1;
        for(i = L; i >= 0; i--) {
            select.remove(i);
        }

        var functions;
        var opt;
        switch(mode)
        {
            case Mode.KB:
                functions = Object.keys(KBKeyFunctions);
                functions.forEach(function(f) {
                    opt = document.createElement('option');
                    opt.value = KBKeyFunctions[f];
                    opt.innerHTML = f;
                    select.appendChild(opt);
                });            
                select.value = this.#function;
                break;
            case Mode.MIDI:
                for(let i=0; i<128;i++){
                    opt = document.createElement('option');
                    opt.value = i;
                    opt.innerHTML = i;
                    select.appendChild(opt);
                };
                select.value = this.#function;
                break;
            case Mode.HID:
                functions = Object.keys(HIDKeyFunctions);
                functions.forEach(function(f) {
                    opt = document.createElement('option');
                    opt.value = HIDKeyFunctions[f];
                    opt.innerHTML = f;
                    select.appendChild(opt);
                });
                select.value = this.#function;
                break;
            default:
                break;
        }      
        
    }

    Serialize()
    {
        var k = [(this.#function>>8),(this.#function & 0xFF)];
        k = k.concat(this.#baseColor.Serialize());
        k = k.concat(this.#accentColor.Serialize());
        k = k.concat([this.#baseInt, this.#accentInt]);
        return k;
    }

    SetBaseColor(hexCol)
    {
        this.#baseColor.setFromHex(hexCol);
    }

    SetAccentColor(hexCol)
    {
        this.#accentColor.setFromHex(hexCol);
    }

    SetFunction(fnc)
    {
        this.#function = fnc;
    }

    getBaseColorHex()
    {
        return this.#baseColor.toHex();
    }

    getAccentColorHex()
    {
        return this.#accentColor.toHex();
    }

    getFunction()
    {
        return this.#function;
    }
}

class Color
{
    #r;
    #g;
    #b;
    
    constructor(r,g,b)
    {
        this.#r = r;
        this.#g = g;
        this.#b = b;
    }

    toRgbString()
    {
        return "rgb("+this.#r+","+this.#g+","+this.#b+")";        
    }

    toHex()
    {
        return "#"+('0'+this.#r.toString(16)).slice(-2)+('0'+this.#g.toString(16)).slice(-2)+('0'+this.#b.toString(16)).slice(-2);
    }

    Serialize()
    {
        return [this.#r, this.#g, this.#b];
    }

    setFromHex(hexCol)
    {
        var result = /^#?([a-f\d]{2})([a-f\d]{2})([a-f\d]{2})$/i.exec(hexCol);
        this.#r = parseInt(result[1], 16);
        this.#g = parseInt(result[2], 16);
        this.#b = parseInt(result[3], 16);
    }
}

class Version
{
    #major;
    #minor;
    #rev;
    constructor(data)
    {
        this.#major = data[0];
        this.#minor = data[1];
        this.#rev = data[2];
    }

    Print()
    {
        document.getElementById("fw-version").innerHTML = this.#major+'.'+this.#minor+'.'+this.#rev;
    }
}

const Mode = Object.freeze({
  UNKNOWN: 0x00,
  KB: 0x10,
  MIDI: 0x20,
  HID: 0x30,
  OFF: 0xFF
});

const KBKeyFunctions = Object.freeze({
    A: 4,
    B: 5,
    C: 6,
    D: 7,
    E: 8,
    F: 9,
    G: 10,
    H: 11,
    I: 12,
    J: 13,
    K: 14,
    L: 15,
    M: 16,
    N: 17,
    O: 18,
    P: 19,
    Q: 20,
    R: 21,
    S: 22,
    T: 23,
    U: 24,
    V: 25,
    W: 26,
    X: 27,
    Y: 28,
    Z: 29,
    NUM_1: 30,
    NUM_2: 31,
    NUM_3: 32,
    NUM_4: 33,
    NUM_5: 34,
    NUM_6: 35,
    NUM_7: 36,
    NUM_8: 37,
    NUM_9: 38,
    NUM_0: 39,
    ENTER: 40,
    ESC: 41,
    BACKSPACE: 42,
    TAB: 43,
    SPACE: 44,
    MINUS: 45,
    EQUAL: 46,
    LEFT_BRACE: 47,
    RIGHT_BRACE: 48,
    BACKSLASH: 49,
    NON_US_NUM: 50,
    SEMICOLON: 51,
    QUOTE: 52,
    TILDE: 53,
    COMMA: 54,
    PERIOD: 55,
    SLASH: 56,
    CAPS_LOCK: 0x0039,
    F1: 0x003A,
    F2: 0x003B,
    F3: 0x003C,
    F4: 0x003D,
    F5: 0x003E,
    F6: 0x003F,
    F7: 0x0040,
    F8: 0x0041,
    F9: 0x0042,
    F10: 0x0043,
    F11: 0x0044,
    F12: 0x0045,
    PRINTSCREEN: 0x0046,
    SCROLL_LOCK: 0x0047,
    PAUSE: 0x0048,
    INSERT: 0x0049,
    HOME: 0x004A,
    PAGE_UP: 0x004B,
    DELETE: 0x004C,
    END: 0x004D,
    PAGE_DOWN: 0x004E,
    RIGHT_ARROW: 0x004F,
    LEFT_ARROW: 0x0050,
    DOWN_ARROW: 0x0051,
    UP_ARROW: 0x0052,
    NUM_LOCK: 0x0053,
    KEYPAD_DIVIDE: 0x0054,
    KEYPAD_MULTIPLY: 0x0055,
    KEYPAD_SUBTRACT: 0x0056,
    KEYPAD_ADD: 0x0057,
    KEYPAD_ENTER: 0x0058,
    KEYPAD_1: 0x0059,
    KEYPAD_2: 0x005A,
    KEYPAD_3: 0x005B,
    KEYPAD_4: 0x005C,
    KEYPAD_5: 0x005D,
    KEYPAD_6: 0x005E,
    KEYPAD_7: 0x005F,
    KEYPAD_8: 0x0060,
    KEYPAD_9: 0x0061,
    KEYPAD_0: 0x0062,
    KEYPAD_DOT: 0x0063,
    
    
    NON_US: 0x0064,
    MENU: 0x0065
});

const HIDKeyFunctions = Object.freeze({
    POWER: 0x0030,
	SLEEP: 0x0032,
	BRIGHTNESS_UP: 0x006F,
	BRIGHTNESS_DOWN: 0x0070,
	SCREENSAVER: 0x019E,
	RECORD: 0x00B2,
	FAST_FORWARD: 0x00B3,
	REWIND: 0x00B4,
	NEXT: 0x00B5,
	PREVIOUS: 0x00B6,
	STOP: 0x00B7,
	PLAY_PAUSE: 0x00CD,
	PAUSE: 0x00B0,
	VOLUME_MUTE: 0x00E2,
	VOLUME_UP: 0x00E9,
	VOLUME_DOWN: 0x00EA,
	PROGRAMMABLE_BUTTON_CONFIGURATION: 0x0182,
	CONTROL_CONFIGURATION: 0x0183,
	EMAIL_READER: 0x018A,
	CALCULATOR: 0x0192,
	EXPLORER: 0x0194,
	BROWSER_HOME: 0x0223,
	BROWSER_BACK: 0x0224,
	BROWSER_FORWARD: 0x0225,
	BROWSER_REFRESH: 0x0227,
	BROWSER_BOOKMARKS: 0x022A
});