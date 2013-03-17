I wrote this MD5 password cracker as a project for my Computer Security
course. This may have been the one time that a program was considerably
more fun to code than to run. The hours of running the program and waiting
for it to come back with results was not much fun (seeing as I couldn't do
anything else on my computer on account of the program using all my CPU).
The program was implemented in C, and took advantage of my dual core CPU by
running two threads to reduce search time. A more detailed explanation is 
available in the paper.

## Implementation
This password cracker was implemented in C, using the crypt() function in the crypt library. This line was used to compile:

    gcc –march=core2 –O3 –o crack crack.c –lm –lcrypt

The compiler flag O3 is used to optimize for speed, and march is used to specify what type of processor architecture the computer uses.

A few assumptions were made about the nature of most passwords. Most humans like to use alphanumeric passwords only, and usually they will have the numbers at the end. This program will do an iterative deepening search to a specified depth with an alphanumeric search only, and then will start over from depth 1 using all possible printable characters and then go on indefinitely (changed to max length 10 for realistic purposes).

To take advantage of the fact that numbers and symbols usually come at the end of a password, the password search ordering changed the last character first. Also, the password cracker ran two threads at the same time, one starting from the beginning of the search space at a given search depth, and the other starting at the end and meeting at the middle. This takes advantage of dual core processors, as the two threads can run separately without slowing each other down. This saves an exponential amount of time, 2N, where N is the length of the password.

## Testing

To test the password cracker, I wrote up a hash generator program that would generate a random password, then generate a random salt, and then use that to generate a password hash. The hash_gen program would then execvp an instance of the password cracker.

## Results

    $ ./hash_gen
    Password is: sKH
    Testing cracker:
    $1$wOqQkblz$yiCiroi7plJ92vDbTsAr//
    Forking off second thread.
    Thread 1: Got past length 1 alpha numeric
    Thread 2: Got past length 1 alpha numeric
    Thread 1: Got past length 2 alpha numeric
    Thread 2: Got past length 2 alpha numeric
    Found answer: sKH
    passwords.
    passwords.
    passwords.
    passwords.Time taken: 37.11s
    Password is: OVi
    Testing cracker:
    $1$wOqQkblz$7sxed97DXczX1.GAYR8vS.
    Forking off second thread.
    Thread 1: Got past length 1 alpha numeric
    Thread 2: Got past length 1 alpha numeric
    Thread 1: Got past length 2 alpha numeric
    Thread 2: Got past length 2 alpha numeric
    Found answer: OVi
    Time taken: 43.72s
    Password is: G2H4
    Testing cracker:
    $1$wOqQkblz$krOHMHB1f7Z7m2yJu6ySb0
    Forking off second thread.
    Thread 1: Got past length 1 alpha numeric
    Thread 2: Got past length 1 alpha numeric
    Thread 1: Got past length 2 alpha numeric
    Thread 2: Got past length 2 alpha numeric
    Thread 1: Got past length 3 alpha numeric
    Thread 2: Got past length 3 alpha numeric
    Found answer: G2H4
    Time taken: 3335.98s
    
We see that the password cracker was correct in all those instances. The times are also better than they used to be with the single thread implementation, by a significant amount.
