jQuery( document ).ready(function( $ ) {
	var heartbeatURL = window.chatdomain + "/heartbeat.cgi"
	var pollURL = window.chatdomain + "/poll.cgi"
	var readURL = window.chatdomain + "/read.cgi"

	var serverUp = true

	var userInputs = 'input,textarea,button'
	function modifyInputs(onOrOff){
		$(userInputs).prop('disable',onOrOff)
		$(userInputs).prop('readonly',onOrOff)
	}
	function disableInputs(){
		modifyInputs(true);
		setTimeout(doHeartBeat, 1000)
	}

	function enableInputs(){
		modifyInputs(false)
	}

	function doHeartBeat(){
		$.ajax({
			type: "GET",
			url: heartbeatURL,
			success: function(response){
				serverUp = response.initialized			
				$('marquee').text("Loaded!").blur()
				$('marquee').fadeTo("slow",0.0)
				$('#history').fadeTo("fast",1.0)
				if(serverUp) enableInputs()
                else disableInputs()
			},
			error: function(){
				$('marquee').text("Could not connect to server,retrying in 1 second").blur()
				$('#history').fadeTo("slow",.2)
				disableInputs()
				
			}
		})
	}

	doHeartBeat()
})