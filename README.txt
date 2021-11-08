==================================

     Michalis Karagiannakis

       csd4355@csd.uoc.gr

==================================

This README file describes my implementation of the c linux shell.

In order to use it , you can try "make all" and then "./shell" , or just "make run".

This shell can run :
    -Simple commands such as ls, cd, exit, cat, mkdir etc...
    -Sequences of commands (e.g. ls ; whoami ; pwd)
    -Pipelined commands (e.g. cat data.txt | sort | wc -l)
    -Redirections to and from files (e.g. cat < data.txt , ls > data.txt , ls -l >> data.txt etc..)


Information about the implementation:
    -Redirections are implemented with spaces(e.g. cat < data.txt).
    -Multiple pipes are implemented with spaces for commands with arguments like ls -l(e.g. cat data.txt|sort|uniq but cat data.txt|sort|uniq| wc -l).
        -Of course executions like "cat data.txt | sort | uniq | wc -l" work as well.
    -Single pipes are implemented with spaces(e.g. cat data.txt | sort).
    -Sequences of commands can be executed with and without spaces.(e.g. mkdir a; cd a; touch a.txt  or  mkdir a ; cd a ; touch a.txt)

