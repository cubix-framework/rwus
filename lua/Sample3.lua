TestCoverage = {coverage={}}

testFilePath = 13520
testExtraArgs = 24045

loadFileRes = 42040

expectedEditor = nil

lineNo = 0

testMask = 64262
testLine = 6427
testLineDyn = 4242

testEditor = {}

all_markers = {}
function testEditor:MarkerGetAll()
    assert(lineNo == 0)
    lineNo = lineNo + 1
    return all_markers
end

line_count1 = 0
line_count2 = 0
function testEditor:GetLineCount()
    if lineNo == 1 then
        lineNo = lineNo + 1
        return line_count1
    else
        assert(lineNo == 4)
        lineNo = lineNo + 1
        return line_count2
    end
end

lineDynCalls = 0

targ_line_dyn = 0

function testEditor:GetLineDyn(line)
    if lineNo == 1 then
        lineDynCalls = lineDynCalls + 1
        assert(line == testLine)
        return testLineDyn
    else
        assert(lineNo >= 5)
        lineNo = lineNo + 1
        if line == targ_line_dyn then
            return testLineDyn
        else
            return testLineDyn + 1
        end
    end
end

function testEditor:MarkerDeleteAll(x)
    assert(x == -1)
    assert(lineNo == 3)
    lineNo = lineNo + 1
    assert(#all_markers > 0)
end

marker_add_set_called = false
function testEditor:MarkerAddSet(l, m)
    marker_add_set_called = true
    if line_count1 == line_count2 then
        assert(l == testLine)
        assert(m == testMask)
        assert(lineNo == 5)
        lineNo = lineNo + 1
    else
       lineNo = lineNo + 1
       assert(l == targ_line_dyn)
       assert(m == testMask)
    end
end

function LoadFile(fp, ed, ...)
    if not ed then
        assert(lineNo == 0)
    else
        assert(lineNo == 2)
        lineNo = lineNo + 1
    end
    assert(fp == testFilePath)
    assert(ed == expectedEditor)
    assert(... == testExtraArgs)
    return loadFileRes
end

function PackageEventHandle(s, e)
    assert(s == "onEditorMarkerUpdate")
    assert(e == testEditor)

    if #all_markers == 0 then
        assert(lineNo == 3)
    elseif line_count1 == line_count2 then
        assert(lineNo == 6)
    end

    lineNo = lineNo + 1
end

unpack = table.unpack

#START OF SAMPLE
function ReLoadFile(filePath, editor, ...)
    if not editor then return LoadFile(filePath, editor, ...) end

    local markers = editor:MarkerGetAll()
    for _, marker in ipairs(markers) do marker[3] = editor:GetLineDyn(marker[1]) end
    local lines = editor:GetLineCount()

    editor = LoadFile(filePath, editor, ...)
    if not editor then return end

    if #markers > 0 then
    editor:MarkerDeleteAll(-1)
    local samelinecount = lines == editor:GetLineCount()
    for _, marker in ipairs(markers) do
        local line, mask, text = unpack(marker)
        if samelinecount then
            editor:MarkerAddSet(line, mask)
        else
            for _, l in ipairs({line, line-1, line-2, line+1, line+2}) do
                if text == editor:GetLineDyn(l) then
                    editor:MarkerAddSet(l, mask)
                    break
                end
            end
        end
    end
    PackageEventHandle("onEditorMarkerUpdate", editor)
    end

    return editor
end
#END OF SAMPLE

expectedEditor = nil
lineNo = 0
lineDynCalls = 0
assert(ReLoadFile(testFilePath, nil, testExtraArgs) == loadFileRes)
assert(lineNo == 0)


expectedEditor = testEditor
loadFileRes = nil
lineNo = 0
lineDynCalls = 0
all_markers = {{testLine , testMask}, {testLine , testMask}}
assert(ReLoadFile(testFilePath, testEditor, testExtraArgs) == nil)
assert(lineDynCalls == 2)
assert(lineNo == 3)


expectedEditor = testEditor
loadFileRes = nil
lineNo = 0
lineDynCalls = 0
all_markers = {}
assert(ReLoadFile(testFilePath, testEditor, testExtraArgs) == nil)
assert(lineDynCalls == 0)
assert(lineNo == 3)

-- Really don't want to make a test harness that checks for multiple iterations through loop.

marker_add_set_called = false
expectedEditor = testEditor
loadFileRes = testEditor
lineNo = 0
lineDynCalls = 0
all_markers = {{testLine, testMask} }
line_count1 = 27
line_count2 = 27
assert(ReLoadFile(testFilePath, testEditor, testExtraArgs) == testEditor)
assert(lineDynCalls == 1)
assert(lineNo == 7)
assert(marker_add_set_called == true)

marker_add_set_called = false
expectedEditor = testEditor
loadFileRes = testEditor
lineNo = 0
lineDynCalls = 0
all_markers = {{testLine, testMask} }
line_count1 = 27
line_count2 = 28
targ_line_dyn = testLine - 2
assert(ReLoadFile(testFilePath, testEditor, testExtraArgs) == testEditor)
assert(lineDynCalls == 1)
assert(lineNo == 10)
assert(marker_add_set_called == true)

marker_add_set_called = false
expectedEditor = testEditor
loadFileRes = testEditor
lineNo = 0
lineDynCalls = 0
all_markers = {{testLine, testMask} }
line_count1 = 27
line_count2 = 28
targ_line_dyn = testLine - 3
assert(ReLoadFile(testFilePath, testEditor, testExtraArgs) == testEditor)
assert(lineDynCalls == 1)
assert(lineNo == 11)
assert(marker_add_set_called == false)