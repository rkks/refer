
var sq3 = Math.sqrt(3);

function mkhex(context, xo, yo, s) {
   s = s-05;
   context.moveTo(xo, yo);
   context.lineTo(s * sq3/2 + xo, s*(-0.5) + yo);
   context.lineTo(s * sq3 + xo, 0 + yo);
   context.lineTo(s * sq3 + xo, 1*s + yo);
   context.lineTo(s * sq3/2 + xo, s*1.5 + yo);
   context.lineTo(xo, s*1 + yo);
   //context.lineTo(xo, yo);
   context.closePath();
   
   // ink
   context.strokeStyle = "#eee";
   context.stroke();
}

function startup() {
   var can = document.getElementById("can");
   var context = can.getContext("2d");

   var s= 50;
   var xo=100, yo=100;
   for(x=0; x<5; x++) {
    for(y=0; y<3; y++) {
      dx = sq3*x + sq3/2*(y%2);
      dy = 1.5*y;
      mkhex(context, s*dx+xo, s*dy+yo, s);
    }
   }
 }

