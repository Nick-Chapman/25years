 
/* Connect-four program to practise using Emacs */

#include <stdio.h>

#define INFINITY 1000
#define SEARCH_DEPTH 3

#define WIDTH 7
#define HEIGHT 6

#define BORDER 2
#define EMPTY 0
#define MIN -1
#define MAX 1

#define PLAYERS 2
#define HUMAN 1
#define MACHINE -1

#define TRUE 1
#define FALSE 0
typedef int bool_t;

/* Directions in terms of steps to move in board */
#define N   1
#define S  -1
#define E   8
#define W  -8
#define NE  9
#define SW -9
#define NW  7
#define SE -7

#define MAX_PIECES 42
static short weight[WIDTH+1][HEIGHT+1] = {
    { 0, 0, 0, 0, 0, 0, 0 },
    { 0, 3, 4, 5, 5, 4, 3 },
    { 0, 4, 6, 7, 7, 6, 4 },
    { 0, 5, 7, 9, 9, 7, 5 },
    { 0, 7, 10,13,13,10,7 },
    { 0, 5, 7, 9, 9, 7, 5 },
    { 0, 4, 6, 7, 7, 6, 4 },
    { 0, 3, 4, 5, 5, 4, 3 } };

static short board[WIDTH+2][HEIGHT+2];
static short height[WIDTH+1];
static short pieces; /* 0-42 */
static short score;  /* in terms of the last player who played */
static short player; /* last player who played */
static short winner; /* MIN, MAX or EMPTY */

/* best order to search the board */
static short order[WIDTH+1] = { 0, 4, 3, 5, 2, 6, 1, 7 };

/* Fix it so the MACHINE is always MIN and the HUMAN is always MAX */
static short colour_of[PLAYERS] = {MAX, MIN};

static bool_t pruning = TRUE;
static short depth = 5;
static short trace = 0;
static int adds = 0;

static bool_t iterative_deepening = TRUE;
static int min_adds = 40000;

/* function prototypes */

void play_game();
short human_move();
short machine_move();
short mini_max();

void init_board();
bool_t game_over();
void add_piece();
void remove_piece();
short search();
void display_board();
short random_to_n();

main()
{
    play_game();
}


void stats()
{
    short i;
    
    /*
    printf("height  :");
    for (i = 1; i <= WIDTH; i++ )
	printf(" %d", height[i]);
    printf("\n");
    printf("pieces : %d\n", pieces );
    printf("player : %d\n", player );
    printf("winner : %d\n\n", winner );
    */
    printf("score  : %d\n", score );
    printf("depth  : %d\n", depth );
    printf("trace  : %d\n", trace );
    printf("prune  : %s\n", pruning?"on":"off" );
    printf("iterative deepening : %s\n", iterative_deepening?"on":"off" );
    printf("minimum adds : %d\n", min_adds );
}


void play_game()
{
    short go;
    short move;
    
    go = HUMAN;
    init_board();
    stats();
    display_board();

    while ( !game_over() )
	{
	    if ( go == HUMAN )
		{
		    if ( move = human_move() )
			{
			    add_piece( move );
			    display_board();
			}
		}		    
	    else
		{
		    add_piece( machine_move() );
		    display_board();
		}
	    go = go * -1;
	}

    if ( winner == MIN )
	printf("Min wins\n" );
    else if ( winner == MAX )
	printf("Max wins\n" );
    else
	printf("Draw\n");

} 


short human_move()
{
    char c;
    int move = -1;

    printf("\n");
    while (move < 0 || move > 7)
	{
	    printf("Enter Move [1-7|d|p|t] : ");
	    scanf(" %c", &c);
	    if (c >= '1' && c <= '7')
		{
		    move = c - '0';
		    printf("move in : %d\n", move);
		}
	    else if (c == 's')
		{
		    move = 0;
		    printf("swap playing positions\n");
		}
	    else if (c == 'p')
		{
		    pruning = ! pruning;
		    printf("pruning : %s\n", pruning?"on":"off" );
		}
	    else if (c == 't')
		{
		    printf("trace level [0-3] : ");
		    scanf("\n%hd", &trace);
		    printf("trace set to : %d\n", trace);
		}
	    else if (c == 'd')
		{
		    printf("depth required [1..] : ");
		    scanf("%hd", &depth);
		    printf("depth set to : %d\n", depth);
		}
	    else if (c == 'i')
		{
		    iterative_deepening = ! iterative_deepening;
		    if (iterative_deepening)
			{
			    printf("minimum adds required [1..] : ");
			    scanf("%d", &min_adds);
			    printf("minimum adds set to : %d\n", min_adds);
			}
		}
	    else
		{
		    printf("try again\n");
		}

	    if (move < 0)
		stats();
	    
	}
	    
    return( move );
}

void tab(level)
    short level;
{
    short i;

    for (i = 0; i < 4*(depth-level); i++)
	printf(" ");
}

short machine_move()
{
    short level;
    short scores[WIDTH+1];
    short best = INFINITY * -1; /* best score found */
    short best_count = 0;       /* Number of moves with the best score */
    short choice;               /* random choice amongst bests */
    short this;                 /* score of current column */
    short chosen;               /* index of column chosen */
    short i;

    adds = 0;
    printf("\n");
    printf("thinking..");

    if (iterative_deepening)
	{
	    level = 1;
	}
    else
	{
	    level = depth;
	}
	    
    do
	{
	    printf("%d..", level );
	    fflush(stdout);
	    best = INFINITY * -1;
	    best_count = 0;
	    
	    for ( i = 1; i <= WIDTH; i++ )
		{
		    if ( height[order[i]] < 6 )
			{
			    add_piece( order[i] );
			    this = mini_max( level-1, best * -1 + 1 );
			    remove_piece( order[i] );
			    scores[order[i]] = this;
			    if ( this == best )
				best_count++;
			    if ( this > best )
				{
				    best = this;
				    best_count = 1;
				}
			}
		}

	    level++;
	}
    while ( level + pieces <= MAX_PIECES && iterative_deepening && adds <= min_adds );

    choice = random_to_n( best_count );
    
    printf("\n");
    printf("adds : %d\n", adds );
    printf("best-count : %d\n", best_count);
    printf("choice : %d\n", choice);
    printf("choices :");

    for ( i = 1; i <= WIDTH; i++ )
	{
	    printf(" %d", scores[i]);
	    if ( height[i] < 6 && scores[i] == best )
		{
		    choice--;
		    if ( choice == 0 )
			chosen = i;
		}
	}
    printf("\n");
    if ( best == INFINITY )
	printf("I will win now !\n");
    if ( best == -1 * INFINITY )
	printf("You will win now !\n");
    printf("move in : %d\n", chosen);
    printf("\n");
    
    return( chosen );
}



short mini_max( level, prune )
    short level;
    short prune;
{
    short best = INFINITY * -1;
    short this;
    short i;

    if ( game_over() )
	{
	    if (trace > 3) { printf("\n"); tab(level); printf("STOP\n"); }

	    if ( winner )
		return( INFINITY );
	    else
		return( 0 );
	}
	
    if ( level == 0 )
	return( score );

    if (trace > 3) { printf("\n"); tab(level);
		     printf("L : %d, P : %d", level, prune); }

    if (trace > 3) printf("\n");
    if (trace > 3 && level == 1) tab(level);
	    
    for ( i = 1; i <= WIDTH; i++ )
	{
	    if ( height[order[i]] < 6 )
		{
		    add_piece( order[i] );
		    this = mini_max( level-1, best * -1);
		    remove_piece( order[i] );

		    if (trace > 3) {
			if (level > 1) tab(level);
			printf("%d ", this); }
		    
		    if ( this > best )
			best = this;
		    if ( best >= prune && pruning )
			{
/*			    best = INFINITY; */
			    break;
			}
		}
	}

    if (trace > 3) { printf("\n"); tab(level); printf("best : %d\n", best); }
    
    return( best * -1 );
}

void init_board()
{
    short i,j;

    for (i = 0; i < WIDTH+2; i++)
	{
	    for (j = 0; j < HEIGHT+2; j++)
		{
		    board[i][j] = EMPTY;
		}
	}
    for (i = 1; i <= WIDTH; i++)
	{
	    height[i] = 0;
	}
    pieces = 0;
    score = 0;
    winner = EMPTY;
    player = MAX; /* so MIN will be the first player */
}

bool_t game_over()
{
    return( winner || (pieces == MAX_PIECES) );
}

					  
void add_piece( i )

    short i;
{
    short j = height[i]+1;

    adds++;

    player = player * -1;
    (height[i])++;
    board[i][j] = player;
    pieces++;
    score = weight[i][j] - score;
    if ( search( i, j,  E ) + search( i, j,  W ) >=3 ||
	 search( i, j, NE ) + search( i, j, SW ) >=3 ||
	 search( i, j, SE ) + search( i, j, NW ) >=3 ||
	 search( i, j, S  ) >= 3 )
	{
	    winner = player;
	}
}


void remove_piece( i )
    short i;
{
    short j = height[i];

    player = player * -1;
    board[i][j] = EMPTY;
    (height[i])--;
    pieces--;
    score = weight[i][j] - score;
    winner = EMPTY;
}
    
short search( i, j, d )
    short i, j, d;
{
    short length = 0;
    short *sq;

    sq = &board[i][j];
    sq = sq+d;

    while ( *sq == player )
	{
	    sq+=d;
	    length++;
	}
    return( length );
}


void display_board()
{
    short i, j;

    printf("   _____ _____ _____ _____ _____ _____ _____ \n");
    for (j = HEIGHT; j >= 1; j--)
	{
	    printf("  |     |     |     |     |     |     |     |\n");
	    printf("  |");
	    for (i = 1; i <= WIDTH; i++)
		{
		    if ( board[i][j] == EMPTY )
			printf("     |");
		    else if (board[i][j] == MAX)
			printf("  X  |");
		    else
			printf("  o  |");
		}
	    printf("\n");
	    printf("  |_____|_____|_____|_____|_____|_____|_____|\n");
	}
}


short random_to_n( n )
{
/*    return( 1 + (((double)random() / exp2(31.0)) * (double)n) ); */
    return( 1 + (((double)random() / (double)2.1475e9) * (double)n) );
}

    
