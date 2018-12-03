<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
<title>hackme</title>
<link href="style.css" rel="stylesheet" type="text/css" media="screen" />
<?php
	include('connect.php');
	include('header.php');
	$DB = connect();
?>
<div class="post">
	<div class="post-bgtop">
		<div class="post-bgbtm">
        <h2 class = "title">hackme Registration</h2>
        <?php
		//if the registration form is submitted 
		if (isset($_POST['submit'])) {
			
			$_POST['uname'] = trim($_POST['uname']);
			if(!$_POST['uname'] | !$_POST['password'] |
				!$_POST['fname'] | !$_POST['lname']) {
 				die('<p>You did not fill in a required field.
				Please go back and try again!</p>');
			}
			$num = preg_match('@[0-9]@', $_POST['password']);

			if(strlen(trim($_POST['password'])) < 9 | !$num)
				die('Password is not long enough or does not contain a number');

			$password = $_POST['password'];
			
			$check = mysqli_query($DB, "SELECT * FROM users WHERE username = '".$_POST['uname']."'") or die(mysqli_error($DB));
 
 		//Gives error if user already exist
 		$check2 = mysqli_num_rows($check);
		if ($check2 != 0) {
			die('<p>Sorry, user name already exisits.</p>');
		}
		else
		{
			$uname = mysqli_real_escape_string($DB, $_POST['uname']);
			$fname = mysqli_real_escape_string($DB, $_POST['fname']);
			$lname = mysqli_real_escape_string($DB, $_POST['lname']);
			mysqli_query($DB, "INSERT INTO users (username, pass, fname, lname) VALUES ('".$uname."', '". crypt($password, "thesalt") ."', '". $fname."', '". $lname ."');")or die(mysqli_error());
			
			echo "<h3> Registration Successful!</h3> <p>Welcome ". $_POST['fname'] ."! Please log in...</p>";
		} 
        ?>    
        <?php
		}else{
        ?>
        	<form  method="post" action="register.php">
            <table>
                <tr>
                    <td> Username </td> 
                    <td> <input type="text" name="uname" maxlength="20"/> </td>
                    <td> <em>choose a login id</em> </td>
                </tr>
                <tr>
                    <td> Password </td>
                    <td> <input type="password" name="password" maxlength="40"/> </td>
                </tr>
                <tr>
                    <td> First Name </td>
                    <td> <input type="text" name="fname" maxlength="25"/> </td>
                </tr>
                 <tr>
                    <td> Last Name </td>
                    <td> <input type="text" name="lname" maxlength="25"/> </td>
                </tr>
                <tr>
                    <td> <input type="submit" name="submit" value="Register" /> </td>
                </tr>
            </table>
            </form>
        <?php
		}
		?>
        </div>
    </div>
</div>
<?php
	include('footer.php');
?>
</body>
</html>
