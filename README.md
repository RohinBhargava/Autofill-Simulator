# Autofill-Simulator

A simple Autofill Simulator composed of the following parts:

1. A hashing function.

2. A parsing set of functions.

3. A lookup method.

Here is the transcript for the test of the hashing function on "/usr/share/dict/words":

nsit-dhcp-205-208-007-109:Autofill Simulator legmonkey$ gcc -c word_tree.c hash.c

nsit-dhcp-205-208-007-109:Autofill Simulator legmonkey$ gcc word_tree.o hash.o -o test

nsit-dhcp-205-208-007-109:Autofill Simulator legmonkey$ ./test

	int main(int argc, char **argv)
	{
		int *yes = malloc(10000000000 * sizeof(int)) ; 
		long i;
		for (i = 0; i < (10000000000); ++i)
		 	yes[i] = 0;
		FILE * inf = fopen("/usr/share/dict/words", "r");
		char *mystring = (char *) malloc(100);
		int z = 0;
		while ((mystring = fgets(mystring, 100, inf)) != NULL)
		{
			mystring[strlen(mystring)-1] = '\0';
			++yes[hash(mystring) & ((1 << 31) -1)];
			++z;
		}
	
		int max = 0;
		int ones = 0;
		int twos = 0;
		int threes = 0;
		int sum = 0;
		for (i = 0; i < (10000000000); ++i)
		{
			if (yes[i] > max)
				max = yes[i];
			if (yes[i] == 1)
				++ones;
			if (yes[i] == 2)
				twos += 2;
			if (yes[i] >= 3)
				threes += yes[i];
			sum += yes[i];
		}
	
		printf("\n max value: %d, number of ones: %d, number of twos: %d, number of threes and above: %d.\n", max, ones, twos, threes);
		printf("total collisions: %d. Number of inputs: %d.\n", twos + threes ,sum);
		fclose(inf);
	}

max value: 2, number of ones: 235874, number of twos: 12, number of threes and above: 0.

total collisions: 12. Number of inputs: 235886.

The texts were taken from Mark Stoehr's Lab 2, and the get_sentence code is a modified version of CMSC162's get_word method.
