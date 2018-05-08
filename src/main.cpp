#if 0
std::vector <Component*> ComponentVector;
std::vector <Pin*> PinVector;
std::unordered_map < std::string, int > ComponentHash, PinHash;

#if 0
gint main(int argc, char const *argv[])
{
	start_gui();
	return EXIT_SUCCESS;
}
#endif
void start_gui()
{
//
// Local Variables
	GtkWidget 	*_maincanvas_hbox,		// Contains maincanvas and vertical scrollbar.
				*_maincanvas_vbox,		// Contains maincanvashbox and horizontal scrollbar.
				*_mainwindow_vbox,		// Contains toolbar and the horizontal pane.
				*_mainwindow_menubar,
				*_mainwindow_toolbar,
				*_vpane, 				// Vertical pane.
				*_hpane, 				// Horizontal pane.
				*_mainframe; 			// Main canvas frame.
	
	GdkColor _color;
	gdk_color_parse ("black", &_color);

	gtk_init(NULL,  NULL);
	_mainwindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(_mainwindow), "Physical CAD Tools");
	gtk_window_set_default_size(GTK_WINDOW(_mainwindow), WINDOW_H_SIZE, WINDOW_V_SIZE);
	g_signal_connect(G_OBJECT(_mainwindow), "destroy", G_CALLBACK(_quit_action), _mainwindow);

	_maincanvas_hbox = gtk_hbox_new(FALSE, 0);
	_maincanvas_vbox = gtk_vbox_new(FALSE, 0);
	_mainwindow_vbox = gtk_vbox_new(FALSE, 0);

	_mainwindow_toolbar = gtk_toolbar_new ();

	_open_btn = gtk_tool_button_new_from_stock(GTK_STOCK_OPEN);
	_save_btn = gtk_tool_button_new_from_stock(GTK_STOCK_SAVE);
	_play_btn = gtk_tool_button_new_from_stock(GTK_STOCK_MEDIA_PLAY);
//	_next_btn = gtk_tool_button_new_from_stock(GTK_STOCK_MEDIA_NEXT);
	_stop_btn = gtk_tool_button_new_from_stock(GTK_STOCK_MEDIA_STOP);
	_zoom_in_btn = gtk_tool_button_new_from_stock(GTK_STOCK_ZOOM_IN);
	_zoom_out_btn = gtk_tool_button_new_from_stock(GTK_STOCK_ZOOM_OUT);
	_zoom_fit_btn = gtk_tool_button_new_from_stock(GTK_STOCK_ZOOM_FIT);
//	_preferences_btn = gtk_tool_button_new_from_stock(GTK_STOCK_PREFERENCES);

	gtk_toolbar_insert(GTK_TOOLBAR(_mainwindow_toolbar), GTK_TOOL_ITEM(_open_btn), 0);
	gtk_toolbar_insert(GTK_TOOLBAR(_mainwindow_toolbar), GTK_TOOL_ITEM(_save_btn), 1);
	gtk_toolbar_insert(GTK_TOOLBAR(_mainwindow_toolbar), GTK_TOOL_ITEM(_play_btn), 2);
//	gtk_toolbar_insert(GTK_TOOLBAR(_mainwindow_toolbar), GTK_TOOL_ITEM(_next_btn), 3);
	gtk_toolbar_insert(GTK_TOOLBAR(_mainwindow_toolbar), GTK_TOOL_ITEM(_stop_btn), 3);
	gtk_toolbar_insert(GTK_TOOLBAR(_mainwindow_toolbar), GTK_TOOL_ITEM(_zoom_in_btn), 4);
	gtk_toolbar_insert(GTK_TOOLBAR(_mainwindow_toolbar), GTK_TOOL_ITEM(_zoom_out_btn), 5);
	gtk_toolbar_insert(GTK_TOOLBAR(_mainwindow_toolbar), GTK_TOOL_ITEM(_zoom_fit_btn), 6);
//	gtk_toolbar_insert(GTK_TOOLBAR(_mainwindow_toolbar), GTK_TOOL_ITEM(_preferences_btn), 8);

	gtk_signal_connect(GTK_OBJECT(_zoom_fit_btn), "clicked", GTK_SIGNAL_FUNC(_fit_window), (gpointer) "fit window button");
	gtk_signal_connect(GTK_OBJECT(_open_btn), "clicked", GTK_SIGNAL_FUNC(_open_file), (gpointer) "fit window button");
	gtk_signal_connect(GTK_OBJECT(_save_btn), "clicked", GTK_SIGNAL_FUNC(_save_file), (gpointer) "fit window button");
	gtk_signal_connect(GTK_OBJECT(_play_btn), "clicked", GTK_SIGNAL_FUNC(_play), (gpointer) "fit window button");
	gtk_signal_connect(GTK_OBJECT(_stop_btn), "clicked", GTK_SIGNAL_FUNC(_stop), (gpointer) "fit window button");


	_mainwindow_menubar = gtk_menu_bar_new ();
	_scale = 1.0;

// Layout Frame.
	_mainframe = gtk_frame_new("Layout");
	_hierarchy_frame = gtk_frame_new("Hierarchy");

	// Canvas Drawing Area, Frames and Associated Callbacks //
	_maincanvas = gtk_drawing_area_new();

	gtk_widget_modify_fg (_maincanvas, GTK_STATE_NORMAL, &_color);
	gtk_widget_modify_bg (_maincanvas, GTK_STATE_NORMAL, &_color);

	gtk_widget_set_size_request(_maincanvas, _maincanvas_width, _maincanvas_height);

  // maincanvas Event Handlers //
	g_signal_connect(G_OBJECT(_maincanvas), "expose-event", G_CALLBACK(_expose), _maincanvas);

	gtk_widget_add_events(_maincanvas, GDK_SCROLL);
	gtk_widget_add_events(_maincanvas, GDK_CONTROL_MASK);
	gtk_widget_add_events(_maincanvas, GDK_BUTTON_PRESS_MASK);

//	g_signal_connect(G_OBJECT(_maincanvas), "scroll-event", G_CALLBACK(scroll), _maincanvas);
//	g_signal_connect(G_OBJECT(_maincanvas), "button-press-event", G_CALLBACK(mousebutton), maincanvas);
  
// setup horizontal and vertical scrollbar
// setup adjustments
	_maincanvas_vscrollbar_adjustment = gtk_adjustment_new(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
	_maincanvas_hscrollbar_adjustment = gtk_adjustment_new(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
	_setup_scroll_adjustments();

	_maincanvas_vscrollbar = gtk_vscrollbar_new(GTK_ADJUSTMENT(_maincanvas_vscrollbar_adjustment));
	_maincanvas_hscrollbar = gtk_hscrollbar_new(GTK_ADJUSTMENT(_maincanvas_hscrollbar_adjustment));

  //g_signal_connect(G_OBJECT(_maincanvas_vscrollbar), "change-value", G_CALLBACK(_maincanvas_vscroll), _maincanvas_vscrollbar);
  //g_signal_connect(G_OBJECT(_maincanvas_hscrollbar), "change-value", G_CALLBACK(_maincanvas_hscroll), _maincanvas_hscrollbar);

  // Two Boxes and one Pane Layout //

	gtk_box_pack_start(GTK_BOX(_maincanvas_hbox), _maincanvas, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(_maincanvas_hbox), _maincanvas_vscrollbar, FALSE, FALSE, 0);

	gtk_box_set_child_packing(GTK_BOX(_maincanvas_hbox), _maincanvas, TRUE, TRUE, 0, GTK_PACK_START);
	gtk_box_set_child_packing(GTK_BOX(_maincanvas_hbox), _maincanvas_vscrollbar, FALSE, FALSE, 0, GTK_PACK_END);

	gtk_box_pack_start(GTK_BOX(_maincanvas_vbox), _maincanvas_hbox, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(_maincanvas_vbox), _maincanvas_hscrollbar, FALSE, FALSE, 0);

	gtk_box_set_child_packing(GTK_BOX(_maincanvas_vbox), _maincanvas_hbox, TRUE, TRUE, 0, GTK_PACK_START);
	gtk_box_set_child_packing(GTK_BOX(_maincanvas_vbox), _maincanvas_hscrollbar, FALSE, FALSE, 0, GTK_PACK_END);

	gtk_container_add(GTK_CONTAINER(_mainframe), _maincanvas_vbox);
	g_signal_connect(G_OBJECT(_maincanvas), "size-allocate", G_CALLBACK(_resize_maincanvas), _maincanvas);

	_hpane = gtk_hpaned_new(); 
	gtk_paned_add1(GTK_PANED(_hpane), _hierarchy_frame);
	gtk_paned_add2(GTK_PANED(_hpane), _mainframe);

	gtk_box_pack_start(GTK_BOX(_mainwindow_vbox), _mainwindow_toolbar, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(_mainwindow_vbox), _hpane, FALSE, FALSE, 0);
	gtk_box_set_child_packing(GTK_BOX(_mainwindow_vbox), _mainwindow_toolbar, FALSE, FALSE, 0, GTK_PACK_START);
	gtk_box_set_child_packing(GTK_BOX(_mainwindow_vbox), _hpane, TRUE, TRUE, 0, GTK_PACK_END);
	gtk_container_add(GTK_CONTAINER(_mainwindow), _mainwindow_vbox);
	gtk_widget_show_all(_mainwindow);
	gtk_main();
	_fit_window();
}

static void _quit_action()
{
	printf("Thank you. Bye now.\n");
	gtk_main_quit();
}

static void _setup_scroll_adjustments()
{
	gtk_adjustment_configure(GTK_ADJUSTMENT(_maincanvas_vscrollbar_adjustment), 0, 0, 0, 0, 0, 0);
	gtk_adjustment_configure(GTK_ADJUSTMENT(_maincanvas_hscrollbar_adjustment), 0, 0, 0, 0, 0, 0);
}

static void _expose(GtkWidget *widget, GdkEventExpose *event, gpointer data)
{
	_maincanvas_paint(widget, event, data);
}

static void _maincanvas_paint(GtkWidget *widget, GdkEventExpose *event, gpointer data)
{
	_maincanvas_drawable = _maincanvas->window;
	_maincanvas_cs = gdk_cairo_create(_maincanvas_drawable);
	_draw(_maincanvas_cs);
	cairo_destroy(_maincanvas_cs);
}

static void _draw(cairo_t *cr)
{
	int x, y, w, h, temp;
	int core_width = 4950, core_height = 5184;
	int num_rows = (int) round(core_height / COMPONENT_HEIGHT);
	int num_cols = (int) round(core_width / COMPONENT_WIDTH);
	//extern int core_width, core_height, num_rows, num_cols;
	srand(time(NULL));

	cairo_set_source_rgb(cr, 0.5f, 0.5f, 0.5f);
	cairo_rectangle(cr, _drawble_x_min, _drawble_y_min, _drawble_x_max, _drawble_y_max);
	cairo_stroke(cr);

	cairo_set_source_rgb(cr, 0, 1, 0);

	x = _core_x_min = _drawble_x_min + CORE_X_OFF;
	y = _core_y_min = _drawble_y_min + CORE_Y_OFF;
	w = _core_x_max = core_width + 2*CORE_X_OFF;
	h = _core_y_max = core_height + 2*CORE_Y_OFF;
	_core_x_max += _circuit_x_min;
	_core_y_max += _circuit_y_min;

	_circuit_x_min = _core_x_min;
	_circuit_x_max = _core_x_max;// + 2*CORE_X_OFF;
	_circuit_y_min = _core_y_min;
	_circuit_y_max = _core_y_max;// + 2*CORE_Y_OFF;
	_translate(&x, &y, &w, &h);
	cairo_rectangle(cr, x, y, w, h);
	cairo_stroke(cr);

	x += (int) round(CORE_X_OFF * _scale);
	y += (int) round(CORE_Y_OFF * _scale);
	w -= (int) round(2*CORE_X_OFF * _scale);
	h -= (int) round(2*CORE_Y_OFF * _scale);
	cairo_set_source_rgb(cr, 1, 1, 1);
	cairo_rectangle(cr, x, y, w, h);
	cairo_stroke(cr);
	temp = y;
#if 1
	for (int i=0; (i <= num_rows); ++i)
	{
		if (i%2)
			cairo_set_source_rgb(cr, 0, 0, 1);
		else
			cairo_set_source_rgb(cr, 1, 0, 0);
		y = (_core_y_min + _maincanvas_Oy) + (int) round(((i+1) * CORE_Y_OFF * _scale));
		cairo_move_to(cr, x, y);
		cairo_line_to (cr, x+w, y);
		cairo_stroke(cr);
	}
	y = temp;
#endif
#if 0
	cairo_set_source_rgb(cr, 1, 1, 1);
	for (int i=0; (i <= 55); ++i)
	{
		x = (_core_y_min + _maincanvas_Ox) + (int) round(((i+2) * CORE_X_OFF * _scale));
		cairo_move_to(cr, x, y);
		cairo_line_to (cr, x, y+h);
		cairo_stroke(cr);
	}
#endif
#if 0
	for (int i=0; (i < num_rows); ++i)
	{
		for (int j=0; (j < num_cols); ++j)
		{
			x = _circuit_x_min + (j * COMPONENT_WIDTH * _scale);
			y = _circuit_y_min + (i * COMPONENT_HEIGHT * _scale);
			w = COMPONENT_WIDTH;
			h = COMPONENT_HEIGHT;
			_translate(&x, &y, &w, &h);
			cairo_rectangle(cr, x, y, w, h);
			cairo_set_source_rgb(cr, (float)(rand())/RAND_MAX, (float)(rand())/RAND_MAX, (float)(rand())/RAND_MAX);
			cairo_fill(cr);
			cairo_set_source_rgb(cr, 1, 1, 1);
			cairo_rectangle(cr, x, y, w, h);
			cairo_stroke(cr);
			cairo_move_to(cr, x, y);
			cairo_line_to (cr, x+w, y+h);
			cairo_stroke(cr);
			cairo_move_to(cr, x+w, y);
			cairo_line_to (cr, x, y+h);
			cairo_stroke(cr);
		}
	}
#endif
#if 1
	for (auto &it : ComponentVector)
	{
		x = _circuit_x_min + (((it->location().x() * 1000.0) + CORE_X_OFF) * _scale);
		y = _circuit_y_min + (((it->location().y() * 1000.0) + CORE_Y_OFF) * _scale);
		w = COMPONENT_WIDTH;
		h = COMPONENT_HEIGHT;
		_translate(&x, &y, &w, &h);
		cairo_rectangle(cr, x, y, w, h);
		cairo_set_source_rgb(cr, it->R(), it->G(), it->B());
		cairo_fill(cr);
		cairo_set_source_rgb(cr, 1, 1, 1);
		cairo_rectangle(cr, x, y, w, h);
		cairo_stroke(cr);
		cairo_move_to(cr, x, y);
		cairo_line_to (cr, x+w, y+h);
		cairo_stroke(cr);
		cairo_move_to(cr, x+w, y);
		cairo_line_to (cr, x, y+h);
		cairo_stroke(cr);
	}
#endif
#if 0
	for (auto &it : PinVector)
	{
		x = _circuit_x_min + (it->location().x() * _scale);
		y = _circuit_y_min + (it->location().y() * _scale);

		x = _circuit_x_min + ((it->location().x() + CORE_X_OFF) * _scale);
		y = _circuit_y_min + ((it->location().y() + CORE_Y_OFF) * _scale);

		w = COMPONENT_WIDTH/10;
		h = COMPONENT_HEIGHT/10;
		_translate(&x, &y, &w, &h);
		cairo_rectangle(cr, x, y, w, h);
		cairo_set_source_rgb(cr, (float)(254.0f)/RAND_MAX, (float)(226.0f)/RAND_MAX, (float)(62.0f)/RAND_MAX);
		cairo_fill(cr);
		cairo_set_source_rgb(cr, 1, 1, 1);
		cairo_rectangle(cr, x, y, w, h);
		cairo_stroke(cr);
	}
#endif
}

static void _play()
{
//	GtkWidget *_dialog, *_label, *_value;
//	std::string label = "Wire-Length (HPBB)";
	float WL;
	if (_sel == 0)
	{
		minimise_QPWL();
	}
	else if (_sel == 1)
	{
		minimise_density();
		//tw_minimize_WL(100);
		//WL = report_WL ();
	}
	_sel++;
#if 0
	_dialog = gtk_dialog_new();
	_label = gtk_label_new (label.c_str());
	_value = gtk_label_new (std::to_string(WL).c_str());

	gtk_container_add (GTK_CONTAINER (GTK_DIALOG(_dialog)->vbox), _label);
	gtk_container_add (GTK_CONTAINER (GTK_DIALOG(_dialog)->vbox), _value);
	gtk_widget_show_all (_dialog);
#endif
	gtk_widget_queue_draw (_maincanvas);
}

static void _stop()
{
	for (auto &it : ComponentVector)
	{
		it->set_location(Point<float>(0,0));
	}
	_sel = 0;
	gtk_widget_queue_draw (_maincanvas);
}

static void _open_file()
{
	static bool temp = false;
	if (temp) return;
	temp = true;
	GtkWidget *dialog;
	dialog = gtk_file_chooser_dialog_new ("Open File", GTK_WINDOW(_mainwindow), GTK_FILE_CHOOSER_ACTION_OPEN,
							GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT, NULL);
	if (gtk_dialog_run (GTK_DIALOG (dialog)) != GTK_RESPONSE_ACCEPT)
		gtk_widget_destroy (dialog);

	char *filename;
	std::ifstream iFile;
	float x, y, core_width, core_height;

	filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
	read_design (filename);
	g_free (filename);
	_fit_window ();
	gtk_widget_destroy (dialog);
}

static void _save_file()
{
	GtkWidget *dialog;
	dialog = gtk_file_chooser_dialog_new (	"Save File",
											GTK_WINDOW(_mainwindow),
											GTK_FILE_CHOOSER_ACTION_SAVE,
											GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
											GTK_STOCK_SAVE, GTK_RESPONSE_ACCEPT,
											NULL);
	gtk_file_chooser_set_do_overwrite_confirmation (GTK_FILE_CHOOSER (dialog), TRUE);
	gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (dialog), ".");
	gtk_file_chooser_set_current_name (GTK_FILE_CHOOSER (dialog), "Untitled document");
	if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
	{
		char *filename;
		filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
		
		g_free (filename);
	}
	gtk_widget_destroy (dialog);
}
static void _resize_maincanvas(GtkWidget *widget, GdkRectangle *gdkrect, gpointer data)
{
	_maincanvas_width  = gdkrect->width;
	_maincanvas_height = gdkrect->height;
	_drawble_x_max = _maincanvas_width - 2*_drawble_x_min;
	_drawble_y_max = _maincanvas_height - 2*_drawble_y_min;
	_fit_window();
}

static void _fit_window()
{
	_scale = MIN((double)(_drawble_x_max - _drawble_x_min) / (_circuit_x_max - _circuit_x_min),(double)
		(_drawble_y_max - _drawble_y_min) / (_circuit_y_max - _circuit_y_min));
	_center(_maincanvas_width, 0.0, _maincanvas_height, 0.0,
			_circuit_x_max, _circuit_x_min, _circuit_y_max, _circuit_y_min);
	gtk_widget_queue_draw (_maincanvas);
}

static void _translate(int *x0, int *y0, int *width, int *height)
{
	*x0 += _maincanvas_Ox;
	*y0 += _maincanvas_Oy;
	*width *= _scale;
	*height *= _scale;
}

static void _center(int x0_max, int x0_min, int y0_max, int y0_min,
					int x1_max, int x1_min, int y1_max, int y1_min)
{
	_maincanvas_Ox = (((x0_max - x0_min) + 2*x0_min) - (((x1_max - x1_min) * _scale) + 2*x1_min))/2;
	_maincanvas_Oy = (((y0_max - y0_min) + 2*y0_min) - (((y1_max - y1_min) * _scale) + 2*y1_min))/2;
}
#endif