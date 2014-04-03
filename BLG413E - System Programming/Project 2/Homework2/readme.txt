Two different source directories (original and modified) are examined with "#diff -r -N" command recursively.
The corresponding output is written into "source.diff" file.
Unnecessary lines are discarded (difference for config file), so that you could see which fundamental parts are added or changed for new kernel.


Our test programs (test_set_hidden.c and test_fork.c) are also added.
Usage of test_set_hidden: "./program pid flag" or "sudo ./program pid flag"
Usage of test_fork: "sudo ./program"
Both test programs outputs necessary information to prove correct working of homework.