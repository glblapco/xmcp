/*gcc -o xmcp xmcp.c -L/usr/X11/lib -lX11*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/cursorfont.h>

typedef struct rgb {
  unsigned char red;
  unsigned char green;
  unsigned char blue;
} rgb;

rgb getColor(Display *dis, Window win) {
  rgb channels;

  Window root_out;
  int root_x_out;
  int root_y_out;
  Window child_out;
  int child_x_out;
  int child_y_out;
  unsigned int mask_out;
  
  XImage *image;
  XColor colors;

  XQueryPointer(dis, win, &root_out, &child_out, &root_x_out, &root_y_out, &child_x_out, &child_y_out, &mask_out);

  if (child_out != None) {
    image = XGetImage(dis, child_out, child_x_out, child_y_out, 1, 1, AllPlanes, ZPixmap);
  }
  
  else {
    image = XGetImage(dis, root_out, root_x_out, root_y_out, 1, 1, AllPlanes, ZPixmap);
  }

  colors.pixel = XGetPixel(image, 0, 0);
  
  channels.red = (colors.pixel & image->red_mask) >> 16;
  
  channels.green = (colors.pixel & image->green_mask) >> 8;

  channels.blue = colors.pixel & image->blue_mask;

  XFree(image);
 
  return(channels);
}

int main(int argc, char *argv[]) {

  Display *display = XOpenDisplay(NULL);
  Window root_win = RootWindow(display, DefaultScreen(display)); 
  Cursor cursor;
  cursor = XCreateFontCursor(display, XC_target);

  unsigned long e_mask = ButtonPressMask;
  
  int state;

  XGrabPointer(display, root_win, False, e_mask, GrabModeAsync, GrabModeAsync, None, cursor, CurrentTime);

  XGetInputFocus(display, &root_win, &state);

  XEvent event;
  XNextEvent(display, &event);

  rgb rgb = getColor(display, root_win);
    
  printf("#%02x%02x%02x\n", rgb.red, rgb.green, rgb.blue);
  
  return 0;
}
