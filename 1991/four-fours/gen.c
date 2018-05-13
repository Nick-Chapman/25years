
#include<stdio.h>
#define MAX_LEN 80

void main(argc, argv)
    int argc;
    char *argv[];
{
    FILE *left, *right;
    char buf[MAX_LEN];

    int a[1000];
    int b[1000];

    int a_len = 0;
    int b_len = 0;
    int i, j, t;

    if ( argc != 3 )
	{
	    printf("usage : %s <file-name1> <file-name2>\n", argv[0]);
	    exit(1);
	}
    
    left  = fopen(argv[1], "r");
    right = fopen(argv[2], "r");
	
    while ( fgets( buf, MAX_LEN, left ) != NULL )
	{
	    a[++a_len] = atoi( buf );
	}

    while ( fgets( buf, MAX_LEN, right ) != NULL )
	{
	    b[++b_len] = atoi( buf );
	}

    for (i = 1; i <= a_len; i++)
	{
	    for (j = 1; j <= b_len; j++)
		{
		    printf("%d\n", a[i] + b[j]);
		    printf("%d\n", a[i] * b[j]);
		    printf("%d\n", a[i] - b[j]);
		    printf("%d\n", b[j] - a[i]);

		    t = pow( a[i], b[j] );
		    if (t >= 0) printf("%d\n", t);
		    t = pow( b[j], a[i] );
		    if (t >= 0) printf("%d\n", t);
		    
		    if (b[j] != 0 && a[i] % b[j] == 0)
			printf("%d\n", a[i] / b[j]);
		    
		    if (a[i] != 0 && b[j] % a[i] == 0)
			printf("%d\n", b[j] / a[i]);

		}
	}

    fclose(left);
    fclose(right);
}

int pow( x, y )
    int x, y;
{
    int res = 1;

    if (y < 0 || y > 9) return(-1);
    
    while (y-- > 0)
	res *= x;

    return( res );
}
