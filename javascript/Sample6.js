TestCoverage = {coverage: []}

function assertThat(predicate){
  if (!predicate){
    throw new Error("Assertion Failed");
  }
}

var parseInputs = [
	["—ò–∞–Ω—É–∞—Ä", "—ò–∞–Ω."],
	["—Ñ–µ–±—Ä—É–∞—Ä", "—Ñ–µ–±."],
	["–º–∞—Ä—Ç", "–º–∞—Ä."],
	["–∞–ø—Ä–∏–ª", "–∞–ø—Ä."],
	["–º–∞—ò", "–º–∞—ò"],
	["—ò—É–Ω", "—ò—É–Ω"],
	["—ò—É–ª", "—ò—É–ª"],
	["–∞–≤–≥—É—Å—Ç", "–∞–≤–≥."],
	["—Å–µ–ø—Ç–µ–º–±–∞—Ä", "—Å–µ–ø."],
	["–æ–∫—Ç–æ–±–∞—Ä", "–æ–∫—Ç."],
	["–Ω–æ–≤–µ–º–±–∞—Ä", "–Ω–æ–≤."],
	["–¥–µ—Ü–µ–º–±–∞—Ä", "–¥–µ—Ü."]
]

var calls = []

var momentCallCount = 0
function moment(input, format){
	calls.push(["moment", [input, format]])
	momentCallCount++
	return {
		month: function() { return momentCallCount }
	}
}

function equal(v1, v2, debugStr){
	calls.push(["equal", [v1,v2,debugStr]])
}

var assertObj = {
	equal: equal
}

#START OF SAMPLE    
function parse(assert) {
	var tests = '—ò–∞–Ω—É–∞—Ä —ò–∞–Ω._—Ñ–µ–±—Ä—É–∞—Ä —Ñ–µ–±._–º–∞—Ä—Ç –º–∞—Ä._–∞–ø—Ä–∏–ª –∞–ø—Ä._–º–∞—ò –º–∞—ò_—ò—É–Ω —ò—É–Ω_—ò—É–ª —ò—É–ª_–∞–≤–≥—É—Å—Ç –∞–≤–≥._—Å–µ–ø—Ç–µ–º–±–∞—Ä —Å–µ–ø._–æ–∫—Ç–æ–±–∞—Ä –æ–∫—Ç._–Ω–æ–≤–µ–º–±–∞—Ä –Ω–æ–≤._–¥–µ—Ü–µ–º–±–∞—Ä –¥–µ—Ü.'.split('_'),
		i;
	function equalTest(input, mmm, i) {
		assert.equal(moment(input, mmm).month(), i, input + ' should be month ' + (i + 1));
	}
	for (i = 0; i < 12; i++) {
		tests[i] = tests[i].split(' ');
		equalTest(tests[i][0], 'MMM', i);
		equalTest(tests[i][1], 'MMM', i);
		equalTest(tests[i][0], 'MMMM', i);
		equalTest(tests[i][1], 'MMMM', i);
		equalTest(tests[i][0].toLocaleLowerCase(), 'MMMM', i);
		equalTest(tests[i][1].toLocaleLowerCase(), 'MMMM', i);
		equalTest(tests[i][0].toLocaleUpperCase(), 'MMMM', i);
		equalTest(tests[i][1].toLocaleUpperCase(), 'MMMM', i);
	}
}
#END OF SAMPLE

// tests
var mFormats = ["MMM", "MMMM", "MMMM", "MMMM"]
var transforms = [null, null, "toLocaleLowerCase", "toLocaleUpperCase"]

parse(assertObj)
for(var i = 0; i < 12; i++) {
	for (var m = 0; m < 4; m++){
		var mFormat = mFormats[m]
		var transform = transforms[m]
		for (var j = 0; j < 2; j++){
			var testIdx = i*8 + m*2 + j
			var callIdx = testIdx*2;

			var momentCall = calls[callIdx]
			var equalCall = calls[callIdx+1]
			var input = transform ? parseInputs[i][j][transform]() : parseInputs[i][j]
			assertThat(momentCall[0] === "moment")
				assertThat(momentCall[1][0] === input)
				assertThat(momentCall[1][1] === mFormat)
			assertThat(equalCall[0] === "equal")
				assertThat(equalCall[1][0] === testIdx+1)
				assertThat(equalCall[1][1] === i)
				assertThat(equalCall[1][2] === input + ' should be month ' + (i + 1))

		}
	}
}
