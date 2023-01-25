#include <stdio.h>
#include <furi.h>
#include <gui/gui.h>
#include <input/input.h>
#include <notification/notification_messages.h>

// Developed by Zach Hutton
// https://zachhutton.com

char* coin_state = "Heads";
char* flip_state = "";
bool is_flipping = false;


static  void  draw_callback (Canvas* canvas, void * ctx) {
    UNUSED (ctx);
    canvas_clear (canvas);
    canvas_set_font (canvas, FontPrimary);
    canvas_draw_str (canvas, 0 , 10 , "Coin Flipper!");
    canvas_draw_str (canvas, 0, 30, coin_state);

   if (is_flipping) {
		canvas_draw_str (canvas, 0, 50, "Flipping...");
		furi_delay_ms(800);
		is_flipping = false;
		coin_state = (rand() % 2 == 0) ? "Heads" : "Tails";
   }
	
}

static  void  input_callback (InputEvent* input_event, void * ctx) {
    furi_assert (ctx);
    FuriMessageQueue* event_queue = ctx;
    furi_message_queue_put (event_queue, input_event, FuriWaitForever);

    if (input_event->key == InputKeyOk) {				
		is_flipping = true;
    }
}

int32_t  Coin_Flipper_app ( void * p) {
    UNUSED (p);

    InputEvent event;
    FuriMessageQueue* event_queue = furi_message_queue_alloc ( 8 , sizeof (InputEvent));

    ViewPort* view_port = view_port_alloc ();
    view_port_draw_callback_set (view_port, draw_callback, NULL );
    view_port_input_callback_set (view_port, input_callback, event_queue);

    Gui* gui = furi_record_open (RECORD_GUI);
    gui_add_view_port (gui, view_port, GuiLayerFullscreen);

    while ( 1 ) {
        furi_check ( furi_message_queue_get (event_queue, &event, FuriWaitForever) == FuriStatusOk);

        if (event. key == InputKeyBack) {
            break ;
        } 
    }

    furi_message_queue_free (event_queue);
    gui_remove_view_port (gui,view_port);
	view_port_free (view_port);
	furi_record_close (RECORD_GUI);

	return 0;
}
