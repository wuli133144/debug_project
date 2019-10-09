FILE(REMOVE_RECURSE
  "CMakeFiles/test.dir/main.c.o"
  "CMakeFiles/test.dir/ctor.c.o"
  "CMakeFiles/test.dir/dtor.c.o"
  "CMakeFiles/test.dir/reg_heap.c.o"
  "CMakeFiles/test.dir/registry.c.o"
  "bin/linux/test.pdb"
  "bin/linux/test"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang C)
  INCLUDE(CMakeFiles/test.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
