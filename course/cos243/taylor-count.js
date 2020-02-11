const request = require('request');

function countOccurrences(target, source){
	const a = new RegExp(target, 'g');
	let x = 0;
	while (a.exec(source) !== null){
		x++
	};
	return x;
}

request.get("https://www.taylor.edu",
	(error, httpResponse, body) => {
		if (error) {
			throw new Error("Request failed")
		}  
		console.log(`The word 'taylor' occurs ${countOccurrences("taylor", body.toLowerCase())} times in the TU home page.`)
	});
