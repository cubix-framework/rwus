The RWUS (Real World, Unchanged Semantics) benchmark suite consists of 50 samples of real-world code --- 10 each from C, Java, JavaScript, Lua, and Python --- together with a test suite thorough enough to detect all semantic changes to the program. It averages 7 lines of test code per line of program code. All samples are functions between 10 and 50 lines randomly chosen from the top 20 starred GitHub projects for each language.

It was built for the OOPSLA 2018 paper "One Tool, Many Languages: Language-Parametric Transformation with Incremental Parametric Syntax" in order to ensure that our semantics-preserving transformations (as well as humans asked to do the same transformations by hand) did indeed preserve semantics on a corpus of real-world code. We expect it to be useful for testing other semantics-preserving transformation, especially ones written to work across multiple languages.

A description of how the 50 samples in this corpus were chosen can be found in Appendix B of the arXiV version of the OOPSLA 2018 paper, and a full log of the random-selection process can be found in the accompanying Docker image containing all information regarding the OOPSLA 2018 experiments [link pending].

# Structure of a benchmark sample

Each sample comes as a standalone file designed to be compiled without any dependencies. They contain the sample code to be transformed, mocks for all referenced symbols, and the tests. They do not contain any `import` statements.

Within each file, the sample code to be transformed is delimited by the lines "#START OF SAMPLE" and "#END OF SAMPLE".

# The included helper scripts

We have included two helper scripts.

`extract_sample.rb` extracts the function to be transformed from a sample file into its own standalone file, without text. Operationally, it extracts all text between "#START OF SAMPLE" and "#END OF SAMPLE" markers.

Example usage:

    ruby scripts/extract_sample.rb c/Sample1.c sample1_out.c

`test_generic.rb` takes a transformed function and the original sample file, and runs the tests on the transformed code. As a sanity check, it also runs the tests on the original, un-transformed code. It uses the commands `gcc`, `javac`, `node`, `lua`, and `python` appropriately for each language.

Example usage:

    ruby scripts/test_generic.rb c sample1_out_modified.c c/Sample1.c
    
The files `test_c.sh`, `test_java.sh`, `test_js.sh`, `test_lua.sh`, and `test_python.sh` are aliases for `ruby test_generic.rb c`, `ruby test_generic.rb java`, etc respectively. 

Example usage:

    ./scripts/test_c.sh sample1_out_modified.c c/Sample1.c
    
