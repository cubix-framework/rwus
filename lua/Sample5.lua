TestCoverage = {coverage={}}

local test_resource_string = "oueoubstbs"
local test_resource_int = 6462646
statSync_called = false
fstatSync_called = false
fsyncSync_called = false


FS = {}

function FS.statSync(r)
    assert(r == test_resource_string)
    assert(statSync_called == false)
    statSync_called = true
end

function FS.fstatSync(r)
    assert(fsyncSync_called == true)
    assert(fstatSync_called == false)
    assert(r == test_resource_int)
    fstatSync_called = true
end

function FS.fsyncSync(r)
    assert(fsyncSync_called == false)
    fsyncSync_called = true
    assert(r == test_resource_int)
end

#START OF SAMPLE
function stat_resource(resource)
    if type(resource) == 'string' then
        return FS.statSync(resource)
    else
        FS.fsyncSync(resource)
        return FS.fstatSync(resource)
    end
end
#END OF SAMPLE

stat_resource(test_resource_string)
assert(statSync_called == true)
assert(fstatSync_called == false)
assert(fsyncSync_called == false)

statSync_called = false
stat_resource(test_resource_int)
assert(statSync_called == false)
assert(fstatSync_called == true)
assert(fsyncSync_called == true)