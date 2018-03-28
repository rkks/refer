#!/usr/bin/env node

// npm install express

var fs = require('fs');
var util = require('util');
var http = require('http');
var express = require('express');

var argv = require('optimist')
      .usage("canvas experiment server\n Usage: $0 [-v]")
      .boolean('v')   // turns argv.v into a boolean
      .argv;

var REST_PORT = 9000;
var app = express();


//---------------------------------------------------------------------------
// boot_rest_api
//---------------------------------------------------------------------------
function boot_rest_api() {
  app.use('/', express.static(__dirname + "/static"));
  app.listen(REST_PORT);
  console.log('Web & API server running at http://127.0.0.1:'+REST_PORT+'/');
}

//---------------------------------------------------------------------------
// main
//---------------------------------------------------------------------------
boot_rest_api();

