//using jquery
var v = 0;
var c = 0;
$(document).ready(startup);

function poll() {
  $.get("ajax.php", {val: v, ctr: c},
   function(data)
   {
     var h = JSON.parse(data);
     v = h.value;
     c = h.counter;
     $('#status').html(h.name);
     $("body").css("background-position",v+"% 0%");
   });
}

var periodic;
function startup() {
  periodic = setInterval(poll, 500);
}
