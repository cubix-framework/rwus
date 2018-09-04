TestCoverage = {coverage={}}

local total = 0
local pass = 0
local expected_status = ""

local dec_res = "euoeu"
local test_message = 645563037
local print_called = false

local function dec(a,b)
    assert(a == test_message)
    assert(b == 68)
    return dec_res
end

local function print(s)
    assert(s == ((('%02d. %68s: %s'):format(total, dec_res, expected_status))))
    print_called = true
end

local function succeed()
    return true
end

local function fail()
    error(1)
end

#START OF SAMPLE
local function run(message, f)
    total = total + 1
    local ok, err = pcall(f)
    if ok then pass = pass + 1 end
    local status = ok and 'PASSED' or 'FAILED'
    print(('%02d. %68s: %s'):format(total, dec(message,68), status))
end
#END OF SAMPLE


expected_status = "PASSED"
print_called = false
assert(run(test_message, succeed) == nil)
assert(print_called == true)
assert(total == 1)
assert(pass == 1)

expected_status = "FAILED"
print_called = false
assert(run(test_message, fail) == nil)
assert(print_called == true)
assert(total == 2)
assert(pass == 1)