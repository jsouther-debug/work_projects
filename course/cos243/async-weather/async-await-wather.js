const request = require('request');
const moment = require('moment');


function getTime(){
	return moment().format("ddd MMM d YYYY h:mm:ss zz");
}

let uri = 'https://api.openweathermap.org/data/2.5/weather'
let qs = {
	appid: '32843bad9e96bb36c7935458544b1628',
	id: '4927510',
	units: 'imperial'
}

function reportError(error){
	console.log(`Something went wrong: '${error}'`);
}

function reportWeather(result){
	console.log(`On ${getTime()}, it's ${result.main.temp} degrees.`)
}

function requestPromisified (uri, qs){
	return new Promise ((resolve, reject) => {

	
	request({uri, qs}, (error, httpResponse, body) =>{
		let result = JSON.parse(body)
		if (error) {
			reject(error);
		}
		else {
			resolve(result)
		}
	})
})

}

function getTimeAndTempPromise(uri, qs){
	requestPromisified(uri, qs)
		.then(result => reportWeather(result))
		.catch(error => reportError(error));
}

async function getTimeAndTempAsync(uri, qs){
	try{
		let result = await requestPromisified(uri, qs);
		await reportWeather(result);
	} catch (error) {
		reportError(error)
	}
}


getTimeAndTempPromise(uri,qs);
getTimeAndTempAsync(uri, qs);