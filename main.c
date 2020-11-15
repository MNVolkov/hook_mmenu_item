/*
	(С) Волков Максим 2020 ( Maxim.N.Volkov@yandex.ru )
	
	Анимация значков главного меню

*/

#include <libbip.h>
#include "main.h"
					
int main(int proc, char** argv){	//	proc содержит указатель на proc

struct ani_struct_** 	anidata_p = get_ptr_temp_buf_2(); 	//	указатель на указатель на данные программы
struct ani_struct_ *	anidata;							//	указатель на на данные программы

anidata = (struct ani_struct_*)pvPortMalloc(sizeof(struct ani_struct_));
_memclr(anidata, sizeof(struct ani_struct_));

//	получим указатель на данные запущенного процесса структура Elf_proc_
anidata->proc = get_proc_by_addr(main);

/*
log_printf(5, "[main] anidata=0x%X; anidata->proc=0x%x; argv=0x%X; proc->argv=0x%X; \r\n", 
		anidata, 
		anidata->proc,
		argv, 
		anidata->proc->argv);
*/

// копируем данные отрисовки значков главного меню
_memcpy(&anidata->mi, (struct menu_items_draw_*) argv, sizeof(struct menu_items_draw_));

/*
log_printf(5, "[main] proc=0x%X; mi.regmenu=0x%X; mi.regmenu->current_screen=%d; mi.screen=%d;\r\n",
					  proc,    anidata->mi.regmenu, anidata->mi.regmenu->curr_scr, anidata->mi.screen);

log_printf(5, "[main] mi.prev_screen=%d; current_screen=%d; mi.prev_sscreen=%d; swipe_screen=%d; \r\n", 
					  anidata->mi.prev_screen,  
					  get_var_current_screen(), 
					  anidata->mi.prev_sscreen, 
					  get_var_swipe_screen_active_number());

log_printf(5, "[main] mi.icon_res.icon_main=%d; mi.icon_res.icon_label_ru=%d; mi.icon_res.icon_label_en=%d; \r\n", 
					  anidata->mi.icon_res.icon_main, 
					  anidata->mi.icon_res.icon_label_ru, 
					  anidata->mi.icon_res.icon_label_en);
*/

reg_menu(anidata->mi.regmenu, 0);

set_var_swipe_screen_active_number(anidata->mi.screen);

*anidata_p = anidata;

// запомним адрес указателя на функцию в которую необходимо вернуться после завершения данного экрана
if ( proc && anidata->proc->elf_finish ) 			//	если указатель на возврат передан, то возвоащаемся на него
	anidata->ret_f = anidata->proc->elf_finish;
else					//	если нет, то на циферблат
	anidata->ret_f = show_watchface;
		
// первый кадр нулевой
anidata->frame = 0;

// установка номера ресурса для надписи пункта меню
anidata->label =  (get_selected_locale() == locale_ru_RU)?(anidata->mi.icon_res.icon_label_ru):(anidata->mi.icon_res.icon_label_en);

switch (anidata->mi.regmenu->curr_scr){
	// установка кадров анимации для конкретного пункта меню
	case MAIN_MENU_STATUS:		{ anidata->frame = MAIN_MENU_STATUS_FIRST_FRAME;	anidata->last_frame = MAIN_MENU_STATUS_LAST_FRAME; 		break;}
	case MAIN_MENU_ACTIVITY:	{
		switch (anidata->mi.screen){
			case 0:	{ anidata->frame = MAIN_MENU_ACTIVITY_FIRST_FRAME; 	anidata->last_frame = MAIN_MENU_ACTIVITY_LAST_FRAME; 	break;}
			default: return ERROR_ITEM_NOT_FOUNT;
		}
		break;
	}
	case MAIN_MENU_WEATHER:	    { anidata->frame = MAIN_MENU_WEATHER_FIRST_FRAME; 	anidata->last_frame = MAIN_MENU_WEATHER_LAST_FRAME; 	break;}
	case MAIN_MENU_ALARM:		{ anidata->frame = MAIN_MENU_ALARM_FIRST_FRAME; 	anidata->last_frame = MAIN_MENU_ALARM_LAST_FRAME; 		break;}
	case MAIN_MENU_TIMER:		{
		switch (anidata->mi.screen){
			case 0:	{ anidata->frame = MAIN_MENU_TIMER_FIRST_FRAME; 	anidata->last_frame = MAIN_MENU_TIMER_LAST_FRAME; 		break;}
			default: return ERROR_ITEM_NOT_FOUNT;
		}
		break;
	}
	case MAIN_MENU_COMPASS:	    { anidata->frame = MAIN_MENU_COMPASS_FIRST_FRAME; 	anidata->last_frame = MAIN_MENU_COMPASS_LAST_FRAME; 	break;}
	case MAIN_MENU_OPTIONS:	    { anidata->frame = MAIN_MENU_OPTIONS_FIRST_FRAME; 	anidata->last_frame = MAIN_MENU_OPTIONS_LAST_FRAME; 	break;}
	case MAIN_MENU_ALIPAY:	    { anidata->frame = MAIN_MENU_ALIPAY_FIRST_FRAME; 	anidata->last_frame = MAIN_MENU_ALIPAY_LAST_FRAME; 		break;}
	default:	return ERROR_ITEM_NOT_FOUNT;
				//				{ anidata->frame = MAIN_MENU_UNDEF_FIRST_FRAME; 	anidata->last_frame = MAIN_MENU_UNFEF_LAST_FRAME; 		break;}
	
}

	
set_bg_color(COLOR_BLACK);
fill_screen_bg();
set_graph_callback_to_ram_1();
load_font();

//set_ptr_key_press_func(dispatch_animation);		//	установим обработчик нажатий на кнопку на период анимации
//set_ptr_menu_dispatch_func(dispatch_animation);	//	установим обработчик нажатий на период анимации
set_ptr_screen_job_func(screen_job);			//	установим обработчик экранного таймера
set_update_period(1, ANIMATION_FRAME_TIME);		//	установим период вызова обработчика экранного таймера
//vibrate(2, 100,100);
return ERROR_NONE;
}

int dispatch_animation(void *param){
struct ani_struct_** 	anidata_p = get_ptr_temp_buf_2(); 		//	указатель на указатель на номер кадра
struct ani_struct_ *	anidata = *anidata_p;					//	указатель на номер кадра


set_ptr_key_press_func(anidata->mi.regmenu->key_press);	//	установим обработчик нажатий на кнопку
set_ptr_menu_dispatch_func(anidata->mi.regmenu->dispatch_func);	//	установим обработчик нажатий пункта меню
set_ptr_screen_job_func(NULL);	//	установим обработчик экранного таймера
set_update_period(0, 0);		//	установим период вызова обработчика экранного таймера

// вызываем функцию возврата (обычно это меню запуска), в качестве параметра указываем адрес функции нашего приложения
show_menu(anidata->ret_f, (int) main);	
return 0;
}

int screen_job(){

struct ani_struct_** 	anidata_p = get_ptr_temp_buf_2();		//	указатель на указатель на номер кадра
struct ani_struct_ *	anidata = *anidata_p;					//	указатель на номер кадра

struct res_params_ res_params;
get_res_params(ELF_INDEX_SELF, anidata->frame, &res_params);

int ticks = get_tick_count();

/*
log_printf(5, "[screen_job] mi.icon_res.icon_main=%d; mi.icon_res.icon_label_ru=%d; mi.icon_res.icon_label_en=%d; anidata->label=%d; \r\n", 
					  anidata->mi.icon_res.icon_main, 
					  anidata->mi.icon_res.icon_label_ru, 
					  anidata->mi.icon_res.icon_label_en, 
					  anidata->label);
*/

set_bg_color(COLOR_BLACK);
fill_screen_bg();
set_graph_callback_to_ram_1();
load_font();

if (res_params.height > 136){
	// отрисовка только знака пункта меню
	show_elf_res_by_id(ELF_INDEX_SELF, anidata->frame, (176-res_params.width)/2, (176-res_params.height)/2);
	repaint_screen();
} else {
	//	отрисовка значка пугкта меню
	show_elf_res_by_id(ELF_INDEX_SELF, anidata->frame, (176-res_params.width)/2, (136-res_params.height)/2);
	repaint_screen_lines((136-res_params.height)/2, (136+res_params.height)/2);
	
	
	
	// отрисовка надписи пункта меню
	int label_y = 136 + anidata->last_frame - anidata->frame;	//	вертикальная координата надписи
	get_res_params(INDEX_MAIN_RES, anidata->label, &res_params);
	show_elf_res_by_id(INDEX_MAIN_RES, anidata->label, (176-res_params.width)/2, label_y);
	repaint_screen_lines(label_y, label_y + res_params.height);
}

ticks = ANIMATION_FRAME_TIME - (get_tick_count() - ticks);

 if ( anidata->frame < anidata->last_frame ) {
	anidata->frame++;
	
	//log_printf(5, "[screen_job] frame=%d/%d; label=%d; ticks=%d \r\n", anidata->frame, anidata->last_frame, anidata->label, ticks);

    set_update_period(1, ANIMATION_FRAME_TIME);
 }
  else
 {  
/*
	log_printf(5, "[screen_job] mi.regmenu=0x%X; mi.regmenu->current_screen=0x%X\r\n", 
								anidata->mi.regmenu, anidata->mi.regmenu->curr_scr);
	
	log_printf(5, "[screen_job] before: key_press=0x%X; dispatch=0x%X; screen_job=0x%x \r\n", 
					get_ptr_key_press_func(), 
					get_ptr_menu_dispatch_func(), 
					get_ptr_screen_job_func());


	log_printf(5, "[screen_job] set: key_press=0x%X; dispatch=0x%X; screen_job=0x%x \r\n", 
					anidata->mi.regmenu->key_press,
					anidata->mi.regmenu->dispatch_func,
					NULL);
	*/
//	set_ptr_key_press_func(anidata->mi.regmenu->key_press);	//	установим обработчик нажатий на кнопку
//	set_ptr_menu_dispatch_func(anidata->mi.regmenu->dispatch_func);	//	установим обработчик нажатий пункта меню
	set_ptr_screen_job_func(NULL);	//	установим обработчик экранного таймера
/*
	log_printf(5, "[screen_job] after: key_press=0x%X; dispatch=0x%X; screen_job=0x%x \r\n", 
					get_ptr_key_press_func(), 
					get_ptr_menu_dispatch_func(), 
					get_ptr_screen_job_func());
*/
	set_update_period(0, 0);		//	установим период вызова обработчика экранного таймера

	//log_printf(5, "[screen_job] ret_f=0x%X \r\n", anidata->ret_f);
	show_menu(anidata->ret_f, (int) main);
 }
 
 return 0;
}