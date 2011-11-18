import threading
import inspect
import ctypes
import time

class KThread(threading.Thread):
    def kill(self): self.raiseExc(SystemExit)

    def raiseExc(self,excType):
        """Raises the given exception in the context of this thread.LockType

           If the thread is busy in a system call (time.sleep(),
           socket.acccept(), etc), the exception is simply ignored.

           If you are sure that the exception should terminate the thread,
           use raseExcWait instead, which will attempt to raise the exception
           multiple times."""
        if not inspect.isclass(excType):
            raise TypeError('Only types can be raised (not instances)')
        res = ctypes.pythonapi.PyThreadState_SetAsyncExc(
            self.ident,
            ctypes.py_object(excType),
            )
        if res == 0:
            return
            #raise ValueError('invalid thread id')
        elif res != 1:
            # If it returns a number > 1, you're in trouble,
            # and shoud call it again with exc=NULL to revert the effect
            ctypes.pythonapi.PyThreadState_SetAsyncExc(
                self.ident,
                0,
                )
            raise SystemError('PyThreadState_SetAsyncExec failed')

    def raiseExcWait(self,excType,wait=0.1):
        self.raiseExc(excType)
        while self.isAlive():
            time.sleep(wait)
            self.raiseExc(excType)