*** This tar file contains the following files:

* Test suite for the first submission code (libjp1.a)
* Test suite for the second submission code (libjp2.a)
* Test suites for the third submission code (libjpopen.a, libjpclose.a,libjpwrite.a, libjpread.a, libjpdup.a, libjpunlink.a)
* README_E1.txt: shows the exact output that you should see from one of the tests in libjp1.a 
* README_E2.txt: this file specifies the pre-requisites for each test in libjp2.a.  
* README_E3.txt: this file specifies the pre-requisites for each test in libjp3.a.                       

*** Usage:

* You have to create a soft link named libjp.a pointing to the library with the tests that you want to run (libjp1.a, libjp2.a, libjopen.a, etc). Be sure that in the Makefile, 
the user target is linked always with the libjp.a file

* You should NOT modify the .bochsrc file that we have included in the tar file with the zeos base files. For example, if you modify the number of instruction per second 
that the virtual machine executes (ips), then some tests may fail.

* You have to call the runjp() routine from the main function of the user.c source file. You have to take into account that you only can run one test suite at the same time.

* You have to take into account that the tests in a test suite are cumulative. That is, if one test fails then the execution of the rest of the tests may be inconsistent.

* You have to enable Bochs to write on the console, in order to read all the test messages without problems (this is something already done in the system image that you use in the 
course laboratories).

* You can execute just a range of tests, replacing  the call to runjp() in the main function by a call to runjp_rank(int first, int last), where 
"first" is the identifier of the first test in the range that you want to execute, and "last" is the last one.

