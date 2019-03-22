Name: Rishab Sukumar
UID: 304902259
TA: Zhiyi Zhang
File: readme.txt

The task was to modify the ray tracer code so that it allowed for more than one thread
to be run, thus improving its performance.

The first challenge I faced was figuring out what portion of main.c could be parallelized
to yield the greatest improvement in performance. I quickly realised that the large series
of nested for loops in main.c was the best place to begin.

In order to use multiple threads I had to use the POSIX function pthread_create which can
only execute a single function that is passed to it with each thread. Thus, I realised that
I would have to move the nested for loop to a separate function which I could run multiple
threads on.

Moving the loops to a separate function brought up another issue. Several variables that were
declared in the main function were used within these loops. Thus, I defined and initialized
those constants at the beginning of my new function.

I realised that the function needed to be passed the value of the scene variable as well.
I also kept the final print statements at the end of the inner py loop in the main function.
Thus, I had to come up with a way to store the color values in a larger structure that would
account for each pixel. I created a 3D array with size equal to height * width * 3 as 3 color
values are printed for every pixel. In order to pass this 3D array as well as the scene
value and the current thread number and total number of threads to the function I added,
I had to define a struct that contained this information.

I called this struct tdetails to signify that it contained the details for each thread. I
created an array of tdetails type to hold the details for every thread created and passed
each element to the function I created as its argument when I executed pthread_create.

Another issue I ran into was that the function that is passed to pthread_create must return
a void pointer. I realised that such a function must return NULL if nothing else is returned.

In order to truly parallelise the nested loops I had to break the image into chunks. Each
thread would process a different section of the image. I had to account for this using an
if statement at the beginning of my function img_split.

I had to remember to use pthread_join after running all my threads through the img_split
function.

I also had to free all the variables I had dynamically allocated memory for in my main
function.

I realised that initialising constants within my img_split function would add to the
program's execution time as it would do this for every thread that was run.
However, the constants could not be declared and initialised globally without causing an
error in execution. Thus, I left them in the img_split function.

The running of multiple threads resulted in significant performance boosts.

Running 1 thread:
time ./srt 1-test.ppm >1-test.ppm.tmp

real	0m54.959s
user	0m54.943s
sys	0m0.002s

Running 2 threads:
time ./srt 2-test.ppm >2-test.ppm.tmp

real	0m30.680s
user	0m57.371s
sys	0m0.006s

Running 4 threads:
time ./srt 4-test.ppm >4-test.ppm.tmp

real	0m18.798s
user	1m7.910s
sys	0m0.014s

Running 8 threads:
time ./srt 8-test.ppm >8-test.ppm.tmp

real	0m11.176s
user	1m17.704s
sys	0m0.009s

The time taken to execute the ray tracing code is almost halved as we double the number
of threads we run. This is because as we increase the number of threads we run a number
of tasks concurrently instead of one after the other. Thus, we parallelise the task and
improve the performance.
