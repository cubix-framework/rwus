TestCoverage = {coverage={}}

local lineNo = 0

local lfs = {}

test_t1 = {__mode=nil, path="oeuoeuoebbkjn"}
test_t2 = {__mode=nil, path="allesubu"}

function lfs.attributes(p, s)
    assert(s == "mode")
    if lineNo == 0 then
        lineNo = lineNo + 1
        assert(p == test_t2.path)
        return test_t2.__mode
    else
        assert(lineNo == 1)
        lineNo = lineNo + 1
        assert(p == test_t1.path)
        return test_t1.__mode
    end
end

#START OF SAMPLE
local function sort_by_key(t1, t2)
    local second_mode = lfs.attributes(t2.path, "mode") == "directory"
    if lfs.attributes(t1.path, "mode") == "directory" then
        if second_mode then
            if t1.order == "ascending" then
                return t1.sort_key < t2.sort_key
            else
                return t1.sort_key > t2.sort_key
            end
        else
            return true
        end
    else
        if second_mode then
            return false
        else
            if t1.order == "ascending" then
                return t1.sort_key < t2.sort_key
            else
                return t1.sort_key > t2.sort_key
            end
        end
    end
end
#END OF SAMPLE

test_t2.__mode = "directory"
test_t1.__mode = "directory"
test_t1.order = "ascending"
test_t1.sort_key = 0
test_t2.sort_key = 1
lineNo = 0
assert(sort_by_key(test_t1, test_t2) == true)
assert(lineNo == 2)
test_t1.sort_key = 1
test_t2.sort_key = 0
lineNo = 0
assert(sort_by_key(test_t1, test_t2) == false)
assert(lineNo == 2)
test_t1.sort_key = 0
test_t2.sort_key = 0
lineNo = 0
assert(sort_by_key(test_t1, test_t2) == false)
assert(lineNo == 2)

test_t2.__mode = "directory"
test_t1.__mode = "directory"
test_t1.order = nil
test_t1.sort_key = 0
test_t2.sort_key = 1
lineNo = 0
assert(sort_by_key(test_t1, test_t2) == false)
assert(lineNo == 2)
test_t1.sort_key = 1
test_t2.sort_key = 0
lineNo = 0
assert(sort_by_key(test_t1, test_t2) == true)
assert(lineNo == 2)
test_t1.sort_key = 0
test_t2.sort_key = 0
lineNo = 0
assert(sort_by_key(test_t1, test_t2) == false)
assert(lineNo == 2)

test_t2.__mode = nil
test_t1.__mode = "directory"
test_t1.order = nil
test_t1.sort_key = 0
test_t2.sort_key = 1
lineNo = 0
assert(sort_by_key(test_t1, test_t2) == true)
assert(lineNo == 2)
test_t1.sort_key = 1
test_t2.sort_key = 0
lineNo = 0
assert(sort_by_key(test_t1, test_t2) == true)
assert(lineNo == 2)
test_t1.sort_key = 0
test_t2.sort_key = 0
lineNo = 0
assert(sort_by_key(test_t1, test_t2) == true)
assert(lineNo == 2)

test_t2.__mode = "directory"
test_t1.__mode = nil
test_t1.order = nil
test_t1.sort_key = 0
test_t2.sort_key = 1
lineNo = 0
assert(sort_by_key(test_t1, test_t2) == false)
assert(lineNo == 2)
test_t1.sort_key = 1
test_t2.sort_key = 0
lineNo = 0
assert(sort_by_key(test_t1, test_t2) == false)
assert(lineNo == 2)
test_t1.sort_key = 0
test_t2.sort_key = 0
lineNo = 0
assert(sort_by_key(test_t1, test_t2) == false)
assert(lineNo == 2)

test_t2.__mode = nil
test_t1.__mode = nil
test_t1.order = "ascending"
test_t1.sort_key = 0
test_t2.sort_key = 1
lineNo = 0
assert(sort_by_key(test_t1, test_t2) == true)
assert(lineNo == 2)
test_t1.sort_key = 1
test_t2.sort_key = 0
lineNo = 0
assert(sort_by_key(test_t1, test_t2) == false)
assert(lineNo == 2)
test_t1.sort_key = 0
test_t2.sort_key = 0
lineNo = 0
assert(sort_by_key(test_t1, test_t2) == false)
assert(lineNo == 2)

test_t2.__mode = nil
test_t1.__mode = nil
test_t1.order = nil
test_t1.sort_key = 0
test_t2.sort_key = 1
lineNo = 0
assert(sort_by_key(test_t1, test_t2) == false)
assert(lineNo == 2)
test_t1.sort_key = 1
test_t2.sort_key = 0
lineNo = 0
assert(sort_by_key(test_t1, test_t2) == true)
assert(lineNo == 2)
test_t1.sort_key = 0
test_t2.sort_key = 0
lineNo = 0
assert(sort_by_key(test_t1, test_t2) == false)
assert(lineNo == 2)