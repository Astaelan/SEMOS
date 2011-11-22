ENTRY (Boot)

SECTIONS
{
    . = 0x00100000;

	KernelStart = .;

	.text ALIGN (0x1000) :
	{
	    *(.text)
	}

	.rodata ALIGN (0x1000) :
	{
	    *(.rodata*)
	}

	.data ALIGN (0x1000) :
	{
	    *(.data)
	}

	.bss :
	{
	    sbss = .;
		*(COMMON)
		*(.bss)
		ebss = .;
	}

	KernelEnd = .;

	_Dmain = .;
	_end = .;
	write = .;
	pthread_detach = .;
	__emutls_v._tlsend = .;
	__emutls_v._tlsstart = .;
	pthread_getspecific = .;
	sigfillset = .;
	sigdelset = .;
	sem_post = .;
	sigsuspend = .;
	pthread_self = .;
	pthread_getschedparam = .;
	sched_get_priority_min = .;
	sched_get_priority_max = .;
	pthread_kill = .;
	sem_wait = .;
	pthread_attr_init = .;
	pthread_attr_setstacksize = .;
	pthread_attr_setdetachstate = .;
	pthread_create = .;
	sched_yield = .;
	pthread_join = .;
	pthread_setspecific = .;
	pthread_cleanup_push = .;
	pthread_cleanup_pop = .;
	sigaction = .;
	sem_init = .;
	pthread_key_create = .;
	pthread_setschedparam = .;
	getErrno = .;
	nanosleep = .;
	clock_getres = .;
	clock_gettime = .;
	pthread_mutex_trylock = .;
	pthread_mutex_destroy = .;
	pthread_mutexattr_init = .;
	pthread_mutexattr_settype = .;
	pthread_mutexattr_destroy = .;
	pthread_mutex_init = .;
	pthread_mutex_unlock = .;
	pthread_mutex_lock = .;
	__libc_stack_end = .;
	etext = .;
	_exit = .;
	kill = .;
	getpid = .;
}