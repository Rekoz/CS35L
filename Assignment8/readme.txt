Because every time we the loop is executed, the results are completely 
independent of the previous results. Therefore I divide the loop into n parts
by increasing the stride from 1 to the number of threads, and assign each
thread to do its corresponding part of the loop.

The time is inversely proportional to the number of threads, which makes sense
because we have n threads simultaneously doing the work instead of only one
thread executing the whole loop.
