#include "selectwindow_linux.h"

#include <QDebug>

#include <sys/types.h> // pid_t

#include <X11/Xlib.h>
#include <X11/Xatom.h>  // XInternAtom()
#include <X11/cursorfont.h> // XC_crosshair

#include <X11/Xmu/WinUtil.h>  // apt-get install libxmu-dev

#include <stdio.h>
#include <stdlib.h>

int bad_window_handler(Display*, XErrorEvent *err)
{    
    qDebug()<< QObject::tr("No such window with id %1.")
               .arg(err->resourceid);
    return 0;
}

Window selectWindow(Display *dpy,int screen,QString& error) // Window Select_Window(dpy)
{
  Window root = RootWindow(dpy,screen);

  /* Make the target cursor */
  Cursor  cursor = XCreateFontCursor(dpy, XC_crosshair);

  /* Grab the pointer using target cursor, letting it room all over */
  int status = XGrabPointer(dpy, root, False, ButtonPressMask|ButtonReleaseMask, GrabModeSync, GrabModeAsync, root, cursor, CurrentTime);
  if (status != GrabSuccess)
  {
    error= QObject::tr("Can not grab pointer.");
    return None;
  }

  /* Let the user select a window... */
  int buttons = 0;
  XEvent event;
  Window target_win = None;

  while ((target_win == None) || (buttons != 0))
  {
    /* allow one more event */
    XAllowEvents(dpy, SyncPointer, CurrentTime);
    XWindowEvent(dpy, root, ButtonPressMask|ButtonReleaseMask, &event);
    switch (event.type)
    {
    case ButtonPress:
      if(target_win == None)
      {
        target_win = event.xbutton.subwindow; /* window selected */
        if(target_win == None)
            target_win = root;
      }
      buttons++;
      break;
    case ButtonRelease:
      if (buttons > 0) /* there may have been some down before we started */
    buttons--;
       break;
    }
  }
  XUngrabPointer(dpy, CurrentTime);      /* Done with pointer */
  error.clear();
  return target_win;
}


pid_t getPidByWindow(Display* display, Window window, QString& error)
{
  Atom prop = XInternAtom(display,"_NET_WM_PID",True); // http://tronche.com/gui/x/xlib/window-information/XInternAtom.html
  Atom type;

  QString errorFormat= QObject::tr("XInternAtom() return %1");
  switch(prop)
  {
    case None:     { error= errorFormat.arg("None");     return -1; }
    case BadAlloc: { error= errorFormat.arg("BadAlloc"); return -1; }
    case BadValue: { error= errorFormat.arg("BadValue"); return -1; }
    default:/* ok */ break;
  }

  int form= 0;
  unsigned long remain= 0, len= 0;
  unsigned char *propPId= 0;

  errorFormat=
    QObject::tr("XGetWindowProperty() return %1");
  int result=
    XGetWindowProperty(display,  window, prop,  // http://tronche.com/gui/x/xlib/window-information/XGetWindowProperty.html
                       0, 1024,  False,  AnyPropertyType,
                       &type, &form, &len, &remain, &propPId);
  switch(result)
  {
    case Success:  break;
    case BadAtom:  { error= errorFormat.arg("BadAtom"); return -1; }
    case BadValue: { error= errorFormat.arg("BadValue");return -1; }
    case BadWindow:{ error= errorFormat.arg("BadWindow"); return -1; }
    default:       { error= errorFormat.arg("unknown value"); return -1; }
  }

  pid_t pid= *((pid_t*)propPId);
  XFree(propPId);
  error.clear();
  return pid;
}

pid_t pidBySelectedWindow(QString& error)
{
   Display* display = XOpenDisplay(0); // http://tronche.com/gui/x/xlib/display/opening.html
   if(!display)
   {
     error= QObject::tr("Can not open display.");
     return -1;
   }

   int screen = XDefaultScreen(display);
   Window window= selectWindow(display,screen,error);
   if(window)
   {
       Window rootWindow;
       int dummyi;
       unsigned int dummy;

       if(XGetGeometry(display, window, &rootWindow,
                       &dummyi, &dummyi,
                       &dummy,  &dummy,
                       &dummy,  &dummy) &&
          window != rootWindow)

       window = XmuClientWindow(display, window);
   }

   { // ???
     Window rootWindow;
     int x, y;
     unsigned width, height, bw, depth;
     XErrorHandler old_handler;

     old_handler = XSetErrorHandler(bad_window_handler);
     XGetGeometry(display, window, &rootWindow, &x, &y, &width, &height, &bw, &depth);
     XSync(display, False);
     (void)XSetErrorHandler(old_handler);
   }

   pid_t pid= getPidByWindow(display, window,error);
   XCloseDisplay(display);
   return pid;
}

