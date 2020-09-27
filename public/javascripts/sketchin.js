function isNumeric(value) {
        return /^-?\d+$/.test(value);
}

function sumLetters(str) {
	str = str.toString()
    let sum = 0;
    for (let i = 0; i < str.length; i++) {
        sum += str.charCodeAt(i)
    }
    return sum;
};

var getFrequency = function(message) {
	if (isNumeric(message.toString())) {
		return Math.min(20 + 5*parseInt(message),655)
	} else {
		// 
		drawText([250,100+75*Math.random()], message.toString(), 24 + Math.floor(48*Math.random()) + "px", "#fff", 500, 0, "Helvetica", "graph")
		return 20 + sumLetters(message)%635
	}
}

var randomColor = function() {
	var palette = ["#dc2f02", "#e85d04", "#f48c06", "#f48c06", "#faa307", "#ffba08"]
	return palette[Math.floor(Math.random()*palette.length)]
}

$("#run-button").click(function() {

	$("#incoming-label").show()
	drawRectangle([0,0], 500, 500, "#000", "graph")		

	requirejs(["Tone"], function(Tone) {

		const feedbackDelay = new Tone.FeedbackDelay("8n", 0.5).toDestination();

		const synth = new Tone.MonoSynth({
			oscillator: {
				type: "sine"
			},
			envelope: {
				attack: 0.5
			}
		}).connect(feedbackDelay);


		requirejs(["mqtt"], function(mqtt) {
			var client = mqtt.connect('mqtt://try:try@broker.shiftr.io', {
		  		clientId: 'agent.p'
			});

			client.on('connect', function(){
			  console.log('client has connected!');

			  client.subscribe('/try/table_7/#');
			});

			client.on('message', function(topic, message) {
			  if (Math.random() < 0.05) {
			  	$("#messages").empty()
			  	$("#graph").empty()
			  	drawRectangle([0,0], 500, 500, randomColor(), "graph")	
			  }
			  let freq = getFrequency(message)
			  let color = randomColor()
			  $("#messages").append('<div class="msg">' + topic.split("/").slice(-1)[0] + " " + message.toString() + " = " + getFrequency(message) + "Hz" + '</div>')
			  drawCircle([Math.random()*500,Math.random()*500], freq/7, color, "graph")		
			  try {
			  	synth.triggerAttackRelease(freq, "4n");
			  } catch (err) { }
			});

		});


	});	
})

