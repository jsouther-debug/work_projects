milestone1;

Jared Souther
Dr. Stanley
COS-232
11/18/18

	The password management system used by hack me is not the best. It didn't enforce much security and was vulnerable to many attacks, probably still is vulnerable to some. I did enforce some stronger passwords though, starting by making it have to be atleast eight characters and require at least one number in the password when making the account. Next was making sure the data was secure, or at least not easily readable if anyone obtained a copy of the database. So I encrypted it when it was stored in the database, and then used the same encryption when it was checking to see if it was the right password when someone tried to login. I also encrypted it in the cookie so it wasn't easily visible to anyone trying to snoop. To resist the brute force attack I made it time out the users log in after five failed attempts for several hours, they can try and log in over and over but the request never gets sent, it is always killed. As far as enforcing the last part of this, I made it so they could no longer just create a cookie with fake credentials to get in and post and delete and view other post by doing a check on the cookie to make sure it had actual user credentials both username and password had to be in the database.
