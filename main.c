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
//


//----- TIMER -----
ALLEGRO_TIMER *timer = NULL;

//-------
//------ SOUNDS ------
ALLEGRO_SAMPLE *menuSound = NULL ;
ALLEGRO_SAMPLE *gameSound = NULL ;
ALLEGRO_SAMPLE *recordSound = NULL ;
ALLEGRO_SAMPLE *selectOptionSound = NULL;
ALLEGRO_SAMPLE *changeOptionSound = NULL;
ALLEGRO_SAMPLE *keyboardSound = NULL;

ALLEGRO_SAMPLE_INSTANCE *inst_menuSound = NULL ;
ALLEGRO_SAMPLE_INSTANCE *inst_gameSound = NULL ;
ALLEGRO_SAMPLE_INSTANCE *inst_recordSound = NULL ;
//------


// ---------- FONTES ----------
ALLEGRO_FONT *font10 = NULL;
ALLEGRO_FONT *font15 = NULL;
ALLEGRO_FONT *font20 = NULL;
ALLEGRO_FONT *font40 = NULL;

//----------------

// ----------------- INICIALIZAÇÃO DA ALLEGRO --------------

ALLEGRO_DISPLAY *display = NULL;
ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;

//---------------

//----------- BITMAP -----------

ALLEGRO_BITMAP *imagemMenu = NULL;
ALLEGRO_BITMAP *imagemGame = NULL;
ALLEGRO_BITMAP *imagemRecord = NULL;
ALLEGRO_BITMAP *imagemGameOver = NULL;
ALLEGRO_BITMAP *slogan = NULL;
ALLEGRO_BITMAP *p1 = NULL;
ALLEGRO_BITMAP *p2 = NULL;
ALLEGRO_BITMAP *p3 = NULL;
ALLEGRO_BITMAP *p4 = NULL;
ALLEGRO_BITMAP *p5 = NULL;

//------------------

//------- ESCREVENDO TEXTO NA TELA -------//
void manipular_entrada(ALLEGRO_EVENT evento, char jogador[]);
//------- ESCREVENDO TEXTO NA TELA -------//


// ---------------------- pieces -----------
void init_piecesQueue(int queue[]);
void piecesQueue(int queue[]);
void init_piece(int map[][COLUNAS], int piece, int coordenada[], bool *portugues, int letras[]);
void draw_piece(int map[][COLUNAS]);
void girar(int map[][COLUNAS], int coordenada[], int p_atual);
void checar_linha(int map[][COLUNAS], int *score);
void limpar_matriz(int map[][COLUNAS]);
void shadow_piece(int map[][COLUNAS], int coordenada[]);
// ------------------

//---------- MOVIMENTO -----------
void move_baixo(int map[][COLUNAS], int coordenada_blocos[], bool *new_p, bool *game_over);
void move_esquerda(int map[][COLUNAS], int cordenada_blocos[]);
void move_direita(int map[][COLUNAS], int cordenada_blocos[]);
//-----------

void record();
void start_game(bool *portugues);
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

	al_reserve_samples(6);


    //CRIAR FILA DE EVENTOS E AFINS
    fila_eventos = al_create_event_queue();
    //

    //------ LOAD BITMAP ------
    imagemMenu = al_load_bitmap("res/image/menu.jpg");
    imagemGame = al_load_bitmap("res/image/start_game.jpg");
    imagemGameOver = al_load_bitmap("res/image/game_over.jpg");
    imagemRecord = al_load_bitmap("res/image/record.jpg");
    slogan = al_load_bitmap("res/image/slogan.png");
    p1 = al_load_bitmap("res/image/pieces/p0.png");
    p2 = al_load_bitmap("res/image/pieces/p1.png");
    p3 = al_load_bitmap("res/image/pieces/p2.png");
    p4 = al_load_bitmap("res/image/pieces/p3.png");
    p5 = al_load_bitmap("res/image/pieces/p4.png");
    //---------

    // --- SOUNDS ---
    menuSound = al_load_sample("res/sound/background_music/menu.wav");
    inst_menuSound = al_create_sample_instance(menuSound);
    al_attach_sample_instance_to_mixer(inst_menuSound, al_get_default_mixer());
    al_set_sample_instance_playmode(inst_menuSound, ALLEGRO_PLAYMODE_LOOP);
    al_set_sample_instance_gain(inst_menuSound, 0.6 );
    al_set_sample_instance_speed(inst_menuSound, 0.8 );

    gameSound = al_load_sample("res/sound/background_music/tokyo-ghoul.wav");
    inst_gameSound = al_create_sample_instance(gameSound);
    al_attach_sample_instance_to_mixer(inst_gameSound, al_get_default_mixer());
    al_set_sample_instance_playmode(inst_gameSound, ALLEGRO_PLAYMODE_LOOP);
    al_set_sample_instance_gain(inst_gameSound, 0.8 );
    al_set_sample_instance_speed(inst_gameSound, 1 );

    recordSound = al_load_sample("res/sound/background_music/record.wav");
    inst_recordSound = al_create_sample_instance(recordSound);
    al_attach_sample_instance_to_mixer(inst_recordSound, al_get_default_mixer());
    al_set_sample_instance_playmode(inst_recordSound, ALLEGRO_PLAYMODE_LOOP);
    al_set_sample_instance_gain(inst_recordSound, 0.6 );
    al_set_sample_instance_speed(inst_recordSound, 0.8 );

    selectOptionSound = al_load_sample("res/sound/effects/select_option.wav");
    changeOptionSound = al_load_sample("res/sound/effects/change_option.wav");
    keyboardSound = al_load_sample("res/sound/effects/keyboard.wav");

    // ---

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
    font10 = al_load_ttf_font("res/arial.TTF", 10, 0);
    font15 = al_load_ttf_font("res/arial.TTF", 15, 0);
    font20 = al_load_ttf_font("res/arial.TTF", 20, 0);
    font40 = al_load_ttf_font("res/arial.TTF", 40, 0);
    //

    al_start_timer(timer);

    menu(true, false);

    //---------- FIM ----------
	al_destroy_event_queue(fila_eventos);
   	al_destroy_display(display);
    al_destroy_sample(selectOptionSound);
    al_destroy_sample(changeOptionSound);
    al_destroy_sample(menuSound);
    al_destroy_sample_instance(inst_menuSound);
    al_destroy_font(font10);
    al_destroy_font(font15);
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
	bool menuGame = true;
	bool modeGame = false;
	bool *portugues = false;
	bool desenhoMenu = false;
	bool desenhoModeGame = false;
    //-------------------

    // --- MENU GAME ---
    while (!fim){
        while(menuGame){
            // ------------------- INSTALAÇÃO E INICIAÇÃO -----------------
            ALLEGRO_EVENT ev;
            al_play_sample_instance(inst_menuSound);
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

                        if (pos_y == 250){
                            desenhoMenu = false;
                            menuGame = false;
                            desenhoModeGame = true;
                            modeGame = true;
                        }
                        else if (pos_y == 290){
                            al_stop_sample_instance (inst_menuSound);
                            record();
                        }
                        else if (pos_y == 330){
                            menuGame = false;
                            desenhoMenu = false;
                            fim = true;
                        }
                        break;
                }
            }
            //

            else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            {
                menuGame = false;
                desenhoMenu = false;
                fim = true;
            }
            else if(ev.type == ALLEGRO_EVENT_TIMER){
                desenhoMenu = true;
            }


            if(desenhoMenu && al_is_event_queue_empty(fila_eventos)){
            // ---------------- DESENHO ---------------

                al_clear_to_color(al_map_rgb(0, 0, 0));
                al_draw_bitmap(imagemMenu, 0, 0, 0);
                al_draw_rectangle(largura_t/2 - 80, altura_t/4 - 10,  largura_t/2 + 80, altura_t/4 + 50, al_map_rgb(255, 255, 255), 3);
                al_draw_text(font40, al_map_rgb(255, 255, 255), largura_t/2, altura_t/4, ALLEGRO_ALIGN_CENTRE, "TETRIS");
                //al_draw_bitmap(slogan, largura_t/3 - 10, altura_t/4 - 40, 0);
                al_draw_text(font20, al_map_rgb(255, 255, 255), largura_t/2, altura_t/2, ALLEGRO_ALIGN_CENTRE, "Jogar");
                al_draw_text(font20, al_map_rgb(255, 255, 255), largura_t/2, altura_t/2 + 40, ALLEGRO_ALIGN_CENTRE, "Recordes");
                al_draw_text(font20, al_map_rgb(255, 255, 255), largura_t/2, altura_t/2 + 80, ALLEGRO_ALIGN_CENTRE, "Sair");
                al_draw_filled_circle(pos_x, pos_y, 10, al_map_rgb(255, 255, 0));
                al_flip_display();

            //------------------------
            }
        }
        //----------------

        // --- SELECT MODE GAME ---
        while(modeGame)
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

                        if (pos_y == 250){
                            al_stop_sample_instance (inst_menuSound);
                            modeGame = false;
                            desenhoModeGame = false;
                            start_game(&portugues);
                            menuGame = true;
                        }
                        else if (pos_y == 290){

                        }
                        break;
                    case ALLEGRO_KEY_ESCAPE:

                        modeGame = false;
                        desenhoModeGame = false;
                        menuGame = true;
                        pos_y = altura_t/2 + 10;

                        break;
                }
            }
            //

            else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            {
                modeGame = false;
                desenhoModeGame = false;
                fim = true;
            }
            else if(ev.type == ALLEGRO_EVENT_TIMER){
                desenhoModeGame = true;
            }


            if(desenhoModeGame && al_is_event_queue_empty(fila_eventos)){
            // ---------------- DESENHO ---------------

                al_clear_to_color(al_map_rgb(0, 0, 0));
                al_draw_bitmap(imagemMenu, 0, 0, 0);
                al_draw_rectangle(largura_t/2 - 80, altura_t/4 - 10,  largura_t/2 + 80, altura_t/4 + 50, al_map_rgb(255, 255, 255), 3);
                al_draw_text(font40, al_map_rgb(255, 255, 255), largura_t/2, altura_t/4, ALLEGRO_ALIGN_CENTRE, "TETRIS");
                al_draw_text(font20, al_map_rgb(255, 255, 255), largura_t/2, altura_t/2, ALLEGRO_ALIGN_CENTRE, "Usual");
                al_draw_text(font20, al_map_rgb(255, 255, 255), largura_t/2, altura_t/2 + 40, ALLEGRO_ALIGN_CENTRE, "Português");
                al_draw_text(font20, al_map_rgb(255, 255, 255), largura_t/2, altura_t/2 + 80, ALLEGRO_ALIGN_CENTRE, "Matemática");
                al_draw_filled_circle(pos_x, pos_y, 10, al_map_rgb(255, 255, 0));
                al_flip_display();

            //------------------------
            }
        }
    //----------------
    }

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

void init_piece(int map[][COLUNAS], int piece, int coordenada[], bool *portugues, int letras[]){
    int i;
    for(i = 0; i < BLOCOS; i++){
        int bloco = pieces[piece][i];
        coordenada[i] = bloco;
        map[0][bloco] = 1;
    }
}

void limpar_matriz(int map[][COLUNAS]){
    int i, j;
    // --- RESETAR A MATRIZ ---
    for(i = 0; i < LINHAS; i++){
        for(j = 0; j < COLUNAS; j++){
            if(map[i][j] == -1){
                map[i][j] = 0;
            }
        }
    }
}
void shadow_piece(int map[][COLUNAS], int coordenada[]){
    int i, j, aux_coordenada[4];
    bool stop = false;

    // --- COPIAR AS COORDENADAS PARA UMA AUXILIAR ---
    for(i = 0; i < BLOCOS; i++){
        aux_coordenada[i] = coordenada[i];
    }

    while(!stop){

        for(i = 0; i < BLOCOS; i++){
            if(aux_coordenada[i] + 10 >= LINHAS*10 || map[0][aux_coordenada[i] + 10] == 2){
                for(j = 0; j < BLOCOS; j++){
                    if(map[0][aux_coordenada[j]] != 1){
                        map[0][aux_coordenada[j]] = -1;
                    }
                }
                stop = true;
                break;

            }
        }

        for(i = 0; i < BLOCOS; i++){
            aux_coordenada[i] += 10;
        }
    }


}

void draw_piece(int map[][COLUNAS])
{
    int i, j, k;
    for(i = 0; i < LINHAS; i++){
        for(j = 0; j < COLUNAS; j++){
            if(map[i][j] == -1){
                al_draw_filled_rectangle(tela_x + (j*DIMENSAO_P), tela_y + (i*DIMENSAO_P) - 90, tela_x + (j*DIMENSAO_P) + 30, tela_y + (i*DIMENSAO_P) + 30 - 90, al_map_rgb(126,85,157));
            }
            else if(map[i][j] == 1 || map[i][j] == 2){
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

void girar(int map[][COLUNAS],int coordenada[], int p_atual)
{

    // --- VARIÁVEIS AUXILIARES ---
    int i, j, cont = 0;
    int coordenada_auxiliar[4];
    int menor_linha, menor_coluna, maior_linha, maior_coluna;
    // ---


    // --- COORDENADA DE START E FIM (LINHA E COLUNA) ---
    menor_linha = coordenada[0]/10;
    maior_linha = coordenada[0]/10;
    menor_coluna = coordenada[0]%10;
    maior_coluna = coordenada[0]%10;


    for(i = 0; i < 3; i++){
        if(coordenada[i+1]%10 < menor_coluna){
            menor_coluna = coordenada[i+1]%10;
        }
        if(coordenada[i+1]%10 > maior_coluna){
            maior_coluna = coordenada[i+1]%10;
        }

        if(coordenada[i+1]/10 < menor_linha){
            menor_linha = coordenada[i+1]/10;
        }
        if(coordenada[i+1]/10 > maior_linha){
            maior_linha = coordenada[i+1]/10;
        }
    }
    // ---

    // --- ALTERAR AS COORDENADAS (GIRAR A PEÇA) ---
    int altura = maior_coluna - menor_coluna;
    int comprimento = maior_linha - menor_linha;

    for(i = 0; i < 4; i++){
        int coluna;
        int linha;;

        coluna = coordenada[i]%10 - menor_coluna + maior_linha - altura;
        linha = menor_coluna + comprimento  - (coordenada[i]/10 - menor_linha);
        coordenada_auxiliar[i] = 10*coluna + linha;

        while(menor_coluna + comprimento > COLUNAS - 1){
            for(j = 0; j <= i; j++){
                coordenada_auxiliar[j]--;
            }
            comprimento--;
        }


    }
    // ---
    // --- CHECAR SE HÁ BLOCOS ADJACENTES ---
    for(i = 0; i < 4; i++){
        // --- SE HÁ, ELE NAO APLICA AS NOVAS COORDENADAS (BLOCOS) ---
        if(map[0][coordenada_auxiliar[i]] == 2){
            break;
        }
        // --- SE NÃO HÁ, ELE TROCA AS ANTIGAS COORDENADAS PELAS NOVAS ---
        if(i == 3){

            for(j = 0; j < 4; j++){
                map[0][coordenada[j]] = 0;
                coordenada[j] = coordenada_auxiliar[j];
            }
        }
    }
    //

    // --- APLICAR NO MAP ---
    for(i = 0; i < 4; i++){
        map[0][coordenada[i]] = 1;
    }
    // ---


    // --- DESENHAR ---
    draw_piece(map);
    //


}

void move_baixo(int map[][COLUNAS], int coordenada_blocos[], bool *new_p, bool *game_over)
{
    int i;
    for(i = 0; i< BLOCOS ; i++)
    {

        //SE ACHAR UMA PEÇA OU O LIMITE DO MAPA >> STOP
        if(coordenada_blocos[i] + 10 >= LINHAS*10 || map[0][coordenada_blocos[i] + 10] == 2)
        {
            //----- GAME OVER -----
            if(coordenada_blocos[i] <= 40){
                al_stop_sample_instance (inst_gameSound);
                *game_over = true;
                return;
            }
            // ---------
            for(i = 0; i<BLOCOS; i++)
            {
                map[0][coordenada_blocos[i]] = 2;
            }
            *new_p = true;
            return;
        }
    }
    for(i = 0; i < BLOCOS; i++)
    {
        map[0][coordenada_blocos[i]] = 0;
    }

    for(i = 0; i < BLOCOS; i++)
    {
        map[0][coordenada_blocos[i] + 10] = 1;
        coordenada_blocos[i] = coordenada_blocos[i] + 10;
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

void start_game(bool *portugues){

    //------------ VARIÁVEIS DO JOGO -------------
    bool *game_over = false;
    bool *new_p = true;
    bool pause = false;
    bool desenho = false;
    bool move_down = false;
    bool new_record = false;

    int map[18][10] = {0};
    int cordenada_blocos[BLOCOS], j, i;
    int letras[BLOCOS];
    int seg = 0, min = 0, contador = 0;
    int p_atual;
    int *score = 0;
    int score_table[10];

    enum {BAIXO, ESQUERDA, DIREITA};
    ALLEGRO_EVENT ev;

    char jogador_table[10][10];
    char jogador[17];

    FILE *dados;


    // --------------------

    //--------------- INICIAR ---------------
    init_piecesQueue(pieces_queue);
    al_play_sample_instance(inst_gameSound);
    strcpy(jogador, "");
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
				    al_stop_sample_instance (inst_gameSound);
				    game_over = true;
				    break;
                case ALLEGRO_KEY_SPACE:
                    girar(map, cordenada_blocos, p_atual);
                    break;
                case ALLEGRO_KEY_DOWN:
                    move_baixo(map, cordenada_blocos, &new_p, &game_over);
                    break;
                case ALLEGRO_KEY_UP:
                    while(!new_p && !(game_over)){
                        move_baixo(map, cordenada_blocos, &new_p, &game_over);
                    }
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
            // ----- CRONOMETRO ------
            contador++;
            if(contador == FPS){
                move_baixo(map, cordenada_blocos, &new_p, &game_over);
                seg++;
                contador = 0;

                if(seg == 60){
                    min++;
                    seg = 0;
                }
            }
            if(move_down && (contador == FPS/2)){
                    move_baixo(map, cordenada_blocos, &new_p, &game_over);
                    move_baixo(map, cordenada_blocos, &new_p, &game_over);
                }


            desenho = true;

	    }
	    //------------------------


	    // ------------DESENHO--------------

	    if(desenho){

            al_clear_to_color(al_map_rgb(0, 0, 0));
            al_draw_bitmap(imagemGame, 0, 0, 0);

            //RETANGULO DO GAME (300 x 4508)
            al_draw_rectangle(tela_x ,-10 , tela_x + 300, 450, al_map_rgb(255,62,0), 6);
            al_draw_filled_rectangle(tela_x ,0 , tela_x + 300, 450, al_map_rgb(31,10,47));
            //

            //INFORMAÇÕES DA DIREITA
            al_draw_textf(font20, al_map_rgb(255,231,6), 420, 20, ALLEGRO_ALIGN_INTEGER , "SCORE : %d", score);
            al_draw_textf(font20, al_map_rgb(255,231,6), 420, 60, ALLEGRO_ALIGN_INTEGER, "TIME : %d:%d", min, seg);
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
                init_piece(map, pieces_queue[0], cordenada_blocos, &portugues, letras);
                p_atual = pieces_queue[0];
                piecesQueue(pieces_queue);
                new_p = false;
            }

            limpar_matriz(map);
            shadow_piece(map, cordenada_blocos);
            draw_piece(map);
            al_flip_display();
            desenho = false;
        }

            //------------------------
    }
    //--------------------------


    // --- PROCURAR UM SCORE NA TABELA DE RECORDS MENOR QUE O DO JOGADOR ---
    dados = fopen("records.txt", "r");
    for(i = 0; i < 10; i++){
        fscanf(dados, "%s%d", &jogador_table[i], &score_table[i]);

        if(score > score_table[i]){
            new_record = true;
        }
    }
    close(dados);

    while(game_over){

        al_wait_for_event(fila_eventos, &ev);
        al_clear_to_color(al_map_rgb(0, 0, 0));
        al_clear_to_color(al_map_rgb(0, 0, 0));
        al_draw_bitmap(imagemGameOver, 0, 0, 0);

        if(new_record){
            // --- DIGITANDO NA TELA ---

            manipular_entrada(ev, jogador);

            if (ev.type == ALLEGRO_EVENT_KEY_DOWN && ev.keyboard.keycode == ALLEGRO_KEY_ENTER)
            {
                dados = fopen("records.txt", "w");
                for(i = 0; i < 10; i++){
                    if(score_table[i] < score){
                        fprintf(dados, "%s\t%d\n", jogador, score);
                        for(j = i; j < 9; j++){
                            fprintf(dados, "%s\t%d\n", jogador_table[j], score_table[j]);
                        }
                        break;
                    }
                    fprintf(dados, "%s\t%d\n", jogador_table[i], score_table[i]);
                }

                fclose(dados);
                game_over = false;
            }

            // ---


            // --- DESENHO ---

            al_draw_filled_rectangle(170 ,200 , 470, 400, al_map_rgb(100, 100, 100));
            al_draw_rectangle(170 ,200 , 470, 400, al_map_rgb(255, 255, 255), 2);

            al_draw_filled_rectangle(190 ,280 , 450, 320, al_map_rgb(0, 0, 0));
            al_draw_rectangle(190 ,280 , 450, 320, al_map_rgb(255, 255, 255), 2);


            al_draw_text(font20, al_map_rgb(255, 255, 255), largura_t / 2,
            ((altura_t - al_get_font_ascent(font20)) / 2),
            ALLEGRO_ALIGN_CENTRE, "NICKNAME");

            al_draw_text(font20, al_map_rgb(255, 255, 255), largura_t / 2,
            (altura_t - al_get_font_ascent(font20)) / 2 + 60,
            ALLEGRO_ALIGN_CENTRE, jogador);

            al_draw_text(font20, al_map_rgb(255, 255, 255), largura_t / 2, 140,
            ALLEGRO_ALIGN_CENTRE, "NOVO RECORD!" );

            // ---
        }
        else{

            if (ev.type == ALLEGRO_EVENT_KEY_DOWN && ev.keyboard.keycode == ALLEGRO_KEY_ENTER)
            {
                game_over = false;
            }

        }

        al_draw_text(font20, al_map_rgb(255, 255, 255), largura_t / 2, 100,
        ALLEGRO_ALIGN_CENTRE, "GAME OVER" );

        al_draw_textf(font20, al_map_rgb(255, 255, 255), largura_t / 2,
        ((altura_t - al_get_font_ascent(font20)) / 2) + 120,
        ALLEGRO_ALIGN_CENTRE, "SCORE: %d", score);

        al_flip_display();
    }
}

void record(){

    bool record = true;
    FILE *dados;
    int score[10];
    char jogador[10][17];
    int i;

    // --- INICIAR ---
    al_play_sample_instance(inst_recordSound);
    // ---

    // --- EVENTOS E LÓGICA ---
    while(record)
    {
	     // --- INSTALAÇÃO E INICIAÇÃO ---
        ALLEGRO_EVENT ev;
	    al_wait_for_event(fila_eventos, &ev);
        //_________________________

	    // --- DESENHO ---
	   	al_clear_to_color(al_map_rgb(0, 0, 0));
        al_draw_bitmap(imagemRecord, 0, 0, 0);
        al_draw_filled_rectangle(100 ,80 , 540, 400, al_map_rgb(100, 100, 100));
        al_draw_rectangle(100 ,80 , 540, 400, al_map_rgb(255, 255, 255), 5);
        al_draw_text(font15, al_map_rgb(255,255, 255), 60, 10, ALLEGRO_ALIGN_CENTRE, "ESC - VOLTAR");
        al_draw_text(font20, al_map_rgb(255,255, 255), largura_t/2, 100, ALLEGRO_ALIGN_CENTRE, "RECORDS");

        dados = fopen("records.txt", "r");

        for(i = 0; i < 5; i++){
            fscanf(dados, "%s%d", &jogador[i], &score[i]);
            al_draw_textf(font20, al_map_rgb(255,255, 255), 130, 160 + i*40, ALLEGRO_ALIGN_INTEGER, "%d - %s", i+1, jogador[i]);
            al_draw_textf(font20, al_map_rgb(255,255, 255), 280, 160 + i *40, ALLEGRO_ALIGN_INTEGER,"%d", score[i]);
        }
        for(i = 5; i < 10; i++){
            fscanf(dados, "%s%d", &jogador[i], &score[i]);
            al_draw_textf(font20, al_map_rgb(255,255, 255), 350, 160 + (i-5)*40, ALLEGRO_ALIGN_INTEGER,"%d - %s", i+1, jogador[i]);
            al_draw_textf(font20, al_map_rgb(255,255, 255), 500, 160 + (i-5)*40, ALLEGRO_ALIGN_INTEGER,"%d", score[i]);
        }

        close(dados);
        al_flip_display();

        //_ ---

        //MOVIMENTO E SELECT
        if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
	    {
			switch(ev.keyboard.keycode)
			{
				case ALLEGRO_KEY_ESCAPE:
				    al_stop_sample_instance (inst_recordSound);
					return;
			}
	    }
	    //______________________________________
    }
    //____________________________________
}

void manipular_entrada(ALLEGRO_EVENT evento, char jogador[])
{
    if (evento.type == ALLEGRO_EVENT_KEY_CHAR)
    {

        al_play_sample(keyboardSound, 2.0 , 0 , 1.5 , ALLEGRO_PLAYMODE_ONCE, 0 );

        if (strlen(jogador) <= 16)
        {
          char temp[] = {evento.keyboard.unichar, '\0'};
          if (evento.keyboard.unichar == ' ')
          {
            strcat(jogador, temp);
          }
          else if (evento.keyboard.unichar >= '0' &&
              evento.keyboard.unichar <= '9')
          {
            strcat(jogador, temp);
          }
          else if (evento.keyboard.unichar >= 'A' &&
              evento.keyboard.unichar <= 'Z')
          {
            strcat(jogador, temp);
          }
          else if (evento.keyboard.unichar >= 'a' &&
              evento.keyboard.unichar <= 'z')
          {
            strcat(jogador, temp);
          }
        }

        if (evento.keyboard.keycode == ALLEGRO_KEY_BACKSPACE && strlen(jogador) != 0)
        {
          jogador[strlen(jogador) - 1] = '\0';
        }
    }
}


