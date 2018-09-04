#!/bin/ruby
# coding: utf-8

def strip_ext(s, ext)
  s.gsub("." + ext, "")
end

def putfile(s, fil)
  File.open(fil, "w") {|f| f.puts s}
end

SAMPLE_START_INDICATOR = "#START OF SAMPLE"
SAMPLE_END_INDICATOR = "#END OF SAMPLE"

def replace_sample(s, rep)
  s.gsub(/#{SAMPLE_START_INDICATOR}.*?#{SAMPLE_END_INDICATOR}/m, rep)
end

def runC(fil)
  comp_text = `gcc -std=c99 #{fil}`
  if $? != 0
    puts "Compilation error"
    puts comp_text
    return false
  end
  `./a.out`
  res = ($? == 0)
  File.delete("a.out")
  res
end

#OMG so hacky
TMP_DIR_NAME = "tmp_compstrat_study"
def runJava(fil)
  fil =~ /(Sample\d+).*?\.java/
  class_name = $1
  Dir.mkdir(TMP_DIR_NAME)
  `cp #{fil} #{TMP_DIR_NAME}/#{class_name}.java`
  Dir.chdir(TMP_DIR_NAME)
  comp_text = `javac #{class_name}.java`

  res = false
  
  if $? != 0
    puts "Compilation error"
    puts comp_text
    res = false
  else
   #-ea is "enable assertions"
   `java -ea #{class_name}`
   res = ($? == 0)
  end

  Dir.chdir("..")  
  `rm -rf #{TMP_DIR_NAME}`

  res
end

def runJS(fil)
  `node #{fil}`
  return $? == 0
end

def runLua(fil)
  `lua #{fil}`
  return $? == 0
end

def runPy(fil)
  `python #{fil}`
  return $? == 0
end

def runprog(fil, lang)
  case lang
  when "c"
    runC(fil)
  when "java"
    runJava(fil)
  when "js"
    runJS(fil)
  when "lua"
    runLua(fil)
  when "py"
    runPy(fil)
  else
    raise "Unrecognized language extension: " + lang
  end
end

if ARGV.length != 3 then
  puts "Usage: ./test_<lang>.sh <lang> <sample-file>.<lang> <test-file>.<lang>"
  puts "<lang> must be one of c, java, js, lua, py"
  exit
end

$lang = ARGV[0]

$sample_fil = ARGV[1]
$test_fil = ARGV[2]

$sample_out = strip_ext($sample_fil, $lang) + "_tmp_testing" + "." + $lang
$test_out = strip_ext($test_fil, $lang) + "_tmp_testing" + "." + $lang

$test_text = File.read($test_fil).force_encoding('UTF-8')


def run_base_test()
  sanity_check_text = $test_text.gsub(SAMPLE_START_INDICATOR, "").gsub(SAMPLE_END_INDICATOR, "")
  putfile(sanity_check_text, $test_out)
  res = runprog($test_out, $lang)
  File.delete($test_out)
  res
end

def run_sample_test()
  prog_text = replace_sample($test_text, File.read($sample_fil))
  putfile(prog_text, $sample_out)
  res = runprog($sample_out, $lang)
  File.delete($sample_out)
  res
end



puts "Running basic test"
if not run_base_test()
  puts "The unaltered program failed the test. Please contact jkoppel@mit.edu for support"
else
  puts "Testing transformed code sample"
  if run_sample_test()
    puts "Test passed!"
  else
    puts "Your transformed program failed the test"
    exit 1
  end
end
