<html>
<body>

<?php
if (isset($_GET['item'])) {
	$newitem = $_GET['item']."\n";
	file_put_contents("cookies.txt", $newitem, FILE_APPEND);
}?>

<p>
<pre>
<?php
readfile("cookies.txt");
?>
</pre>
</p>

<p>
add cookie:
<form action="xssm3.php">
<input type="text" name="item">
<input type="submit">
</form>
</p>

</body>
</html>
