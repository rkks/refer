
function start_websocket() {

 var l = window.location;
 var host = l.hostname;
 var port = parseInt(l.port) + 1; // yes. numeric addition please. :-)

 var ws = new WebSocket("ws://" + host + ":" + port);
 ws.onmessage = function(e) { 
    var d = JSON.parse(e.data); 
    update_sim(d);
 }
 ws.onopen = function()   { $("#status").addClass("connected"); 
                            $("#status").html("Connected");
                          }
 ws.onclose = function(e) { $("#status").removeClass("connected"); 
                            $("#status").html("Disconnected");
                          }
 ws.onerror = function(e) { console.log("WebSocket error: ", e.reason); }
}

var counter=0;
function update_sim(d) {
}
