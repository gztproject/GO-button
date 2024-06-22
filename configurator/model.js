class Preset {
    constructor(data) 
    {        
        this.id = data[0];
        this.name = new TextDecoder().decode(data.slice(1,16));
        this.mode = data[17];
        this.Keys = [new Key(data,0),new Key(data,1),new Key(data,2),new Key(data,3),new Key(data,4)];
        this.color = new Color(data[68],data[69],data[70]);
        this.intensity = data[71];
    }

    Print()
    {
        
        document.getElementById("preset-val").innerHTML = this.id + ": " + this.name + " - " + this.mode;
        document.getElementById("preset-col-p").style.backgroundColor = this.color.toRgbString();

        for (let i=0; i < this.Keys.length; i++) {  
            this.Keys[i].Print();
        }
    }
}


class Key
{
    constructor(data, id)
    {
        let start = 18;
        let keyLength = 10;

        this.id = id;
        this.keyFunction = (data[start + id*keyLength + 0] << 8) + data[start + id*keyLength + 1],
        this.baseColor = new Color(data[start + id*keyLength + 2], data[start + id*keyLength + 3], data[start + id*keyLength + 4]);
        this.accentColor = new Color(data[start + id*keyLength + 5], data[start + id*keyLength + 6], data[start + id*keyLength + 7]);
        this.baseInt = data[start + id*keyLength + 8];
        this.accentInt = data[start + id*keyLength + 9];
    }

    Print()
    {        
        document.getElementById("key-"+this.id+"-val").innerHTML = this.keyFunction;
        document.getElementById("key-"+this.id+"-col-p").style.backgroundColor = this.baseColor.toRgbString();
        document.getElementById("key-"+this.id+"-col-a").style.backgroundColor = this.accentColor.toRgbString();
    }
}

class Color
{
    constructor(r,g,b)
    {
        this.r = r;
        this.g = g;
        this.b = b;
    }

    toRgbString()
    {
        return "rgb("+this.r+","+this.g+","+this.b+")";        
    }
}

class Version
{
    constructor(data)
    {
        this.major = data[0];
        this.minor = data[1];
        this.rev = data[2];
    }

    Print()
    {
        document.getElementById("fw-version").innerHTML = this.major+'.'+this.minor+'.'+this.rev;
    }
}