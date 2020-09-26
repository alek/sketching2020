$("#run-button").click(function() {

	requirejs(["Tone"], function(Tone) {
		const synth = new Tone.Synth().toDestination();

		requirejs(["mqtt"], function(mqtt) {
			var client = mqtt.connect('mqtt://try:try@broker.shiftr.io', {
		  		clientId: 'agent.p'
			});

			client.on('connect', function(){
			  console.log('client has connected!');

			  client.subscribe('/sketching');
			  // client.unsubscribe('/example');

			  setInterval(function(){
			    client.publish('/sketching', (Math.random() < 0.5 ) ? "BLEEP" : "BLOOP");
			  }, 2000);
			});

			client.on('message', function(topic, message) {
			  if (Math.random() < 0.1) {
			  	$("#messages").empty()
			  }
			  $("#messages").append('<div class="msg">' + message.toString() + '</div>')
			  synth.triggerAttackRelease("C3", "8n");
			});

		});


	});	
})

