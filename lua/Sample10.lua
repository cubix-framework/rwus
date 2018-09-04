TestCoverage = {coverage={}}

asrt = assert

nql = {}
nql.transitions = {}
nql.minibatch_size = 56347447
nql.dw = {}
nql.network = {}
nql.wc = 5647544
nql.w = {}
nql.numSteps = 3576733
nql.learn_start = 775684422
nql.lr_start = 53.5364543
nql.lr_end = 53.5643256
nql.lr_endt = 38565.534
nql.g = {}
nql.g2 = {}
nql.tmp = {}
nql.deltas = {}

lineNo = 0

function nql.transitions:size()
    return 10
end

function lineNoIs(n)
    asrt(lineNo == n)
    lineNo = lineNo + 1
end


function assert(a)
    lineNoIs(0)
    asrt(a == (nql.transitions:size() > nql.minibatch_size))
end

test_s = 674543544
test_a = 7653337
test_r = 53644577344
test_s2 = 85753376
test_term = 5364776
function nql.transitions:sample(m)
    lineNoIs(1)
    asrt(m == nql.minibatch_size)
    return test_s, test_a, test_r, test_s2, test_term
end

test_targets = 7564754
function nql:getQUpdate(opts)
    lineNoIs(2)
    asrt(opts.s == test_s and opts.a == test_a and opts.r == test_r and opts.s2 == test_s2 and opts.term == test_term and opts.update_qmax == true)
    return test_targets, nil, nil
end

function nql.dw:zero()
    lineNoIs(3)
end

function nql.network:backward(s, t)
    asrt(s == test_s)
    asrt(t == test_targets)
    lineNoIs(4)
end

function nql.dw:add(a, b)
    lineNoIs(5)
    asrt(a == -nql.wc and b == nql.w)
end

math = {}

test_t = 53.5326
test_lr = 87645
function math.max(a,b)
    if lineNo == 6 then
        lineNoIs(6)
        asrt(a == 0 and b == nql.numSteps - nql.learn_start)
        return test_t
    else
        lineNoIs(7)
        expected_lr = (nql.lr_start - nql.lr_end) * (nql.lr_endt - test_t)/nql.lr_endt +
                nql.lr_end
        asrt(a == expected_lr and b == nql.lr_end)
        return test_lr
    end
end

function nql.g:mul(x)
    asrt(x == 0.95)
    local r = {}
    function r:add(a,b)
        asrt(a == 0.05 and b == nql.dw)
        lineNoIs(8)
    end
    return r
end

function nql.tmp:cmul(a, b)
    if lineNo == 9 then
        asrt(a == nql.dw and b == nql.dw)
        lineNoIs(9)
    else
        lineNoIs(11)
        asrt(a == nql.g and b == nql.g)
    end
end

function nql.g2:mul(x)
    asrt(x == 0.95)
    local r = {}
    function r:add(a,b)
        asrt(a == 0.05 and b == nql.tmp)
        lineNoIs(10)
    end
    return r
end

function nql.tmp:mul(a)
    lineNoIs(12)
    asrt(a == -1)
end

function nql.tmp:add(a)
    if lineNo == 13 then
        lineNoIs(13)
        asrt(a == nql.g2)
    else
        lineNoIs(14)
        asrt(a == 0.01)
    end
end

function nql.tmp.sqrt()
    lineNoIs(15)
end

function nql.deltas:mul(a)
    asrt(a == 0)
    local r = {}
    function r:addcdiv(b,c,d)
        asrt(b == nql.lr and c == nql.dw and d == nql.tmp)
        lineNoIs(16)
    end
    return r
end

function nql.w:add(a)
    asrt(a == nql.deltas)
    lineNoIs(17)
end

#START OF SAMPLE
function nql:qLearnMinibatch()
    assert(self.transitions:size() > self.minibatch_size)

    local s, a, r, s2, term = self.transitions:sample(self.minibatch_size)

    local targets, delta, q2_max = self:getQUpdate{s=s, a=a, r=r, s2=s2,
        term=term, update_qmax=true}

    self.dw:zero()

    self.network:backward(s, targets)

    self.dw:add(-self.wc, self.w)

    local t = math.max(0, self.numSteps - self.learn_start)
    self.lr = (self.lr_start - self.lr_end) * (self.lr_endt - t)/self.lr_endt +
            self.lr_end
    self.lr = math.max(self.lr, self.lr_end)

    self.g:mul(0.95):add(0.05, self.dw)
    self.tmp:cmul(self.dw, self.dw)
    self.g2:mul(0.95):add(0.05, self.tmp)
    self.tmp:cmul(self.g, self.g)
    self.tmp:mul(-1)
    self.tmp:add(self.g2)
    self.tmp:add(0.01)
    self.tmp:sqrt()

    self.deltas:mul(0):addcdiv(self.lr, self.dw, self.tmp)
    self.w:add(self.deltas)
end
#END OF SAMPLE

lineNo = 0
asrt(nql.l == test_lr2)
nql.minibatch_size = 100
asrt(nql:qLearnMinibatch() == nil)
asrt(lineNo == 18)


lineNo = 0
asrt(nql.l == test_lr2)
nql.minibatch_size = 5
asrt(nql:qLearnMinibatch() == nil)
asrt(lineNo == 18)