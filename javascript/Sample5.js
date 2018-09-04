TestCoverage = {coverage: []}

function assert(predicate){
  if (!predicate){
    throw new Error("Assertion Failed");
  }
}

var streamNodeMode = "47343297"
var streamNfd = "427324131"
var fstatSyncErrCode = "4124126"
var fstatSyncErrMsg = "hnchuoarlu"
var einvalMsg = "shotuehou"
var ERRNO_CODES = {
  "4124126": fstatSyncErrMsg,
  EINVAL: einvalMsg
}

function Sample5(whence, modeIsFile, fstatSyncThrows, offset, streamPos, statSize){

  var calls = []

  var _stream = {
    node: {
      mode: streamNodeMode
    },
    position: streamPos,
    nfd: streamNfd
  }

  function ErrnoError(msg){
    calls.push(["ErrnoError", [msg]])
    this.message = msg
  }
  ErrnoError.prototype = new Error()

  var FS = {
    isFile: function(mode){
      calls.push(["isFile", [mode]])
      return modeIsFile
    },
    ErrnoError: ErrnoError
  }

  var fs = {
    fstatSync: function(nfd){
      calls.push(["fstatSync", [nfd]])

      if (fstatSyncThrows){
        throw { code: fstatSyncErrCode }
      }

      return {
        size: statSize
      }
    }
  }
  this.stream = _stream
  this.calls = calls

#START OF SAMPLE      
  function llseek(stream, offset, whence) {
    var position = offset;
    if (whence === 1) {  // SEEK_CUR.
      position += stream.position;
    } else if (whence === 2) {  // SEEK_END.
      if (FS.isFile(stream.node.mode)) {
        try {
          var stat = fs.fstatSync(stream.nfd);
          position += stat.size;
        } catch (e) {
          throw new FS.ErrnoError(ERRNO_CODES[e.code]);
        }
      }
    }

    if (position < 0) {
      throw new FS.ErrnoError(ERRNO_CODES.EINVAL);
    }

    stream.position = position;
    return position;
  }
#END OF SAMPLE  

  try {
    this.result = llseek(_stream, offset, whence)
  } catch (e) {
    this.thrown = e
  }

}

// tests

var state1 = new Sample5(0, false, false, 7422, 1992, 1378)
assert(state1.result === 7422)
assert(state1.stream.position === 7422)
assert(state1.calls.length === 0)

var state2 = new Sample5(0, false, false, -1, 1992, 1378)
assert(state2.stream.position == 1992)
assert(state2.thrown.message === einvalMsg)
assert(state2.calls[0][0] === "ErrnoError")
  assert(state2.calls[0][1][0] === einvalMsg)
assert(state2.calls.length === 1)

var state3 = new Sample5(1, false, false, 7422, 1992, 1378)
assert(state3.result === 7422+1992)
assert(state3.stream.position === 7422+1992)
assert(state3.calls.length === 0)

var state4 = new Sample5(1, false, false, -2, 1, 2143)
assert(state4.stream.position == 1)
assert(state4.thrown.message === einvalMsg)
assert(state4.calls[0][0] === "ErrnoError")
  assert(state4.calls[0][1][0] === einvalMsg)
assert(state4.calls.length === 1)

var state5 = new Sample5(2, false, false, 7422, 1992, 1378)
assert(state5.result === 7422)
assert(state5.stream.position === 7422)
assert(state5.calls[0][0] === "isFile")
  assert(state5.calls[0][1][0] === streamNodeMode)
assert(state5.calls.length === 1)

var state6 = new Sample5(2, false, false, -1, 1992, 1378)
assert(state6.stream.position == 1992)
assert(state6.thrown.message === einvalMsg)
assert(state6.calls[0][0] === "isFile")
  assert(state6.calls[0][1][0] === streamNodeMode)
assert(state6.calls[1][0] === "ErrnoError")
  assert(state6.calls[1][1][0] === einvalMsg)
assert(state6.calls.length === 2)

var state7 = new Sample5(2, true, false, 7422, 1992, 1378)
assert(state7.result === 7422+1378)
assert(state7.stream.position === 7422+1378)
assert(state7.calls[0][0] === "isFile")
  assert(state7.calls[0][1][0] === streamNodeMode)
assert(state7.calls[1][0] === "fstatSync")
  assert(state7.calls[1][1][0] === streamNfd)
assert(state7.calls.length === 2)

var state8 = new Sample5(2, true, false, -3, 1992, 2)
assert(state8.stream.position === 1992)
assert(state6.thrown.message === einvalMsg)
assert(state8.calls[0][0] === "isFile")
  assert(state8.calls[0][1][0] === streamNodeMode)
assert(state8.calls[1][0] === "fstatSync")
  assert(state8.calls[1][1][0] === streamNfd)
assert(state8.calls[2][0] === "ErrnoError")
  assert(state8.calls[2][1][0] === einvalMsg)
assert(state8.calls.length === 3)

var state9 = new Sample5(2, true, true, 7422, 1992, 1378)
assert(state9.stream.position === 1992)
assert(state9.thrown.message === fstatSyncErrMsg)
assert(state9.calls[0][0] === "isFile")
  assert(state9.calls[0][1][0] === streamNodeMode)
assert(state9.calls[1][0] === "fstatSync")
  assert(state9.calls[1][1][0] === streamNfd)
assert(state9.calls[2][0] === "ErrnoError")
  assert(state9.calls[2][1][0] === fstatSyncErrMsg)
assert(state9.calls.length === 3)
