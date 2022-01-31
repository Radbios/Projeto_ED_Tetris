#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

//VARIÁVEIS DO JOGO
const int largura_t = 640;
const int altura_t = 480;

void record(ALLEGRO_DISPLAY *display, ALLEGRO_EVENT_QUEUE *fila_eventos);

void start_game(ALLEGRO_DISPLAY *display, ALLEGRO_EVENT_QUEUE *fila_eventos);

void menu(ALLEGRO_DISPLAY *display, ALLEGRO_EVENT_QUEUE *fila_eventos);

int main(int argc, char **argv)
{
	fprintf(stderr, "Aperte algo para mostrar a tela do jogo!\n");

	bool fim = false;
	bool jogar = false;

    //INICIALIZAÇÃO DA ALLEGRO
   ALLEGRO_DISPLAY *display = NULL;
   ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;

   if(!al_init()) {
      fprintf(stderr, "failed to initialize allegro!\n");
      return -1;
   }
    //

    //CRIAR JANELA
   display = al_create_display(largura_t, altura_t);

   if(!display) {
      fprintf(stderr, "failed to create display!\n");
      return -1;
   }
    //

   //INICIALIZAÇÃO DE ADDONS
	al_init_primitives_addon();

		if(!al_init_primitives_addon()) {
		fprintf(stderr, "failed to initialize primitives!\n");
		return -1;
    }
    //

	al_init_font_addon();
	al_init_ttf_addon();
	al_install_audio();
	al_install_keyboard();
	al_init_acodec_addon();
	al_init_image_addon();

    //INICIAR FILA DE EVENTOS
    fila_eventos = al_create_event_queue();

	if (!fila_eventos)
	{
		fprintf(stderr, "Falha ao criar fila de eventos.\n");
		al_destroy_display(display);
		return -1;
	}
	//

	al_register_event_source(fila_eventos, al_get_keyboard_event_source());
	al_register_event_source(fila_eventos, al_get_display_event_source(display));

	al_reserve_samples(1);

    //MENU DO JOGO
    menu(display, fila_eventos);
    //

	al_destroy_event_queue(fila_eventos);
   	al_destroy_display(display);

   return 0;
}

void menu(ALLEGRO_DISPLAY *display, ALLEGRO_EVENT_QUEUE *fila_eventos){

    int pos_x = largura_t/2 - 60;
	int pos_y = altura_t/2 + 10;

    bool fim = false;

    ALLEGRO_SAMPLE *soundEffect = al_load_sample("menu.wav");
    ALLEGRO_FONT *font20 = NULL;
    ALLEGRO_FONT *font40 = NULL;
    ALLEGRO_BITMAP *imagem = NULL;

    while(!(fim))
    {
        ALLEGRO_EVENT ev;

	    //CRIAR FONTE
		font20 = al_load_ttf_font("arial.TTF", 20, 0);
		font40 = al_load_ttf_font("arial.TTF", 40, 0);
	    //FUNÇÃO DE DESENHO
	    al_draw_text(font40, al_map_rgb(255, 255, 255), largura_t/2, altura_t/4, ALLEGRO_ALIGN_CENTRE, "TETRIS");
		al_draw_text(font20, al_map_rgb(255, 255, 255), largura_t/2, altura_t/2, ALLEGRO_ALIGN_CENTRE, "Jogar");
		al_draw_text(font20, al_map_rgb(255, 255, 255), largura_t/2, altura_t/2 + 40, ALLEGRO_ALIGN_CENTRE, "Recordes");
		al_draw_text(font20, al_map_rgb(255, 255, 255), largura_t/2, altura_t/2 + 80, ALLEGRO_ALIGN_CENTRE, "Sair");

		al_play_sample(soundEffect, 0.8, 0, 0.8, ALLEGRO_PLAYMODE_LOOP, 0);

	    al_wait_for_event(fila_eventos, &ev);

	    if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
	    {
			switch(ev.keyboard.keycode)
			{
				case ALLEGRO_KEY_UP:
					if (pos_y <= 250){
						pos_y = altura_t/2 + 10;
						break;
					}
					else{
						pos_y -= 40;
						break;
					}
				case ALLEGRO_KEY_DOWN:
					if (pos_y >= 330){
						pos_y = 330;
						break;
					}
					else {
						pos_y += 40;
						break;
					}
				case ALLEGRO_KEY_ENTER:
					if (pos_y == 330) fim = true;
					else if (pos_y == 250){
                        start_game(display, fila_eventos);
					}
					else if (pos_y == 290){
                        record(display, fila_eventos);
					}
			}
	    }
		else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			fim = true;
		}

		al_draw_filled_circle(pos_x, pos_y, 10, al_map_rgb(255, 255, 0));

	    al_flip_display();

	    //FUNDO DO MENU
	   	al_clear_to_color(al_map_rgb(0, 0, 0));
	   	imagem = al_load_bitmap("vaporwave.jpg");
	   	al_draw_bitmap(imagem, 0, 0, 0);

	   //OBJETOS DO MENU
	   	al_draw_rectangle(0, 0,  largura_t, altura_t, al_map_rgb(0, 0, 0), 3);
	   	al_draw_rectangle(largura_t/2 - 80, altura_t/4 - 10,  largura_t/2 + 80, altura_t/4 + 50, al_map_rgb(255, 255, 255), 3);

    }
    //FIM DA FUNÇÃO (CLEAR)
    al_destroy_sample(soundEffect);
   	al_destroy_font(font20);
   	al_destroy_font(font40);
   	//
}

void start_game(ALLEGRO_DISPLAY *display, ALLEGRO_EVENT_QUEUE *fila_eventos){

    bool jogar = true;

    while(jogar)
    {
	    al_flip_display();
        ALLEGRO_EVENT ev;

	    al_wait_for_event(fila_eventos, &ev);



	    if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
	    {
			switch(ev.keyboard.keycode)
			{
				case ALLEGRO_KEY_ESCAPE:
					return;
			}
	    }

	    //FUNDO DO MENU
	   	al_clear_to_color(al_map_rgb(0, 0, 0));
    }
}

void record(ALLEGRO_DISPLAY *display, ALLEGRO_EVENT_QUEUE *fila_eventos){

    bool jogar = true;

    while(jogar)
    {
	    al_flip_display();
        ALLEGRO_EVENT ev;

	    al_wait_for_event(fila_eventos, &ev);



	    if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
	    {
			switch(ev.keyboard.keycode)
			{
				case ALLEGRO_KEY_ESCAPE:
					return;
			}
	    }

	    //FUNDO DO MENU
	   	al_clear_to_color(al_map_rgb(0, 0, 0));
    }
}

