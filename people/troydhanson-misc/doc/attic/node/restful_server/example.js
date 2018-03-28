#!/usr/bin/env node

//
// npm install optimist
// npm install express
// npm install websocket

var fs = require('fs');
var util = require('util');
var http = require('http');
var express = require('express');
var ws = require('websocket');

var argv = require('optimist')
      .usage("server\n Usage: $0 [-v]")
      .boolean('v')   // turns argv.v into a boolean
      .argv;

var CONFIG_FILE = "config.json";
var REST_PORT = 9000;
var WSKT_PORT = 9001;
var app = express();

// update the browser websockets, if any.
function service_websock() 
{
  var msg = [];
  var json = JSON.stringify(msg);
  for (var c in ws_conn) ws_conn[c].sendUTF(json);
}


//---------------------------------------------------------------------------
// read the config file 
//---------------------------------------------------------------------------
function load_config() {
 var f = fs.readFileSync(CONFIG_FILE);
 var config = JSON.parse(f);
}
//---------------------------------------------------------------------------
// rest api service functions
//---------------------------------------------------------------------------
function rest_api (req, res){
  var url_parts = req.url.split("/");
  var rgb_spec = url_parts[1];
  res.send('ok');
}

//---------------------------------------------------------------------------
// boot_rest_api
//---------------------------------------------------------------------------
function boot_rest_api() {
  app.use('/api', rest_api);
  app.use('/', express.static(__dirname + "/static"));
  app.listen(REST_PORT);
  console.log('Web & API server running at http://127.0.0.1:'+REST_PORT+'/');
}

//---------------------------------------------------------------------------
// boot_websocket
//---------------------------------------------------------------------------
var ws_conn = [];  // connections to any websockets 
function boot_websocket() {
  var ws_http_server = http.createServer(function(request,response) {
                                           response.writeHead(404);
                                           response.end();  });
  ws_http_server.listen(WSKT_PORT);
  console.log('Websocket server running at http://127.0.0.1:'+WSKT_PORT+'/');
  var ws_server = new ws.server( {
                      httpServer: ws_http_server,
                      autoAcceptConnections: true });
  ws_server.on('connect', function( conn ) { ws_conn.push(conn); }); 
  ws_server.on('close',   function( conn ) { adel(ws_conn, conn);});
}

//---------------------------------------------------------------------------
// main
//---------------------------------------------------------------------------
load_config();
boot_websocket();
boot_rest_api();

