TestCoverage = {coverage={}}

asrt = assert
lineNo = 0


local dirtable_res = {4,3,2,1}

function lineNoIs(n)
    asrt(lineNo == n)
    lineNo = lineNo + 1
end

function checkLineNo(n)
    if lineNo == n then
        lineNo = lineNo + 1
        return true
    end
    return false
end

test_link = {1,2,3,4 }

test_link.lo = {}

test_link.lo.flags = {up=75632, loopback=20604,running=5320420, broadcast=true, multicast=true }
test_link.lo.macaddr = 20534
test_link.lo.loopback = 756261
test_link.lo.mtu = 5362
test_link.eth0 = nil
test_link.eth1 = nil
test_link.wlan0 = nil

nl = {}

function nl.getlink()
    return test_link
end

function assert(a,b)
    if checkLineNo(0) then
        asrt(a == test_link and b == nil)
    elseif checkLineNo(2) then
        asrt(a == dirtable_res and b == nil)
    elseif checkLineNo(4) then
        asrt(a == test_link.lo and b == "expect a loopback interface")
    elseif checkLineNo(5) then
        asrt(a == test_link.lo.flags.up and b == "loopback interface should be up")
    elseif checkLineNo(6) then
        asrt(a == test_link.lo.flags.loopback and b == "loopback interface should be marked as loopback")
    elseif checkLineNo(7) then
        asrt(a == test_link.lo.flags.running and b == "loopback interface should be running")
    elseif checkLineNo(8) then
        asrt(a == (not test_link.lo.flags.broadcast) and b == "loopback interface should not be broadcast")
    elseif checkLineNo(9) then
        asrt(a == (not test_link.lo.flags.multicast) and b == "loopback interface should not be multicast")
    elseif checkLineNo(11) then
        asrt(a == test_link.lo.loopback and b == "loopback interface type should be loopback")
    elseif checkLineNo(12) then
        asrt(a == (test_link.lo.mtu >= 16436) and b == "expect lo MTU at least 16436")
    else
        if test_link.eth0 or test_link.eth1 then
            local eth = test_link.eth0 or test_link.eth1
            if checkLineNo(13) then
                asrt(a == eth.flags.broadcast and b == "ethernet interface should be broadcast")
            elseif checkLineNo(14) then
                asrt(a == eth.flags.multicast and b == "ethernet interface should be multicast")
            elseif checkLineNo(15) then
                asrt(a == eth.ether and b == "ethernet interface type should be ether")
            elseif checkLineNo(17) then
                asrt(a == readfile_res and b == "expect eth to have address file in /sys")
            elseif checkLineNo(20) then
                asrt(a == readfile_res and b == "expect eth to have mtu in /sys")
            else
                error("bad lineno")
            end
        elseif test_link.wlan0 then
            local wlan = test_link.wlan0
            if checkLineNo(13) then
                asrt(a == wlan.ether and b == "wlan interface type should be ether")
            else
                lineNoIs(15)
                asrt(a == readfile_res and b == "expect wlan to have address file in /sys")
            end
        else
            error("bad lineno")
        end
    end

    return a
end

function assert_equal(a,b,c)
    if checkLineNo(3) then
        asrt(a == 4 and b == 4 and c == "expect same number of interfaces as /sys/class/net")
    elseif checkLineNo(10) then
        asrt(a == tostring(test_link.lo.macaddr) and b == "00:00:00:00:00:00" and c == "null hardware address on loopback")
    else
        local eth = test_link.eth0 or test_link.eth1
        if eth then
            if checkLineNo(16) then
                asrt(a == eth.addrlen and b == 6 and c == "ethernet hardware address length is 6")
            elseif checkLineNo(18) then
                asrt(a == tostring(eth.macaddr) .. '\n' and b == readfile_res and c == "mac address hsould match that from /sys")
            elseif checkLineNo(19) then
                asrt(a == tostring(eth.broadcast) and b == "ff:ff:ff:ff:ff:ff" and c == "ethernet broadcast mac")
            else
                lineNoIs(21)
                asrt(a == eth.mtu and b == tonumber(readfile_res) and c == "expect ethernet MTU to match /sys")
            end
        else
            local wlan = test_link.wlan0
            if checkLineNo(14) then
                asrt(a == wlan.addrlen and b == 6 and c == "wireless hardware address length is 6")
            else
                lineNoIs(16)
                asrt(a == tostring(wlan.macaddr) .. '\n' and b == readfile_res and c == "mac address should match that from /sys")
            end
        end
    end
end

local S = {}

local stat_res = nil

function S.stat(s)
    lineNoIs(1)
    asrt(s == "/sys/class/net")
    return stat_res
end


local util = {}

function util.dirtable(p, b)
    asrt(p == "/sys/class/net" and b == true)
    return dirtable_res
end

readfile_res = "535376"
function util.readfile(s)
    if test_link.eth0 or test_link.eth1 then
        if lineNo == 17 then
            asrt(s == "/sys/class/net/" .. (test_link.eth0 or test_link.eth1).name .. "/address")
        else
            asrt(lineNo == 20)
            asrt(s == "/sys/class/net/" .. (test_link.eth0 or test_link.eth1).name .. "/mtu")
        end
    end

    if test_link.wlan0 then
        asrt(lineNo == 15)
        asrt(s == "/sys/class/net/" .. test_link.wlan0.name .. "/address")
    end

    return readfile_res
end


test_eth = {flags={broadcast=9456777, multicast=77452633,}, ether=5644333, macaddr=5364222, broadcast=5364222, mtu=765432277, name="eoeuhn" }
test_wlan = {ether=34522544, addrlen=53642566, name="AESUHEUNUHXEHH", macaddr=563425444}


#START OF SAMPLE
test_getlink = function()
    local i = assert(nl.getlink())
    local st, err = S.stat("/sys/class/net") -- just in case sysfs not mounted
    if not st then error "skipped" end
    local df = assert(util.dirtable("/sys/class/net", true))
    assert_equal(#df, #i, "expect same number of interfaces as /sys/class/net")
    assert(i.lo, "expect a loopback interface")
    local lo = i.lo
    assert(lo.flags.up, "loopback interface should be up")
    assert(lo.flags.loopback, "loopback interface should be marked as loopback")
    assert(lo.flags.running, "loopback interface should be running")
    assert(not lo.flags.broadcast, "loopback interface should not be broadcast")
    assert(not lo.flags.multicast, "loopback interface should not be multicast")
    assert_equal(tostring(lo.macaddr), "00:00:00:00:00:00", "null hardware address on loopback")
    assert(lo.loopback, "loopback interface type should be loopback")
    assert(lo.mtu >= 16436, "expect lo MTU at least 16436")
    local eth = i.eth0 or i.eth1 -- may not exist
    if eth then
        assert(eth.flags.broadcast, "ethernet interface should be broadcast")
        assert(eth.flags.multicast, "ethernet interface should be multicast")
        assert(eth.ether, "ethernet interface type should be ether")
        assert_equal(eth.addrlen, 6, "ethernet hardware address length is 6")
        local mac = assert(util.readfile("/sys/class/net/" .. eth.name .. "/address"), "expect eth to have address file in /sys")
        assert_equal(tostring(eth.macaddr) .. '\n', mac, "mac address hsould match that from /sys")
        assert_equal(tostring(eth.broadcast), 'ff:ff:ff:ff:ff:ff', "ethernet broadcast mac")
        local mtu = assert(util.readfile("/sys/class/net/" .. eth.name .. "/mtu"), "expect eth to have mtu in /sys")
        assert_equal(eth.mtu, tonumber(mtu), "expect ethernet MTU to match /sys")
    end
    local wlan = i.wlan0
    if wlan then
        assert(wlan.ether, "wlan interface type should be ether")
        assert_equal(wlan.addrlen, 6, "wireless hardware address length is 6")
        local mac = assert(util.readfile("/sys/class/net/" .. wlan.name .. "/address"), "expect wlan to have address file in /sys")
        assert_equal(tostring(wlan.macaddr) .. '\n', mac, "mac address should match that from /sys")
    end
end
#END OF SAMPLE

lineNo = 0
stat_res = nil
status, err = pcall(test_getlink)
asrt(status == false)
asrt(lineNo == 2)
-- Lua is being weird and prefixing things to the error message; can't check

test_link.eth0 = nil
test_link.eth1 = nil
test_link.wlan0 = nil

lineNo = 0
stat_res = true
test_link.lo.flags.broadcast = true
test_link.lo.flags.multicast = true
test_link.lo.mtu = 16436
asrt(test_getlink() == nil)
asrt(lineNo == 13)

lineNo = 0
stat_res = true
test_link.lo.flags.broadcast = false
test_link.lo.flags.multicast = false
test_link.lo.mtu = 16435
asrt(test_getlink() == nil)
asrt(lineNo == 13)

-- Maybe I should test conjection of eth and wlan, but it's easier to test them exclusively
lineNo = 0
test_link.eth0 = test_eth
test_link.eth1 = nil
test_link.wlan0 = nil
asrt(test_getlink() == nil)
asrt(lineNo == 22)

lineNo = 0
test_link.eth0 = nil
test_link.eth1 = test_eth
test_link.wlan0 = nil
asrt(test_getlink() == nil)
asrt(lineNo == 22)


lineNo = 0
test_link.eth0 = nil
test_link.eth1 = nil
test_link.wlan0 = test_wlan
asrt(test_getlink() == nil)
asrt(lineNo == 17)