/********************************************************
 * Kernels to be optimized for the CS:APP Performance Lab
 ********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "defs.h"

/* 
 * Please fill in the following team struct 
 */
team_t team = {
    "201701562",           	/* Team name */

    "Song Byeong Jun",     	/* First member full name */
    "potados99@gmail.com",  /* First member email address */

    "",                   	/* Second member full name (leave blank if none) */
    ""                    	/* Second member email addr (leave blank if none) */
};

/**************
 * Symbol names
 **************/
#define _sym_dim			dim
#define _sym_i				i
#define _sym_j				j
#define _sym_dim_minus_one 	(_sym_dim-1)
#define _sym_dst			dst
#define _sym_src			src
#define _sym_avg			avg_better
#define _sym_sum			sum
#define neighber(offset_x, offset_y)			_sym_src[RIDX(_sym_i+offset_x, _sym_j+offset_y, _sym_dim)]

/*******************
 * Macros for rotate
 ******************/
#define level 				32
#define mov(n) 				_sym_dst[RIDX(_sym_dim_minus_one-_sym_j, _sym_i+n, _sym_dim)]=_sym_src[RIDX(_sym_i+n, _sym_j, _sym_dim)]
#define mov4(n)				do { mov(n); mov(n+1); mov(n+2); mov(n+3); } while(0)
#define mov16(n)			do { mov4(n); mov4(n+4); mov4(n+8); mov4(n+12); } while (0)
#define mov32(n)			do { mov16(n); mov16(n+16); } while(0)

/*******************
 * Macros for smooth
 *******************/
#define add_color(offset_x, offset_y, color)	_sym_sum.color+=neighber(offset_x, offset_y).color
#define add_number()							_sym_sum.num++
#define add_pixel(offset_x, offset_y)							\
do {															\
	add_color(offset_x, offset_y, red);							\
	add_color(offset_x, offset_y, green);						\
	add_color(offset_x, offset_y, blue);						\
	add_number();												\
} while(0)
	
#define add_3x3_mid()											\
do {															\
	add_pixel(-1, -1); add_pixel(-1,  0); add_pixel(-1,  1);	\
	add_pixel( 0, -1); add_pixel( 0,  0); add_pixel( 0,  1);	\
	add_pixel( 1, -1); add_pixel( 1,  0); add_pixel( 1,  1);	\
} while(0)

#define add_2x2_tl()											\
do {															\
	add_pixel( 0,  0); add_pixel( 0,  1);						\
	add_pixel( 1,  0); add_pixel( 1,  1);						\
} while(0)																

#define add_2x2_tr()											\
do {															\
	add_pixel( 0, -1); add_pixel( 0,  0);						\
	add_pixel( 1, -1); add_pixel( 1,  0);						\
} while(0)

#define add_2x2_bl()											\
do {															\
	add_pixel(-1,  0); add_pixel(-1,  1);						\
	add_pixel( 0,  0); add_pixel( 0,  1);						\
} while(0)

#define add_2x2_br()											\
do {															\
	add_pixel(-1, -1); add_pixel(-1,  0);						\
	add_pixel( 0, -1); add_pixel( 0,  0);						\
} while(0)

#define add_2x3_t()												\
do {															\
	add_pixel( 0, -1); add_pixel( 0,  0); add_pixel( 0,  1);	\
	add_pixel( 1, -1); add_pixel( 1,  0); add_pixel( 1,  1);	\
} while(0)

#define add_2x3_b()												\
do {															\
	add_pixel(-1, -1); add_pixel(-1,  0); add_pixel(-1,  1);	\
	add_pixel( 0, -1); add_pixel( 0,  0); add_pixel( 0,  1);	\
} while(0)

#define add_3x2_l()												\
do {															\
	add_pixel(-1,  0); add_pixel(-1,  1);						\
	add_pixel( 0,  0); add_pixel( 0,  1);						\
	add_pixel( 1,  0); add_pixel( 1,  1);						\
} while(0)

#define add_3x2_r()												\
do {															\
	add_pixel(-1, -1); add_pixel(-1,  0);						\
	add_pixel( 0, -1); add_pixel( 0,  0);						\
	add_pixel( 1, -1); add_pixel( 1,  0);						\
} while(0)

#define sum2pixel(pixel)										\
do {															\
	pixel.red = sum.red/sum.num;								\
	pixel.green = sum.green/sum.num;							\
	pixel.blue = sum.blue/sum.num;								\
} while(0)

#define clear_sum() 											\
do {															\
	sum.red=0;													\
	sum.green=0;												\
	sum.blue=0;													\
	sum.num=0;													\
} while(0)

#define add_to_pixel(add_function, pixel)						\
do {															\
	clear_sum();												\
	add_function();												\
	sum2pixel(pixel);											\
} while(0)

/***************
 * ROTATE KERNEL
 ***************/

/******************************************************
 * Your different versions of the rotate kernel go here
 ******************************************************/
	
/* 
 * naive_rotate - The naive baseline version of rotate 
 */
char naive_rotate_descr[] = "naive_rotate: Naive baseline implementation";
void naive_rotate(int dim, pixel *src, pixel *dst) 
{
    int i, j;

    for (i = 0; i < dim; i++)
		for (j = 0; j < dim; j++)
		    dst[RIDX(dim-1-j, i, dim)] = src[RIDX(i, j, dim)];
}

/* 
 * rotate - Your current working version of rotate
 * IMPORTANT: This is the version you will be graded on
 */
char rotate_descr[] = "rotate: Current working version";
void rotate(int dim, pixel *src, pixel *dst) 
{
	/**
	 * Rotate left 90 deg.
	 *
	 * possible value for the dim:
	 * 96, 64, 128, 256, 512, 1024.
	 * 
	 * GCD of them is 32.
	 */
	for (int i = 0; i < dim; i+=level) for (int j = 0; j < dim; j++) mov32(0);
}


/*********************************************************************
 * register_rotate_functions - Register all of your different versions
 *     of the rotate kernel with the driver by calling the
 *     add_rotate_function() for each test function. When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 *********************************************************************/

void register_rotate_functions() 
{
    add_rotate_function(&naive_rotate, naive_rotate_descr);   
    add_rotate_function(&rotate, rotate_descr);   
    /* ... Register additional test functions here */
}


/***************
 * SMOOTH KERNEL
 **************/

/***************************************************************
 * Various typedefs and helper functions for the smooth function
 * You may modify these any way you like.
 **************************************************************/

/* A struct used to compute averaged pixel value */
typedef struct {
    int red;
    int green;
    int blue;
    int num;
} pixel_sum;

/* Compute min and max of two integers, respectively */
static int min(int a, int b) { return (a < b ? a : b); }
static int max(int a, int b) { return (a > b ? a : b); }

/* 
 * initialize_pixel_sum - Initializes all fields of sum to 0 
 */
static void initialize_pixel_sum(pixel_sum *sum) 
{
    sum->red = sum->green = sum->blue = 0;
    sum->num = 0;
    return;
}

/* 
 * accumulate_sum - Accumulates field values of p in corresponding 
 * fields of sum 
 */
static void accumulate_sum(pixel_sum *sum, pixel p) 
{
    sum->red += (int) p.red;
    sum->green += (int) p.green;
    sum->blue += (int) p.blue;
    sum->num++;
    return;
}

/* 
 * assign_sum_to_pixel - Computes averaged pixel value in current_pixel 
 */
static void assign_sum_to_pixel(pixel *current_pixel, pixel_sum sum) 
{
    current_pixel->red = (unsigned short) (sum.red/sum.num);
    current_pixel->green = (unsigned short) (sum.green/sum.num);
    current_pixel->blue = (unsigned short) (sum.blue/sum.num);
    return;
}

/* 
 * avg - Returns averaged pixel value at (i,j) 
 */
static pixel avg(int dim, int i, int j, pixel *src) 
{
    int ii, jj;
    pixel_sum sum;
    pixel current_pixel;

    initialize_pixel_sum(&sum);

    for(ii = max(i-1, 0); ii <= min(i+1, dim-1); ii++) 
	for(jj = max(j-1, 0); jj <= min(j+1, dim-1); jj++)
	accumulate_sum(&sum, src[RIDX(ii, jj, dim)]);
		
    assign_sum_to_pixel(&current_pixel, sum);
    
	return current_pixel;
}

/******************************************************
 * Your different versions of the smooth kernel go here
 ******************************************************/

/*
 * naive_smooth - The naive baseline version of smooth 
 */
char naive_smooth_descr[] = "naive_smooth: Naive baseline implementation";
void naive_smooth(int dim, pixel *src, pixel *dst) 
{
    int i, j;

    for (i = 0; i < dim; i++)
	for (j = 0; j < dim; j++)
	    dst[RIDX(i, j, dim)] = avg(dim, i, j, src);
}

/*
 * smooth - Your current working version of smooth. 
 * IMPORTANT: This is the version you will be graded on
 */
char smooth_descr[] = "smooth: Current working version";
void smooth(int dim, pixel *src, pixel *dst) 
{
	/**
	 * Handle corner and edge pixels before a main loop.
	 * No if.
	 */

	int 		i = 0;
	int 		j = 0;
	pixel_sum 	sum = {0, };
	pixel		current_pixel = {0, };

	// Top left corner
	i=0; j=0;
	add_to_pixel(add_2x2_tl, current_pixel);
	dst[RIDX(0, 0, dim)] = current_pixel;

	// Top right corner
	i=0; j=dim-1;
	add_to_pixel(add_2x2_tr, current_pixel);
	dst[RIDX(0, dim-1, dim)] = current_pixel;

	// Bottom left corner
	i=dim-1; j=0;
	add_to_pixel(add_2x2_bl, current_pixel);
	dst[RIDX(dim-1, 0, dim)] = current_pixel;

	// Bottom right corner
	i=dim-1; j=dim-1;
	add_to_pixel(add_2x2_br, current_pixel);
	dst[RIDX(dim-1, dim-1, dim)] = current_pixel;

	// Top edge
	i=0;
	for (j = 1; j < dim-1; ++j) {
		add_to_pixel(add_2x3_t, current_pixel);
		dst[RIDX(0, j, dim)] = current_pixel;
	}	

	// Bottom edge
	i=dim-1;
	for (j = 1; j < dim-1; ++j) {
		add_to_pixel(add_2x3_b, current_pixel);
		dst[RIDX(dim-1, j, dim)] = current_pixel;
	}

	// Left edge
	j=0;
	for (i = 1; i < dim-1; ++i) {
		add_to_pixel(add_3x2_l, current_pixel);
		dst[RIDX(i, 0, dim)] = current_pixel;
	}

	// Right edge
	j=dim-1;
	for (i = 1; i < dim-1; ++i) {
		add_to_pixel(add_3x2_r, current_pixel);
		dst[RIDX(i, dim-1, dim)] = current_pixel;
	}

	// Inside
	for (i = 1; i < dim-1; ++i) {
		for (j = 1; j < dim-1; ++j) {
			add_to_pixel(add_3x3_mid, current_pixel);
			dst[RIDX(i, j, dim)] = current_pixel;
		}
	}
}



/********************************************************************* 
 * register_smooth_functions - Register all of your different versions
 *     of the smooth kernel with the driver by calling the
 *     add_smooth_function() for each test function.  When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 *********************************************************************/

void register_smooth_functions() {
    add_smooth_function(&smooth, smooth_descr);
    add_smooth_function(&naive_smooth, naive_smooth_descr);
    /* ... Register additional test functions here */
}

