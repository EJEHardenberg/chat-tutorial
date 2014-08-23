jQuery( document ).ready(function( $ ) {
	var heartbeatURL = window.chatdomain + "/heartbeat.cgi"
	var pollURL = window.chatdomain + "/poll.cgi"
	var readURL = window.chatdomain + "/read.cgi"

	var serverUp = true

	function disableInputs(){
		$('input,textarea,button').prop('disable',true)
		$('input,textarea,button').prop('readonly',true)
	}

	function enableInputs(){
		$('input,textarea,button').prop('disable',false)
		$('input,textarea,button').prop('readonly',false)	
	}

	function doHeartBeat(){
		$.ajax({
			type: "GET",
			url: heartbeatURL,
			success: function(response){
				serverUp = response.initialized			
				$('[name=m]').text("").blur()
				$('marquee').fadeTo("slow",0.0)
				$('#history').fadeTo("fast",1.0)
				enableInputs()
			},
			error: function(){
				$('marquee').text("Could not connect to server,retrying in 1 second").blur()
				$('#history').fadeTo("slow",.2)
				disableInputs()
				setTimeout(doHeartBeat, 1000)
			}
		})
	}

	doHeartBeat()
})