milestone1;

Jared Souther
Dr. Stanley
COS-232
11/18/18

	The password management system used by hack me is not the best. It didn't enforce much security and was vulnerable to many attacks, probably still is vulnerable to some. I did enforce some stronger passwords though, starting by making it have to be atleast eight characters and require at least one number in the password when making the account. Next was making sure the data was secure, or at least not easily readable if anyone obtained a copy of the database. So I encrypted it when it was stored in the database, and then used the same encryption when it was checking to see if it was the right password when someone tried to login. I also encrypted it in the cookie so it wasn't easily visible to anyone trying to snoop. To resist the brute force attack I made it time out the users log in after five failed attempts for several hours, they can try and log in over and over but the request never gets sent, it is always killed. As far as enforcing the last part of this, I made it so they could no longer just create a cookie with fake credentials to get in and post and delete and view other post by doing a check on the cookie to make sure it had actual user credentials both username and password had to be in the database.


milestone2;
11/19/18;

	I started by going back to the cross site request forgery lab and then used example two as my platform for the attack and put the file in my public_html on my local drive. I added an input line for the other variable that needed to be changed, the first one being title, the second being the message. Then I added the method=POST to the form tag so it could post. Then as the last thing I added the name="post_submit" so it would know to actually post it. I think that was all there was to getting the attack to work, but I could be forgetting something. As far as the fix goes, I made a session start, created a token variable. I added a hidden token field to the html that gets set when the user opens the post page. Then I did the check to make sure the token key existed and that it matched the token created by our page, this ensured that the user was on the correct page and not visiting some link or clicking some button they shouldn't be. Then if either of those checks failed it would exit and repoint them to members.php. Then the last thing I did was destroy the sessions upon logging out. I think that was everything, could be forgetting something though.
