#include "lv_app.h"
#include "stdio.h"
lv_obj_t * root_page;
Page_Sign_ Page_Sign;

static void Page_sign_False()
{
    Page_Sign.Sport = false;
    Page_Sign.Arm = false;
    Page_Sign.Motro_Communication = false;
    Page_Sign.BLE = false;
    Page_Sign.IMU = false;
    Page_Sign.Tracking = false;
    Page_Sign.Battery = false;
}

static void Page_Btn_Event_CB(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    uint8_t id = (uint8_t)e->user_data;
    if(code == LV_EVENT_CLICKED) 
    {
        Page_sign_False();
        *(&Page_Sign.Sport + id) = true;
    }
}

void lv_demo_myself()
{	
    lv_app_function_init();

    lv_obj_t * menu = lv_menu_create(lv_scr_act());
    /*背景颜色调整*/
    lv_color_t bg_color = lv_obj_get_style_bg_color(menu, 0);
    if(lv_color_brightness(bg_color) > 127) 
    {
        lv_obj_set_style_bg_color(menu, lv_color_darken(lv_obj_get_style_bg_color(menu, 0), 12), 0);
    }
    else
    {
        lv_obj_set_style_bg_color(menu, lv_color_darken(lv_obj_get_style_bg_color(menu, 0), 50), 0);
    }
	lv_obj_set_size(menu, lv_disp_get_hor_res(NULL), lv_disp_get_ver_res(NULL));
    lv_obj_center(menu);

    lv_obj_t * cont;
    lv_obj_t * section;

    /*返回按钮设置*/
    lv_obj_t * back_btn = lv_menu_get_main_header_back_btn(menu);
    lv_obj_clean(back_btn);
    
    lv_obj_set_size(back_btn,75,32);
    create_text(back_btn, USR_SYMBOL_Back, &my_style_pic, "返回", &my_style_font_DingTalk_JinBuTi_Chinese_16);

    /*隐藏界面*/
    /*PID*/
	lv_obj_t * sub_page_PID = lv_menu_page_create(menu, NULL);
    lv_obj_set_style_pad_hor(sub_page_PID, lv_obj_get_style_pad_left(lv_menu_get_main_header(menu), 0), 0);
    lv_menu_separator_create(sub_page_PID);
    PID_create(sub_page_PID);
    /*车轮调试*/
    lv_obj_t * sub_page_Wheel = lv_menu_page_create(menu, NULL);
    lv_obj_set_style_pad_hor(sub_page_Wheel, lv_obj_get_style_pad_left(lv_menu_get_main_header(menu), 0), 0);
	WHEEL_create(sub_page_Wheel);
    /*运动*/
    lv_obj_t * sub_page_Sport = lv_menu_page_create(menu, NULL);
    lv_obj_set_style_pad_hor(sub_page_Sport, lv_obj_get_style_pad_left(lv_menu_get_main_header(menu), 0), 0);
    lv_menu_separator_create(sub_page_Sport);/*分隔符*/
    section = lv_menu_section_create(sub_page_Sport);
    cont = create_text(section, USR_SYMBOL_Cycle, &my_style_pic, "PID", &my_style_font_DingTalk_JinBuTi_Chinese_16);
    lv_menu_set_load_page_event(menu, cont, sub_page_PID);
    cont = create_text(section, USR_SYMBOL_Wheel, &my_style_pic, "车轮调试", &my_style_font_DingTalk_JinBuTi_Chinese_16);
    lv_menu_set_load_page_event(menu, cont, sub_page_Wheel);

    /*机械臂*/
    lv_obj_t * sub_page_ARM = lv_menu_page_create(menu, NULL);
    lv_obj_set_style_pad_hor(sub_page_ARM, lv_obj_get_style_pad_left(lv_menu_get_main_header(menu), 0), 0);
	ARM_create(sub_page_ARM);
    /*电机通讯*/
    lv_obj_t * sub_page_Wheel_Communication = lv_menu_page_create(menu, NULL);
    lv_obj_set_style_pad_hor(sub_page_Wheel_Communication, lv_obj_get_style_pad_left(lv_menu_get_main_header(menu), 0), 0);
	Motro_Control_create(sub_page_Wheel_Communication);
    /*蓝牙*/
    lv_obj_t * sub_page_BLE = lv_menu_page_create(menu, NULL);
    lv_obj_set_style_pad_hor(sub_page_BLE, lv_obj_get_style_pad_left(lv_menu_get_main_header(menu), 0), 0);
	BLE_create(sub_page_BLE);
    /*陀螺仪*/
    lv_obj_t * sub_page_IMU = lv_menu_page_create(menu, NULL);
    lv_obj_set_style_pad_hor(sub_page_IMU, lv_obj_get_style_pad_left(lv_menu_get_main_header(menu), 0), 0);
	IMU_create(sub_page_IMU);
    /*循迹*/
    lv_obj_t * sub_page_Tracking = lv_menu_page_create(menu, NULL);
    lv_obj_set_style_pad_hor(sub_page_Tracking, lv_obj_get_style_pad_left(lv_menu_get_main_header(menu), 0), 0);
	Tracking_create(sub_page_Tracking);
    /*电池*/
    lv_obj_t * sub_page_Battery = lv_menu_page_create(menu, NULL);
    lv_obj_set_style_pad_hor(sub_page_Battery, lv_obj_get_style_pad_left(lv_menu_get_main_header(menu), 0), 0);
	Battery_create(sub_page_Battery);
    /*TF卡*/
    // lv_obj_t * sub_page_TF = lv_menu_page_create(menu, NULL);
    // lv_obj_set_style_pad_hor(sub_page_TF, lv_obj_get_style_pad_left(lv_menu_get_main_header(menu), 0), 0);
	// SD_create(sub_page_TF);
    
    /*主界面*/
	root_page = lv_menu_page_create(menu, NULL);
    lv_obj_set_style_pad_hor(root_page, lv_obj_get_style_pad_left(lv_menu_get_main_header(menu), 0), 0);
    /*第一块——控制*/
    create_text(root_page, USR_SYMBOL_Control, &my_style_pic, "控制", &my_style_font_DingTalk_JinBuTi_Chinese_16);
    section = lv_menu_section_create(root_page);
    /*运动*/
    cont = create_text(section, USR_SYMBOL_Car, &my_style_pic, "运动", &my_style_font_DingTalk_JinBuTi_Chinese_16);
    lv_menu_set_load_page_event(menu, cont, sub_page_Sport);
    lv_obj_add_event_cb(cont, Page_Btn_Event_CB,LV_EVENT_ALL,0);
    /*机械臂*/
    cont = create_text(section, USR_SYMBOL_Arm, &my_style_pic, "机械臂", &my_style_font_DingTalk_JinBuTi_Chinese_16);
    lv_menu_set_load_page_event(menu, cont, sub_page_ARM);
    lv_obj_add_event_cb(cont, Page_Btn_Event_CB,LV_EVENT_ALL,(void*)1);
    /*电机通讯*/
    cont = create_text(section, USR_SYMBOL_Communication, &my_style_pic, "电机通讯", &my_style_font_DingTalk_JinBuTi_Chinese_16);
    lv_menu_set_load_page_event(menu, cont, sub_page_Wheel_Communication);
    lv_obj_add_event_cb(cont, Page_Btn_Event_CB,LV_EVENT_ALL,(void*)2);
    /*第二块——信息*/
    create_text(root_page, USR_SYMBOL_Infomation, &my_style_pic, "信息", &my_style_font_DingTalk_JinBuTi_Chinese_16);
    section = lv_menu_section_create(root_page);
    /*蓝牙*/
    cont = create_text(section, USR_SYMBOL_BLE, &my_style_pic, "蓝牙", &my_style_font_DingTalk_JinBuTi_Chinese_16);
    lv_menu_set_load_page_event(menu, cont, sub_page_BLE);
    lv_obj_add_event_cb(cont, Page_Btn_Event_CB,LV_EVENT_ALL,(void*)3);
    /*陀螺仪*/
    cont = create_text(section, USR_SYMBOL_IMU, &my_style_pic, "陀螺仪", &my_style_font_DingTalk_JinBuTi_Chinese_16);
    lv_menu_set_load_page_event(menu, cont, sub_page_IMU);
    lv_obj_add_event_cb(cont, Page_Btn_Event_CB,LV_EVENT_ALL,(void*)4);
    /*循迹*/
    cont = create_text(section, USR_SYMBOL_Grid, &my_style_pic, "循迹", &my_style_font_DingTalk_JinBuTi_Chinese_16);
    lv_menu_set_load_page_event(menu, cont, sub_page_Tracking);
    lv_obj_add_event_cb(cont, Page_Btn_Event_CB,LV_EVENT_ALL,(void*)5);
    /*电池*/
    cont = create_text(section, USR_SYMBOL_Battery, &my_style_pic, "电池", &my_style_font_DingTalk_JinBuTi_Chinese_16);
    lv_menu_set_load_page_event(menu, cont, sub_page_Battery);
    lv_obj_add_event_cb(cont, Page_Btn_Event_CB,LV_EVENT_ALL,(void*)6);
    /*TF卡*/
    // cont = create_text(section, USR_SYMBOL_Battery, &my_style_pic, "TF卡", &my_style_font_DingTalk_JinBuTi_Chinese_16);
    // lv_menu_set_load_page_event(menu, cont, sub_page_TF);
    
    lv_menu_set_sidebar_page(menu, root_page);
}

void LVGL_Display()
{    
    if(Page_Sign.Sport)                     PID_Display();
    else if(Page_Sign.Arm)                  Arm_Display();
    else if(Page_Sign.Motro_Communication)  Motro_Control_Display();
    else if(Page_Sign.IMU)                  IMU_Display();
    else if(Page_Sign.BLE)                  BLE_Display();
    else if(Page_Sign.Tracking)             Tracking_Display();
    else if(Page_Sign.Battery)              Battery_Display();
}
