<?
$value=$_GET['val'];
$counter=$_GET['ctr'];
# value takes on a oscillating value between 0 and 100 (back and forth)
$value = ((int)(($counter/100)) &1) ? (100-($counter%100)) : $counter%100;
echo json_encode(array("name"=>"ajax.php: {$value}", "value"=>$value, "counter"=>$counter+1));
?>
