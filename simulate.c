/******************************************************************************
*Jiacheng Yu 250799525
******************************************************************************/

#include "simulate.h"

int memory, max_memory;
FILE *fp;
linked_stack_t *jobs;

/*function to excute the job*/
void *do_job(void *ptr)
{
	job_t *job;
	sleep(1);
	while (jobs && jobs->size > 0)
	{
		job = pop(jobs);
		print_starting(fp, job->number);
		memory -= job->required_memory; 
		/*excuted once memory is allocated*/
		print_allocate_memory(fp,memory,job->required_memory);
		sleep(job->required_time);
		print_completed(fp, job->number);
		memory += job->required_memory;
		/*excuted once memory is deallocated*/
		print_deallocate_memory(fp,memory,job->required_memory);
	}
}
void simulate(int memory_value, linked_stack_t *stack)
{
	job_t *job;
	pthread_t th[NUMBER_OF_THREADS];
	max_memory = memory_value;
	memory = memory_value;
	fp = fopen(SYSTEM_OUTPUT,WRITE);
	
	while (stack && stack->size > 0) {
		/* pop a job */
		job = pop(stack);
		
		if ((job->required_memory) > max_memory) {
			/* print_exceed_memory\n */
			print_exceed_memory(fp, job->number);
		} else if ((job->required_memory) > memory) {
			
			/* print_insufficient_memory\n */
			print_insufficient_memory(fp, job->number);
			push(stack,job);
		} else {
			/* add to jobs */
			if ( jobs ) {
				push(jobs,job);
			} else {
				jobs = init_linked_stack(job);
			}
		}
	}
	/* initialize multiple threads */
	for (int i = 0; i < NUMBER_OF_THREADS; i++)
		pthread_create( &th[i], NULL, do_job, (void*)NULL);
	for (int i = 0; i < NUMBER_OF_THREADS; i++)
		pthread_join( th[i], NULL);
	fclose(fp);
}
