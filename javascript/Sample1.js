TestCoverage = {coverage: []}

function assert(predicate){
	if (!predicate){
		throw new Error("Assertion Failed");
	}
}

const tt = {
	dot: "1284712342",
	parenL: "803002003827",
	parenR: "388773929673043"
}
const parseIdentStart = "5734593827938754"
const nodeStart = "23429384573923"
const finishNodeRetVal = "3772778392814"
const parseSubscriptsRetVal = "422110204703275"
const startVal = "827710093840"
const startLocVal = "1830582113"
const parseExprAtomRetVal = "33028400294766"
const parseExprListRetVal = "980824739105432"
const empty = "48838010980"

function Sample1(ecmaVersion,
								eatDot,
								parseIdentName,
								inFunction,
								eatParenL){
	this.eatDot = eatDot
	this.parseIdentRetVal = {
		name: parseIdentName,
		start: parseIdentStart
	}
	this.inFunction = inFunction
	this.eatParenL = eatParenL

	this.node = {
		start: nodeStart
	}
	this.options = {
		ecmaVersion: ecmaVersion
	}
	this.start = startVal
	this.startLoc = startLocVal

	this.calls = []


	this.startNode = function() {
		this.calls.push(["startNode", []])
		return this.node
	}

	this.parseIdent = function(b){
		this.calls.push(["parseIdent", [b]])
		assert (b == true)
		return this.parseIdentRetVal
	}

	this.eat = function(arg){
		this.calls.push(["eat", [arg]])
		if (arg === tt.dot) return this.eatDot;
		if (arg === tt.parenL) return this.eatParenL;
		throw "unexpected call to 'eat'"
	}

	this.raiseRecoverable = function(start, msg){
		this.calls.push(["raiseRecoverable", [start, msg]])
	}

	this.finishNode = function(node, str) {
		this.calls.push(["finishNode", [node, str]])
		return finishNodeRetVal
	}

	this.parseSubscripts = function(atom, startPos, startLoc, b){
		this.calls.push(["parseSubscripts", [atom, startPos, startLoc, b]])
		assert(atom === parseExprAtomRetVal)
		assert(startPos === startVal)
		assert(startLoc === startLocVal)
		assert(b === true)
		return parseSubscriptsRetVal
	}

	this.parseExprAtom = function() {
		this.calls.push(["parseExprAtom", []])
		return parseExprAtomRetVal
	}

	this.parseExprList = function(parenR, b) {
		this.calls.push(["parseExprList", [parenR, b]])
		assert(parenR === tt.parenR)
		assert(b === false)
		return parseExprListRetVal
	}

#START OF SAMPLE        
	this.parseNew = function() {
		var node = this.startNode()
		var meta = this.parseIdent(true)
		if (this.options.ecmaVersion >= 6 && this.eat(tt.dot)) {
			node.meta = meta
			node.property = this.parseIdent(true)
			if (node.property.name !== "target")
				this.raiseRecoverable(node.property.start, "The only valid meta property for new is new.target")
			if (!this.inFunction)
				this.raiseRecoverable(node.start, "new.target can only be used in functions")
			return this.finishNode(node, "MetaProperty")
		}
		var startPos = this.start, startLoc = this.startLoc
		node.callee = this.parseSubscripts(this.parseExprAtom(), startPos, startLoc, true)
		if (this.eat(tt.parenL)) node.arguments = this.parseExprList(tt.parenR, false)
		else node.arguments = empty
		return this.finishNode(node, "NewExpression")
	}
#END OF SAMPLE        
}

// tests

function testBranch0(state){
	var res = state.parseNew()
	assert(state.meta === undefined)
	assert(state.property === undefined)
	assert(state.node.callee === parseSubscriptsRetVal)
	assert(state.node.arguments === empty)
	assert(state.calls[0][0] === "startNode")
	assert(state.calls[1][0] === "parseIdent")
	assert(state.calls[2][0] === "parseExprAtom")
	assert(state.calls[3][0] === "parseSubscripts")
	assert(state.calls[4][0] === "eat")
		assert(state.calls[4][1][0] === tt.parenL)
	assert(state.calls[5][0] === "finishNode")
		assert(state.calls[5][1][1] === "NewExpression")
	assert(state.calls.length === 6)
	assert(res == finishNodeRetVal)
}

function testBranch0b(state){
	var res = state.parseNew()
	assert(state.meta === undefined)
	assert(state.property === undefined)
	assert(state.node.callee === parseSubscriptsRetVal)
	assert(state.node.arguments === empty)
	assert(state.calls[0][0] === "startNode")
	assert(state.calls[1][0] === "parseIdent")
	assert(state.calls[2][0] === "eat")
		assert(state.calls[2][1][0] === tt.dot)
	assert(state.calls[3][0] === "parseExprAtom")
	assert(state.calls[4][0] === "parseSubscripts")
	assert(state.calls[5][0] === "eat")
		assert(state.calls[5][1][0] === tt.parenL)
	assert(state.calls[6][0] === "finishNode")
		assert(state.calls[6][1][1] === "NewExpression")
	assert(state.calls.length === 7)
	assert(res == finishNodeRetVal)
}

function testBranch1(state){
	var res = state.parseNew()
	assert(state.meta === undefined)
	assert(state.property === undefined)
	assert(state.node.callee === parseSubscriptsRetVal)
	assert(state.node.arguments === parseExprListRetVal)
	assert(state.calls[0][0] === "startNode")
	assert(state.calls[1][0] === "parseIdent")
	assert(state.calls[2][0] === "parseExprAtom")
	assert(state.calls[3][0] === "parseSubscripts")
	assert(state.calls[4][0] === "eat")
	assert(state.calls[5][0] === "parseExprList")
	assert(state.calls[6][0] === "finishNode")
		assert(state.calls[6][1][1] === "NewExpression")
	assert(state.calls.length === 7)
	assert(res == finishNodeRetVal)
}

function testBranch2(state){
	var res = state.parseNew()
	assert(state.node.meta === state.parseIdentRetVal)
	assert(state.node.property === state.parseIdentRetVal)
	assert(state.node.callee === undefined)
	assert(state.node.arguments === undefined)
	assert(state.calls[0][0] === "startNode")
	assert(state.calls[1][0] === "parseIdent")
	assert(state.calls[2][0] === "eat")
		assert(state.calls[2][1][0] === tt.dot)
	assert(state.calls[3][0] === "parseIdent")
	assert(state.calls[4][0] === "finishNode")
		assert(state.calls[4][1][1] === "MetaProperty")
	assert(state.calls.length === 5)

	assert(res == finishNodeRetVal)
}

function testBranch3(state){
	var res = state.parseNew()
	assert(state.node.meta === state.parseIdentRetVal)
	assert(state.node.property === state.parseIdentRetVal)
	assert(state.node.callee === undefined)
	assert(state.node.arguments === undefined)
	assert(state.calls[0][0] === "startNode")
	assert(state.calls[1][0] === "parseIdent")
	assert(state.calls[2][0] === "eat")
		assert(state.calls[2][1][0] === tt.dot)
	assert(state.calls[3][0] === "parseIdent")
	assert(state.calls[4][0] === "raiseRecoverable")
		assert(state.calls[4][1][0] === nodeStart)
		assert(state.calls[4][1][1] === "new.target can only be used in functions")
	assert(state.calls[5][0] === "finishNode")
		assert(state.calls[5][1][1] === "MetaProperty")
	assert(state.calls.length === 6)

	assert(res == finishNodeRetVal)
}

function testBranch4(state){
	var res = state.parseNew()
	assert(state.node.meta === state.parseIdentRetVal)
	assert(state.node.property === state.parseIdentRetVal)
	assert(state.node.callee === undefined)
	assert(state.node.arguments === undefined)
	assert(state.calls[0][0] === "startNode")
	assert(state.calls[1][0] === "parseIdent")
	assert(state.calls[2][0] === "eat")
		assert(state.calls[2][1][0] === tt.dot)
	assert(state.calls[3][0] === "parseIdent")
	assert(state.calls[4][0] === "raiseRecoverable")
		assert(state.calls[4][1][0] === parseIdentStart)
		assert(state.calls[4][1][1] === "The only valid meta property for new is new.target")
	assert(state.calls[5][0] === "finishNode")
		assert(state.calls[5][1][1] === "MetaProperty")
	assert(state.calls.length === 6)

	assert(res == finishNodeRetVal)
}

function testBranch5(state){
	var res = state.parseNew()
	assert(state.node.meta === state.parseIdentRetVal)
	assert(state.node.property === state.parseIdentRetVal)
	assert(state.node.callee === undefined)
	assert(state.node.arguments === undefined)
	assert(state.calls[0][0] === "startNode")
	assert(state.calls[1][0] === "parseIdent")
	assert(state.calls[2][0] === "eat")
		assert(state.calls[2][1][0] === tt.dot)
	assert(state.calls[3][0] === "parseIdent")
	assert(state.calls[4][0] === "raiseRecoverable")
		assert(state.calls[4][1][0] === parseIdentStart)
		assert(state.calls[4][1][1] === "The only valid meta property for new is new.target")
	assert(state.calls[5][0] === "raiseRecoverable")
		assert(state.calls[5][1][0] === nodeStart)
		assert(state.calls[5][1][1] === "new.target can only be used in functions")
	assert(state.calls[6][0] === "finishNode")
		assert(state.calls[6][1][1] === "MetaProperty")
	assert(state.calls.length === 7)

	assert(res == finishNodeRetVal)
}

testBranch0(new Sample1(5, false, "target", true, false))
testBranch0(new Sample1(5, true, "target", true, false))
testBranch0b(new Sample1(6, false, "target", true, false))
testBranch0b(new Sample1(7, false, "target", true, false))

testBranch1(new Sample1(5, false, "target", true, true))

testBranch2(new Sample1(6, true, "target", true, false))
testBranch2(new Sample1(7, true, "target", true, false))

testBranch3(new Sample1(6, true, "target", false, false))

testBranch4(new Sample1(6, true, "somethingElse", true, false))

testBranch5(new Sample1(6, true, "somethingElse", false, false))
