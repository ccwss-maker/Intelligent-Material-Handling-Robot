#include "lv_app_pid.h"
#include "pid.h"
#include "wheel.h"
#include "Master_Slave_Communication.h"
#include "math.h"
#include "stdio.h"
#include "stdlib.h"
lv_obj_PID_ lv_obj_PID;
lv_obj_t * sub_page_speed_cont;
lv_obj_t * sub_page_position_cont;

static void chart_clear(lv_obj_t * obj)
{
    lv_obj_t * sw_pid_debug = obj;
    lv_obj_t * sw_ble_control = lv_obj_get_child(obj->parent, 3);
    lv_obj_t * ref_label = lv_obj_get_child(obj->parent, 4);
    lv_obj_t * set_label = lv_obj_get_child(obj->parent, 5);
    lv_obj_t * slider = lv_obj_get_child(obj->parent, 7);
    uint8_t ID_PID = (uint8_t)obj->parent->parent->parent->user_data;
    lv_obj_PID_KID_ *lv_obj_PID_KID = &lv_obj_PID.speed+ID_PID;

    lv_obj_clear_state(sw_pid_debug, LV_STATE_CHECKED);
    lv_obj_clear_state(sw_ble_control, LV_STATE_CHECKED);
    lv_obj_add_state(sw_ble_control, LV_STATE_DISABLED);
    lv_obj_add_state(slider, LV_STATE_DISABLED);
    lv_slider_set_value(slider,0 , LV_ANIM_ON);
    lv_label_set_text(ref_label, "ref : 0");
    lv_label_set_text(set_label, "set : 0");
    lv_chart_set_all_value(lv_obj_PID_KID->Chart.Chart, lv_obj_PID_KID->Chart.ref.series, 0);
    lv_chart_set_all_value(lv_obj_PID_KID->Chart.Chart, lv_obj_PID_KID->Chart.set.series, 0);
    lv_chart_refresh(lv_obj_PID_KID->Chart.Chart);
}

static void switch_event_cb_PID_Setting(lv_event_t * e)
{
    lv_obj_t * obj = lv_event_get_target(e);
    if(lv_obj_has_state(obj, LV_STATE_CHECKED))
    {
        Wheel.PID_Sign = true;
        lv_obj_clear_state(sub_page_speed_cont, LV_STATE_DISABLED);
        lv_obj_clear_state(sub_page_position_cont, LV_STATE_DISABLED);
    }
    else
    {
        Wheel.PID_Sign = false;
        lv_obj_add_state(sub_page_speed_cont, LV_STATE_DISABLED);
        lv_obj_add_state(sub_page_position_cont, LV_STATE_DISABLED);
        PID[0].debug_sign = false;
        PID[1].debug_sign = false;
        PID[0].BLE_debug_sign = false;
        PID[1].BLE_debug_sign = false;
        PID_Debug_Set_Value = 0;
        chart_clear(lv_obj_PID.speed.sw_pid_debug);
        chart_clear(lv_obj_PID.position.sw_pid_debug);
    }
}

static void switch_event_cb_PID_Debug(lv_event_t * e)
{
    lv_obj_t * obj = lv_event_get_target(e);
    lv_obj_t * sw_ble_control = lv_obj_get_child(obj->parent, 3);
    lv_obj_t * slider = lv_obj_get_child(obj->parent, 7);
    uint8_t ID_PID = (uint8_t)lv_event_get_user_data(e);
    if(lv_obj_has_state(obj, LV_STATE_CHECKED))
    {
        switch (ID_PID)
        {
        case 0:
            PID[1].debug_sign = false;
            PID[1].BLE_debug_sign = false;
            PID_Debug_Set_Value = 0;
            chart_clear(lv_obj_PID.position.sw_pid_debug);
            break;
        case 1:
            PID[0].debug_sign = false;
            PID[0].BLE_debug_sign = false;
            PID_Debug_Set_Value = 0;
            chart_clear(lv_obj_PID.speed.sw_pid_debug);
            break;
        }
        lv_obj_clear_state(sw_ble_control, LV_STATE_DISABLED);
        lv_obj_clear_state(slider, LV_STATE_DISABLED);
        PID_clear(&PID[ID_PID]);
        PID[ID_PID].debug_sign = true;
        // Wheel_Open();
    }
    else
    {
        PID[ID_PID].debug_sign = false;
        chart_clear(obj);
        // Wheel_Close();
    }
}

static void switch_event_cb_BLE_Debug(lv_event_t * e)
{
    lv_obj_t * obj = lv_event_get_target(e);
    lv_obj_t * slider_value_set = lv_obj_get_child(obj->parent, 7);
    uint8_t ID_PID = (uint8_t)lv_event_get_user_data(e);
    if(lv_obj_has_state(obj, LV_STATE_CHECKED))
    {
        PID[ID_PID].BLE_debug_sign = true;
        lv_obj_add_state(slider_value_set, LV_STATE_DISABLED);
    }
    else
    {
        PID[ID_PID].BLE_debug_sign = false;
        lv_obj_clear_state(slider_value_set, LV_STATE_DISABLED);
    }
}

// static void slider_event_cb_value_set(lv_event_t * e)
// {
//     lv_obj_t * obj = lv_event_get_target(e);
//     lv_obj_t * obj_label = lv_obj_get_child(obj->parent, 3);
//     char buf[20];
//     int slider_value = (int)lv_slider_get_value(obj);
//     lv_snprintf(buf, sizeof(buf), "set : %d", slider_value);
//     lv_label_set_text(obj_label, buf);
// }

static void btn_event_handler_save(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED) 
	{
        lv_obj_t * obj = lv_event_get_target(e);
        uint8_t ID_PID = (uint8_t)lv_event_get_user_data(e);/*0:速度 1:角度*/  
        char data[100]={0};
        sprintf(data, "P:%07.3f\nI:%07.3f\nD:%07.3f\nMIO:%07.3f\nMO:%07.3f\nIST:%07.3f\nDZ:%07.3f\n",
                PID[ID_PID].Kp,PID[ID_PID].Ki,PID[ID_PID].Kd,PID[ID_PID].max_iout,PID[ID_PID].max_out,PID[ID_PID].Integral_Separation_Threshold,PID[ID_PID].Dead_Zone);
        lv_fs_file_t lv_file;
		lv_fs_res_t  lv_res;
        switch (ID_PID)
        {
            case 0:
                lv_res = lv_fs_open( &lv_file, "0:/PID/PID_Speed.txt", LV_FS_MODE_WR);
                break;
            case 1:
                lv_res = lv_fs_open( &lv_file, "0:/PID/PID_Position.txt", LV_FS_MODE_WR);
                break;
            default:
                return;
        }
		if ( lv_res == LV_FS_RES_OK) 
		{
			uint32_t write_num;
            lv_res = lv_fs_write(&lv_file, data, strlen(data), &write_num);
			lv_fs_close(&lv_file);
            if(lv_res == LV_FS_RES_OK)
            {
                lv_obj_t * btn_save_label = lv_obj_get_child(obj, 0);
                lv_label_set_text(btn_save_label, "成功");
                lv_obj_add_state(obj, LV_STATE_DISABLED);
            }
		} 
		else
		{
            lv_obj_t * btn_save_label = lv_obj_get_child(obj, 0);
            lv_label_set_text(btn_save_label, "失败");
		}
    }
}

static void btn_event_handler_clear(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED) 
	{
        uint8_t ID_PID = (uint8_t)lv_event_get_user_data(e);
        lv_obj_t * obj = lv_event_get_target(e);
        lv_obj_t * button_clear_label = lv_obj_get_child(obj, 0);
        lv_obj_t* spinbox;
        for(uint8_t i=0; i<7; i++)
        {
            spinbox = lv_obj_get_child(lv_obj_get_child(obj->parent->parent, 2+i), 2);
            lv_spinbox_set_value(spinbox, 0);
            *(&(PID[ID_PID].Kp)+i) = 0; 
        }
        lv_obj_add_state(obj, LV_STATE_DISABLED);
        lv_label_set_text(button_clear_label, "成功");
    }
}

static void btn_event_handler_add_sub(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_target(e);
    lv_obj_t * spinbox = lv_obj_get_child(obj->parent, 2);
    if(code == LV_EVENT_SHORT_CLICKED) 
	{
        lv_obj_t * button_clear = lv_obj_get_child(lv_obj_get_child(obj->parent->parent, 1), 1);
        lv_obj_t * button_clear_label = lv_obj_get_child(button_clear, 0);
        uint8_t ID_PID = (uint8_t)lv_event_get_user_data(e);
        switch(lv_obj_get_child_id(obj))
        {
            case 1:
                lv_spinbox_decrement(spinbox);
                break;
            case 3:
                lv_spinbox_increment(spinbox);
        }
        float divisor = (float)pow(10, ((lv_spinbox_t*)spinbox)->digit_count - ((lv_spinbox_t*)spinbox)->dec_point_pos);
        *(&(PID[ID_PID].Kp)+(uint8_t)spinbox->user_data) = lv_spinbox_get_value((lv_obj_t*)spinbox)/divisor;
        lv_obj_t * btn_save = lv_obj_get_child(lv_obj_get_child(obj->parent->parent, 0), 1);
        lv_obj_t * btn_save_label = lv_obj_get_child(btn_save, 0);
        lv_label_set_text(btn_save_label, "执行");
        lv_obj_clear_state(btn_save, LV_STATE_DISABLED);
        lv_obj_clear_state(button_clear, LV_STATE_DISABLED);
        lv_label_set_text(button_clear_label, "执行");
    }
    else if(code == LV_EVENT_LONG_PRESSED)
    {
        switch(lv_obj_get_child_id(obj))
        {
            case 1:
                lv_spinbox_step_prev(spinbox);
                break;
            case 3:
                lv_spinbox_step_next(spinbox);
        }
    }
}

static lv_obj_t * legend_label_creat(lv_obj_t * parent, const char * txt_0, const char * txt_1, const char * txt_2,
                                    lv_coord_t min, lv_coord_t max, lv_coord_t val)
{
	lv_obj_t * obj = lv_menu_cont_create(parent);
	static lv_coord_t col_dsc[] = {1, 55, 50, 62, LV_GRID_TEMPLATE_LAST};
	static lv_coord_t row_dsc[] = {25, 25, 7, 135, 15, LV_GRID_TEMPLATE_LAST}; /*3 100 px tall rows*/
	static lv_style_t style_red, style_blue;
	lv_obj_t * label;
    lv_obj_t * sw;
	lv_obj_set_grid_dsc_array(obj, col_dsc, row_dsc);

	lv_style_init(&style_red);
	lv_style_set_text_color(&style_red,lv_palette_main(LV_PALETTE_RED));
	lv_style_set_text_font(&style_red,&DingTalk_JinBuTi_English_14);
	lv_style_init(&style_blue);
	lv_style_set_text_color(&style_blue,lv_palette_main(LV_PALETTE_BLUE));
	lv_style_set_text_font(&style_blue,&DingTalk_JinBuTi_English_14);
	/*表头*/
	label = lv_label_create(obj);
	lv_obj_add_style(label, &my_style_font_DingTalk_JinBuTi_Chinese_16,LV_STATE_DEFAULT);
	lv_label_set_text(label, txt_0);
	lv_obj_set_grid_cell(label, LV_GRID_ALIGN_START, 0, 1, LV_GRID_ALIGN_CENTER, 0, 1);
	/*PID调试开关*/
    sw = lv_switch_create(obj);
    lv_obj_set_width(sw,70);
	lv_obj_set_grid_cell(sw, LV_GRID_ALIGN_END, 0, 4, LV_GRID_ALIGN_CENTER, 0, 1);
    /*蓝牙控制*/
	label = lv_label_create(obj);
	lv_obj_add_style(label, &my_style_font_DingTalk_JinBuTi_Chinese_16,LV_STATE_DEFAULT);
	lv_label_set_text(label, "蓝牙调试");
	lv_obj_set_grid_cell(label, LV_GRID_ALIGN_START, 0, 1, LV_GRID_ALIGN_CENTER, 1, 1);
	/*蓝牙控制开关*/
    sw = lv_switch_create(obj);
    lv_obj_set_width(sw,70);
	lv_obj_set_grid_cell(sw, LV_GRID_ALIGN_END, 0, 4, LV_GRID_ALIGN_CENTER, 1, 1);
    /*图例*/
	label = lv_label_create(obj);
	lv_obj_add_style(label, &style_red, LV_STATE_DEFAULT);
	lv_label_set_text(label, txt_1);
	lv_obj_set_grid_cell(label, LV_GRID_ALIGN_START, 1, 1, LV_GRID_ALIGN_START, 2, 1);
	label = lv_label_create(obj);
	lv_obj_add_style(label, &style_blue, LV_STATE_DEFAULT);
	lv_label_set_text(label, txt_2);
	lv_obj_set_grid_cell(label, LV_GRID_ALIGN_START, 3, 1, LV_GRID_ALIGN_START, 2, 1);
	/*表格*/
	lv_obj_t * chart;
	chart = lv_chart_create(obj);
	lv_obj_set_size(chart,160,130);
	lv_chart_set_update_mode(chart,LV_CHART_UPDATE_MODE_SHIFT);
	lv_chart_set_range(chart, LV_CHART_AXIS_PRIMARY_Y, 10*min, 10*max);
	lv_chart_set_div_line_count(chart,5,4);
	lv_chart_set_axis_tick(chart, LV_CHART_AXIS_PRIMARY_Y, 10, 5, 5, 5, true, 40);
	lv_obj_set_grid_cell(chart, LV_GRID_ALIGN_END, 0, 4, LV_GRID_ALIGN_END, 3, 1);
    /*滑杆*/
    lv_obj_t * slider = lv_slider_create(obj);
	lv_obj_set_width(slider,160);
    lv_slider_set_range(slider, min*10, max*10);
    lv_slider_set_value(slider, val, LV_ANIM_ON);
    lv_obj_add_flag(slider, LV_OBJ_FLAG_FLEX_IN_NEW_TRACK);
	lv_obj_set_grid_cell(slider, LV_GRID_ALIGN_CENTER, 0, 4, LV_GRID_ALIGN_END, 4, 1);

	return obj;
}

static lv_obj_t * lv_menu_page_create_PID(lv_obj_t *parent, const char *const title, lv_obj_PID_KID_ *lv_obj_PID_KID)
{
    uint8_t ID_PID = lv_obj_PID_KID->ID;
    lv_obj_t * sub_page = lv_menu_page_create(parent, NULL);
    sub_page->user_data = ID_PID;
    lv_obj_set_style_pad_hor(sub_page, lv_obj_get_style_pad_left(lv_menu_get_main_header(parent), 0), 0);
    
    lv_obj_t * cont;
    lv_obj_t * section;

    /*第一块*/
    section = lv_menu_section_create(sub_page);
    /*Z轴速度*/
	/*图例*/
    if(ID_PID == 0)     /* 速度   */   
    {
        cont = legend_label_creat(section, title, "ref : 0", "set : 0", -2, 2, 0);
    }
    else if(ID_PID == 1)/* 角度   */   
    {
        cont = legend_label_creat(section, title, "ref : 0", "set : 0", -90, 90, 0);
    }
    /*调试开关*/
    lv_obj_PID_KID->sw_pid_debug = lv_obj_get_child(cont, 1);
    lv_obj_add_event_cb(lv_obj_PID_KID->sw_pid_debug, switch_event_cb_PID_Debug, LV_EVENT_VALUE_CHANGED, ID_PID);
    /*蓝牙控制开关*/
    lv_obj_PID_KID->sw_ble_control = lv_obj_get_child(cont, 3);
    lv_obj_add_state(lv_obj_PID_KID->sw_ble_control, LV_STATE_DISABLED);
	lv_obj_add_event_cb(lv_obj_PID_KID->sw_ble_control, switch_event_cb_BLE_Debug, LV_EVENT_VALUE_CHANGED, ID_PID);
    /*表格*/
    lv_obj_PID_KID->Chart.ref.label = lv_obj_get_child(cont, 4);
	lv_obj_PID_KID->Chart.set.label = lv_obj_get_child(cont, 5);
	lv_obj_PID_KID->Chart.Chart = lv_obj_get_child(cont, 6);
	lv_obj_PID_KID->Chart.ref.series = lv_chart_add_series(lv_obj_PID_KID->Chart.Chart, lv_palette_main(LV_PALETTE_RED),LV_CHART_AXIS_PRIMARY_Y);
	lv_obj_PID_KID->Chart.set.series = lv_chart_add_series(lv_obj_PID_KID->Chart.Chart, lv_palette_main(LV_PALETTE_BLUE),LV_CHART_AXIS_PRIMARY_Y);
    /*滑杆*/
    lv_obj_PID_KID->slider_value_set = lv_obj_get_child(cont,7);
    lv_obj_add_state(lv_obj_PID_KID->slider_value_set, LV_STATE_DISABLED);
    // lv_obj_add_event_cb(lv_obj_PID_KID->slider_value_set, slider_event_cb_value_set, LV_EVENT_VALUE_CHANGED, NULL);
    /*第二块*/
    lv_menu_separator_create(sub_page);
    section = lv_menu_section_create(sub_page);
    /*保存配置按钮*/
    cont = create_btn(section,NULL, NULL, "保存配置", &my_style_font_DingTalk_JinBuTi_Chinese_16);
	lv_obj_t* btn_save = lv_obj_get_child(cont, 1);
    lv_obj_set_width(btn_save,70);
    lv_obj_add_event_cb(btn_save, btn_event_handler_save,LV_EVENT_ALL,ID_PID);
    lv_obj_add_state(btn_save, LV_STATE_DISABLED);
    lv_obj_t* btn_save_label = lv_label_create(btn_save);	
    lv_obj_add_style(btn_save_label,&my_style_font_DingTalk_JinBuTi_Chinese_16,LV_STATE_DEFAULT);
    lv_label_set_text(btn_save_label, "执行");
    lv_obj_align(btn_save_label, LV_ALIGN_CENTER, 0, 0);
    /*参数清零*/
    cont = create_btn(section,NULL, NULL, "参数清零", &my_style_font_DingTalk_JinBuTi_Chinese_16);
	lv_obj_t* btn_clear = lv_obj_get_child(cont, 1);
    lv_obj_set_width(btn_clear,70);
    lv_obj_add_event_cb(btn_clear, btn_event_handler_clear,LV_EVENT_ALL,ID_PID);
    lv_obj_add_state(btn_clear, LV_STATE_DISABLED);
    lv_obj_t* btn_clear_label = lv_label_create(btn_clear);	
    lv_obj_add_style(btn_clear_label,&my_style_font_DingTalk_JinBuTi_Chinese_16,LV_STATE_DEFAULT);
    lv_label_set_text(btn_clear_label, "执行");
    lv_obj_align(btn_clear_label, LV_ALIGN_CENTER, 0, 0);
    /*微调器*/
    float divisor;
    lv_obj_t* spinbox;
    /*P*/
    cont = create_spinbox(section, NULL, NULL, "P", &my_style_font_DingTalk_JinBuTi_English_14, 5,1,0);
    lv_obj_PID_KID->PID.P.button.Sub = lv_obj_get_child(cont,1);
    lv_obj_PID_KID->PID.P.spinbox = lv_obj_get_child(cont,2);
    lv_obj_PID_KID->PID.I.spinbox->user_data = (void*)0;
    spinbox = lv_obj_PID_KID->PID.P.spinbox;
    divisor = (float)pow(10, ((lv_spinbox_t*)spinbox)->digit_count - ((lv_spinbox_t*)spinbox)->dec_point_pos);
    lv_spinbox_set_value(spinbox, PID[ID_PID].Kp*divisor);
    lv_obj_PID_KID->PID.P.button.Add = lv_obj_get_child(cont,3);
    lv_obj_add_event_cb(lv_obj_PID_KID->PID.P.button.Sub, btn_event_handler_add_sub,LV_EVENT_ALL,ID_PID);
    lv_obj_add_event_cb(lv_obj_PID_KID->PID.P.button.Add, btn_event_handler_add_sub,LV_EVENT_ALL,ID_PID);
    /*I*/
    cont = create_spinbox(section, NULL, NULL, "I", &my_style_font_DingTalk_JinBuTi_English_14, 5,1,0);
    lv_obj_PID_KID->PID.I.button.Sub = lv_obj_get_child(cont,1);
    lv_obj_PID_KID->PID.I.spinbox = lv_obj_get_child(cont,2);
    lv_obj_PID_KID->PID.I.spinbox->user_data = (void*)1;
    spinbox = lv_obj_PID_KID->PID.I.spinbox;
    divisor = (float)pow(10, ((lv_spinbox_t*)spinbox)->digit_count - ((lv_spinbox_t*)spinbox)->dec_point_pos);
    lv_spinbox_set_value(spinbox, PID[ID_PID].Ki*divisor);
    lv_obj_PID_KID->PID.I.button.Add = lv_obj_get_child(cont,3);
    lv_obj_add_event_cb(lv_obj_PID_KID->PID.I.button.Sub, btn_event_handler_add_sub,LV_EVENT_ALL,ID_PID);
    lv_obj_add_event_cb(lv_obj_PID_KID->PID.I.button.Add, btn_event_handler_add_sub,LV_EVENT_ALL,ID_PID);
    /*D*/
    cont = create_spinbox(section, NULL, NULL, "D", &my_style_font_DingTalk_JinBuTi_English_14, 5,1,0);
    lv_obj_PID_KID->PID.D.button.Sub = lv_obj_get_child(cont,1);
    lv_obj_PID_KID->PID.D.spinbox = lv_obj_get_child(cont,2);
    lv_obj_PID_KID->PID.D.spinbox->user_data = (void*)2;
    spinbox = lv_obj_PID_KID->PID.D.spinbox;
    divisor = (float)pow(10, ((lv_spinbox_t*)spinbox)->digit_count - ((lv_spinbox_t*)spinbox)->dec_point_pos);
    lv_spinbox_set_value(spinbox, PID[ID_PID].Kd*divisor);
    lv_obj_PID_KID->PID.D.button.Add = lv_obj_get_child(cont,3);
    lv_obj_add_event_cb(lv_obj_PID_KID->PID.D.button.Sub, btn_event_handler_add_sub,LV_EVENT_ALL,ID_PID);
    lv_obj_add_event_cb(lv_obj_PID_KID->PID.D.button.Add, btn_event_handler_add_sub,LV_EVENT_ALL,ID_PID);
    /*最大输出*/
    cont = create_spinbox(section, NULL, NULL, "MO", &my_style_font_DingTalk_JinBuTi_English_14, 5,2,0);
    lv_obj_PID_KID->PID.max_out.button.Sub = lv_obj_get_child(cont,1);
    lv_obj_PID_KID->PID.max_out.spinbox = lv_obj_get_child(cont,2);
    lv_obj_PID_KID->PID.max_out.spinbox->user_data = (void*)3;
    spinbox = lv_obj_PID_KID->PID.max_out.spinbox;
    divisor = (float)pow(10, ((lv_spinbox_t*)spinbox)->digit_count - ((lv_spinbox_t*)spinbox)->dec_point_pos);
    lv_spinbox_set_value(spinbox, PID[ID_PID].max_out*divisor);
    lv_obj_PID_KID->PID.max_out.button.Add = lv_obj_get_child(cont,3);
    lv_obj_add_event_cb(lv_obj_PID_KID->PID.max_out.button.Sub, btn_event_handler_add_sub,LV_EVENT_ALL,ID_PID);
    lv_obj_add_event_cb(lv_obj_PID_KID->PID.max_out.button.Add, btn_event_handler_add_sub,LV_EVENT_ALL,ID_PID);
    /*积分最大输出*/
    cont = create_spinbox(section, NULL, NULL, "MIO", &my_style_font_DingTalk_JinBuTi_English_14, 5,2,0);
    lv_obj_PID_KID->PID.max_iout.button.Sub = lv_obj_get_child(cont,1);
    lv_obj_PID_KID->PID.max_iout.spinbox = lv_obj_get_child(cont,2);
    lv_obj_PID_KID->PID.max_iout.spinbox->user_data = (void*)4;
    spinbox = lv_obj_PID_KID->PID.max_iout.spinbox;
    divisor = (float)pow(10, ((lv_spinbox_t*)spinbox)->digit_count - ((lv_spinbox_t*)spinbox)->dec_point_pos);
    lv_spinbox_set_value(spinbox, PID[ID_PID].max_iout*divisor);
    lv_obj_PID_KID->PID.max_iout.button.Add = lv_obj_get_child(cont,3);
    lv_obj_add_event_cb(lv_obj_PID_KID->PID.max_iout.button.Sub, btn_event_handler_add_sub,LV_EVENT_ALL,ID_PID);
    lv_obj_add_event_cb(lv_obj_PID_KID->PID.max_iout.button.Add, btn_event_handler_add_sub,LV_EVENT_ALL,ID_PID);
    /*积分分离*/
    cont = create_spinbox(section, NULL, NULL, "IST", &my_style_font_DingTalk_JinBuTi_English_14, 5,2,0);
    lv_obj_PID_KID->PID.Integral_Separation_Threshold.button.Sub = lv_obj_get_child(cont,1);
    lv_obj_PID_KID->PID.Integral_Separation_Threshold.spinbox = lv_obj_get_child(cont,2);
    lv_obj_PID_KID->PID.Integral_Separation_Threshold.spinbox->user_data = (void*)5;
    spinbox = lv_obj_PID_KID->PID.Integral_Separation_Threshold.spinbox;
    divisor = (float)pow(10, ((lv_spinbox_t*)spinbox)->digit_count - ((lv_spinbox_t*)spinbox)->dec_point_pos);
    lv_spinbox_set_value(spinbox, PID[ID_PID].Integral_Separation_Threshold*divisor);
    lv_obj_PID_KID->PID.Integral_Separation_Threshold.button.Add = lv_obj_get_child(cont,3);
    lv_obj_add_event_cb(lv_obj_PID_KID->PID.Integral_Separation_Threshold.button.Sub, btn_event_handler_add_sub,LV_EVENT_ALL,ID_PID);
    lv_obj_add_event_cb(lv_obj_PID_KID->PID.Integral_Separation_Threshold.button.Add, btn_event_handler_add_sub,LV_EVENT_ALL,ID_PID);
    /*死区空间*/
    cont = create_spinbox(section, NULL, NULL, "DZ", &my_style_font_DingTalk_JinBuTi_English_14, 5,2,0);
    lv_obj_PID_KID->PID.Dead_Zone.button.Sub = lv_obj_get_child(cont,1);
    lv_obj_PID_KID->PID.Dead_Zone.spinbox = lv_obj_get_child(cont,2);
    lv_obj_PID_KID->PID.Dead_Zone.spinbox->user_data = (void*)6;
    spinbox = lv_obj_PID_KID->PID.Dead_Zone.spinbox;
    divisor = (float)pow(10, ((lv_spinbox_t*)spinbox)->digit_count - ((lv_spinbox_t*)spinbox)->dec_point_pos);
    lv_spinbox_set_value(spinbox, PID[ID_PID].Dead_Zone*divisor);
    lv_obj_PID_KID->PID.Dead_Zone.button.Add = lv_obj_get_child(cont,3);
    lv_obj_add_event_cb(lv_obj_PID_KID->PID.Dead_Zone.button.Sub, btn_event_handler_add_sub,LV_EVENT_ALL,ID_PID);
    lv_obj_add_event_cb(lv_obj_PID_KID->PID.Dead_Zone.button.Add, btn_event_handler_add_sub,LV_EVENT_ALL,ID_PID);

    return sub_page;
}

void PID_create(lv_obj_t * parent)
{	
    lv_app_pid_init();
    
    lv_obj_t * cont;
    lv_obj_t * section;
    lv_obj_t * menu = lv_obj_get_parent(lv_obj_get_parent(parent));
    /*第一块*/
    section = lv_menu_section_create(parent);
    /*PID总开关*/
    cont = create_switch(section, NULL, NULL, "PID控制", &my_style_font_DingTalk_JinBuTi_Chinese_16);
    lv_obj_PID.sw_pid = lv_obj_get_child(cont, 1);
    lv_obj_set_width(lv_obj_PID.sw_pid,70);
    lv_obj_add_event_cb(lv_obj_PID.sw_pid, switch_event_cb_PID_Setting, LV_EVENT_VALUE_CHANGED, NULL);
    /*控制频率*/
    lv_obj_PID.Control_Frequence.label = create_text_two(cont, "控制频率", &my_style_font_DingTalk_JinBuTi_Chinese_16, "0 Hz", &my_style_font_DingTalk_JinBuTi_English_14);
    /*第二块*/
    lv_menu_separator_create(parent);
    section = lv_menu_section_create(parent);
    /*速度环*/
    lv_obj_PID.speed.ID = 0;
    lv_obj_t *sub_page_speed = lv_menu_page_create_PID(menu, "z轴速度调试", &lv_obj_PID.speed);
    sub_page_speed_cont = create_text(section, NULL, &my_style_pic, "速度环配置", &my_style_font_DingTalk_JinBuTi_Chinese_16);
    lv_menu_set_load_page_event(menu, sub_page_speed_cont, sub_page_speed);
    /*位置环*/
    lv_obj_PID.position.ID = 1;
    lv_obj_t *sub_page_position = lv_menu_page_create_PID(menu, "z轴角度调试", &lv_obj_PID.position);
    sub_page_position_cont = create_text(section, NULL, &my_style_pic, "位置环配置", &my_style_font_DingTalk_JinBuTi_Chinese_16);
    lv_menu_set_load_page_event(menu, sub_page_position_cont, sub_page_position);
}

void lv_app_pid_init()
{
    lv_fs_file_t lv_file;
    lv_fs_res_t  lv_res;
    char data_read[100]={'\0'};
    char data[10]={'\0'};
    uint32_t read_num;
    char *start,*end;
    char *p = data_read;
    float data_f;
    
    lv_res = lv_fs_open( &lv_file, "0:/PID/PID_Speed.txt", LV_FS_MODE_RD);
    if ( lv_res == LV_FS_RES_OK ) 
    {
        lv_res = lv_fs_read(&lv_file, data_read, 100, &read_num);
        for(uint8_t i=0; i<7; i++)
        {
            /*获取PID参数*/
            start = strchr(p, ':')+1;
            end = strchr(p, '\n')+1;
			p = end;
            memcpy(data, start, end - start-1);
            data_f = atof(data);
            /*设置PID参数*/
			*(&(PID[0].Kp)+i) = data_f;
            /*内存清零*/
			memset(data, '\0', 10);
        }
        lv_fs_close(&lv_file);
    }
   memset(data_read, '\0', 100);
   p = data_read;
   lv_res = lv_fs_open( &lv_file, "0:/PID/PID_Position.txt", LV_FS_MODE_RD);
   if ( lv_res == LV_FS_RES_OK ) 
   {
       lv_res = lv_fs_read(&lv_file, data_read, 100, &read_num);
       for(uint8_t i=0; i<7; i++)
       {
           /*获取PID参数*/
           start = strchr(p, ':')+1;
           end = strchr(p, '\n')+1;
			p = end;
           memcpy(data, start, end - start-1);
           data_f = atof(data);
           /*设置PID参数*/
			*(&(PID[1].Kp)+i) = data_f;
            /*内存清零*/
			memset(data, '\0', 10);
       }
       lv_fs_close(&lv_file);
   }
}

void PID_Frequence_Detection()
{
    lv_obj_PID.Control_Frequence.times = (uint32_t)(10000.0f/TIM13->CNT);
    TIM13->CNT = 0;
	char string[20]={0};
    /*控制频率信息*/
    sprintf(string, "%d Hz", lv_obj_PID.Control_Frequence.times);
    lv_label_set_text(lv_obj_PID.Control_Frequence.label, string);
}
void PID_Display()
{
	char string[20]={0};
    float slider_val;
    /*速度环信息*/
    if(PID[0].debug_sign == true)
    {
        slider_val = (float)lv_slider_get_value(lv_obj_PID.speed.slider_value_set)/10.0;
        lv_chart_set_next_value(lv_obj_PID.speed.Chart.Chart, lv_obj_PID.speed.Chart.ref.series, 10*Sensor_Msg.IMU_Data.Wz);
        lv_chart_set_next_value(lv_obj_PID.speed.Chart.Chart, lv_obj_PID.speed.Chart.set.series, 10*slider_val);
        lv_chart_refresh(lv_obj_PID.speed.Chart.Chart);
        if(Sensor_Msg.IMU_Data.Wz>=0)
            sprintf(string,"ref : +%.1f",Sensor_Msg.IMU_Data.Wz);
        else
            sprintf(string,"ref : %.1f",Sensor_Msg.IMU_Data.Wz);
        lv_label_set_text(lv_obj_PID.speed.Chart.ref.label, string); 
        
        if(slider_val>=0)
            sprintf(string,"set : +%.1f",slider_val);
        else
            sprintf(string,"set : %.1f",slider_val);
        lv_label_set_text(lv_obj_PID.speed.Chart.set.label, string); 
				PID_Debug_Set_Value = slider_val;
    }
    /*位置环信息*/
    else if(PID[1].debug_sign == true)
    {
        slider_val = (lv_coord_t)lv_slider_get_value(lv_obj_PID.position.slider_value_set)/10.0;
        lv_chart_set_next_value(lv_obj_PID.position.Chart.Chart, lv_obj_PID.position.Chart.ref.series, 10*Sensor_Msg.IMU_Data.Yaw);
        lv_chart_set_next_value(lv_obj_PID.position.Chart.Chart, lv_obj_PID.position.Chart.set.series, 10*slider_val);
        lv_chart_refresh(lv_obj_PID.position.Chart.Chart);
        if(Sensor_Msg.IMU_Data.Yaw>=0)
            sprintf(string,"ref : +%.1f",Sensor_Msg.IMU_Data.Yaw);
        else
            sprintf(string,"ref : %.1f",Sensor_Msg.IMU_Data.Yaw);
        lv_label_set_text(lv_obj_PID.position.Chart.ref.label, string);

        if(slider_val>=0)
            sprintf(string,"set : +%.1f",slider_val);
        else
            sprintf(string,"set : %.1f",slider_val);
        lv_label_set_text(lv_obj_PID.position.Chart.set.label, string); 
				PID_Debug_Set_Value = slider_val;
    }
}

/*手柄控制PID滑杆*/
void PID_Debug_Set_BLE(uint8_t ID_PID)
{
    lv_obj_PID_KID_ *lv_obj_PID_KID = &lv_obj_PID.speed+ID_PID;
    lv_obj_t * slider = lv_obj_PID_KID->slider_value_set;
		int slider_value;
    if(Sensor_Msg.BLE_Data.L_X == 0)    slider_value = 0;
    else                                slider_value = Sensor_Msg.BLE_Data.L_X*(((lv_bar_t *)slider)->max_value-((lv_bar_t *)slider)->min_value)/3500.f;
    lv_slider_set_value(slider, slider_value, LV_ANIM_ON);
}