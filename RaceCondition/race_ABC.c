#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>


struct arg_stc
{
	int id;
	int count;
	int *src;
	int *dst;
	int *location;
};

void *worker_thread(void *arg)
{
	struct arg_stc *my_arg;
	int temp;
	int i;
	int id;
	int k;
	int loc;

	my_arg = (struct arg_stc *)arg;
	id = my_arg->id;
	for(i = 0; i < my_arg->count; i++)
	{
	  	loc = *(my_arg->location);
		temp = *(my_arg->src);
		fprintf(stdout,"thread %d -- read %d, iteration = %d\n",
			id,temp,loc);
		fflush(stdout);
		temp = temp + 1;
		*(my_arg->dst) = temp;
		fprintf(stdout,"thread %d -- wrote %d, iteration = %d\n",
			id,temp,loc);
		fflush(stdout);
		my_arg[0].location[0]=loc+1;
		
		for(k=0; k < 80000; k++);	/* delay loop */

	}

	return(NULL);
}

int main(int argc, char *argv[])
{
	int A;
	int B;
	int C;
	struct arg_stc thread_args[3];
	pthread_t ids[3];
	int err;
	void *status;
	int i;
	int count;


	int loc0=0;
	int loc1=0;
	int loc2=0;
	
	count = 10;

	if(argc > 1)
	{
		count = atoi(argv[1]);
	}

	/*
	 * args for thread 0
	 */
	thread_args[0].id = 0;
	thread_args[0].count = count;
	thread_args[0].src = &A;
	thread_args[0].dst = &B;
	thread_args[0].location = &loc0;

	/*
	 * args for thread 1
	 */
	thread_args[1].id = 1;
	thread_args[1].count = count;
	thread_args[1].src = &B;
	thread_args[1].dst = &C;
	thread_args[1].location = &loc1;

	/*
	 * args for thread 2
	 */
	thread_args[2].id = 2;
	thread_args[2].count = count;
	thread_args[2].src = &C;
	thread_args[2].dst = &A;
	thread_args[2].location = &loc2;

	/*
	 * init A, B, abd C
	 */

	A = 0;
	B = 0;
	C = 0;

	/*
	 * launch the threads
	 */
	err = pthread_create(&ids[0],
		             NULL,
			     worker_thread,
			     (void *)&thread_args[0]);
	if(err != 0)
	{
		fprintf(stderr,"create err\n");
		exit(1);
	}
	err = pthread_create(&ids[1],
		             NULL,
			     worker_thread,
			     (void *)&thread_args[1]);
	if(err != 0)
	{
		fprintf(stderr,"create err\n");
		exit(1);
	}

	err = pthread_create(&ids[2],
		             NULL,
			     worker_thread,
			     (void *)&thread_args[2]);
	if(err != 0)
	{
		fprintf(stderr,"create err\n");
		exit(1);
	}

	/*
	 * reap the threads
	 */
	for(i=0; i < 3; i++)
	{
		err = pthread_join(ids[i],&status);
		if(err != 0)
		{
			fprintf(stderr,"join err\n");
			exit(1);
		}
	}

	return(0);
}
	

	

	

