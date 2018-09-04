TestCoverage = {coverage={}}

-- What's up with this function? Looks like a lot of stuff got deleted or was never written


local function convert_short2long (opts)
    local i = 1
    local len = #opts
    local ret = {}

    for short_opt, accept_arg in opts:gmatch("(%w)(:?)") do
        ret[short_opt]=#accept_arg
    end

    return ret
end

#START OF SAMPLE
function count_entries(t)
    local count = 0
    for a,b in pairs(t) do
        count = count + 1
    end
    return count
end
#END OF SAMPLE


t = convert_short2long("a b: c d:: :: ?e f g h:")

assert(count_entries(t) == 8)
assert(t["a"] == 0)
assert(t["b"] == 1)
assert(t["c"] == 0)
assert(t["d"] == 1)
assert(t["e"] == 0)
assert(t["f"] == 0)
assert(t["g"] == 0)
assert(t["h"] == 1)