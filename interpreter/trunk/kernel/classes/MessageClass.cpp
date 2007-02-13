/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 1995, 2004 IBM Corporation. All rights reserved.             */
/* Copyright (c) 2005-2006 Rexx Language Association. All rights reserved.    */
/*                                                                            */
/* This program and the accompanying materials are made available under       */
/* the terms of the Common Public License v1.0 which accompanies this         */
/* distribution. A copy is also available at the following address:           */
/* http://www.ibm.com/developerworks/oss/CPLv1.0.htm                          */
/*                                                                            */
/* Redistribution and use in source and binary forms, with or                 */
/* without modification, are permitted provided that the following            */
/* conditions are met:                                                        */
/*                                                                            */
/* Redistributions of source code must retain the above copyright             */
/* notice, this list of conditions and the following disclaimer.              */
/* Redistributions in binary form must reproduce the above copyright          */
/* notice, this list of conditions and the following disclaimer in            */
/* the documentation and/or other materials provided with the distribution.   */
/*                                                                            */
/* Neither the name of Rexx Language Association nor the names                */
/* of its contributors may be used to endorse or promote products             */
/* derived from this software without specific prior written permission.      */
/*                                                                            */
/* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS        */
/* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT          */
/* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS          */
/* FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT   */
/* OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,      */
/* SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED   */
/* TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,        */
/* OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY     */
/* OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING    */
/* NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS         */
/* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.               */
/*                                                                            */
/*----------------------------------------------------------------------------*/
/******************************************************************************/
/* REXX Kernel                                                  MessageClass.c       */
/*                                                                            */
/* Primitive Message Class                                                    */
/*                                                                            */
/******************************************************************************/
#include "RexxCore.h"
#include "StringClass.hpp"
#include "DirectoryClass.hpp"
#include "ArrayClass.hpp"
#include "ListClass.hpp"
#include "MethodClass.hpp"
#include "RexxActivity.hpp"
#include "RexxActivation.hpp"
#include "RexxNativeActivation.hpp"
#include "MessageClass.hpp"

#include "oorexx.h"


RexxClass *RexxMessage::classInstance= OREF_NULL;    // the singleton class instance

RexxMethod1(int, nativeMessageRoot, OSELF, self)
{
    context->SendMessage0((RexxObjectPtr)self, "SEND");
    return 0;
}


RexxMessage::RexxMessage(
    RexxObject *target,                /* message target                    */
    RexxObject *message,               /* message to issue                  */
    RexxArray *args)                   /* array of message arguments        */
/******************************************************************************/
/* Function:  Initialize a message object                                     */
/******************************************************************************/
{
  ClearObject(this);                   /* Start out with everythign 0.      */
  this->hashvalue = HASHOREF(this);
                                       /* defult target is target specified */
  OrefSet(this, this->receiver, target);
  OrefSet(this, this->target, target); /* Target specified on new           */
                                       /* Args to be sent wuth tmessage     */
  OrefSet(this, this->args, args);
                                       /* initialize a list of message to be*/
                                       /* once we have a result.            */
  OrefSet(this, this->interestedParties, new RexxList);

  if (isArray(message)) {         /* is message specified as an array? */
    OrefSet(this, this->message, ((RexxString *)((RexxArray *)message)->get(1))->upper());
                                       /* starting lookup scope is ourself. */
    OrefSet(this, this->startscope, (RexxClass *)((RexxArray *)message)->get(2));
  }
  else {                               /* not an array as message.          */
                                       /* Message to be sent.               */
    OrefSet(this, this->message, ((RexxString *)message)->upper());
                                       /* starting lookup scope is ourself. */
    OrefSet(this, this->startscope, (RexxClass *)TheNilObject);
  }
}

void RexxMessage::live(void)
/******************************************************************************/
/* Function:  Normal garbage collection live marking                          */
/******************************************************************************/
{
  setUpMemoryMark
  memory_mark(this->receiver);
  memory_mark(this->target);
  memory_mark(this->message);
  memory_mark(this->startscope);
  memory_mark(this->args);
  memory_mark(this->resultObject);
  memory_mark(this->interestedParties);
  memory_mark(this->condition);
  memory_mark(this->startActivity);
  memory_mark(this->objectVariables);
  memory_mark(this->waitingActivities);
  cleanUpMemoryMark
}

void RexxMessage::liveGeneral(void)
/******************************************************************************/
/* Function:  Generalized object marking                                      */
/******************************************************************************/
{
  setUpMemoryMarkGeneral
  memory_mark_general(this->receiver);
  memory_mark_general(this->target);
  memory_mark_general(this->message);
  memory_mark_general(this->startscope);
  memory_mark_general(this->args);
  memory_mark_general(this->resultObject);
  memory_mark_general(this->interestedParties);
  memory_mark_general(this->condition);
  memory_mark_general(this->startActivity);
  memory_mark_general(this->objectVariables);
  memory_mark_general(this->waitingActivities);
  cleanUpMemoryMarkGeneral
}

void RexxMessage::flatten(RexxEnvelope *envelope)
/******************************************************************************/
/* Function:  Flatten an object                                               */
/******************************************************************************/
{
  setUpFlatten(RexxMessage)

   flatten_reference(newThis->receiver, envelope);
   flatten_reference(newThis->target, envelope);
   flatten_reference(newThis->message, envelope);
   flatten_reference(newThis->startscope, envelope);
   flatten_reference(newThis->args, envelope);
   flatten_reference(newThis->resultObject, envelope);
   flatten_reference(newThis->interestedParties, envelope);
   flatten_reference(newThis->condition, envelope);
   flatten_reference(newThis->startActivity, envelope);
   flatten_reference(newThis->objectVariables, envelope);
   flatten_reference(newThis->waitingActivities, envelope);

  cleanUpFlatten
}

RexxObject *RexxMessage::notify(RexxMessage *message)
/******************************************************************************/
/* Function:  Add a message object to the notification list                   */
/******************************************************************************/
{
                                       /* is argument a real message object?*/
 if (message != OREF_NULL && isOfClass(Message, message)) {
                                       /* Yes, then add it to the           */
                                       /* toBeNotified list.                */

  if (this->allNotified()) {           /* Have all notifications been sent? */
                                       /* Yes, then send notification right */
    message->send(OREF_NULL);          /* away                              */
  }
  else {
                                       /* nope, add it to list and wait for */
                                       /*  for result.                      */
    this->interestedParties->addLast(message);
  }
 }
 else {                                /* nope, its and error, report it.   */
   if ( message == OREF_NULL)
     reportException(Error_Incorrect_method_noarg, IntegerOne);
   else
     reportException(Error_Incorrect_method_nomessage, message);
 }
 return OREF_NULL;                     /* all done, we return nothing       */
}

RexxObject *RexxMessage::result(void)
/******************************************************************************/
/* Function:  Return the message result...will wait if the message isn't done */
/******************************************************************************/
{

    // get the pointer to the current activity
    RexxActivity *activity = RexxActivityManager::currentActivity;
    /* Did send/satrt cause an error     */
    /*condition Yes, we need to raise it */
    /*here.                              */
    if (this->raiseError())
    {
        activity->reraiseException(this->condition);
    }
    else
    {
        /* Quick test to see if result       */
        /*already present                    */
        if (!this->resultReturned())
        {
            /* got an activity available?        */
            if (this->startActivity != OREF_NULL)
                /* go perform dead lock checks       */
                this->startActivity->checkDeadLock(activity);

            /* No result yet, now we need to wait*/
            /*  until we get a result.           */
            /* Is anyone else waiting ????       */
            if (this->waitingActivities == OREF_NULL)
                /* No, Create a waiting list         */
                OrefSet(this, this->waitingActivities, new_list());
            /* add this activity to the list     */
            this->waitingActivities->addLast((RexxObject *)activity);
            /* now go wait to be woken up        */
            activity->waitReserve((RexxObject *)this);
            if (this->raiseError())
            {           /* do we need to raise an error.     */
                        /* yes,                              */
                this->setErrorReported();        /* indicate error was reported, and  */
                                                 /*  report and error.                */
                activity->reraiseException(this->condition);
            }
        }
    }
    return this->resultObject;            /* ok, return the result.            */
}

RexxObject *RexxMessage::send(RexxObject *receiver)
/******************************************************************************/
/* Function:  Send the message contained by this message object               */
/******************************************************************************/
{
  RexxActivity *myActivity;            /* Activity I am running under       */
                                       /* has message already been sent to  */
                                       /* the receiver?                     */
  if (this->msgSent())
                                       /* Yes, this is an error             */
    reportException(Error_Execution_message_reuse);

                                       /* get the activity I'm running under*/
  myActivity = RexxActivityManager::currentActivity;
                                       /* If we have a pending start message*/
                                       /*  sure this send is a result of    */
                                       /*that message dispatch.             */
  if (this->startPending() && myActivity != this->startActivity )
                                       /* Yes, this is an error             */
    reportException(Error_Execution_message_reuse);
  this->setMsgSent();                  /* indicate we were sent a message   */

  if (receiver != OREF_NULL) {         /* new receiver specified?           */
                                       /* Yes, indicate this is the receiver*/
    OrefSet(this, this->receiver, receiver);
  }
                                       /* validate startscope               */
  if (!this->receiver->behaviour->checkScope(this->startscope)) {
    reportException(Error_Incorrect_method_array_noclass, IntegerTwo);
  }
                                       /*  this is a primitive object?      */
                                       /* tell the activation/nativeact, we */
                                       /*are running under to let us know   */
                                       /*if an error occured.               */
  myActivity->setErrorListener(this);
                                       /* set this for resource deadlock    */
                                       /* checking purposes                 */
  OrefSet(this, this->startActivity, myActivity);
                                       /*  call message_send to do the send */
  ProtectedObject result;
                                       /* and assign our result.            */
  if (this->startscope != TheNilObject)/* have a starting scope?            */
  {
      receiver->messageSend(myActivity, this->message, this->args->size(), (RexxObject **)this->args->data(), this->startscope, result);
  }
                                       /* send it with an override          */
  else                                 /* no over ride                      */
  {
      receiver->messageSend(myActivity, this->message, this->args->size(), (RexxObject **)this->args->data(), result);
  }
  OrefSet(this, resultObject, (RexxObject *)result);
  this->setResultReturned();           /* Indicate we have a result.        */
  this->sendNotification();
  return this->resultObject;           /* return the result of the send.    */
}

RexxObject *RexxMessage::start(RexxObject *receiver)
/******************************************************************************/
/* Function:  Since a start is to happen Async, we create a new activity      */
/*            and Native Method, which will invoke the message_nstart method  */
/*            (located in OKNMSG.C) which simple does a send to his message   */
/*            object on the new activity.                                     */
/******************************************************************************/
{
  RexxMethod *newNMethod;              /* NMethod of messaeg_nstart method  */
  RexxActivity *newActivity;           /* Activity the start will be run on */
  RexxActivity *oldActivity;           /* Currently executing activity      */

#ifdef NOTHREADSUPPORT
   reportException(Execution_no_concurrency,
                  new_string("Concurrency not supported"));
#else

                                       /* has message already been sent or  */
                                       /* is another start message pending? */
  if (this->msgSent() || this->startPending())
                                       /* Yes, this is an error             */
    reportException(Error_Execution_message_reuse);
                                       /* indicate object has received a    */
                                       /*start we need this additional bit  */
                                       /*so that the send message will      */
                                       /*accept this msg                    */
  this->setStartPending();


  if (receiver != OREF_NULL) {         /* new receiver specified?           */
                                       /* Yes, indicate this is the receiver*/
   OrefSet(this, this->receiver, receiver);
  }
                                       /* create a native method object     */
  newNMethod = RexxMethod::newEntry((PFN)nativeMessageRoot);
                                       /* get the current activity          */
  oldActivity = RexxActivityManager::currentActivity;

                                       /* Create the new activity           */
  newActivity = new_activity(oldActivity);
                                       /* indicate the activity the send    */
                                       /*message should come in on.         */
  OrefSet(this, this->startActivity, newActivity);

  // have the activity notify us in case there is an error
  newActivity->setErrorListener(this);
                                       /* indicate we want the NativeAct to */
  newActivity->run();                  /*run                                */
#endif                                 // end of NOTHREADSUPPORT
  return OREF_NULL;                    /* all done here, return to caller.  */
}

void RexxMessage::sendNotification(void)
/******************************************************************************/
/* Function : we now have a result from message send/start, so notify         */
/*   all interested parties, and post the waitResult semopohore if it exists  */
/******************************************************************************/
{
  RexxObject *listIndex;               /* index of the list as we traverse  */
  RexxMessage *thisMessage;            /* Message object to noitfy          */
  RexxActivity *waitingActivity;       /* activity to notify                */
  size_t      i;                       /* loop index                        */

                                       /* no longer care about any error    */
                                       /*condition                          */
  RexxActivityManager::currentActivity->setErrorListener(OREF_NULL);
                                       /* others waiting for a result?      */
  if (this->waitingActivities != OREF_NULL) {
    i = this->waitingActivities->count;/* get the waiting count             */
    while (i--) {                      /* while we have items               */
                                       /* get the first item                */
      waitingActivity = (RexxActivity *)this->waitingActivities->removeFirst();
      waitingActivity->postRelease();  /* go wake it up                     */
    }
  }
                                       /* now traverse the list of Iterested*/
                                       /*  parties, and let them know we    */
                                       /*have a result                      */
  for (listIndex = this->interestedParties->firstRexx() ;
       listIndex != TheNilObject ;
       listIndex = this->interestedParties->next(listIndex) ) {
                                       /* Get the next message object to    */
                                       /*process                            */
    thisMessage = (RexxMessage *)this->interestedParties->value(listIndex);
                                       /* now just have this message send   */
                                       /*its message                        */
    thisMessage->send(OREF_NULL);
  }

                                       /* indicate we notified all          */
                                       /*Interested parties.  Not used      */
                                       /*yet....                            */
  this->setAllNotified();
}


void RexxMessage::error(
    RexxDirectory *condition)          /* error condition object            */
/******************************************************************************/
/* Function : called from nativaAct/Activation to notify us that the message  */
/*   from SEND/START.                                                         */
/******************************************************************************/
{
  this->setRaiseError();               /* indicate we had an error condition*/
                                       /* save the condition object in case */
                                       /*we want it.                        */
  OrefSet(this, this->condition, condition);
  this->sendNotification();            /* do cleanup items.                 */
}

RexxObject *RexxMessage::completed(void)
/******************************************************************************/
/* Function:  Give a completed polling status                                 */
/******************************************************************************/
{
                                       /* Test to see if result already     */
                                       /*present or error occured in send?  */
  if (this->resultReturned() || this->raiseError())
   return (RexxObject *)TheTrueObject; /* Yes, return true                  */
  else
   return (RexxObject *)TheFalseObject;/* nope return false.                */
}


/**
 * Check to see if a message has an error condition.  Return false
 * if the message has not completed yet.
 *
 * @return True if the message has terminated with an error condition,
 *         false if it is still running or has completed without error.
 */
RexxObject *RexxMessage::hasError()
{
    if (this->raiseError())
    {
        return TheTrueObject;
    }
    else
    {
        return TheFalseObject;
    }
}

/**
 * Return any error condition information associated with the
 * message.  This method will not block until completion, and
 * will return .nil if the message is still running.
 *
 * @return Any condition object from a terminating error, or .nil if
 *         there was no error or the message is still running.
 */
RexxObject *RexxMessage::errorCondition()
{
    if (this->condition == OREF_NULL)
    {
        return TheNilObject;
    }
    else
    {
        return this->condition;
    }

}

void *RexxMessage::operator new(size_t size)
/******************************************************************************/
/* Function:  Construct a new message object                                  */
/******************************************************************************/
{
    return new_object(size, TheMessageBehaviour);
}

RexxObject *RexxMessage::newRexx(
    RexxObject **arguments,            /* message argument array            */
    size_t       argCount)             /* the number of arguments           */
/******************************************************************************/
/* Function:  Rexx level new routine                                          */
/******************************************************************************/
{
  RexxMessage *newMessage;             /* actual message to be sent.        */
  RexxObject *target;                  /* object to receive message.        */
  RexxObject *message;                 /* message to be sent to receiver.   */
  RexxArray *args;                     /* arguments to be sent with message */
                                       /* the option parameter as a string  */
  RexxString *optionString;
  RexxActivation *activation;          /* current activation                */
  RexxObject *sender;                  /* sending object                    */
  RexxObject *msgName;                 /* msgname to be sent                */
  RexxArray  *msgNameArray;            /* msgname to be sent                */
  RexxString *msgNameStr;              /* msgname as a string object        */
  char option;                         /* how are the args passed.          */
  size_t num_args;                     /* number of args passed.            */

  num_args = argCount;                 /* get number of args passed         */

  if (num_args < 2 ) {                 /* passed less than 2 args?          */
                                       /* Yes, this is an error.            */
    reportException(Error_Incorrect_method_minarg,  IntegerTwo);
  }
  target   = arguments[0];             /* Get the receiver object           */
  if (target == OREF_NULL)             /* no receiver?                      */
                                       /* this is an error                  */
    reportException(Error_Incorrect_method_noarg, IntegerOne);
  message  = arguments[1];             /* get the message .                 */

                                       /* see if this is an array item      */
  msgNameArray = message->requestArray();
  if (msgNameArray != TheNilObject) {  /* is message specified as an array? */
    if (msgNameArray->getDimension() != 1 || msgNameArray->size() != 2)
                                       /* raise an error                    */
      reportException(Error_Incorrect_method_message);
                                       /* Was message name omitted?         */
    msgName = msgNameArray->get(1);
    if (msgName == OREF_NULL)
                                       /* Yes, this is an error, report it. */
      reportException(Error_Incorrect_method_noarg, IntegerOne);

    msgNameStr = (RexxString *)msgName->makeString();
    if (msgNameStr == TheNilObject)    /* got back .nil?                    */
                                       /* raise an error                    */
      reportException(Error_Incorrect_method_array_nostring, IntegerOne);
                                       /* Was starting scope omitted ?      */
    if (OREF_NULL == msgNameArray->get(2))
                                       /* Yes, this is an error, report it. */
      reportException(Error_Incorrect_method_noarg, IntegerTwo);
                                       /* get the top activation            */
    activation = RexxActivityManager::currentActivity->getCurrentContext();
                                       /* have an activation?               */
    if (activation != OREF_NULL) {
                                       /* get the receiving object          */
      sender = (RexxObject *)activation->getReceiver();
      if (sender != target)            /* not the same receiver?            */
                                       /* this is an error                  */
         reportException(Error_Execution_super);
    }
    else
                                       /* this is an error                  */
      reportException(Error_Execution_super);
    message = msgNameArray;            /* Message to be sent.               */
  }
  else {                               /* not an array as message.          */
                                       /* force to a string value           */
    message = REQUIRED_STRING(message, ARG_TWO);
                                       /* Message to be sent.               */
  }

                                       /* are there arguments to be sent    */
                                       /*with the message?                  */
  if (num_args > 2 ) {
                                       /* get 3rd arg only concerned w/ 1st */
    optionString = (RexxString *)arguments[2];
                                       /*  Did we really get an option      */
                                       /*passed?                            */
    if (optionString == OREF_NULL) {
                                       /* nope, use null array as argument  */
      args = (RexxArray *)TheNullArray->copy();
    }
    else {
                                       /* Convert it into a string.         */
      optionString = REQUIRED_STRING(optionString, ARG_THREE);
                                       /*  char and make it lower case      */
      option = (char)tolower(optionString->getStringData()[0]);
      if (option == 'a') {             /* args passed as an array?          */
                                       /* are there less than 4 required    */
                                       /*args?                              */
        if (num_args < 4)              /* this is an error                  */
          reportException(Error_Incorrect_method_minarg, IntegerFour);

                                       /* are there more than 4 required    */
                                       /*args?                              */
        if (num_args > 4)              /* this is an error                  */
          reportException(Error_Incorrect_method_maxarg, IntegerFour);

                                       /* get the array of arguments        */
        args = (RexxArray *)arguments[3];
        if (args == OREF_NULL)         /* no array given?                   */
                                       /* this is an error                  */
          reportException(Error_Incorrect_method_noarg, IntegerFour);
                                       /* force to array form               */
        args = args->requestArray();
                                       /* not an array?                     */
        if (args == TheNilObject || args->getDimension() != 1)
                                       /* raise an error                    */
          reportException(Error_Incorrect_method_noarray, arguments[3]);
      }
      else if (option == 'i' ){        /* specified as individual?          */
                                       /* yes, build array of all arguments */
        args = new (argCount - 3, arguments + 3) RexxArray;
      }
      else {
        reportException(Error_Incorrect_method_option, (stringchar_t *)"AI", arguments[2]);
      }
    }
  }
  else {
                                       /* no args, use a null array.        */
    args = (RexxArray *)TheNullArray->copy();
  }
                                       /* all args are parcelled out, go    */
                                       /*create the new message object...   */
  newMessage = new RexxMessage(target, message, args);
                                       /* actually a subclassed item?       */
  if (((RexxClass *)this)->isPrimitive()){
                                       /* Give new object its behaviour     */
    BehaviourSet(newMessage, ((RexxClass *)this)->instanceBehaviour);
    newMessage->sendMessage(RexxActivityManager::currentActivity, OREF_INIT);/* call any rexx inits               */
  }
  return newMessage;                   /* return the new message            */
}

