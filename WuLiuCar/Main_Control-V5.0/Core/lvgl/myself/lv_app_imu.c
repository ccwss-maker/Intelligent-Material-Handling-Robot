#include "lv_app_imu.h"
#include "Master_Slave_Communication.h"
#include "stdio.h"
lv_obj_IMU_ lv_obj_IMU;

static lv_obj_t * legend_label_creat(lv_obj_t * parent, const char * txt_0, const char * txt_1, const char * txt_2, const char * txt_3, lv_coord_t min, lv_coord_t max)
{
	lv_obj_t * obj = lv_menu_cont_create(parent);
	static lv_coord_t col_dsc[] = {1, 55, 55, 57, LV_GRID_TEMPLATE_LAST};
	static lv_coord_t row_dsc[] = {17, 7, 135, LV_GRID_TEMPLATE_LAST}; /*3 100 px tall rows*/
	static lv_style_t style_red, style_blue, style_yellow;
	lv_obj_t * label;

	lv_obj_set_grid_dsc_array(obj, col_dsc, row_dsc);

	lv_style_init(&style_red);
	lv_style_set_text_color(&style_red,lv_palette_main(LV_PALETTE_RED));
	lv_style_set_text_font(&style_red,&DingTalk_JinBuTi_English_14);
	lv_style_init(&style_blue);
	lv_style_set_text_color(&style_blue,lv_palette_main(LV_PALETTE_BLUE));
	lv_style_set_text_font(&style_blue,&DingTalk_JinBuTi_English_14);
	lv_style_init(&style_yellow);
	lv_style_set_text_color(&style_yellow,lv_palette_main(LV_PALETTE_YELLOW));
	lv_style_set_text_font(&style_yellow,&DingTalk_JinBuTi_English_14);
	/*表头*/
	label = lv_label_create(obj);
	lv_obj_add_style(label, &my_style_font_DingTalk_JinBuTi_Chinese_16,LV_STATE_DEFAULT);
	lv_label_set_text(label, txt_0);
	lv_obj_set_grid_cell(label, LV_GRID_ALIGN_START, 0, 1, LV_GRID_ALIGN_START, 0, 1);
	/*图例*/
	label = lv_label_create(obj);
	lv_obj_add_style(label, &style_red, LV_STATE_DEFAULT);
	lv_label_set_text(label, txt_1);
	lv_obj_set_grid_cell(label, LV_GRID_ALIGN_START, 1, 1, LV_GRID_ALIGN_START, 1, 1);
	label = lv_label_create(obj);
	lv_obj_add_style(label, &style_blue, LV_STATE_DEFAULT);
	lv_label_set_text(label, txt_2);
	lv_obj_set_grid_cell(label, LV_GRID_ALIGN_START, 2, 1, LV_GRID_ALIGN_START, 1, 1);
	label = lv_label_create(obj);
	lv_obj_add_style(label, &style_yellow, LV_STATE_DEFAULT);
	lv_label_set_text(label, txt_3);
	lv_obj_set_grid_cell(label, LV_GRID_ALIGN_START, 3, 1, LV_GRID_ALIGN_START, 1, 1);
	/*表格*/
	lv_obj_t * chart; 
	chart = lv_chart_create(obj);
	lv_obj_set_size(chart,160,130);	
	lv_chart_set_update_mode(chart,LV_CHART_UPDATE_MODE_SHIFT);
	lv_chart_set_range(chart, LV_CHART_AXIS_PRIMARY_Y, min, max);
	lv_chart_set_div_line_count(chart,5,4);
	lv_chart_set_axis_tick(chart, LV_CHART_AXIS_PRIMARY_Y, 10, 5, 5, 5, true, 40);
	lv_obj_set_grid_cell(chart, LV_GRID_ALIGN_END, 0, 4, LV_GRID_ALIGN_END, 2, 1);

	return obj;
}

lv_obj_t * chart_creat(lv_obj_t * parent, lv_coord_t min, lv_coord_t max)
{
	lv_obj_t * obj = lv_menu_cont_create(parent);
	lv_obj_t * chart; 
	chart = lv_chart_create(obj);
	lv_obj_set_size(chart,150,130);	
	lv_chart_set_update_mode(chart,LV_CHART_UPDATE_MODE_SHIFT);
	lv_chart_set_range(chart, LV_CHART_AXIS_PRIMARY_Y, min, max);
	lv_chart_set_div_line_count(chart,5,4);
	lv_chart_set_axis_tick(chart, LV_CHART_AXIS_PRIMARY_Y, 10, 5, 5, 5, true, 40);

	lv_obj_set_flex_align(obj, LV_FLEX_ALIGN_END, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

	return obj;
}

void IMU_create(lv_obj_t * parent)
{	
	lv_obj_t * cont;
    lv_obj_t * section;

    /*第一块*/
    lv_menu_separator_create(parent);
    section = lv_menu_section_create(parent);
	/*RPY*/
	/*图例*/
	cont = legend_label_creat(section, "欧拉角", "R : 000", "P : 000", "Y : 000", -180, 180);
	lv_obj_IMU.Roll.label = lv_obj_get_child(cont, 1);
	lv_obj_IMU.Pitch.label = lv_obj_get_child(cont, 2);
	lv_obj_IMU.Yaw.label = lv_obj_get_child(cont, 3);
	/*表格*/
	lv_obj_IMU.Chart_RPY = lv_obj_get_child(cont, 4);
	lv_obj_IMU.Roll.series = lv_chart_add_series(lv_obj_IMU.Chart_RPY, lv_palette_main(LV_PALETTE_RED),LV_CHART_AXIS_PRIMARY_Y);
	lv_obj_IMU.Pitch.series = lv_chart_add_series(lv_obj_IMU.Chart_RPY, lv_palette_main(LV_PALETTE_BLUE),LV_CHART_AXIS_PRIMARY_Y);
	lv_obj_IMU.Yaw.series = lv_chart_add_series(lv_obj_IMU.Chart_RPY, lv_palette_main(LV_PALETTE_YELLOW),LV_CHART_AXIS_PRIMARY_Y);
	
	/*第三块*/
    lv_menu_separator_create(parent);
	section = lv_menu_section_create(parent);
	/*角速度*/
	/*图例*/
	cont = legend_label_creat(section, "角速度", "Wx : -0.0", "Wy : -0.0", "Wz : -0.0", -2, 2);
	lv_obj_IMU.Wx.label = lv_obj_get_child(cont, 1);
	lv_obj_IMU.Wy.label = lv_obj_get_child(cont, 2);
	lv_obj_IMU.Wz.label = lv_obj_get_child(cont, 3);
	/*表格*/
	lv_obj_IMU.Chart_W = lv_obj_get_child(cont, 4);
	lv_obj_IMU.Wx.series = lv_chart_add_series(lv_obj_IMU.Chart_W, lv_palette_main(LV_PALETTE_RED),LV_CHART_AXIS_PRIMARY_Y);
	lv_obj_IMU.Wy.series = lv_chart_add_series(lv_obj_IMU.Chart_W, lv_palette_main(LV_PALETTE_BLUE),LV_CHART_AXIS_PRIMARY_Y);
	lv_obj_IMU.Wz.series = lv_chart_add_series(lv_obj_IMU.Chart_W, lv_palette_main(LV_PALETTE_YELLOW),LV_CHART_AXIS_PRIMARY_Y);
}

void IMU_Display()
{
	char string[20]={0};
	if(Sensor_Msg.IMU_Data.chart_sign.W)
	{
		lv_chart_set_next_value(lv_obj_IMU.Chart_W,lv_obj_IMU.Wx.series,(lv_coord_t)Sensor_Msg.IMU_Data.Wx);
		lv_chart_set_next_value(lv_obj_IMU.Chart_W,lv_obj_IMU.Wy.series,(lv_coord_t)Sensor_Msg.IMU_Data.Wy);
		lv_chart_set_next_value(lv_obj_IMU.Chart_W,lv_obj_IMU.Wz.series,(lv_coord_t)Sensor_Msg.IMU_Data.Wz);
		lv_chart_refresh(lv_obj_IMU.Chart_W);
		
		if(Sensor_Msg.IMU_Data.Wx>=0)
			sprintf(string,"Wx:+%.1f",Sensor_Msg.IMU_Data.Wx);
		else
			sprintf(string,"Wx:%.1f",Sensor_Msg.IMU_Data.Wx);
		lv_label_set_text(lv_obj_IMU.Wx.label, string); 

		if(Sensor_Msg.IMU_Data.Wy>=0)
			sprintf(string,"Wy:+%.1f",Sensor_Msg.IMU_Data.Wy);
		else
			sprintf(string,"Wy:%.1f",Sensor_Msg.IMU_Data.Wy);
		lv_label_set_text(lv_obj_IMU.Wy.label, string);

		if(Sensor_Msg.IMU_Data.Wz>=0)
			sprintf(string,"Wz:+%.1f",Sensor_Msg.IMU_Data.Wz);
		else
			sprintf(string,"Wz:%.1f",Sensor_Msg.IMU_Data.Wz);
		lv_label_set_text(lv_obj_IMU.Wz.label, string); 
		Sensor_Msg.IMU_Data.chart_sign.W = false;
	}
	if(Sensor_Msg.IMU_Data.chart_sign.R)
	{
		lv_chart_set_next_value(lv_obj_IMU.Chart_RPY,lv_obj_IMU.Roll.series,(lv_coord_t)Sensor_Msg.IMU_Data.Roll);
		lv_chart_set_next_value(lv_obj_IMU.Chart_RPY,lv_obj_IMU.Pitch.series,(lv_coord_t)Sensor_Msg.IMU_Data.Pitch);
		lv_chart_set_next_value(lv_obj_IMU.Chart_RPY,lv_obj_IMU.Yaw.series,(lv_coord_t)Sensor_Msg.IMU_Data.Yaw);
		lv_chart_refresh(lv_obj_IMU.Chart_RPY);
		
		if(Sensor_Msg.IMU_Data.Roll>=0)
			sprintf(string,"R:+%.1f",Sensor_Msg.IMU_Data.Roll);
		else
			sprintf(string,"R:%.1f",Sensor_Msg.IMU_Data.Roll);
		lv_label_set_text(lv_obj_IMU.Roll.label, string); 

		if(Sensor_Msg.IMU_Data.Pitch>=0)
			sprintf(string,"P:+%.1f",Sensor_Msg.IMU_Data.Pitch);
		else
			sprintf(string,"P:%.1f",Sensor_Msg.IMU_Data.Pitch);
		lv_label_set_text(lv_obj_IMU.Pitch.label, string); 

		if(Sensor_Msg.IMU_Data.Yaw>=0)
			sprintf(string,"Y:+%.1f",Sensor_Msg.IMU_Data.Yaw);
		else
			sprintf(string,"Y:%.1f",Sensor_Msg.IMU_Data.Yaw);
		lv_label_set_text(lv_obj_IMU.Yaw.label, string); 
		Sensor_Msg.IMU_Data.chart_sign.R = false;
	}
}