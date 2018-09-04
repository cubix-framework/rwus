TestCoverage = {coverage: []}

function assertThat(predicate){
	if (!predicate){
		throw new Error("Assertion Failed");
	}
}

function arraysEqual(a1, a2){
	function arrayOrValuesEqual(v1, v2){
		if (Array.isArray(v1) && Array.isArray(v2)){
			return arraysEqual(v1, v2)
		} else {
			return v1 === v2
		}
	}
	if (a1.length != a2.length) return false

	var res = true
	for (var i = 0; i < a1.length; i++){
		if (!arrayOrValuesEqual(a1[i], a2[i])){
			res = false
		}
	}
	return res;
}

var momentWithParamsOutput = "9uhaonuh3zzwq"
var isSameOutput = "wqhonu893nao"
var formatOutput = "98e0uwcecnq"
var hoursOutput = "fahosqw083w"

function Sample9(parsedIsValid){
	var calls = []

	var momentObj = {
		isSame: function(date, unit){
			calls.push(["isSame", [date, unit]])
			return isSameOutput
		},
		format: function(str){
			calls.push(["format", [str]])
			return formatOutput
		},
		isValid: function(){
			calls.push(["isValid", []])
			return parsedIsValid
		},
		hours: function(){
			calls.push(["hours", []])
			return hoursOutput
		}
	}

	function moment(date, format, m, b){
		calls.push(["moment", [date, format, m, b]])
		return momentObj
	}

	var assertObj = {
		ok: function(b){
			calls.push(["ok", [b]])
		},
		equal: function(a,b,msg){
			calls.push(["equal", [a,b,msg]])
		}
	}

	this.calls = calls
	this.moment = momentObj

#START OF SAMPLE        
	function parseMeridiem (assert) {
		var i,
			b = moment(),
			meridiemTests = [
				// h a patterns, expected hours, isValid
				['10 ekuseni',   10, true],
				['11 emini',   11, true],
				['3 entsambama',   15, true],
				['4 entsambama',   16, true],
				['6 entsambama',   18, true],
				['7 ebusuku',   19, true],
				['12 ebusuku',   0, true],
				['10 am',   10, false],
				['10 pm',   10, false]
			],
			parsed;

		// test that a formatted moment including meridiem string can be parsed back to the same moment
		assert.ok(b.isSame(moment(b.format('h:mm:ss a'), 'h:mm:ss a', 'ss', true), 'seconds'), b.format('h:mm:ss a') + ' should be equal to ' + moment(b.format('h:mm:ss a'), 'h:mm:ss a', 'ss', true).format('h:mm:ss a'));

		// test that a formatted moment having a meridiem string can be parsed with strict flag
		assert.ok(moment(b.format('h:mm:ss a'), 'h:mm:ss a', 'ss', true).isValid(), b.format('h:mm:ss a') + ' should be parsed as valid');

		for (i = 0; i < meridiemTests.length; i++) {
			parsed = moment(meridiemTests[i][0], 'h a', 'ss', true);
			assert.equal(parsed.isValid(), meridiemTests[i][2], 'validity for ' + meridiemTests[i][0]);
			if (parsed.isValid()) {
				assert.equal(parsed.hours(), meridiemTests[i][1], 'hours for ' + meridiemTests[i][0]);
			}
		}
	}
#END OF SAMPLE        

	parseMeridiem(assertObj)
}

// tests

var meridiemTests = [
	['10 ekuseni',   10, true],
	['11 emini',   11, true],
	['3 entsambama',   15, true],
	['4 entsambama',   16, true],
	['6 entsambama',   18, true],
	['7 ebusuku',   19, true],
	['12 ebusuku',   0, true],
	['10 am',   10, false],
	['10 pm',   10, false]
]

var state1 = new Sample9(false)
assertThat(arraysEqual(state1.calls[0], [ 'moment', [ undefined, undefined, undefined, undefined ] ]))
assertThat(arraysEqual(state1.calls[1], [ 'format', [ 'h:mm:ss a' ] ]))
assertThat(arraysEqual(state1.calls[2], [ 'moment', [ formatOutput, 'h:mm:ss a', 'ss', true ] ]))
assertThat(arraysEqual(state1.calls[3], [ 'isSame', [ state1.moment, 'seconds' ] ]))
assertThat(arraysEqual(state1.calls[4], [ 'format', [ 'h:mm:ss a' ] ]))
assertThat(arraysEqual(state1.calls[5], [ 'format', [ 'h:mm:ss a' ] ]))
assertThat(arraysEqual(state1.calls[6], [ 'moment', [ formatOutput, 'h:mm:ss a', 'ss', true ] ]))
assertThat(arraysEqual(state1.calls[7], [ 'format', [ 'h:mm:ss a' ] ]))
assertThat(arraysEqual(state1.calls[8], [ 'ok', [ isSameOutput ] ]))
assertThat(arraysEqual(state1.calls[9], [ 'format', [ 'h:mm:ss a' ] ]))
assertThat(arraysEqual(state1.calls[10], [ 'moment', [ formatOutput, 'h:mm:ss a', 'ss', true ] ]))
assertThat(arraysEqual(state1.calls[11], [ 'isValid', [] ]))
assertThat(arraysEqual(state1.calls[12], [ 'format', [ 'h:mm:ss a' ] ]))
assertThat(arraysEqual(state1.calls[13], [ 'ok', [ false ] ]))
for (i = 0; i < meridiemTests.length; i++) {
	var callIdx = 14 + i*4
	assertThat(arraysEqual(state1.calls[callIdx+0], [ 'moment', [ meridiemTests[i][0], 'h a', 'ss', true ] ]))
	assertThat(arraysEqual(state1.calls[callIdx+1], [ 'isValid', [] ]))
	assertThat(arraysEqual(state1.calls[callIdx+2], [ 'equal', [ false, meridiemTests[i][2], 'validity for '+meridiemTests[i][0] ] ]))
	assertThat(arraysEqual(state1.calls[callIdx+3], [ 'isValid', [] ]))
}
assertThat(state1.calls.length === 50)

var state2 = new Sample9(true)
assertThat(arraysEqual(state2.calls[0], [ 'moment', [ undefined, undefined, undefined, undefined ] ]))
assertThat(arraysEqual(state2.calls[1], [ 'format', [ 'h:mm:ss a' ] ]))
assertThat(arraysEqual(state2.calls[2], [ 'moment', [ formatOutput, 'h:mm:ss a', 'ss', true ] ]))
assertThat(arraysEqual(state2.calls[3], [ 'isSame', [ state2.moment, 'seconds' ] ]))
assertThat(arraysEqual(state2.calls[4], [ 'format', [ 'h:mm:ss a' ] ]))
assertThat(arraysEqual(state2.calls[5], [ 'format', [ 'h:mm:ss a' ] ]))
assertThat(arraysEqual(state2.calls[6], [ 'moment', [ formatOutput, 'h:mm:ss a', 'ss', true ] ]))
assertThat(arraysEqual(state2.calls[7], [ 'format', [ 'h:mm:ss a' ] ]))
assertThat(arraysEqual(state2.calls[8], [ 'ok', [ isSameOutput ] ]))
assertThat(arraysEqual(state2.calls[9], [ 'format', [ 'h:mm:ss a' ] ]))
assertThat(arraysEqual(state2.calls[10], [ 'moment', [ formatOutput, 'h:mm:ss a', 'ss', true ] ]))
assertThat(arraysEqual(state2.calls[11], [ 'isValid', [] ]))
assertThat(arraysEqual(state2.calls[12], [ 'format', [ 'h:mm:ss a' ] ]))
assertThat(arraysEqual(state2.calls[13], [ 'ok', [ true ] ]))
for (i = 0; i < meridiemTests.length; i++) {
	var callIdx = 14 + i*6
	assertThat(arraysEqual(state2.calls[callIdx+0], [ 'moment', [ meridiemTests[i][0], 'h a', 'ss', true ] ]))
	assertThat(arraysEqual(state2.calls[callIdx+1], [ 'isValid', [] ]))
	assertThat(arraysEqual(state2.calls[callIdx+2], [ 'equal', [ true, meridiemTests[i][2], 'validity for '+meridiemTests[i][0] ] ]))
	assertThat(arraysEqual(state2.calls[callIdx+3], [ 'isValid', [] ]))
	assertThat(arraysEqual(state2.calls[callIdx+4], [ 'hours', [] ]))
  assertThat(arraysEqual(state2.calls[callIdx+5], [ 'equal', [ hoursOutput, meridiemTests[i][1], 'hours for '+meridiemTests[i][0] ] ]))
}
assertThat(state2.calls.length === 68)

