
/*
  Hamming numbers in C

  This is pretty ugly.
  I expect even in C it doesn't have to be this bad.
  */ 


#define TRUE 1
#define FALSE 0


struct link {
    int i;
    struct link *next;
}


main ()
{
    struct link *two;
    struct link *three;
    struct link *five;

    struct link *current;
    struct link *new;
    struct link *destroy;

    int a,b,c;
    int positive, duplicates; /* boolean actually */
    
    printf("[1, ");

    current = (struct link*)malloc (sizeof (current));
    current->i = 1;

    two = current;
    three = current;
    five = current;

    positive = TRUE;
    while (positive)
	{
	    new = (struct link*)malloc (sizeof (new));
	    current->next = new;
	    current = new;
	    
	    duplicates = TRUE;
	    while (duplicates) {

		a = 2*(two->i);
		b = 3*(three->i);
		c = 5*(five->i);
	    
		duplicates = FALSE;
		if ( a < b )
		    {
			if ( a < c)
			    {
				/* a smallest */

				if (a<0) {
				    positive = FALSE;
				    printf("...]\n");
				    break;
				}

				printf("%d, ", a);
				current->i = a;
				
				two = two->next;
			    }
			else if ( a > c )
			    {
				/* c smallest */
				printf("%d, ", c);
				current->i = c;
				
				destroy = five;
				five = five->next;
				free( destroy );
			    }
			else
			    {
				/* a & c smallest */
				two = two->next;
				duplicates = TRUE;
			    }
		    }
		else if ( a > b )
		    {
			if ( b < c )
			    {
				/* b smallest */
				printf("%d, ", b);
				current->i = b;
				
				three = three->next;
			    }
			else if ( b > c )
			    {
				/* c smallest */
				printf("%d, ", c);
				current->i = c;
				
				destroy = five;
				five = five->next;
				free( destroy );
			    }
			else
			    {
				/* b & c smallest */
				three = three->next;
				duplicates = TRUE;
			    }
		    }
		else
		    {
			if ( a < c )
			    {
				/* a & b smallest */
				two = two->next;
				duplicates = TRUE;
			    }
			else if ( a > c )
			    {
				/* c smallest */
				printf("%d, ", c);
				current->i = c;
				
				destroy = five;
				five = five->next;
				free( destroy );
			    }
			else
			    {
				/* a & b & c smallest */
				two = two->next;
				three = three->next;
				duplicates = TRUE;
			    }
		    }
	    }
	}
}

