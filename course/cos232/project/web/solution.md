milestone1;

Jared Souther
Dr. Stanley
COS-232
11/18/18

	The password management system used by hack me is not the best. It didn't enforce much security and was vulnerable to many attacks, probably still is vulnerable to some. I did enforce some stronger passwords though, starting by making it have to be atleast eight characters and require at least one number in the password when making the account. Next was making sure the data was secure, or at least not easily readable if anyone obtained a copy of the database. So I encrypted it when it was stored in the database, and then used the same encryption when it was checking to see if it was the right password when someone tried to login. I also encrypted it in the cookie so it wasn't easily visible to anyone trying to snoop. To resist the brute force attack I made it time out the users log in after five failed attempts for several hours, they can try and log in over and over but the request never gets sent, it is always killed. As far as enforcing the last part of this, I made it so they could no longer just create a cookie with fake credentials to get in and post and delete and view other post by doing a check on the cookie to make sure it had actual user credentials both username and password had to be in the database.


milestone2;
11/19/18;
(code for the attack is in csrfm2.html);


	I started by going back to the cross site request forgery lab and then used example two as my platform for the attack and put the file in my public_html on my local drive. I added an input line for the other variable that needed to be changed, the first one being title, the second being the message. Then I added the method=POST to the form tag so it could post. Then as the last thing I added the name="post_submit" so it would know to actually post it. I think that was all there was to getting the attack to work, but I could be forgetting something. As far as the fix goes, I made a session start, created a token variable. I added a hidden token field to the html that gets set when the user opens the post page. Then I did the check to make sure the token key existed and that it matched the token created by our page, this ensured that the user was on the correct page and not visiting some link or clicking some button they shouldn't be. Then if either of those checks failed it would exit and repoint them to members.php. Then the last thing I did was destroy the sessions upon logging out. I think that was everything, could be forgetting something though.


milestone3;
11/29/18;
(code for the attack is in xssm3.js);

	I started by verifying I could get the cookie by using an alert(document.cookie). Then I set up the second website in my /var/www/html it's called xssm3.php (I may move this to the same directory as this file, but will ask first). This website is extremely similar to the second exercise in the xss lab. This uses a file called cookies.txt to store all of the data. I then did a lot of looking up how to generate a http request in javascript. I found a way and then using XMLHttpRequest(); I opened a get request, then using csrf I put the link of my php page with the item vairable filled in with the document.cookie. then I sent the request, and that worked. It now stores the cookie which consist of the username, password, and session Id of any user that clicks on my post. As far as patching it goes, I put a preg_replace in post.php looking for < and replacing it with "less then". It replaces it in both the title and the message, and I tried messing around with this and getting other xss attacks to work. I think that covers patching it because it no longer executes code recognized by a web browser. I actually came back to this later and decided to use the htmlspecialchar() function to escape the browser reading in the characters as executable code. Got this idea after I did the sql injection fix. 



milestone4;
12/3/18;

	I got the attack to work by playing around with the url by viewing the post that were already there. This is the url I used and then patched http://secfarm-111/hackme/show.php?pid=1' union select * from users where username = user2', the title is the users name, the posted by is the users password, and the message is the users last name. I know there was also a weakness in all of the places that ran a sql query with user input, but the only other one I attempted to exploit was the one in the message box of the post.php page. Once I identified all of the sql queries that are ran with user input in show.php, post.php, and register.php, I googled a lot until I came across a sanitization function that was called mysqli_real_escape_string. That basically escapes all the special characters so you can't just break out of a string and execute another query. I wrapped all the user input queries in that, by pulling out the user input, sticking it into a different variable that had that function called on it, and inserting that variable in it's place. That patched what I could test, and I think I have patched all the sql injection vulnerabilities. 
