TestCoverage = {coverage: []}

function assert(predicate){
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

var callOutput = "tteo80ulle"

function Sample10(order, a, b){
	var calls = []

	var Ignore = {
		prototype: {
			sort: {
				call: function(thisObj, a, b){
					calls.push(["call", [thisObj, a, b]])
					return callOutput
				}
			}
		}
	}

	this.calls = calls

#START OF SAMPLE        
	var sort = function(a, b) {
		for (var i = 0, l = order.length; i < l; i++) {
			var o = order[i]
			if (typeof o === 'string') {
				if (a === o) return -1
				if (b === o) return 1
			} else {
				if (a.match(o)) return -1
				if (b.match(o)) return 1
			}
		}

		// deps go in the back
		if (a === 'node_modules') return 1
		if (b === 'node_modules') return -1

		return Ignore.prototype.sort.call(this, a, b)
	}
#END OF SAMPLE        

	this.result = sort.call(this, a, b)
}

// tests

var state1 = new Sample10([], 'a', 'b')
assert(state1.result === callOutput)
assert(arraysEqual(state1.calls[0], [ 'call', [ state1, 'a', 'b' ] ]))
assert(state1.calls.length === 1)

var state2 = new Sample10([1], 'a', 'b')
assert(state2.result === callOutput)
assert(arraysEqual(state2.calls[0], [ 'call', [ state2, 'a', 'b' ] ]))
assert(state2.calls.length === 1)

var state3 = new Sample10([], 'a', 'node_modules')
assert(state3.result === -1)
assert(state3.calls.length === 0)

var state4 = new Sample10([], 'node_modules', 'b')
assert(state4.result === 1)
assert(state4.calls.length === 0)

var state4b = new Sample10([], 'node_modules', 'node_modules')
assert(state4b.result === 1)
assert(state4b.calls.length === 0)

var state5 = new Sample10([/[bxyz]/], 'a', 'b')
assert(state5.result === 1)
assert(state5.calls.length === 0)

var state6 = new Sample10([/[axyz]/], 'a', 'b')
assert(state6.result === -1)
assert(state6.calls.length === 0)

var state6b = new Sample10([/[ab]/], 'a', 'b')
assert(state6b.result === -1)
assert(state6b.calls.length === 0)

var state7 = new Sample10(["c"], 'a', 'b')
assert(state7.result === callOutput)
assert(arraysEqual(state7.calls[0], [ 'call', [ state7, 'a', 'b' ] ]))
assert(state7.calls.length === 1)

var state8 = new Sample10(['b'], 'a', 'b')
assert(state8.result === 1)
assert(state8.calls.length === 0)

var state9 = new Sample10(['a'], 'a', 'b')
assert(state9.result === -1)
assert(state9.calls.length === 0)

var state10 = new Sample10(['c', /c/, /[bxyz]/], 'a', 'b')
assert(state10.result === 1)
assert(state10.calls.length === 0)

var state11 = new Sample10(['c', /c/, /[axyz]/], 'a', 'b')
assert(state11.result === -1)
assert(state11.calls.length === 0)

var state11b = new Sample10(['c', /c/, /[ab]/], 'a', 'b')
assert(state11b.result === -1)
assert(state11b.calls.length === 0)

var state12 = new Sample10(['c', /c/, 'b'], 'a', 'b')
assert(state12.result === 1)
assert(state12.calls.length === 0)

var state13 = new Sample10(['c', /c/, 'a'], 'a', 'b')
assert(state13.result === -1)
assert(state13.calls.length === 0)
