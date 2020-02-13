const request = require('request');
const moment = require('moment');


function getTime(){
	return moment().format("h:mm a");
}
request({uri: 'https://api.openweathermap.org/data/2.5/weather?appid=32843bad9e96bb36c7935458544b1628&id=4927510&units=imperial'},
	(error, httpResponse, body) => {
	if (error) {
		throw new Error("Request Failed");
	}
	console.log(`At ${getTime()}, it's ${JSON.parse(body).main.temp} degrees.`)
});

