TestCoverage = {coverage={}}

my_error = 6426

old_assert = assert

function asrt(b)
    old_assert(b, "")
end

expected_truth = false

assert_called = false

function assert(b, s)
    asrt(b == expected_truth)
    asrt(s == 'Expected n >= 0!')
    assert_called = true

    if not b then
        error(my_error)
    end
end

#START OF SAMPLE
local function fib_iter(n)
    assert(n > 0, 'Expected n >= 0!')
    return coroutine.wrap(function()
        local a, b, k = 0, 1, 0
        while k < n do
            a, b = b, a+b
            k = k + 1
            coroutine.yield(k, a)
        end
    end)
end
#END OF SAMPLE

expected_truth = false
status, ret = pcall(fib_iter, -1)
asrt(status == false)
asrt(ret == my_error)
asrt(assert_called == true)


expected_truth = true
assert_called = false
f = fib_iter(6)
asrt(assert_called == true)
x, y = f()
asrt(x == 1 and y == 1)
x, y = f()
asrt(x == 2 and y == 1)
x, y = f()
asrt(x == 3 and y == 2)
x, y = f()
asrt(x == 4 and y == 3)
x, y = f()
asrt(x == 5 and y == 5)
x, y = f()
asrt(x == 6 and y == 8)

asrt(f() == nil)