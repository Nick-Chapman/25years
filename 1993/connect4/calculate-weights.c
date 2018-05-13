
#define abs(x)((x) < (0) ? (-1 * (x)) : (x))
#define min(x,y)((x) < (y) ? (x) : (y))

#define weight(i,j)({ \
			  int a,b; \
			  a = abs((((i)+4)%8)-4); \
			  b = abs((((j)+3)%7)-3); }

main()
{
    int i, j;

    for (j = 6; j >= 1; j--, printf("\n"))
	for (i = 1; i <= 7; i++)
	    {
		int a,b;
		
		a = abs(((i+4)%8)-4);
		b = abs(((j+3)%7)-3);
		printf("%d ", a+b+min(a,b)+(a==4)*b);
	    }
}
