TestCoverage = {coverage: []}

function assert(predicate){
  if (!predicate){
    throw new Error("Assertion Failed");
  }
}

var initBody = "boaue89aou7"
var loginValue = "u90ou894az"
var resStatusCode = "zhuro830bu"
var resHeaders = "89e7gou02h"
var jsonStringify = "ffenoh38oh"

function Sample7(userB, loginB){
	var calls = []

	var body = initBody

	var JSON = {
		parse: function(body){
			calls.push(["JSON.parse", [body]])
			return {
				user: userB
					? { login: loginB ? loginValue : null}
					: null
			}
		},
		stringify: function(input){
			calls.push(["JSON.stringify", [input]])
			return jsonStringify
		}
	}

	var grunt = {
		option: function(key, value){
			calls.push(["grunt.option", [key, value]])
		},
		task: {
			run: function(cmd){
				calls.push(["grunt.task.run", [cmd]])
			}
		},
		log: {
			writeln: function(str){
				calls.push(["grunt.log.writeln", [str]])
			}
		}
	}

	var done = function(b){
		calls.push(["done", [b]])
	}

	var resObj = {
		on: function(event, callback){
			calls.push(["res.on", [event, callback]])
		},
		statusCode: resStatusCode,
		headers: resHeaders
	}

	this.calls = calls
	this.getBody = function(){ return body }

#START OF SAMPLE        
	// This was an anonymous function passed as the second parameter to https.request(). I gave it the name config.
	function config(res) {
		res.on("data", function (chunk) {
			body += chunk;
		});

		res.on("end", function () {
			var json    = JSON.parse(body),
				login   = json.user && json.user.login;

			if (login) {
				grunt.option("user", login);
				grunt.task.run("cla-check");

				done();
			} else {
				grunt.log.writeln("Unexpected response from api.github.com");
				grunt.log.writeln("statusCode: " + res.statusCode);
				grunt.log.writeln("headers: " + JSON.stringify(res.headers));
				grunt.log.writeln("data: " + body);

				done(false);
			}
		});

		res.on("error", function (err) {
			grunt.log.writeln(err);
			done(false);
		});
	}
#END OF SAMPLE        

	config(resObj)

}

// tests

var state0 = new Sample7(false, false)

assert(state0.calls[0][0] === "res.on")
	assert(state0.calls[0][1][0] === "data")
assert(state0.calls[1][0] === "res.on")
	assert(state0.calls[1][1][0] === "end")
assert(state0.calls[2][0] === "res.on")
	assert(state0.calls[2][1][0] === "error")

assert(state0.calls.length === 3)

var state1 = new Sample7(false, false)
var dataF = state1.calls[0][1][1]
var chunk = "247o0u8a0o"
dataF(chunk)
assert(state1.getBody() === initBody + chunk)

var state2a = new Sample7(false, false)
var endFa = state2a.calls[1][1][1]
endFa()
assert(state2a.calls[3][0] === "JSON.parse")
	assert(state2a.calls[3][1][0] === initBody)
assert(state2a.calls[4][0] === "grunt.log.writeln")
	assert(state2a.calls[4][1][0] === "Unexpected response from api.github.com")
assert(state2a.calls[5][0] === "grunt.log.writeln")
	assert(state2a.calls[5][1][0] === "statusCode: " + resStatusCode)
assert(state2a.calls[6][0] === "JSON.stringify")
	assert(state2a.calls[6][1][0] === resHeaders)
assert(state2a.calls[7][0] === "grunt.log.writeln")
	assert(state2a.calls[7][1][0] === "headers: " + jsonStringify)
assert(state2a.calls[8][0] === "grunt.log.writeln")
	assert(state2a.calls[8][1][0] === "data: " + initBody)
assert(state2a.calls[9][0] === "done")
	assert(state2a.calls[9][1][0] === false)
assert(state2a.calls.length === 10)

var state2b = new Sample7(true, false)
var endFb = state2b.calls[1][1][1]
endFb()
assert(state2b.calls[3][0] === "JSON.parse")
	assert(state2b.calls[3][1][0] === initBody)
assert(state2b.calls[4][0] === "grunt.log.writeln")
	assert(state2b.calls[4][1][0] === "Unexpected response from api.github.com")
assert(state2b.calls[5][0] === "grunt.log.writeln")
	assert(state2b.calls[5][1][0] === "statusCode: " + resStatusCode)
assert(state2b.calls[6][0] === "JSON.stringify")
	assert(state2b.calls[6][1][0] === resHeaders)
assert(state2b.calls[7][0] === "grunt.log.writeln")
	assert(state2b.calls[7][1][0] === "headers: " + jsonStringify)
assert(state2b.calls[8][0] === "grunt.log.writeln")
	assert(state2b.calls[8][1][0] === "data: " + initBody)
assert(state2b.calls[9][0] === "done")
	assert(state2b.calls[9][1][0] === false)
assert(state2a.calls.length === 10)

var state2c = new Sample7(true, true)
var endFc = state2c.calls[1][1][1]
endFc()
assert(state2c.calls[3][0] === "JSON.parse")
	assert(state2c.calls[3][1][0] === initBody)
assert(state2c.calls[4][0] === "grunt.option")
	assert(state2c.calls[4][1][0] === "user")
	assert(state2c.calls[4][1][1] === loginValue)
assert(state2c.calls[5][0] === "grunt.task.run")
	assert(state2c.calls[5][1][0] === "cla-check")
assert(state2c.calls[6][0] === "done")
	assert(state2c.calls[6][1][0] === undefined)
assert(state2c.calls.length === 7)

var state3 = new Sample7(false, false)
var errorF = state3.calls[2][1][1]
var err = "8egu09o0a803212"
errorF(err)
assert(state3.calls[3][0] === "grunt.log.writeln")
	assert(state3.calls[3][1][0] === err)
assert(state3.calls[4][0] === "done")
	assert(state3.calls[4][1][0] === false)
assert(state3.calls.length === 5)
