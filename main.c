#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <time.h>
#include "objetos/pieces.h"


//------ VARIÁVEIS DO JOGO -------
const int largura_t = 640;
const int altura_t = 480;
const int DIMENSAO_P = 30;
const int LINHAS = 18;
const int COLUNAS = 10;
const int BLOCOS = 4;
const int tela_x = 100;
const int tela_y = 0;
int FPS = 60;
char str[17];

//

//----- TIMER -----
ALLEGRO_TIMER *timer = NULL, *contador = 0;

//-------
//------ SOUNDS ------
ALLEGRO_SAMPLE *selectOptionSound = NULL;
ALLEGRO_SAMPLE *changeOptionSound = NULL;
//------


// ---------- FONTES ----------
ALLEGRO_FONT *font10 = NULL;
ALLEGRO_FONT *font20 = NULL;
ALLEGRO_FONT *font40 = NULL;

//----------------

// ----------------- INICIALIZAÇÃO DA ALLEGRO --------------

ALLEGRO_DISPLAY *display = NULL;
ALLEGRO_EVENT_QUEUE *fila_eventos = NULL, *fila_contador = NULL;

//---------------

//----------- BITMAP -----------

ALLEGRO_BITMAP *imagemMenu = NULL;
ALLEGRO_BITMAP *imagemGame = NULL;
ALLEGRO_BITMAP *imagemRecord = NULL;
ALLEGRO_BITMAP *p1 = NULL;
ALLEGRO_BITMAP *p2 = NULL;
ALLEGRO_BITMAP *p3 = NULL;
ALLEGRO_BITMAP *p4 = NULL;
ALLEGRO_BITMAP *p5 = NULL;

//------------------

//------- ESCREVENDO TEXTO NA TELA -------//
void manipular_entrada(ALLEGRO_EVENT evento);
void exibir_texto_centralizado();
//------- ESCREVENDO TEXTO NA TELA -------//


// ---------------------- pieces -----------
void init_piecesQueue(int queue[]);
void piecesQueue(int queue[]);
void init_piece(int map[][COLUNAS], int piece, int cordenada[]);
void draw_piece(int map[][COLUNAS], int cordenada[]);
void girar(int map[][COLUNAS], int cordenada[], int p_atual);
void checar_linha(int map[][COLUNAS], int *score);
// ------------------

//---------- MOVIMENTO -----------
void move_baixo(int map[][COLUNAS], int cordenada_blocos[], bool *new_p, bool *game_over);
void move_esquerda(int map[][COLUNAS], int cordenada_blocos[]);
void move_direita(int map[][COLUNAS], int cordenada_blocos[]);
//-----------

void record();
void start_game();
void menu();



int main(int argc, char **argv)
{
	fprintf(stderr, "Aperte algo para mostrar a tela do jogo!\n");

    //VARIÁVEIS
    bool fim = false;
    bool iniciar = false;
    int intro = 0;
    //
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

    //INSTALAÇÃO E INICIAÇÃO
	al_init_font_addon();
	al_init_ttf_addon();
	al_install_audio();
	al_install_keyboard();
	al_init_acodec_addon();
	al_init_image_addon();
    timer = al_create_timer(1.0/FPS);
    //

	al_reserve_samples(3);


    //CRIAR FILA DE EVENTOS E AFINS
    fila_eventos = al_create_event_queue();
    //

    //------ LOAD BITMAP ------
    imagemMenu = al_load_bitmap("image/menu.jpg");
    p1 = al_load_bitmap("image/pieces/p0.png");
    p2 = al_load_bitmap("image/pieces/p1.png");
    p3 = al_load_bitmap("image/pieces/p2.png");
    p4 = al_load_bitmap("image/pieces/p3.png");
    p5 = al_load_bitmap("image/pieces/p4.png");
    imagemGame = al_load_bitmap("image/start_game.jpg");
    imagemRecord = al_load_bitmap("image/record.jpg");
    //---------

	if (!fila_eventos)
	{
		fprintf(stderr, "Falha ao criar fila de eventos.\n");
		al_destroy_display(display);
		return -1;
	}

	//

	//REGISTRO DE SOURCES
	al_register_event_source(fila_eventos, al_get_keyboard_event_source());
	al_register_event_source(fila_eventos, al_get_display_event_source(display));
	al_register_event_source(fila_eventos, al_get_timer_event_source(timer));
    //

    //CRIAR FONTE
    font10 = al_load_ttf_font("arial.TTF", 10, 0);
    font20 = al_load_ttf_font("arial.TTF", 20, 0);
    font40 = al_load_ttf_font("arial.TTF", 40, 0);
    //

    al_start_timer(timer);

    menu();

    //---------- FIM ----------
	al_destroy_event_queue(fila_eventos);
   	al_destroy_display(display);
    al_destroy_sample(selectOptionSound);
    al_destroy_sample(changeOptionSound);
    al_destroy_font(font10);
    al_destroy_font(font20);
    al_destroy_font(font40);
    al_destroy_bitmap(imagemMenu);
    al_destroy_bitmap(imagemGame);
    al_destroy_bitmap(imagemRecord);
    al_destroy_bitmap(p1);
    al_destroy_bitmap(p2);
    al_destroy_bitmap(p3);
    al_destroy_bitmap(p4);
    al_destroy_bitmap(p5);
    al_destroy_timer(timer);

    //------------

   return 0;
}

void menu()
{
    //--------------- VARIÁVEIS --------------
    int pos_x = largura_t/2 - 60;
	int pos_y = altura_t/2 + 10;
	bool fim = false;
	bool desenho = true;

    //-------------------
 //SOUND
    selectOptionSound = al_load_sample("sound/effects/select_option.wav");
    changeOptionSound = al_load_sample("sound/effects/change_option.wav");
    //

    //EVENTOS E LÓGICA
    while(!(fim))
    {
        // ------------------- INSTALAÇÃO E INICIAÇÃO -----------------
        ALLEGRO_EVENT ev;
        //-----------

	    al_wait_for_event(fila_eventos, &ev);

        //--------------- MOVIMENTO E SELECT -------------
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
                        al_play_sample(changeOptionSound, 0.8 , 0 , 0.8 , ALLEGRO_PLAYMODE_ONCE, 0 );
						pos_y -= 40;
						break;
					}
				case ALLEGRO_KEY_DOWN:
					if (pos_y >= 330){
						pos_y = 330;
						break;
					}
					else {
                        al_play_sample(changeOptionSound, 0.8 , 0 , 0.8 , ALLEGRO_PLAYMODE_ONCE, 0 );
						pos_y += 40;
						break;
					}
				case ALLEGRO_KEY_ENTER:

                    al_play_sample(selectOptionSound, 2 , 0 , 1.5 , ALLEGRO_PLAYMODE_ONCE, 0 );

					if (pos_y == 330) fim = true;
					else if (pos_y == 250){
                        start_game();
					}
					else if (pos_y == 290){
                        record();
					}
					break;
			}
	    }
	    //

		else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			fim = true;
		}
        else if(ev.type == ALLEGRO_EVENT_TIMER){
            desenho = true;
        }


        if(desenho && al_is_event_queue_empty(fila_eventos)){
        // ---------------- DESENHO ---------------

            al_clear_to_color(al_map_rgb(0, 0, 0));
            al_draw_bitmap(imagemMenu, 0, 0, 0);
            al_draw_rectangle(largura_t/2 - 80, altura_t/4 - 10,  largura_t/2 + 80, altura_t/4 + 50, al_map_rgb(255, 255, 255), 3);
            al_draw_text(font40, al_map_rgb(255, 255, 255), largura_t/2, altura_t/4, ALLEGRO_ALIGN_CENTRE, "TETRIS");
            al_draw_text(font20, al_map_rgb(255, 255, 255), largura_t/2, altura_t/2, ALLEGRO_ALIGN_CENTRE, "Jogar");
            al_draw_text(font20, al_map_rgb(255, 255, 255), largura_t/2, altura_t/2 + 40, ALLEGRO_ALIGN_CENTRE, "Recordes");
            al_draw_text(font20, al_map_rgb(255, 255, 255), largura_t/2, altura_t/2 + 80, ALLEGRO_ALIGN_CENTRE, "Sair");
            al_draw_filled_circle(pos_x, pos_y, 10, al_map_rgb(255, 255, 0));
            al_flip_display();

		//------------------------
        }
    }
    //----------------
}

void init_piecesQueue(int queue[])
{
    time_t t;
    int i;
    srand((unsigned) time(&t));
    for(i = 0; i< BLOCOS; i++)
    {
        queue[i] = (rand() % 5);
    }
}

void piecesQueue(int queue[])
{
    time_t t;
    int i;
    srand((unsigned) time(&t));
    for(i = 0; i< BLOCOS; i++)
    {
        if(i < 3){
            queue[i] = queue[i + 1];
        }
        else{
            queue[i] = (rand() % 5);
        }
    }
}

void init_piece(int map[][COLUNAS], int piece, int cordenada[]){
    int i;
    for(i = 0; i < BLOCOS; i++){
        int bloco = pieces[piece][i];
        cordenada[i] = bloco;
        map[0][bloco] = 1;
    }
}


void draw_piece(int map[][COLUNAS], int cordenada[])
{
    int i, j;
    for(i = 0; i < LINHAS; i++){
        for(j = 0; j < COLUNAS; j++){
            if(map[i][j] == 1 || map[i][j] == 2){
                al_draw_filled_rectangle(tela_x + (j*DIMENSAO_P), tela_y + (i*DIMENSAO_P) - 90, tela_x + (j*DIMENSAO_P) + 30, tela_y + (i*DIMENSAO_P)+ 30 - 90, al_map_rgb(0, 0, 255));
                al_draw_rectangle(tela_x + (j*DIMENSAO_P), tela_y + (i*DIMENSAO_P) - 90, tela_x + (j*DIMENSAO_P) + 30, tela_y + (i*DIMENSAO_P) + 30 - 90, al_map_rgb(255, 0, 200), 2);
            }
        }
    }
}

void checar_linha(int map[][COLUNAS], int *score)
{
    int i, j, k, h;
    for(i = 0; i < LINHAS; i++){
        for(j = 0; j < COLUNAS; j++){
            if(map[i][j] == 0)
            {
                break;
            }
            else if(j == 9){
                for(k = 0; k < COLUNAS; k++){
                    map[i][k] = 0;
                }
                for(k = i; k > 0 ; k--){
                    for(h = 0; h < COLUNAS ; h++){
                        map[k][h] = map[k-1][h];
                    }
                }
                *score = *score + 100;
            }
        }

    }
}

void girar(int map[][COLUNAS],int cordenada[], int p_atual)
{
    if(p_atual != 0){
        //------ ALTURA E COMPRIMENTO DA PEÇA -------
        int altura = (cordenada[3]/10 - cordenada[0]/10) + 1;
        int comprimento = (cordenada[3]%10 - cordenada[0]%10) + 1;
        // --------------

        // ------ CORDENADAS DO PRIMIRO BLOCO ----
        int coluna0 = cordenada[0]%10;
        int linha0 = cordenada[0]/10;
        // --------------

        //--------- VARIAVEIS AUXILIARES -------
        int i, j;
        bool verificar = true;
        int aux[BLOCOS][BLOCOS];
        int aux1 = linha0;
        int aux2 = coluna0 + 3;
        int bloco = 0;
        //-------------



    }
}

void move_baixo(int map[][COLUNAS], int cordenada_blocos[], bool *new_p, bool *game_over)
{
    int i;
    for(i = 0; i< BLOCOS ; i++)
    {

        //SE ACHAR UMA PEÇA OU O LIMITE DO MAPA >> STOP
        if(cordenada_blocos[i] + 10 >= LINHAS*10 || map[0][cordenada_blocos[i] + 10] == 2)
        {
            //----- GAME OVER -----
            if(cordenada_blocos[i] <= 40){
                *game_over = true;
                return;
            }
            // ---------
            for(i = 0; i<BLOCOS; i++)
            {
                map[0][cordenada_blocos[i]] = 2;
            }
            *new_p = true;
            return;
        }
    }
    for(i = 0; i < BLOCOS; i++)
    {
        map[0][cordenada_blocos[i]] = 0;
    }

    for(i = 0; i < BLOCOS; i++)
    {
        map[0][cordenada_blocos[i] + 10] = 1;
        cordenada_blocos[i] = cordenada_blocos[i] + 10;
    }
}

void move_esquerda(int map[][COLUNAS], int cordenada_blocos[])
{
    int i;

    for(i = 0; i< BLOCOS ; i++)
    {
        if(cordenada_blocos[i]%10 == 0 || map[0][cordenada_blocos[i] - 1] == 2 ){
            return;
        }
    }
    for(i = 0; i < BLOCOS; i++)
    {
        map[0][cordenada_blocos[i]] = 0;
    }

    for(i = 0; i < BLOCOS; i++)
    {
        map[0][cordenada_blocos[i] - 1] = 1;
        cordenada_blocos[i] = cordenada_blocos[i] - 1;
    }
}

void move_direita(int map[][COLUNAS], int cordenada_blocos[])
{
    int i;

    for(i = 0; i< BLOCOS ; i++)
    {
        if(cordenada_blocos[i]%10 == 9 || map[0][cordenada_blocos[i] + 1] == 2 ){
            return;
        }
    }
    for(i = 0; i < BLOCOS; i++)
    {
        map[0][cordenada_blocos[i]] = 0;
    }

    for(i = 0; i < BLOCOS; i++)
    {
        map[0][cordenada_blocos[i] + 1] = 1;
        cordenada_blocos[i] = cordenada_blocos[i] + 1;
    }
}

bool stop_move(int map[][COLUNAS], int cordenada_blocos[])
{
    int i;
    for(i = 0; i < BLOCOS; i++)
    {

    }
    return false;
}

void start_game(){

    //------------ VARIÁVEIS DO JOGO -------------
    bool *game_over = false;
    bool *new_p = true;
    bool pause = false;
    bool desenho = false;
    bool move[] = {false, false, false};
    int cordenada_blocos[BLOCOS], j;
    int p_atual;
    int *score = 0;
    enum {BAIXO, ESQUERDA, DIREITA};
    ALLEGRO_EVENT ev;
    int map[18][10] = {0};

    // --------------------

    //--------------- INICIAR ---------------
    init_piecesQueue(pieces_queue);
    //-------------------

    // --------------- EVENTOS E LÓGICA -------------
    while(!game_over)
    {

        al_wait_for_event(fila_eventos, &ev);

        // ---------------- MOVIMENTO E SELECT ----------------
	    if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
	    {
			switch(ev.keyboard.keycode)
			{
				case ALLEGRO_KEY_ESCAPE:
				    return;
				    break;
                case ALLEGRO_KEY_SPACE:
                    girar(map, cordenada_blocos, p_atual);
                    break;
                case ALLEGRO_KEY_DOWN:
                    move_baixo(map, cordenada_blocos, &new_p, &game_over);
                    break;
                case ALLEGRO_KEY_LEFT:
                    move_esquerda(map, cordenada_blocos);
                    break;
                case ALLEGRO_KEY_RIGHT:
                    move_direita(map, cordenada_blocos);
                    break;

			}
	    }
	    else if(ev.type == ALLEGRO_EVENT_TIMER){
            move_baixo(map, cordenada_blocos, &new_p, &game_over);
            desenho = true;

	    }
	    //------------------------


	    // ------------DESENHO--------------

	    if(desenho){

            al_clear_to_color(al_map_rgb(100, 100, 100));
            al_draw_bitmap(imagemGame, 0, 0, 0);




            //RETANGULO DO GAME (300 x 4508)
            al_draw_rectangle(tela_x ,-10 , tela_x + 300, 450, al_map_rgb(255,62,0), 6);
            al_draw_filled_rectangle(tela_x ,0 , tela_x + 300, 450, al_map_rgb(31,10,47));
            //

            //INFORMAÇÕES DA DIREITA
            al_draw_textf(font20, al_map_rgb(255,231,6), 420, 20, ALLEGRO_ALIGN_INTEGER , "SCORE : %d", score);
            al_draw_textf(font20, al_map_rgb(255,231,6), 420, 60, ALLEGRO_ALIGN_INTEGER, "LEVEL : 1");
            //al_draw_textf(font20, al_map_rgb(255,231,6), 420, 100, ALLEGRO_ALIGN_INTEGER, "SPEED : 0");
            al_draw_textf(font20, al_map_rgb(255,231,6), 420, 140, ALLEGRO_ALIGN_INTEGER, "TIME :");
            //

            //ESPAÇO PARA PREVISÃO DAS PEÇAS (50 x 50)
            al_draw_filled_rectangle(40 ,5 , 90, 55, al_map_rgb(0, 0, 0));
            al_draw_filled_rectangle(40 ,60 , 90, 110, al_map_rgb(0, 0, 0));
            al_draw_filled_rectangle(40 ,115 , 90, 165, al_map_rgb(0, 0, 0));
            al_draw_filled_rectangle(40 ,170 , 90, 220, al_map_rgb(0, 0, 0));

            al_draw_rectangle(40, 5, 90, 55, al_map_rgb(255,62,0), 3);
            al_draw_rectangle(40, 60, 90, 110, al_map_rgb(255,62,0), 3);
            al_draw_rectangle(40, 115, 90, 165, al_map_rgb(255,62,0), 3);
            al_draw_rectangle(40, 170, 90, 220, al_map_rgb(255,62,0), 3);

            for( j = 0; j < BLOCOS; j++)
            {
                switch(pieces_queue[j])
                {
                    case 0:
                        al_draw_scaled_bitmap(p1, 0, 0, 50, 50, 40, 5 + (j*55), 50, 50, 0);
                        break;
                    case 1:
                        al_draw_scaled_bitmap(p2, 0, 0, 50, 50, 40, 5 + (j*55), 50, 50, 0);
                        break;
                    case 2:
                        al_draw_scaled_bitmap(p3, 0, 0, 50, 50, 40, 5 + (j*55), 50, 50, 0);
                        break;
                    case 3:
                        al_draw_scaled_bitmap(p4, 0, 0, 50, 50, 40, 5 + (j*55), 50, 50, 0);
                        break;
                    case 4:
                        al_draw_scaled_bitmap(p5, 0, 0, 50, 50, 40, 5 + (j*55), 50, 50, 0);
                        break;
                }
            }

            //

            //SKILL (50 x 50)
            al_draw_rectangle(410, 400, 460, 450, al_map_rgb(255,62,0), 6);
            al_draw_filled_rectangle(410 ,400 , 460, 450, al_map_rgb(0, 0, 0));
            //

            //BLOCOS
            if(new_p){
                int i;
                checar_linha(map, &score);
                init_piece(map, pieces_queue[0], cordenada_blocos);
                p_atual = pieces_queue[0];
                piecesQueue(pieces_queue);
                new_p = false;
            }

            draw_piece(map, cordenada_blocos);

            al_flip_display();
            desenho = false;
        }

            //------------------------
    }
    //--------------------------

    while(game_over){

        al_clear_to_color(al_map_rgb(0, 0, 0));

        al_wait_for_event(fila_eventos, &ev);



        // ---------------- MOVIMENTO E SELECT ----------------
	    if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
	    {
			switch(ev.keyboard.keycode)
			{
				case ALLEGRO_KEY_SPACE:
				    return;
				    break;
			}
	    }

        al_draw_text(font20, al_map_rgb(255, 255, 255), largura_t/2, altura_t/2, ALLEGRO_ALIGN_CENTRE, "GAME OVER");
        al_flip_display();
    }
}

void record(){

    bool record = true;
    bool concluido = false;

    //EVENTOS E LÓGICA
    while(record)
    {
	     //INSTALAÇÃO E INICIAÇÃO
        ALLEGRO_EVENT ev;
	    al_wait_for_event(fila_eventos, &ev);
        //_________________________

        //-------------- DIGITANDO NA TELA ----------//
		if (!concluido)
	    {
	    manipular_entrada(ev);

	    	if (ev.type == ALLEGRO_EVENT_KEY_DOWN && ev.keyboard.keycode == ALLEGRO_KEY_ENTER)
	        {
	          concluido = true;
	        }
	    }
      //-------------- DIGITANDO NA TELA ----------//

	    //DESENHO
	   	al_clear_to_color(al_map_rgb(0, 0, 0));
        al_draw_bitmap(imagemRecord, 0, 0, 0);
        al_draw_filled_rectangle(140 ,80 , 500, 400, al_map_rgb(100, 100, 100));
        al_draw_rectangle(140 ,80 , 500, 400, al_map_rgb(255, 255, 255), 5);
        al_draw_text(font20, al_map_rgb(255,255, 255), largura_t/2, 100, ALLEGRO_ALIGN_CENTRE, "RECORDS");
        exibir_texto_centralizado();
        al_flip_display();
        //________________________

        //MOVIMENTO E SELECT
        if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
	    {
			switch(ev.keyboard.keycode)
			{
				case ALLEGRO_KEY_ESCAPE:
					return;
			}
	    }
	    //______________________________________
    }
    //____________________________________
}

void manipular_entrada(ALLEGRO_EVENT evento)
{
  if (evento.type == ALLEGRO_EVENT_KEY_CHAR)
  {
    if (strlen(str) <= 16)
    {
      char temp[] = {evento.keyboard.unichar, '\0'};
      if (evento.keyboard.unichar == ' ')
      {
        strcat(str, temp);
      }
      else if (evento.keyboard.unichar >= '0' &&
          evento.keyboard.unichar <= '9')
      {
        strcat(str, temp);
      }
      else if (evento.keyboard.unichar >= 'A' &&
          evento.keyboard.unichar <= 'Z')
      {
        strcat(str, temp);
      }
      else if (evento.keyboard.unichar >= 'a' &&
          evento.keyboard.unichar <= 'z')
      {
        strcat(str, temp);
      }
    }

    if (evento.keyboard.keycode == ALLEGRO_KEY_BACKSPACE && strlen(str) != 0)
    {
      str[strlen(str) - 1] = '\0';
    }
  }
}

void exibir_texto_centralizado()
{
  if (strlen(str) > 0)
  {
    al_draw_text(font20, al_map_rgb(255, 255, 255), largura_t / 2,
        (altura_t - al_get_font_ascent(font20)) / 2,
        ALLEGRO_ALIGN_CENTRE, str);
  }
}
