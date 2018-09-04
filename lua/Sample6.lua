TestCoverage = {coverage={}}

datagram = {}

empty_parse_match_res = 425753

parse_match_calls = 0

test_seq_1 = 536426
test_seq_2 = 56541

test_proto = 601284
proto_nil_on_count = -1

function datagram:parse_match(...)
    parse_match_calls = parse_match_calls + 1
    if ... == nil then
        return empty_parse_match_res
    elseif parse_match_calls == proto_nil_on_count then
        a,b = ...
        assert(a == test_seq_1)
        assert(b == test_seq_2)
        return nil
    else
        -- Lua varargs suck
        a,b = ...
        assert(a == test_seq_1)
        assert(b == test_seq_2)
        return test_proto
    end
end



#START OF SAMPLE
function datagram:parse (seq)
    if not seq then
        return self:parse_match()
    end
    local proto = nil
    local i = 1
    while seq[i] do
        proto = self:parse_match(seq[i][1], seq[i][2])
        if not proto then break end
        i = i+1
    end
    return proto
end
#END OF SAMPLE


parse_match_calls = 0
assert(datagram:parse(nil) == empty_parse_match_res)
assert(parse_match_calls == 1)

parse_match_calls = 0
assert(datagram:parse({}) == nil)
assert(parse_match_calls == 0)


parse_match_calls = 0
assert(datagram:parse({{test_seq_1,test_seq_2},{test_seq_1,test_seq_2},{test_seq_1,test_seq_2}}) == test_proto)
assert(parse_match_calls == 3)

parse_match_calls = 0
proto_nil_on_count = 2
assert(datagram:parse({{test_seq_1,test_seq_2},{test_seq_1,test_seq_2},{test_seq_1,test_seq_2}}) == nil)
assert(parse_match_calls == 2)