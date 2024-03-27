#pragma once

#if defined(WIN32) || defined(_WIN32) /* si vous êtes sous Windows */

#include <windows.h>
#include <iostream>

typedef struct {
	DWORD	tid;
	void	*(*start_routine)(void *);
	void	*arg;
	void	*result;
	HANDLE	handle;
} pthread_t;

struct sched_param { 
	int		sched_priority; 
	int		sched_curpriority; 
	int		reserved[8];  
};

typedef struct {
	int		__flags;
	size_t	__stacksize;
	void	*__stackaddr;
	void	(*__exitfunc)(void *status);
	int		__policy;
	struct sched_param  __param;
} pthread_attr_t;

inline int pthread_create(pthread_t *thread,
						const pthread_attr_t *attr,
						void *(*start_routine)(void *),
						void *arg)
{
	if(thread == NULL)
		return -1;

	thread->start_routine = start_routine;
	thread->arg = arg;
	thread->result = NULL;

	DWORD stackSize = (attr == NULL) ? 0 : attr->__stacksize;
	thread->handle = CreateThread( 
					NULL,										// Attributs de sécurité par défaut
					stackSize,									// Taille de la pile
					(LPTHREAD_START_ROUTINE)start_routine,		// Point d'entrée du thread
					arg,										// Argument à passer à ladite fonction 
					0,											// Flags de création
					&thread->tid);								// Pointeur vers la variable où écrire l'identifiant du thread

	if(thread->handle == NULL)
		return -1;

	return 0;
}

inline int pthread_join(pthread_t thread, void **retval) {
	DWORD ret = WaitForSingleObject(thread.handle, INFINITE);

	if(ret == WAIT_OBJECT_0) {
		DWORD exitCode;
		if(GetExitCodeThread(thread.handle, &exitCode)) {
			*retval = (void*) exitCode;
		}
		else {
			*retval = NULL;
		}

		CloseHandle(thread.handle);
	}

	return ret;
}

inline int pthread_cancel(pthread_t thread) {
	return !TerminateThread(thread.handle, 0);
}

#elif defined (linux) /* si vous êtes sous Linux */

#include <pthread.h>

#else /* sinon vous êtes sur une plateforme non supportée */

#error not defined for this platform

#endif