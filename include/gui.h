#ifndef __GUI__
#define __GUI__

GtkWidget	*_mainwindow, 				// Main window.
			*_mainframe,				// Main canvas frame.
			*_hierarchy_frame,
			*_maincanvas_scroll,		// Scrolled window container for main canvas.
			*_maincanvas_fixed,
			*_maincanvas_hscrollbar,	// Horizontal scrollbar for maincanvas.
			*_maincanvas_vscrollbar,	// Vertical scrollbar for maincanvas.
			*_maincanvas_hbox,			// Contains maincanvas and vertical scrollbar.
			*_maincanvas_vbox,			// Contains maincanvashbox and horizontal scrollbar.
			*_hpane,					// Horizontal pane.
			*_hierarchy_browser_frame,	// Hierarchy browser frame.
			*_maincanvas;				// Main canvas widget.
			
GtkToolItem *_zoom_in_btn,				// Zoom in button.
			*_zoom_out_btn,				// Zoom out button.
			*_zoom_fit_btn,				// Zoom fit button.
			*_open_btn,
			*_save_btn,
			*_play_btn,
			*_next_btn,
			*_stop_btn,
			*_preferences_btn;

GtkObject	*_maincanvas_hscrollbar_adjustment, // Horizontal scrollbar adjustment.
			*_maincanvas_vscrollbar_adjustment; // Vertical scrollbar adjustment.


static cairo_t 	*_maincanvas_cs;			// Main canvas cairo state.
static GdkDrawable *_maincanvas_drawable;	// Drawable layout window of maincanvas.

int _reversey = 0, 				// Reverse y-axis flag, place origin at bottom-left.
	_maincanvas_width = 550,	// Main canvas visible dimension - width.
	_maincanvas_height = 550, 	// Main canvas visible dimension - height.
	_maincanvas_Ox = 20, 		// Main canvas visible origin translation - x offset.
	_maincanvas_Oy = 50, 		// Main canvas visible origin translation - y offset.
	_drawble_x_min = 10, 		// Drawble area x_min.
	_drawble_x_max = 510, 		// Drawble area x_max.
	_drawble_y_min = 10, 		// Drawble area y_min.
	_drawble_y_max = 510, 		// Drawble area y_max.

	_core_x_min = 0xefffffff,	// Drawble area x_min.
	_core_x_max = 0,			// Drawble area x_max.
	_core_y_min = 0xefffffff,	// Drawble area y_min.
	_core_y_max = 0,			// Drawble area y_max.

	_circuit_x_min = 0xefffffff,// Drawble area x_min.
	_circuit_x_max = 0,			// Drawble area x_max.
	_circuit_y_min = 0xefffffff,// Drawble area y_min.
	_circuit_y_max = 0;			// Drawble area y_max.

double _scale = 1.0;			// Horizontal and vertical scrollbars adjustment parameters.
bool check=true;
int _sel=0;
#define check_min_max() { 		   							\
	if(check)												\
	{														\
		_drawble_x_max = MAX((x0+width), _drawble_x_max);	\
		_drawble_y_max = MAX((y0+height), _drawble_y_max);	\
		_drawble_x_min = MIN((x0), _drawble_x_min);			\
		_drawble_y_min = MIN((y0), _drawble_y_min);			\
	}														\
}
#endif