let text1 = "LOAD \"KIELBASA\",8,1%RUN";
var i = 0;
var canType = true;
var canFlashCursor = true;

setInterval(() => {
    if (canFlashCursor == true){
        document.getElementById("flash").style.opacity = 0;
        setTimeout(() => {
            document.getElementById("flash").style.opacity = 1;
        }, 500);
    }else{
        document.getElementById("flash").style.opacity = 0;
    }
}, 1000)

function type() {
    if (i < text1.length) { }
    if (text1.charAt(i) == "!" && canType == true) {
        document.getElementById("typing").innerHTML += "<br>";
        i++;
    }
    else if (text1.charAt(i) == "%" && canType == true) {
        canType = false;
        canFlashCursor = false;
        document.getElementById("typing").innerHTML += "<br><br>SEARCHING FOR \"KIELBASA\"";
        setTimeout(() => {
            document.getElementById("typing").innerHTML += "<br>LOADING";
            setTimeout(() => {
                document.getElementById("typing").innerHTML += "<br>READY.<br>";
                i++;
                canType = true;
                canFlashCursor = true;
            }, 3500);
        }, 2000);
    } else if (canType == true) {
        document.getElementById("typing").innerHTML += text1.charAt(i);
        i++;
    }
}