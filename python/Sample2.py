class TestCoverage:
  coverage = [False] * 10000
lineNo = 0

test_command = 5217131
test_response = 3737531

class PatchObj:
    called = 0

    def __enter__(self):
        global lineNo
        assert lineNo == 0
        lineNo += 1
        return self

    def __exit__(self, exc_type, exc_val, exc_tb):
        global lineNo
        assert lineNo == 3
        lineNo += 1
        pass

test_patch_obj = PatchObj()
test_called = 351753
test_patch_obj.called = test_called


def patch(str):
    assert str == 'ycm.vimsupport.SetQuickFixList'
    return test_patch_obj

class Request:
      _response = 0

      def RunPostCommandActionsIfNeeded(self):
          global lineNo
          assert lineNo == 1
          lineNo += 1

test_request = Request()

def CommandRequest(f):
    assert f == [test_command]
    return test_request

def ok_(called):
    global lineNo
    assert lineNo == 2
    lineNo += 1
    assert called == test_called


#START OF SAMPLE
def GoToListTest( command, response ):
    with patch( 'ycm.vimsupport.SetQuickFixList' ) as set_qf_list:
        request = CommandRequest( [ command ] )
        request._response = response
        request.RunPostCommandActionsIfNeeded()
        ok_( set_qf_list.called )
#END OF SAMPLE




GoToListTest(test_command, test_response)
assert lineNo == 4
assert test_request._response == test_response
