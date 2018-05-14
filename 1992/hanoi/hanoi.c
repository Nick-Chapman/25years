
void hanoi(n)
{
    int x;
    for (x = 1; x < (1<<n); x++)
	printf("move a disc from %d to %d\n", (x&x-1)%3, ((x|x-1)+1)%3);
}

main()
{
    hanoi(5);
}
