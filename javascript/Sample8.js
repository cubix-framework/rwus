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

var calendar_prefix = "8ou830ouao_"
var format_prefix = "wenohua83_"

function Sample8(){
	var calls = []

	function moment(){
		calls.push(["moment", []])
		return {
			add: function(opts){
				assertThat(Object.keys(opts).length === 1)
				assertThat(opts.d)
				var i = opts.d
				calls.push(["add", [i]])
				return {
					calendar: function(){
						calls.push(["calendar", []])
						return calendar_prefix + i
					},
					hours: function(hs){
						calls.push(["hours", [hs]])
						return {
							minutes: function(ms){
								calls.push(["minutes", [ms]])
								return {
									seconds: function(ss){
										calls.push(["seconds", [ss]])
										return {
											milliseconds: function(ms){
												calls.push(["milliseconds", [ms]])
											}
										}
									}
								}
							}
						}
					},
					format: function(str){
						calls.push(["format", [str]])
						return format_prefix + i
					}
				}
			}
		}
	}
	var assertObj = {
		equal: function(a, b, msg){
			calls.push(["equal", [a, b, msg]])
		}
	}

	this.calls = calls

#START OF SAMPLE        
	function calendarNextWeek(assert) {
		var i, m;
		for (i = 2; i < 7; i++) {
			m = moment().add({d: i});
			assert.equal(m.calendar(),       m.format('dddd [pukul] LT'),  'Hari ini + ' + i + ' hari waktu sekarang');
			m.hours(0).minutes(0).seconds(0).milliseconds(0);
			assert.equal(m.calendar(),       m.format('dddd [pukul] LT'),  'Hari ini + ' + i + ' hari permulaan hari');
			m.hours(23).minutes(59).seconds(59).milliseconds(999);
			assert.equal(m.calendar(),       m.format('dddd [pukul] LT'),  'Hari ini + ' + i + ' hari tamat hari');
		}
	}
#END OF SAMPLE        

	calendarNextWeek(assertObj)
}

// tests
var state1 = new Sample8()
for (var i = 2; i < 7; i++) {
	var callIdx = (i-2) * 19
	assertThat(arraysEqual(state1.calls[callIdx+0], [ 'moment', [] ]))
	assertThat(arraysEqual(state1.calls[callIdx+1], [ 'add', [ i ] ]))
	assertThat(arraysEqual(state1.calls[callIdx+2], [ 'calendar', [] ]))
	assertThat(arraysEqual(state1.calls[callIdx+3], [ 'format', [ 'dddd [pukul] LT' ] ]))
	assertThat(arraysEqual(state1.calls[callIdx+4], [ 'equal', [ calendar_prefix+i, format_prefix+i, 'Hari ini + ' + i + ' hari waktu sekarang' ] ]))
	assertThat(arraysEqual(state1.calls[callIdx+5], [ 'hours', [ 0 ] ]))
	assertThat(arraysEqual(state1.calls[callIdx+6], [ 'minutes', [ 0 ] ]))
	assertThat(arraysEqual(state1.calls[callIdx+7], [ 'seconds', [ 0 ] ]))
	assertThat(arraysEqual(state1.calls[callIdx+8], [ 'milliseconds', [ 0 ] ]))
	assertThat(arraysEqual(state1.calls[callIdx+9], [ 'calendar', [] ]))
	assertThat(arraysEqual(state1.calls[callIdx+10], [ 'format', [ 'dddd [pukul] LT' ] ]))
	assertThat(arraysEqual(state1.calls[callIdx+11], [ 'equal', [ calendar_prefix+i, format_prefix+i, 'Hari ini + ' + i + ' hari permulaan hari' ] ]))
	assertThat(arraysEqual(state1.calls[callIdx+12], [ 'hours', [ 23 ] ]))
	assertThat(arraysEqual(state1.calls[callIdx+13], [ 'minutes', [ 59 ] ]))
	assertThat(arraysEqual(state1.calls[callIdx+14], [ 'seconds', [ 59 ] ]))
	assertThat(arraysEqual(state1.calls[callIdx+15], [ 'milliseconds', [ 999 ] ]))
	assertThat(arraysEqual(state1.calls[callIdx+16], [ 'calendar', [] ]))
	assertThat(arraysEqual(state1.calls[callIdx+17], [ 'format', [ 'dddd [pukul] LT' ] ]))
	assertThat(arraysEqual(state1.calls[callIdx+18], [ 'equal', [ calendar_prefix+i, format_prefix+i, 'Hari ini + ' + i + ' hari tamat hari' ] ]))
}
