#ifndef __HEADER_DEF__
#define __HEADER_DEF__

#include <gtk/gtk.h>
#include <gdk/gdkx.h>
#include <gdk/gdkkeysyms.h>
#include <cairo.h>
#include <cairo-xlib.h>
#include <cmath>
#include <ctime>
#include <limits>
#include <string>
#include <iostream>
#include <vector>
#include <thread>

#include <memory>
#include <fstream>
#include <sstream> 
#include <cstdlib>
#include <iterator>
#include <iostream>
#include <algorithm>
#include <list>
#include <unordered_map>
#include "geometry.h"

#define str_toupper(str) {for (auto &it : str) it = toupper(it);}

#define WINDOW_H_SIZE 300
#define WINDOW_V_SIZE 300
#define COMPONENT_WIDTH  1260
#define COMPONENT_HEIGHT 576
#define CORE_X_OFF  90
#define CORE_Y_OFF  576

void start_gui();
static void _quit_action();
static void _setup_scroll_adjustments();
static void _expose(GtkWidget *, GdkEventExpose *, gpointer);
static void _maincanvas_paint(GtkWidget *, GdkEventExpose *, gpointer);
static void _draw(cairo_t *cr);
static void _resize_maincanvas(GtkWidget *, GdkRectangle *, gpointer);

static void _open_file();
static void _save_file();
static void _center(int, int, int, int, int, int, int, int);
static void _translate(int *, int *, int *, int *);
static void _fit_window();

static void _play();
static void _stop();
#endif