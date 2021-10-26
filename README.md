# Shared-Memory-using-C-language-and-pthreads

I created a shared memory using C language for an assignment.
By using pthreads I write into the shared memory and then read from the shared memory.
Code is explaned in the ExplainedCode.pdf under Task 1

Task that is given to me is as follows,

Create a text file which contains 5 names, 5 ages’, and 5 cities. Each word and number
should start in a new line as shown below. (Saved in OS.txt )
		
		Janith
		Kalpa
		Ajantha
		Chathura
		Isuru
		Matara
		Galle
		Colombo
		Jaffna
		Kandy
		22
		29
		24
		85
		25



Write a C program to read those 3 categories by using 3 separate threads and display
them on the terminal. The lines which you displayed must be written into a shared
memory location (RAM), which should be accessible from another process later.

Create and run another C program on a separate terminal which will access and display
the contents stored in the shared memory (RAM).
