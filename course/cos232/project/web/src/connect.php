<?php
	function connect()
	{
		// Connects to the Database 
		return mysqli_connect("localhost", "hackmedbuser", "hackmedbpass", "hackme");
	}
?>
