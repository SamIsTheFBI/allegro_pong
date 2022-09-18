#include <stdio.h>
#include <allegro5/allegro5.h>


#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

#include <allegro5/allegro_primitives.h>	

#include "game.h"
#include "sound.h"
#define KEY_SEEN 1
#define KEY_RELEASED 2


#define DISPLAY_WIDTH 1000
#define DISPLAY_HEIGHT 800

#define BALL_RADIUS 25

#define CENTRE_LINE_WIDTH 5

void must_init(bool test, const char *message){
	if(test)
		return;
	printf("Couldnt initialize %s\n", message);
}

int main(){
	
	must_init(al_init(), "allegro");
	must_init(al_install_keyboard(), "keyboard");
	
	ALLEGRO_TIMER *timer = al_create_timer(1.0 / 60.0);
	must_init(timer, "timer");

	ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
	must_init(event_queue, "event queue");

	al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
    	al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);
    	al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);
	
	Dimension display_dimensions = {.width = 1000, .height = 800};

	ALLEGRO_DISPLAY* disp = al_create_display(display_dimensions.width, display_dimensions.height);
    	must_init(disp, "display");
	
	must_init(al_init_primitives_addon(), "primitives");//drawing basic shapes and lines for my pong game

	must_init(al_install_audio(), "audio");
    	must_init(al_init_acodec_addon(), "audio codecs");
    	must_init(al_reserve_samples(16), "reserve samples");

	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_display_event_source(disp));
	al_register_event_source(event_queue, al_get_timer_event_source(timer));

	init_sound_files();
	printf("meow\n");


	Bar bar1 = {.position = {.x = 100, .y = 640 / 2 - 20 / 2}, .size = {.width = 20, .height = 100}, .color = al_map_rgb(255, 255, 255)};
	Bar bar2 = {.position = {.x = 900 - 20, .y = 640 / 2 - 20 / 2}, .size = {.width = 20, .height = 100}, .color = al_map_rgb(255, 255, 255)};
	Ball ball = {.position = {.x = DISPLAY_WIDTH/2 - BALL_RADIUS/2, .y = DISPLAY_HEIGHT/2 - BALL_RADIUS/2}, {.x = 3.0, .y = 5.0}, BALL_RADIUS, al_map_rgb(255, 255, 255)};

	bool game_over = false;
	bool redraw = false;
	ALLEGRO_EVENT event;
	enum y_direction direction = UP;


	unsigned char key[ALLEGRO_KEY_MAX];
	memset(key, 0, sizeof(key));



	al_start_timer(timer);//generates events, as the timer increments at a constant rate
	while(!game_over){
		
		al_wait_for_event(event_queue, &event);
		switch(event.type){
			case ALLEGRO_EVENT_TIMER:
				redraw = true;//time for the next draw
				
				if(key[ALLEGRO_KEY_W])
				 	move_bar(&bar1, UP, DISPLAY_HEIGHT);

				if(key[ALLEGRO_KEY_S])
					move_bar(&bar1, DOWN, DISPLAY_HEIGHT);
				

				if(key[ALLEGRO_KEY_UP])
					move_bar(&bar2, UP, DISPLAY_HEIGHT);

				
				if(key[ALLEGRO_KEY_DOWN])
					move_bar(&bar2, DOWN, DISPLAY_HEIGHT);
				

				if(key[ALLEGRO_KEY_ESCAPE])
					game_over = true;

				for(int i = 0; i < ALLEGRO_KEY_MAX; i++)
            				key[i] &= KEY_SEEN;

				
				break;

			case ALLEGRO_EVENT_KEY_DOWN:
				key[event.keyboard.keycode] = KEY_SEEN | KEY_RELEASED;
				break;

			case ALLEGRO_EVENT_KEY_UP:
				key[event.keyboard.keycode] &= KEY_RELEASED;
				break;
		}

		
		update_ball(&ball, bar1, bar2, display_dimensions);
		if(redraw){
			al_clear_to_color(al_map_rgb(0, 0, 0));

			//DRAW A THICCC LINE RIGHT IN THE MIDDLE
			al_draw_line(DISPLAY_WIDTH/2 - CENTRE_LINE_WIDTH/2, 0, DISPLAY_WIDTH/2 - CENTRE_LINE_WIDTH/2,
					DISPLAY_HEIGHT,  al_map_rgb(255, 255, 255), CENTRE_LINE_WIDTH);
			

			al_draw_filled_rectangle(bar1.position.x, bar1.position.y, bar1.position.x + bar1.size.width,
                                                 bar1.position.y + bar1.size.height, bar1.color);

			al_draw_filled_rectangle(bar2.position.x, bar2.position.y, bar2.position.x + bar2.size.width, 
						 bar2.position.y + bar2.size.height, bar2.color);

			al_draw_filled_circle(ball.position.x + (float)ball.radius/2, ball.position.y + (float)ball.radius/2, BALL_RADIUS, al_map_rgb(255, 255, 255));


			al_flip_display();
			redraw = false;
		}

	}	

}
