

$("#run-button").click(function() {

	$("#incoming-label").show()

	requirejs(["Tone"], function(Tone) {
		const synth = new Tone.Synth().toDestination();

		requirejs(["mqtt"], function(mqtt) {
			var client = mqtt.connect('mqtt://try:try@broker.shiftr.io', {
		  		clientId: 'agent.p'
			});

			client.on('connect', function(){
			  console.log('client has connected!');

			  client.subscribe('/try/table_7/#');
			  
			  // client.unsubscribe('/example');
			  // setInterval(function(){
			  //   client.publish('/try/table_7/alek', (Math.random() < 0.5 ) ? "BLEEP" : "BLOOP");
			  // }, 2000);

			});

			client.on('message', function(topic, message) {
				console.log('new message:', topic, message.toString());
			  if (Math.random() < 0.1) {
			  	$("#messages").empty()
			  }
			  $("#messages").append('<div class="msg">' + topic.split("/").slice(-1)[0] + " " + message.toString() + " = " + (20+5*message) + "Hz" + '</div>')
			  try {
			  	synth.triggerAttackRelease(20+5*message, "8n");
			  } catch (err) { }
			});

		});


	});	
})

