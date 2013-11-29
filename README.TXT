### How to compile?
Just simply type 'make' in console, which will make 4 executable files.
- omp_allToOne
- omp_oneToAll
- openCL-oneToAll
- openCL-allToOne

### What are the arguments?
For oneToAll programs
	r - number of repititions greater than 0(to increase runtime)
	d - depth of reverse collatz tree to print out between 1 and 19
Example execution ./openCL-allToOne r1 d8 > output.txt


For allToOne progrms
	r - number of repititions greater than 0(to increase runtime)
	n - numbers to compute between 1 and 524288
	s - random seed (optional leave out to seed by time)

Example execution ./openCL-allToOne r3 n524288 s3 > output.txt

For OpenMP programs, you can provide number of processors, using pN. By default, the number of processors set to 16.