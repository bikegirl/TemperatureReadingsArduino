/*
 * Implement all your JavaScript in this file!
 */

$(document).ready(function() {
var globalTemps = [];
var currTemp = 30;
var userInputTemp;
var max_temp = -200;
var min_temp = 200;
var avg_temp = 0;
var count_temp = 0;
var max_num = 3600;
var StandbyMode = false;
var celsius = true;


/*
 * changeBackground() uses the current temperature to toggle between 
 * different background colors WITHOUT user input
 */
function changeBackground() {

	if (currTemp < 29) {

		document.body.style.backgroundColor = "green";


	} else if (currTemp >= 29 && currTemp <= 30){

		document.body.style.backgroundColor = "yellow";


	} else if (currTemp > 30){

		document.body.style.backgroundColor = "red";

	}
}

/*
 * changeBackgroundFromTextBox() takes a specific temperature from the user
 * to toggle background color between the designated temp.
 */
function changeBackgroundFromTextBox() {


	console.log(userInputTemp);

	if ( userInputTemp < currTemp ){

		document.body.style.backgroundColor = "#9999ff";
		//document.getElementById("bg1").style.backgroundColor

	} else if ( userInputTemp >= currTemp ) {

		document.body.style.backgroundColor = "#87CEFA";

	}
}

/*
 * Calculates an average of all the temperature sensor readings for the past hour.
 */
function averageTemp() {
	if(count_temp < max_num){
		globalTemps.push(currTemp);
		if(currTemp > max_temp){
			max_temp = currTemp;
		}
		if(currTemp < min_temp){
			min_temp = currTemp;
		}
		avg_temp = (avg_temp *count_temp + currTemp)/(count_temp + 1);
		count_temp = count_temp + 1;
	}
	else{
		globalTemps.shift();
		globalTemps.push(currTemp);
		max_temp = -200;
		min_temp = 200;
		avg_temp = 0;
		for(i=0; i < globalTemps.length; i++){
			if(globalTemps[i] < min_temp){
				min_temp = globalTemp[i];
			}
			if(globalTemps[i] > max_temp){
				max_temp = globalTemps[i];
			}
			avg_temp = avg_temp + globalTemps[i];
		}
		avg_temp = avg_temp / globalTemps.length;
	}

	if(StandbyMode == true){
		$("#CurrTemp").html("");
		$("#maxTemp").html("");
		$("#minTemp").html("");
		$("#avgTemp").html("");
	} else if(celsius == false) {
		var fcurrTemp = currTemp * (9/5)+32;
		var fmax_temp = max_temp * (9/5)+32;
		var fmin_temp = min_temp * (9/5)+32;
		var favg_temp = avg_temp * (9/5)+32;
		$("#CurrTemp").html(fcurrTemp);
		$("#maxTemp").html(fmax_temp);
		$("#minTemp").html(fmin_temp);
		$("#avgTemp").html(favg_temp);
	} else {
		$("#CurrTemp").html(currTemp);
		$("#maxTemp").html(max_temp);
		$("#minTemp").html(min_temp);
		$("#avgTemp").html(avg_temp);
	}

}

/*
 * Recursive function to timeout() for 1 sec
 * temperature readings are read in from temp every sec to facilitate other functions
 */
function timeout() {
	setTimeout(function () {

		getTemp();
		timeout(); //recursive function to wait 1 sec;

	}, 1000);
}

/*
 * Retrieves user input entered in the text box and change background
 */
function textBoxTemp() {

	console.log("Inside textBoxTemp.") ;
	userInputTemp = $("#textBox").val();
  	changeBackgroundFromTextBox();

}

/*
 * Gets temperature readings from the sensor
 * and saves them to global variables for further calculations
 */
function getTemp() {

  	var promise = $.get("getTemperature", function(data, status){
  	promise.fail(()=>{
  		console.log("No connection");
  	});
  	console.log(data);
  	console.log(Object.keys(data));
  	console.log(status);
  	//if server is disconnected
  	if (Object.keys(data).length == 0) {
		$("#CurrTemp").html("Arduino disconnected");
		$("#maxTemp").html("Arduino disconnected");
		$("#minTemp").html("Arduino disconnected");
		$("#avgTemp").html("Arduino disconnected");
  	} else {
	  	currTemp = Number (data.a); //4 digit string representation of our temperature
  	
	  	if(currTemp > 0 && currTemp < 100){
		  	globalTemps.push(data.a);
		  	//changeBackground();
		  	averageTemp();
		}
  	}
  })
}

/*
 * Creates HTTP request, requests data from the server to display 'Joel' on arduino 7-seg display
 */
function displayJoel(){

	$.get("getJoel", function(data, status){


	})	
}

/*
 * Creates HTTP request, requests data from the server to flash LED light
 */
function displayLED(){

	$.get("turnLEDon", function(data, status){

	})

}

/*
 * Creates HTTP request, requests data from the server to toggle units between
 * celsius and farenheit
 */
function toggleUnits(){
	if(celsius == true){
		celsius = false;
	} else if (celsius == false){
		celsius = true;
	}
	$.get("toggleUnits", function(data, status){

	})

}


/*
 * Creates HTTP request, requests data from the server to enter Standby Mode or Resume status
 */
function handleStatus(val) {


	if ( val == '1') {

		StandbyMode = true;

		$.get("toStandby", function(data, status){ //getRequest to initiate standby

		})

	} else if (val == '2'){

		//resume button is hit, while page is already in resume status status
		if (StandbyMode == false){

			window.alert("Status is already in Active Status.")

		} else {

			StandbyMode = false;


			$.get("fromStandby", function(data, status){ //getRequest to resume back to active status

			})

		}		

	}

}

// function toggleStatus(){
// 	if(StandbyMode == false){
// 		StandbyMode = true;
// 		$.get("toStandby", function(data, status){ //getRequest to initiate standby

// 		})
// 	} else {
// 		StandbyMode = false;
// 		$.get("fromStandby", function(data, status){ //getRequest to resume back to active status

// 		})
// 	}
// 	$.get("toggleUnits", function(data, status){

// 	})

// }


/*
 * All Button functionality mappying button to function when pressed.
 */
$("#button2").click(displayJoel);
$("#button3").click(toggleUnits);
$("#button4").click(displayLED);
// $("#button5").click(toggleStatus);
$("#button5").click(() => {handleStatus('1')});
$("#button6").click(() => {handleStatus('2')});

// $("#button7").click(() => {textBoxTemp()});
// $("#button7").click(textBoxTemp);
//$("#button7").click(()=> {console.log("HIT 1!!!"); textBoxTemp()});  
$("#button7").click(textBoxTemp); 
  
  
//   promise
//   .done(function(data) {
//     console.log( "second success", data );
//   })
//   .fail(function(e) {
//   	console.log(e.responseText);
//   	var res = JSON.parse(e.responseText);
//     console.log( "data ", res.a);
//   })
//   .always(function() {
//     console.log( "finished" );
//   });


/*
 * Calling getTemp() and timeout() to start loop of retreiving/saving temperature from
 * sensor readings
 */
getTemp();
timeout();

});


//$("button").click(function(){
//  $.get("demo_test.asp", function(data, status){
//    alert("Data: " + data + "\nStatus: " + status);
//  });
// });
