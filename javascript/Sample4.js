TestCoverage = {coverage: []}

function assert(predicate){
  if (!predicate){
    throw new Error("Assertion Failed");
  }
}

var prefix = "px"

var cssExpand = {
	"0": "_css0_",
	"1": "_css1_",
	"2": "_css2_",
	"3": "_css3_",
}

var suffix = "sx"

#START OF SAMPLE    
function expand( value ) {
	var i,

		// assumes a single number if not a string
		parts = typeof value === "string" ? value.split(" ") : [ value ],
		expanded = {};

	for ( i = 0; i < 4; i++ ) {
		expanded[ prefix + cssExpand[ i ] + suffix ] =
			parts[ i ] || parts[ i - 2 ] || parts[ 0 ];
	}

	return expanded;
}
#END OF SAMPLE    

// tests

var res1 = expand(1)
assert(Object.keys(res1).length === 4)
assert(res1["px_css0_sx"] === 1)
assert(res1["px_css1_sx"] === 1)
assert(res1["px_css2_sx"] === 1)
assert(res1["px_css3_sx"] === 1)

var res2 = expand("4823")
assert(Object.keys(res2).length === 4)
assert(res2["px_css0_sx"] === "4823")
assert(res2["px_css1_sx"] === "4823")
assert(res2["px_css2_sx"] === "4823")
assert(res2["px_css3_sx"] === "4823")

var res3 = expand("8572 2003")
assert(Object.keys(res3).length === 4)
assert(res3["px_css0_sx"] === "8572")
assert(res3["px_css1_sx"] === "2003")
assert(res3["px_css2_sx"] === "8572")
assert(res3["px_css3_sx"] === "2003")

var res4 = expand("8572 2003 9931")
assert(Object.keys(res4).length === 4)
assert(res4["px_css0_sx"] === "8572")
assert(res4["px_css1_sx"] === "2003")
assert(res4["px_css2_sx"] === "9931")
assert(res4["px_css3_sx"] === "2003")

var res5 = expand("8572 2003 9931 661")
assert(Object.keys(res5).length === 4)
assert(res5["px_css0_sx"] === "8572")
assert(res5["px_css1_sx"] === "2003")
assert(res5["px_css2_sx"] === "9931")
assert(res5["px_css3_sx"] === "661")
