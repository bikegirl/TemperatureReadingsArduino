function displayJoel(e){
	
	
	document.getElementById("display").value = e.value;
	
}

function sendMsg() {	

$.get('/arduino/msg/' + document.msgform.msg.value + '/',
	function(){
		alert("Success");
	}

);