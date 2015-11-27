
/*
  This file is part of Aleph-w system

  Copyright (c) 2002, 2003, 2004, 2005, 2006, 2007, 2008, 2009, 2010,
                2011, 2012, 2013, 2014
  Leandro Rabindranath León
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are
  met: 

  1. Redistributions of source code must retain the above copyright 
     notice, this list of conditions and the following disclaimer.

  2. Redistributions in binary form must reproduce the above copyright 
     notice, this list of conditions and the following disclaimer in the 
     documentation and/or other materials provided with the distribution.

  3. All advertising materials mentioning features or use of this software
     must display the following acknowledgement:

     Copyright (c) 2002-2014 Leandro Rabindranath León. See details of 
     licence.     

     This product includes software developed by the Hewlett-Packard
     Company, Free Software Foundation and Silicon Graphics Computer
     Systems, Inc. 

  4. Neither the name of the ULA nor the names of its contributors may
     be used to endorse or promote products derived from this software
     without specific prior written permission. 

THIS SOFTWARE IS PROVIDED BY Leandro Rabindranath León ''AS IS'' AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

  Aleph-w is distributed in the hope that it will be useful, but WITHOUT
  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
  or FITNESS FOR A PARTICULAR PURPOSE.

  I request users of this software to return to 

  Leandro Rabindranath Leon
  CEMISID 
  Ed La Hechicera 
  3er piso, ala sur
  Facultad de Ingenieria 
  Universidad de Los Andes 
  Merida - REPÚBLICA BOLIVARIANA DE VENEZUELA    or

  leandro.r.leon@gmail.com

  any improvements or extensions that they make and grant me the rights
  to redistribute these changes.  
*/


# include <errno.h>
# include <string.h>
# include <typeinfo>
# include <timeoutQueue.H>


int TimeoutQueue::instanceCounter = 0;


TimeoutQueue::TimeoutQueue() : isShutdown(false)
{
  if (instanceCounter >= 1)
    EXIT("Double instantiation (%d) of TimeoutQueue", instanceCounter);

  ++instanceCounter;

  init_mutex(mutex);

  pthread_cond_init(&cond, NULL);

  const int status = 
    pthread_create(&threadId, NULL , triggerEventThread, this); 

  if (status != 0)
    EXIT("Cannot create triggerEventThread (error code = %d)", status);
}


TimeoutQueue::~TimeoutQueue()
{
  if (not isShutdown)
    EXIT("TimeoutQueue is not shut down");

  destroy_mutex(mutex);

  pthread_cond_destroy(&cond);
}


void TimeoutQueue::schedule_event(const Time &          trigger_time,
				  TimeoutQueue::Event * event) 
{
  assert(event != NULL);

  event->set_trigger_time(trigger_time);
  schedule_event(event);
}


void TimeoutQueue::schedule_event(TimeoutQueue::Event * event) 
{
  assert(event != NULL);
  assert(EVENT_NSEC(event) >= 0 and EVENT_NSEC(event) < NSEC);

  CRITICAL_SECTION(mutex);

  if (event->get_execution_status() == Event::In_Queue)
    throw std::invalid_argument("Event has already inserted in timemeQueue");

  if (isShutdown)
    return;

  event->set_execution_status(Event::In_Queue);

  prioQueue.insert(event);

  pthread_cond_signal(&cond);
}


bool TimeoutQueue::cancel_event(TimeoutQueue::Event* event) 
{
  CRITICAL_SECTION(mutex);

  if (event->get_execution_status() != Event::In_Queue)
    return false;

  event->set_execution_status(Event::Canceled);

  prioQueue.remove(event);

  pthread_cond_signal(&cond);

  return true;
}


void TimeoutQueue::cancel_delete_event(Event *& event)
{
  if (event == NULL)
    return;

  CRITICAL_SECTION(mutex);

  if (event->get_execution_status() == Event::In_Queue)
    prioQueue.remove(event);

  if (event->get_execution_status() == Event::Executing) 
    event->set_execution_status(Event::To_Delete);
  else
    {
      event->set_execution_status(Event::Deleted);
      delete event;
    }

  event = NULL;
  pthread_cond_signal(&cond);
}


void TimeoutQueue::reschedule_event(const Time &          trigger_time,
				    TimeoutQueue::Event * event) 
{
  assert(event != NULL);

  CRITICAL_SECTION(mutex);
  
  if (event->get_execution_status() == Event::In_Queue)
    prioQueue.remove(event);

  event->set_trigger_time(trigger_time);

  event->set_execution_status(Event::In_Queue);

  prioQueue.insert(event);
  
  pthread_cond_signal(&cond);
}


void * TimeoutQueue::triggerEvent()
{
  Event *event_to_schedule;
  Event *event_to_execute;
  int status = 0;

  {
    CRITICAL_SECTION(mutex);

    while (true)
      {
	    /* sleep if there is no events */ 
	while ((prioQueue.size() == 0) and (not isShutdown))
	  pthread_cond_wait(&cond, &mutex);

	if (isShutdown)
	  goto end; /* if shutdown is activated, get out */

	    /* read the soonest event */ 
	event_to_schedule = static_cast<Event*>(prioQueue.top());
	    
	    /* compute time when the event must triggered */ 
	const Time & t = EVENT_TIME(event_to_schedule);
	
	do
	  { /* sleep during t units of time, but be immune to signals
	       interruptions (status will be EINTR in the case where the
	       thread was signalized) */
	    status = pthread_cond_timedwait(&cond, &mutex, &t);

	    if (isShutdown) 
	      goto end;/* thread was signaled because shutdown was requested */

	  } while (status == EINTR);

	if (status == ETIMEDOUT) /* soonest event could be executed */
	  {     /* event to execute could be changed if it was canceled */
	    event_to_execute = static_cast<Event*>(prioQueue.getMin());

	    if (event_to_execute != event_to_schedule and
		EVENT_TIME(event_to_execute) > EVENT_TIME(event_to_schedule))
	      continue; /* go to schedule another event */
	    
	    event_to_execute->set_execution_status(Event::Executing); 

	    critical_section.leave(); 

	    try { event_to_execute->EventFct(); } /* execute event */
	    catch (...) { /* Exceptions are only cauthg */ }

	    critical_section.enter();  

	    if (event_to_execute->get_execution_status() == Event::To_Delete)
	      { 
		event_to_execute->set_execution_status(Event::Deleted); 

		delete event_to_execute; 
	      } 

	    /* from this point, event cannot be longer acceded because
	       it may delete itself */ 
	  } /* end if (status == ETIMEDOUT) */
      } /* end while (1) */

  end: /* shutdown has been requested */ 

        /* extract all events from priority queue */ 
    while (prioQueue.size() > 0)
      static_cast<Event*>(prioQueue.getMin())->
	set_execution_status(Event::Canceled);
  } /* end of critical section */

  pthread_exit(NULL);
}


void TimeoutQueue::shutdown()
{
  CRITICAL_SECTION(mutex);

  isShutdown = true;

  pthread_cond_signal(&cond);
}


void* TimeoutQueue::triggerEventThread(void *obj)
{
  TimeoutQueue *timeoutQueue = static_cast<TimeoutQueue*>(obj);

  return timeoutQueue->triggerEvent();
}

