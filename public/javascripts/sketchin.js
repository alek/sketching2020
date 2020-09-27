var messageCount = 0
const urlParams = new URLSearchParams(window.location.search);

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
		drawText([250,100+300*Math.random()], message.toString(), 24 + Math.floor(48*Math.random()) + "px", "#fff", 500, 0, "Helvetica", "graph")
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

	setInterval(function() {
		let circles = $("circle")
		for (let i=0; i<circles.length; i++) {
			$(circles[i]).attr("r", Math.max(0, $(circles[i]).attr("r") - 0.1 ))
			$(circles[i]).attr("opacity", Math.max(0, $(circles[i]).attr("opacity") - 0.01 ))
		}
		let labels = $("text")
		for (let i=0; i<labels.length; i++) {
			$(labels[i]).attr("opacity", Math.max(0, $(labels[i]).attr("opacity") - 0.005 ))
		}
	}, 25);

	requirejs(["Tone"], function(Tone) {

		const feedbackDelay = new Tone.FeedbackDelay("8n", 0.5).toDestination();

		// const synth = new Tone.MonoSynth({
		// 	oscillator: {
		// 		type: "sine"
		// 	},
		// 	envelope: {
		// 		attack: 0.7,
		// 		decay: 0.6,
		// 		sustain: 0.2
		// 	}
		// }).connect(feedbackDelay);

		const synths = [
			new Tone.MonoSynth({
				oscillator: {
					type: "sine"
				},
				envelope: {
					attack: 0.7,
					decay: 0.6,
					sustain: 0.2
				}
			}).connect(feedbackDelay),
			new Tone.MonoSynth({
				oscillator: {
					type: "sine"
				},
				envelope: {
					attack: 0.1,
					decay: 0.6,
					sustain: 0.2
				}
			}).connect(feedbackDelay),
			new Tone.Synth().toDestination(),
			new Tone.AMSynth().toDestination(),
			new Tone.DuoSynth().toDestination(),
			new Tone.FMSynth().toDestination(),
			new Tone.MembraneSynth().toDestination(),
			new Tone.PluckSynth().toDestination(),
			new Tone.PolySynth().toDestination(),
			new Tone.MembraneSynth().connect(new Tone.FeedbackDelay("4n", 0.1).toDestination()),
			new Tone.MembraneSynth().connect(new Tone.BitCrusher(4).toDestination()).connect(new Tone.Phaser({
				frequency: 95,
				octaves: 5,
				baseFrequency: 1000
			}).toDestination())
		]

		requirejs(["mqtt"], function(mqtt) {
			var client = mqtt.connect('mqtt://try:try@broker.shiftr.io', {
		  		clientId: 'agent.p'
			});

			client.on('connect', function(){

			  let topic = urlParams.get("topic") 
			  if (!topic) {
			  	topic = '/try/table_7/#' 
			  } else {
			  	topic += '#'
			  }
			  $("#incoming-label").html('<span class="strong">' + topic + "</span> incoming messages:")
			  client.subscribe(topic);

			});

			client.on('message', function(topic, message) {

			  if (++messageCount%20 == 0) {
			  	$("#messages").empty()
			  	$("#graph").empty()
			  	drawRectangle([0,0], 500, 500, "#000", "graph")	
			  }
			  let freq = getFrequency(message)
			  let color = randomColor()

			  let synthID = urlParams.get("synthID")
			  if (!synthID || synthID.trim().length == 0) {
			  	synthID = 0
			  } else {
			  	synthID = parseInt(synthID)
			  }

			  $("#messages").append('<div class="msg">' + topic.split("/").slice(-1)[0] + " " + message.toString() + " = " + freq + "Hz" + '</div>')
			  drawCircle([Math.random()*500,Math.random()*500], freq/4, color, "graph")		
			  try {
			  	synths[synthID].triggerAttackRelease(freq, "8n");
			  } catch (err) { }
			});

		});


	});	
})

