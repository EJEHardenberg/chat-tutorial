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
		setTimeout(doServerPoll, 1000)
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
				$('marquee').fadeTo("slow",1.0)
				$('marquee').text("Could not connect to server,retrying in 1 second").blur()
				$('#history').fadeTo("slow",.2)
				disableInputs()
				
			}
		})
	}

	var first = true
	var lastUpdatedTime = (new Date().getTime()/1000).toFixed(0)
	function doServerPoll(){
		$.ajax({
			type: "GET",
			url: pollURL + "?date=" + lastUpdatedTime,
			success: function(response){
				if( response.updated || first){
					first = false
					getChatHistory()
				}
				setTimeout(doServerPoll, 2000)
			},
			error: function(){
				serverUp = false
				alert("Connection to Server Lost! Reconnecting...")
				doHeartBeat()
			}
		})
	}

	var cookieName = "userhandle"
	function setUserHandle(){
		var value = $('input[name=u]').val()
		if(value.trim() == "") return;
        var date = new Date();
        date.setTime(date.getTime()+(365*24*60*60*1000));
        var expires = "; expires="+date.toGMTString();
        
	    document.cookie = cookieName+"="+value+expires+"; path=/";
	}

	function readUserHandle() {
	    var nameEQ = cookieName + "=";
	    var ca = document.cookie.split(';');
	    for(var i=0;i < ca.length;i++) {
	        var c = ca[i];
	        while (c.charAt(0)==' ') c = c.substring(1,c.length);
	        if (c.indexOf(nameEQ) == 0) return c.substring(nameEQ.length,c.length);
	    }
	    return null;
	}

	var savedHandle = readUserHandle()
	if(savedHandle != null){
		$('input[name=u]').val(savedHandle)
	}
	$('input[name=u]').on('keyup',setUserHandle)
	$('input[name=u]').on('change',setUserHandle)

	$('form').submit(function(evt){
		evt.preventDefault()
		if(!serverUp){
			alert("The Server is not available right now. Please wait while we establish your connection")
			return false
		}
		/* Some simple validation */
		if( $('input[name=u]').val().trim() == "" ){
			alert("You must enter a username!")
			return false
		}

		if( $('input[name=u]').val().trim().length > 20 ){
			alert("usernames must be less than 20 characters")
			return false
		}

		if( $('textarea').val().trim() == "" ){
			alert("Please enter a message before sending")
			return false
		}

		
	 	var data = $(this).serialize()
	 	var url = $(this).attr('action')
	 	var method = $(this).attr('method')
	 	$(this).fadeTo("slow",0.5)
	 	$.ajax({
	 		type: method,
			data: data,
			url: url,
			context: this,
			success: function(response){
				var updates = $(this).find('span[name=updates]')
				updates.text(response.message)
				setTimeout(function(){
					updates.text("")
				}, 500)
				$(this).find('textarea').val("")
				$(this).fadeTo("slow",1.0)
				$(this).find('textarea').focus()
			},
			error: function(){
				var updates = $(this).find('span[name=updates]')
				updates.text("There was an error in contacting the server, please hold.")
				setTimeout(function(){
					updates.text("")
				}, 3000)
				$(this).fadeTo("slow",1.0)
				serverUp = false
				doHeartBeat()
			}
	 	})
	 	return false

	})


	function getChatHistory(){
		lastUpdatedTime = (new Date().getTime()/1000).toFixed(0)
		console.info("Polling Server")
	}

	doHeartBeat()
})