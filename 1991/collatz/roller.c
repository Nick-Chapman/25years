
#include<stdio.h>
#define MAX_LEN 80

void main(argc, argv)
    int argc;
    char *argv[];
{
    int x;
    
    if ( argc != 2 )
	{
	    printf("usage : %s <number>\n", argv[0]);
	    exit(1);
	}
    
    x = atoi( argv[1] );

    if (x <= 0)
	{
	    printf("number must be greater than zero\n");
	    exit(1);
	}


    while ( x != 1 )
	{
	    printf("%d\n", x);

	    if ( x % 2 == 0 ) /*even*/
		{
		    x = x / 2;
		}
	    else /*odd*/
		{
		    x = 3 * x + 1;
		}

	}

    printf("%d\n", x);

}
