/*
	(С) Волков Максим 2020 ( Maxim.N.Volkov@yandex.ru )
	
	Анимация значков главного меню

*/

#ifndef __MENU_ANIM_H__
#define __MENU_ANIM_H__

struct ani_struct_ {
	
	struct menu_items_draw_ mi;		//	указатель на исзодные данные пункта меню
	int 	frame;					//	номер отображаемого кадра
	int 	last_frame;				//	последний кадр анимации
	int 	label;					//	номер ресурса для надписи
	Elf_proc_* proc;				//	указатель на данные запущенного процесса
	void* 	ret_f;					//	адрес функции возврата
};

#define MAIN_MENU_STATUS_FIRST_FRAME	0
#define MAIN_MENU_STATUS_LAST_FRAME		8
#define MAIN_MENU_ACTIVITY_FIRST_FRAME	57
#define MAIN_MENU_ACTIVITY_LAST_FRAME	66
#define MAIN_MENU_WEATHER_FIRST_FRAME	42
#define MAIN_MENU_WEATHER_LAST_FRAME	51	 
#define MAIN_MENU_ALARM_FIRST_FRAME	    20
#define MAIN_MENU_ALARM_LAST_FRAME		30
#define MAIN_MENU_TIMER_FIRST_FRAME	    9
#define MAIN_MENU_TIMER_LAST_FRAME		19
#define MAIN_MENU_COMPASS_FIRST_FRAME   31
#define MAIN_MENU_COMPASS_LAST_FRAME	41
#define MAIN_MENU_OPTIONS_FIRST_FRAME	52 
#define MAIN_MENU_OPTIONS_LAST_FRAME	56	 
#define MAIN_MENU_ALIPAY_FIRST_FRAME    67
#define MAIN_MENU_ALIPAY_LAST_FRAME	    73
#define MAIN_MENU_UNDEF_FIRST_FRAME   	67
#define MAIN_MENU_UNFEF_LAST_FRAME	    74

#define ERROR_NONE					0
#define ERROR_ITEM_NOT_FOUNT		1

#define ANIMATION_FRAME_TIME		5

// main.c
int 	screen_job();
void 	show_screen(void* param);
int dispatch_animation(void *param);
void 	key_press_screen();

#endif