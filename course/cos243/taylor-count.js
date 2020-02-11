const request = require('request');

function countOccurrences(target, source){
	let b = source.match(/target/g);
	return b.length;
}

request.get("https://www.taylor.edu",
	(error, httpResponse, body) => {
		if (error) {
			throw new Error("Request failed")
		}  
		console.log(`The word 'taylor' occurs ${countOccurrences("taylor", body.toLowerCase())} times in the TU home page.`)
	});
