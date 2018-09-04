# Extracts the code to transform from a sample in the RWUS suite.
#
# We used a variant of this script to generate the challenges that were given to human programmers in our
# study for the OOPSLA 2018 paper. This variant is released with all other experiment data in the Docker image.

SAMPLE_START_INDICATOR = "#START OF SAMPLE"
SAMPLE_END_INDICATOR = "#END OF SAMPLE"

if ARGV.length != 2
  puts "Usage: ruby extract_sample.rb <benchmark file> <out file>"
  exit
end

fil_name = ARGV[0]
test = File.read(fil_name)
outfil = ARGV[1]

$comment_prefixes = {'.lua' => '--',
                     '.java' => '//',
                     '.c' => '//',
                     '.js' => '//',
                     '.py' => '#'}

$ext = File.extname(fil_name)

# If you are running a human study, use this
# to prefix any instructions to the sample
def add_instruction_line(instr, program)
  $comment_prefixes[$ext] + instr + "\n" + program
end

reduced_sample = test.gsub(/.*?#{SAMPLE_START_INDICATOR}/m, "").gsub(/#{SAMPLE_END_INDICATOR}.*/m, "")

File.open(outfil, "w") {|f| f.puts(reduced_sample) }