# Session 1: April 4th, 2026 11:27
General description of the program:
- 3 tellers, bank opens when they are all ready
- Customer no entry before it's open
  > The door only allows 2 customers in at a time
  > They can withdraw or deposit
  > They must wait in a line to be called (0-100ms)
    -Must go to teller that calls them (the one that is open)
- Only 2 tellers are allowed inside the safe at a time
  > If customer wants to withdraw, need manager permission
    - Only 1 teller at a time can interact w/ manager
- Customer leaves once transaction is complete
- Bank closes once 50 customers have been served and all transactions have been completed

*Teller:*
When created: Unique ID, 3 total
Sequence:
1. Teller will let everyone know it is ready to serve
2. Wait for a customer to approach
3. When signaled by the customer, the teller asks for the transaction
4. Wait until customer gives the transaction
5. If the transaction is a Withdraw, go to the manager for permission.
• The manager always gives permission, but will take some time interacting with the teller
• To represent this interaction, the teller thread should block (sleep) for a random duration
from 5 to 30 ms.
6. Go to the safe, waiting if it is occupied by two tellers
7. In the safe, the teller will physically perform the transaction
• represent this by blocking (sleeping) for a random duration of between 10 and 50 ms.
8. Go back and inform the customer the transaction is done
9. Wait for customer to leave teller

*Customer:*
When created: Unique ID, 50 total
Sequence:
1. The customer will decide (at random) what transaction to perform: Deposit or Withdrawal.
2. The customer will wait between 0 – 100ms
3. The customer will enter the bank (The door only allows two customers to enter at a time).
4. The customer will get in line.
• If there is a teller ready to serve, the customer should immediately go to a ready teller.
• otherwise, the customer should wait until called and then go to a ready teller.
5. The customer will introduce itself (give its id) to the teller.
6. The customer will wait for the teller to ask for the transaction.
7. The customer will tell the teller the transaction.
8. The customer will wait for the teller to complete the transaction.
9. The customer will leave the bank through the door (and the simulation).

*These sequences are copy pasted for me as they make sense and are simplistic enough to follow*

*OUTPUT:*
Teller and customer should print a line for each action they do, following this format:
"THREAD_TYPE ID [THREAD_TYPE ID]: MSG"
THREAD_TYPE: Customer/Teller
ID: PID
MSG: Short decription of action (preset by me i supposeee)

Everytime there is a thread being blocked, there should be 2 lines, same format, action taken before wait, action completion after wait
SHARED RESOURCES 3 lines, teller to resource, thread using resource, thread done using resource

Thoughts:
I know I need shared variables. Im probably doing this in C once more since that's what I did it in previously in a different class. I don't think this will be too
difficult, but I know in the past I've definitely ran into timing issues with threads so we will see.

- @12:48
  did the bare outline for my main program. might be all i do for this session atm

# Session 2: April 16th, 2026 13:05
> Going to work on the main program before I do the void functions. I think I'll work on the customer interaction first since that seems easier (less going on on that end)
- @13:34
  Commited semaphores and pthreads to main() in main.c
- @14:13
  Added loop so that each customer/teller interaction has 3 threads

# Session 3 April 16th, 2026 20:00
> I'm going to try and grind out the rest of this in one sitting. I'm going to finish typing the rest of main, and then going to work on creating the void for the customers next. I know I will need a queue for the customers to wait in, adn will probably need a pointer for the head and the tail (head for next cust, tail for as they come in?) and the rest is just operating the threads.

- @20:15
  finished typing main section. created tellers and customers, joined them and closed bank (main.c), also made the closeing loops inline because it looks nicer

- @21:23
  finished customer void function, and made other fucntions that are necessary to assist inside it (enqueue, dequeue, wait timer), added the sem_t's at the top (i forgot earlier), fixed the #define (i wrote it like as if i was writing a const and not a #define), and added the mutex to lock the queue.
