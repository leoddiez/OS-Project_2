# FILES
**MAIN:**
This is the main file. It contains the main program in which has the customer and teller functions, semaphores, mutex, prototypes, and everything else needed to run threads. It executes the entire project on its own. In the main main section of the code, the teller and customer threads are created and joined, and the semaphores are initialized. The teller and customer void functions work hand and hand and depened on one anothers signals.

# PROGRAM EXECUTION
run: gcc main.c -o main -pthread
then: ./main
