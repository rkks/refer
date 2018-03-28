function do_onload() {
  var el = document.getElementById("do");
  el.onclick = do_update;
}
function do_update() {
  alert("update");
  return false;
}
