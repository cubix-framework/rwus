TestCoverage = {coverage={}}

test_buf_input = nil;
buf = nil;

test_peer = {};

readnext_res1 = nil;
readnext_res2 = nil;

readnext_called = false;
receive_called = false;
set_breakpoint_called = false;
remove_breakpoint_called = false;

receive_res, receive_partial = nil, nil

function max(a,b)
    if a > b then
        return a;
    else
        return b;
    end
end

function readnext(p, n)
    assert(p == test_peer)
    if #test_buf_input == 1 and readnext_called == false then
        assert(n == 1);
        readnext_called = true;
        return readnext_res1;
    else
        assert(n == 5-(max(#test_buf_input, 2)));
        readnext_called = true
        return readnext_res2;
    end
end

function test_peer:receive()
    assert(receive_called == false)
    receive_called = true
    return receive_res, nil, receive_partial
end

old_find = string.find

function string:find(pat)
    assert(pat == "^([A-Z]+)%s+(.-)%s+(%d+)%s*$")
    assert(self == buf .. receive_res)
    return old_find(self, pat)
end

test_file = "hstao"
test_line = "262420"

function set_breakpoint(f, l)
    assert(set_breakpoint_called == false)
    set_breakpoint_called = true
    assert(f == test_file)
    assert(l == tonumber(test_line))
end

function remove_breakpoint(f, l)
    assert(remove_breakpoint_called == false)
    remove_breakpoint_called = true
    assert(f == test_file)
    assert(l == tonumber(test_line))
end

#START OF SAMPLE
local function handle_breakpoint(peer)
    if not buf or not (buf:sub(1,1) == 'S' or buf:sub(1,1) == 'D') then return end

    if #buf == 1 then buf = buf .. readnext(peer, 1) end
    if buf:sub(2,2) ~= 'E' then return end

    buf = buf .. readnext(peer, 5-#buf)
    if buf ~= 'SETB ' and buf ~= 'DELB ' then return end

    local res, _, partial = peer:receive()
    if not res then
        if partial then buf = buf .. partial end
        return
    end

    local _, _, cmd, file, line = (buf..res):find("^([A-Z]+)%s+(.-)%s+(%d+)%s*$")
    if cmd == 'SETB' then set_breakpoint(file, tonumber(line))
    elseif cmd == 'DELB' then remove_breakpoint(file, tonumber(line))
    else
        return
    end

    buf = nil
end
#END OF SAMPLE

readnext_called, set_breakpoint_called, receive_called, remove_breakpoint_called = false, false, false, false
buf = nil;
assert(handle_breakpoint(nil) == nil);
assert(buf == nil)
assert(readnext_called == false and set_breakpoint_called == false and receive_called == false and remove_breakpoint_called == false)


readnext_called, set_breakpoint_called, receive_called, remove_breakpoint_called = false, false, false, false
buf = "sETB";
assert(handle_breakpoint(nil) == nil);
assert(buf == "sETB")
assert(readnext_called == false and set_breakpoint_called == false and receive_called == false and remove_breakpoint_called == false)


readnext_called, set_breakpoint_called, receive_called, remove_breakpoint_called = false, false, false, false
buf = "dELB";
assert(handle_breakpoint(nil) == nil);
assert(buf == "dELB")
assert(readnext_called == false and set_breakpoint_called == false and receive_called == false and remove_breakpoint_called == false)

readnext_called, set_breakpoint_called, receive_called, remove_breakpoint_called = false, false, false, false
test_buf_input = 'S'
buf = test_buf_input
readnext_res1 = 'e'
assert(handle_breakpoint(test_peer) == nil)
assert(buf == "Se")
assert(readnext_called == true)
assert(set_breakpoint_called == false and receive_called == false and remove_breakpoint_called == false)

readnext_called, set_breakpoint_called, receive_called, remove_breakpoint_called = false, false, false, false
test_buf_input = 'S'
buf = test_buf_input
readnext_res1 = 'E'
readnext_res2 = 'TB'
assert(handle_breakpoint(test_peer) == nil)
assert(buf == "SETB")
assert(readnext_called == true)
assert(set_breakpoint_called == false and receive_called == false and remove_breakpoint_called == false)

readnext_called, set_breakpoint_called, receive_called, remove_breakpoint_called = false, false, false, false
test_buf_input = 'S'
buf = test_buf_input
readnext_res1 = 'E'
readnext_res2 = 'TB_'
assert(handle_breakpoint(test_peer) == nil)
assert(buf == "SETB_")
assert(readnext_called == true)
assert(set_breakpoint_called == false and receive_called == false and remove_breakpoint_called == false)

readnext_called, set_breakpoint_called, receive_called, remove_breakpoint_called = false, false, false, false
test_buf_input = 'S'
buf = test_buf_input
readnext_res1 = 'E'
readnext_res2 = 'TB '
receive_res = nil
receive_partial = "LALALA"
assert(handle_breakpoint(test_peer) == nil)
assert(buf == "SETB LALALA")
assert(readnext_called == true and receive_called == true)
assert(set_breakpoint_called == false and remove_breakpoint_called == false)

readnext_called, set_breakpoint_called, receive_called, remove_breakpoint_called = false, false, false, false
test_buf_input = 'SE'
buf = test_buf_input
readnext_res1 = nil
readnext_res2 = 'TB '
receive_res = false
receive_partial = "LALALA"
assert(handle_breakpoint(test_peer) == nil)
assert(buf == "SETB LALALA")
assert(readnext_called == true and receive_called == true)
assert(set_breakpoint_called == false and remove_breakpoint_called == false)

readnext_called, set_breakpoint_called, receive_called, remove_breakpoint_called = false, false, false, false
test_buf_input = 'SE'
buf = test_buf_input
readnext_res1 = nil
readnext_res2 = 'TB '
receive_res = false
receive_partial = nil
assert(handle_breakpoint(test_peer) == nil)
assert(buf == "SETB ")
assert(readnext_called == true and receive_called == true)
assert(set_breakpoint_called == false and remove_breakpoint_called == false)


readnext_called, set_breakpoint_called, receive_called, remove_breakpoint_called = false, false, false, false
test_buf_input = 'SETB '
buf = test_buf_input
readnext_res1 = nil
readnext_res2 = ''
receive_res = test_file .. " " .. test_line .. " a"
receive_partial = nil
assert(handle_breakpoint(test_peer) == nil)
assert(buf == "SETB ")
assert(readnext_called == true and receive_called == true)
assert(set_breakpoint_called == false and remove_breakpoint_called == false)



readnext_called, set_breakpoint_called, receive_called, remove_breakpoint_called = false, false, false, false
test_buf_input = 'SETB '
buf = test_buf_input
readnext_res1 = nil
readnext_res2 = ''
receive_res = test_file .. " " .. test_line
receive_partial = nil
assert(handle_breakpoint(test_peer) == nil)
assert(buf == nil)
assert(readnext_called == true and receive_called == true)
assert(set_breakpoint_called == true)
assert(remove_breakpoint_called == false)

readnext_called, set_breakpoint_called, receive_called, remove_breakpoint_called = false, false, false, false
test_buf_input = 'DELB '
buf = test_buf_input
readnext_res1 = nil
readnext_res2 = ''
receive_res = test_file .. " " .. test_line
receive_partial = nil
assert(handle_breakpoint(test_peer) == nil)
assert(buf == nil)
assert(readnext_called == true and receive_called == true)
assert(set_breakpoint_called == false)
assert(remove_breakpoint_called == true)