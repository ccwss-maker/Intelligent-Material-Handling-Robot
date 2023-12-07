#include "lv_app_arm.h"
#include "ble.h"
#include "Arm.h"
#include "Motro_Control_Basic.h"
#include "Master_Slave_Communication.h"
#include "Motro_Control_Arm.h"
#include "stdio.h"
lv_obj_ARM_ lv_obj_ARM;
static lv_obj_t * sub_page_end_control_lable;
static lv_obj_t * sub_page_motor_debug_lable;

static lv_obj_t * Encoder_Create(lv_obj_t *parent, const char *title)
{
    lv_obj_t * obj = lv_menu_cont_create(parent);
    static lv_coord_t col_dsc[] = {0, 3, 43, 58, 23, 24, LV_GRID_TEMPLATE_LAST};
	static lv_coord_t row_dsc[] = {15, 15, 12, 15, 15, LV_GRID_TEMPLATE_LAST}; /*3 100 px tall rows*/
    lv_obj_set_grid_dsc_array(obj, col_dsc, row_dsc);
    lv_obj_t * label = NULL;

    label = lv_label_create(obj);
    lv_obj_add_style(label,&my_style_font_DingTalk_JinBuTi_Chinese_16,LV_STATE_DEFAULT);
    lv_label_set_text(label, title);
    lv_obj_set_grid_cell(label, LV_GRID_ALIGN_CENTER, 3, 1, LV_GRID_ALIGN_START, 0, 1);
    
    label = lv_label_create(obj);
    lv_obj_add_style(label,&my_style_font_DingTalk_JinBuTi_Chinese_16,LV_STATE_DEFAULT);
    lv_label_set_text(label, "编码器角度");
    lv_obj_set_grid_cell(label, LV_GRID_ALIGN_START, 0, 1, LV_GRID_ALIGN_START, 1, 1);

    label = lv_label_create(obj);
    lv_obj_add_style(label,&my_style_font_DingTalk_JinBuTi_Chinese_16,LV_STATE_DEFAULT);
    lv_label_set_text(label, "实际角度");
    lv_obj_set_grid_cell(label, LV_GRID_ALIGN_END, 5, 1, LV_GRID_ALIGN_START, 1, 1);
    
    label = lv_label_create(obj);
    lv_obj_add_style(label,&my_style_font_DingTalk_JinBuTi_English_14,LV_STATE_DEFAULT);
    lv_label_set_text(label, "000.0");
    lv_obj_set_grid_cell(label, LV_GRID_ALIGN_START, 2, 1, LV_GRID_ALIGN_START, 2, 1);

    label = lv_label_create(obj);
    lv_obj_add_style(label,&my_style_font_DingTalk_JinBuTi_English_14,LV_STATE_DEFAULT);
    lv_label_set_text(label, "+000.0");
    lv_obj_set_grid_cell(label, LV_GRID_ALIGN_START, 4, 1, LV_GRID_ALIGN_START, 2, 1);
    
    label = lv_label_create(obj);
    lv_obj_add_style(label,&my_style_font_DingTalk_JinBuTi_Chinese_16,LV_STATE_DEFAULT);
    lv_label_set_text(label, "超速");
    lv_obj_set_grid_cell(label, LV_GRID_ALIGN_START, 0, 1, LV_GRID_ALIGN_START, 3, 1);

    label = lv_label_create(obj);
    lv_obj_add_style(label,&my_style_font_DingTalk_JinBuTi_Chinese_16,LV_STATE_DEFAULT);
    lv_label_set_text(label, "弱磁");
    lv_obj_set_grid_cell(label, LV_GRID_ALIGN_CENTER, 3, 1, LV_GRID_ALIGN_START, 3, 1);
    
    label = lv_label_create(obj);
    lv_obj_add_style(label,&my_style_font_DingTalk_JinBuTi_Chinese_16,LV_STATE_DEFAULT);
    lv_label_set_text(label, "低压");
    lv_obj_set_grid_cell(label, LV_GRID_ALIGN_END, 5, 1, LV_GRID_ALIGN_START, 3, 1);

    label = lv_label_create(obj);
    lv_obj_add_style(label,&my_style_font_DingTalk_JinBuTi_Chinese_16,LV_STATE_DEFAULT);
    lv_label_set_text(label, "否");
    lv_obj_set_grid_cell(label, LV_GRID_ALIGN_START, 1, 1, LV_GRID_ALIGN_START, 4, 1);

    label = lv_label_create(obj);
    lv_obj_add_style(label,&my_style_font_DingTalk_JinBuTi_Chinese_16,LV_STATE_DEFAULT);
    lv_label_set_text(label, "否");
    lv_obj_set_grid_cell(label, LV_GRID_ALIGN_CENTER, 3, 1, LV_GRID_ALIGN_START, 4, 1);
    
    label = lv_label_create(obj);
    lv_obj_add_style(label,&my_style_font_DingTalk_JinBuTi_Chinese_16,LV_STATE_DEFAULT);
    lv_label_set_text(label, "否");
    lv_obj_set_grid_cell(label, LV_GRID_ALIGN_START, 5, 1, LV_GRID_ALIGN_START, 4, 1);

    return obj;
}
/*机械臂末端控制部分*/
void lv_Arm_Coord_Control()
{
    Point3f_ Point;
    Angle3f_ Angle;
    Point.x = (double)lv_slider_get_value(lv_obj_ARM.Motor_Control.slider_X.slider);
    Point.y = (double)lv_slider_get_value(lv_obj_ARM.Motor_Control.slider_Y.slider);
    Point.z = (double)lv_slider_get_value(lv_obj_ARM.Motor_Control.slider_Z.slider);
    BLE_Arm.x = (int)Point.x;
    BLE_Arm.y = (int)Point.y;
    BLE_Arm.z = (int)Point.z;
    if(Arm_Coord_Control(&Point, &Angle) == true)
    {
        char buf[20];
        sprintf(buf,"%.1f", Angle.a);
        lv_label_set_text(lv_obj_ARM.Motor_Control.label_Angle_X, buf);
        sprintf(buf,"%.1f", Angle.b);
        lv_label_set_text(lv_obj_ARM.Motor_Control.label_Angle_Y, buf);
        sprintf(buf,"%.1f", Angle.c);
        lv_label_set_text(lv_obj_ARM.Motor_Control.label_Angle_Z, buf);
    }
}

void lv_Arm_Coord_Control_Reset()
{
    char buf[20];
    lv_slider_set_value(lv_obj_ARM.Motor_Control.slider_X.slider, Arm_Coord_X_0, LV_ANIM_ON);
    lv_slider_set_value(lv_obj_ARM.Motor_Control.slider_Y.slider, Arm_Coord_Y_0, LV_ANIM_ON);
    lv_slider_set_value(lv_obj_ARM.Motor_Control.slider_Z.slider, Arm_Coord_Z_0, LV_ANIM_ON);
    sprintf(buf, "%d", Arm_Coord_X_0);
    lv_label_set_text(lv_obj_ARM.Motor_Control.slider_X.slider_label, buf);
    sprintf(buf, "%d", Arm_Coord_Y_0);
    lv_label_set_text(lv_obj_ARM.Motor_Control.slider_Y.slider_label, buf);
    sprintf(buf, "%d", Arm_Coord_Z_0);
    lv_label_set_text(lv_obj_ARM.Motor_Control.slider_Z.slider_label, buf);
    
    Point3f_ Point;
    Angle3f_ Angle;
    Point.x = Arm_Coord_X_0;
    Point.y = Arm_Coord_Y_0;
    Point.z = Arm_Coord_Z_0;
    BLE_Arm.x = (int)Point.x;
    BLE_Arm.y = (int)Point.y;
    BLE_Arm.z = (int)Point.z;
    if(Arm_Coord_Control(&Point, &Angle) == true)
    {
        sprintf(buf,"%.1f", Angle.a);
        lv_label_set_text(lv_obj_ARM.Motor_Control.label_Angle_X, buf);
        sprintf(buf,"%.1f", Angle.b);
        lv_label_set_text(lv_obj_ARM.Motor_Control.label_Angle_Y, buf);
        sprintf(buf,"%.1f", Angle.c);
        lv_label_set_text(lv_obj_ARM.Motor_Control.label_Angle_Z, buf);
    }
}

static void btn_rst_event_Coord_Control(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED) 
    {
        lv_Arm_Coord_Control_Reset();
    }
}
static void slider_event_cb_Arm_Control(lv_event_t * e)
{
    lv_obj_t * obj = lv_event_get_target(e);
    lv_obj_t * label = lv_obj_get_child(lv_obj_get_parent(obj), 3);
    char buf[20];
    int slider_value = (int)lv_slider_get_value(obj);
    lv_snprintf(buf, sizeof(buf), "%d", slider_value);
    lv_label_set_text(label, buf);

    lv_Arm_Coord_Control();
}

static lv_obj_t * lv_menu_page_create_end_control(lv_obj_t *parent)
{
    lv_obj_t * cont;
    lv_obj_t * section;
    lv_obj_t * sub_page = lv_menu_page_create(parent, NULL);
    lv_obj_set_style_pad_hor(sub_page, lv_obj_get_style_pad_left(lv_menu_get_main_header(parent), 0), 0);
    /*第一块*/
    section = lv_menu_section_create(sub_page);
    /*复位*/
    cont = create_btn(section, NULL, NULL, "角度控制", &my_style_font_DingTalk_JinBuTi_Chinese_16);
    lv_obj_ARM.Motor_Control.btn_rst = lv_obj_get_child(cont, 1);
    lv_obj_set_width(lv_obj_ARM.Motor_Control.btn_rst,70);
    lv_obj_t* btn_rst_label = lv_label_create(lv_obj_ARM.Motor_Control.btn_rst);	
    lv_obj_add_style(btn_rst_label,&my_style_font_DingTalk_JinBuTi_Chinese_16,LV_STATE_DEFAULT);
    lv_label_set_text(btn_rst_label, "复位");
    lv_obj_align(btn_rst_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_event_cb(lv_obj_ARM.Motor_Control.btn_rst, btn_rst_event_Coord_Control,LV_EVENT_ALL,NULL);
    /*X滑杆*/
    cont = create_slider_two(section, USR_SYMBOL_Complete, &my_style_pic, "X", &my_style_font_DingTalk_JinBuTi_English_14, Arm.Range.X_Min, Arm.Range.X_Max, Arm_Coord_X_0);
    lv_obj_ARM.Motor_Control.label_Label_X = lv_obj_get_child(cont, 0);
    lv_obj_ARM.Motor_Control.Symbol_X = lv_obj_get_child(cont, 1);
    lv_obj_ARM.Motor_Control.label_Angle_X = lv_obj_get_child(cont, 2);
    lv_obj_ARM.Motor_Control.slider_X.slider = lv_obj_get_child(cont, 4);
    lv_obj_add_event_cb(lv_obj_ARM.Motor_Control.slider_X.slider, slider_event_cb_Arm_Control, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_ARM.Motor_Control.slider_X.slider_label = lv_obj_get_child(cont, 3);
    /*Y滑杆*/
    cont = create_slider_two(section, USR_SYMBOL_Complete, &my_style_pic, "Y", &my_style_font_DingTalk_JinBuTi_English_14, Arm.Range.Y_Min, Arm.Range.Y_Max, Arm_Coord_Y_0);
    lv_obj_ARM.Motor_Control.label_Label_Y = lv_obj_get_child(cont, 0);
    lv_obj_ARM.Motor_Control.Symbol_Y = lv_obj_get_child(cont, 1);
    lv_obj_ARM.Motor_Control.label_Angle_Y = lv_obj_get_child(cont, 2);
    lv_obj_ARM.Motor_Control.slider_Y.slider = lv_obj_get_child(cont, 4);
    lv_obj_add_event_cb(lv_obj_ARM.Motor_Control.slider_Y.slider, slider_event_cb_Arm_Control, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_ARM.Motor_Control.slider_Y.slider_label = lv_obj_get_child(cont, 3);
    /*Z滑杆*/
    cont = create_slider_two(section, USR_SYMBOL_Complete, &my_style_pic, "Z", &my_style_font_DingTalk_JinBuTi_English_14, Arm.Range.Z_Min, Arm.Range.Z_Max, Arm_Coord_Z_0);
    lv_obj_ARM.Motor_Control.label_Label_Z = lv_obj_get_child(cont, 0);
    lv_obj_ARM.Motor_Control.Symbol_Z = lv_obj_get_child(cont, 1);
    lv_obj_ARM.Motor_Control.label_Angle_Z = lv_obj_get_child(cont, 2);
    lv_obj_ARM.Motor_Control.slider_Z.slider = lv_obj_get_child(cont, 4);
    lv_obj_add_event_cb(lv_obj_ARM.Motor_Control.slider_Z.slider, slider_event_cb_Arm_Control, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_ARM.Motor_Control.slider_Z.slider_label = lv_obj_get_child(cont, 3);
    return sub_page;
}

/*机械臂电机调试部分*/
void Arm_slider_clear()
{
    char buf[20];
    lv_slider_set_value(lv_obj_ARM.Motor_Debug.slider_Rear_Arm.slider, (int)Rear_Angle_To_0_Set, LV_ANIM_ON);
    lv_slider_set_value(lv_obj_ARM.Motor_Debug.slider_Front_Arm.slider, (int)Front_Angle_To_0_Set, LV_ANIM_ON);
    lv_slider_set_value(lv_obj_ARM.Motor_Debug.slider_Pedestal.slider, (int)Pedestal_Angle_To_0_Set, LV_ANIM_ON);

    lv_snprintf(buf, sizeof(buf), "%d", Rear_Angle_To_0_Set);
    lv_label_set_text(lv_obj_ARM.Motor_Debug.slider_Rear_Arm.slider_label, buf);

    lv_snprintf(buf, sizeof(buf), "%d", Front_Angle_To_0_Set);
    lv_label_set_text(lv_obj_ARM.Motor_Debug.slider_Front_Arm.slider_label, buf);

    lv_snprintf(buf, sizeof(buf), "%d", Pedestal_Angle_To_0_Set);
    lv_label_set_text(lv_obj_ARM.Motor_Debug.slider_Pedestal.slider_label, buf);
}

void lv_obj_Arm_Control(bool state_sw, bool state_slider)
{
    if(state_sw)
    {
        lv_obj_clear_state(lv_obj_ARM.sw_power,  LV_STATE_DISABLED);
    }
    else
    {
        lv_obj_add_state(lv_obj_ARM.sw_power, LV_STATE_DISABLED);
    }

    if(state_slider)
    {
        lv_obj_clear_state(lv_obj_ARM.Motor_Debug.btn_rst, LV_STATE_DISABLED);
        lv_obj_clear_state(sub_page_motor_debug_lable,  LV_STATE_DISABLED);
        lv_obj_clear_state(sub_page_end_control_lable,  LV_STATE_DISABLED);
    }
    else
    {
        lv_obj_add_state(lv_obj_ARM.Motor_Debug.btn_rst, LV_STATE_DISABLED);
        lv_obj_add_state(sub_page_motor_debug_lable,  LV_STATE_DISABLED);
        lv_obj_add_state(sub_page_end_control_lable,  LV_STATE_DISABLED);
    }
}

static void switch_event_cb_debug(lv_event_t * e)
{
   lv_obj_t * obj = lv_event_get_target(e);
   if(lv_obj_has_state(obj, LV_STATE_CHECKED))
   {
       Arm.debug_sign=true;
       lv_obj_Arm_Control(true,false);
   }
   else
   {
       Arm.debug_sign=false;
       Arm_slider_clear();
       lv_obj_clear_state(lv_obj_ARM.sw_power,LV_STATE_CHECKED);
       lv_obj_Arm_Control(false,false);
       Arm_Close();
   }
}

static void switch_event_cb_power(lv_event_t * e)
{
   if(lv_obj_has_state(lv_event_get_target(e), LV_STATE_CHECKED))
   {
       Arm_Open();
       lv_obj_Arm_Control(true, true);
   }
   else
   {
       Arm_Close();
       Arm_slider_clear();
       lv_obj_Arm_Control(true, false);
   }
}
// static void btn_set_0_event_handler(lv_event_t * e)
// {
//     lv_event_code_t code = lv_event_get_code(e);
//     if(code == LV_EVENT_CLICKED) 
//     {
// //        Arm_Set_0();
//     }
// }
static void btn_rst_event_handler(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED) 
    {
        Arm_slider_clear();
        Motor_Absolute_Control(Arm_Rear_Address, Arm_Velocity, Arm_Accelerate, Rear_Angle_To_0_Set);
        Motor_Absolute_Control(Arm_Front_Address, Arm_Velocity, Arm_Accelerate, Front_Angle_To_0_Set);
        Motor_Absolute_Control(Arm_Pedestal_Address, Arm_Velocity, Arm_Accelerate, Pedestal_Angle_To_0_Set);
    }
}

static void slider_event_cb_Arm_Debug(lv_event_t * e)
{
    lv_obj_t * obj = lv_event_get_target(e);
    lv_obj_t * label = lv_obj_get_child(lv_obj_get_parent(obj), 3);
    uint8_t address = (uint8_t)lv_obj_get_user_data(obj);
    char buf[20];
    int slider_value = (int)lv_slider_get_value(obj);
    lv_snprintf(buf, sizeof(buf), "%d", slider_value);
    lv_label_set_text(label, buf);
    Motor_Absolute_Control(address, Arm_Velocity, Arm_Accelerate, (float)slider_value);
}

static lv_obj_t * lv_menu_page_create_motor_debug(lv_obj_t *parent)
{
    lv_obj_t * cont;
    lv_obj_t * section;
    lv_obj_t * sub_page = lv_menu_page_create(parent, NULL);
    lv_obj_set_style_pad_hor(sub_page, lv_obj_get_style_pad_left(lv_menu_get_main_header(parent), 0), 0);
    /*第一块*/
    section = lv_menu_section_create(sub_page);
    // /*爪子控制*/
    // cont = create_btn(section, NULL, NULL, "零点设置", &my_style_font_DingTalk_JinBuTi_Chinese_16);
    // lv_obj_ARM.Motor_Debug.btn_set_0 = lv_obj_get_child(cont, 1);
    // lv_obj_set_width(lv_obj_ARM.Motor_Debug.btn_set_0,70);
    // lv_obj_t* btn_set_0_label = lv_label_create(lv_obj_ARM.Motor_Debug.btn_set_0);	
    // lv_obj_add_style(btn_set_0_label,&my_style_font_DingTalk_JinBuTi_Chinese_16,LV_STATE_DEFAULT);
    // lv_label_set_text(btn_set_0_label, "执行");
    // lv_obj_align(btn_set_0_label, LV_ALIGN_CENTER, 0, 0);
    // lv_obj_add_event_cb(lv_obj_ARM.Motor_Debug.btn_set_0, btn_set_0_event_handler,LV_EVENT_ALL,NULL);
    // lv_obj_add_state(lv_obj_ARM.Motor_Debug.btn_set_0,  LV_STATE_DISABLED);
    /*复位*/
    cont = create_btn(section, NULL, NULL, "角度控制", &my_style_font_DingTalk_JinBuTi_Chinese_16);
    lv_obj_ARM.Motor_Debug.btn_rst = lv_obj_get_child(cont, 1);
    lv_obj_set_width(lv_obj_ARM.Motor_Debug.btn_rst,70);
    lv_obj_t* btn_rst_label = lv_label_create(lv_obj_ARM.Motor_Debug.btn_rst);	
    lv_obj_add_style(btn_rst_label,&my_style_font_DingTalk_JinBuTi_Chinese_16,LV_STATE_DEFAULT);
    lv_label_set_text(btn_rst_label, "复位");
    lv_obj_align(btn_rst_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_event_cb(lv_obj_ARM.Motor_Debug.btn_rst, btn_rst_event_handler,LV_EVENT_ALL,NULL);
    /*大臂滑杆*/
    cont = create_slider_two(section, USR_SYMBOL_Complete, &my_style_pic, "大臂", &my_style_font_DingTalk_JinBuTi_Chinese_16, (int)Arm_Angle_a_Min, (int)Arm_Angle_a_Max, Rear_Angle_To_0_Set);
    lv_obj_ARM.Motor_Debug.Symbol_Rear_Arm = lv_obj_get_child(cont, 1);
    lv_obj_ARM.Motor_Debug.label_Angle_Rear_Arm = lv_obj_get_child(cont, 2);
    lv_obj_ARM.Motor_Debug.slider_Rear_Arm.slider = lv_obj_get_child(cont, 4);
    lv_obj_add_event_cb(lv_obj_ARM.Motor_Debug.slider_Rear_Arm.slider, slider_event_cb_Arm_Debug, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_ARM.Motor_Debug.slider_Rear_Arm.slider_label = lv_obj_get_child(cont, 3);
    lv_obj_ARM.Motor_Debug.slider_Rear_Arm.slider->user_data = (void*)Arm_Rear_Address;
    /*小臂滑杆*/
    cont = create_slider_two(section, USR_SYMBOL_Complete, &my_style_pic, "小臂", &my_style_font_DingTalk_JinBuTi_Chinese_16, (int)Arm_Angle_b_Min, (int)Arm_Angle_b_Max, Front_Angle_To_0_Set);
    lv_obj_ARM.Motor_Debug.Symbol_Front_Arm = lv_obj_get_child(cont, 1);
    lv_obj_ARM.Motor_Debug.label_Angle_Front_Arm = lv_obj_get_child(cont, 2);
    lv_obj_ARM.Motor_Debug.slider_Front_Arm.slider = lv_obj_get_child(cont, 4);
    lv_obj_add_event_cb(lv_obj_ARM.Motor_Debug.slider_Front_Arm.slider, slider_event_cb_Arm_Debug, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_ARM.Motor_Debug.slider_Front_Arm.slider_label = lv_obj_get_child(cont, 3);
    lv_obj_ARM.Motor_Debug.slider_Front_Arm.slider->user_data = (void*)Arm_Front_Address;
    /*底座滑杆*/
    cont = create_slider_two(section, USR_SYMBOL_Complete, &my_style_pic, "底座", &my_style_font_DingTalk_JinBuTi_Chinese_16, (int)Arm_Angle_c_Min, (int)Arm_Angle_c_Max, Pedestal_Angle_To_0_Set);
    lv_obj_ARM.Motor_Debug.Symbol_Pedestal = lv_obj_get_child(cont, 1);
    lv_obj_ARM.Motor_Debug.label_Angle_Pedestal = lv_obj_get_child(cont, 2);
    lv_obj_ARM.Motor_Debug.slider_Pedestal.slider = lv_obj_get_child(cont, 4);
    lv_obj_add_event_cb(lv_obj_ARM.Motor_Debug.slider_Pedestal.slider, slider_event_cb_Arm_Debug, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_ARM.Motor_Debug.slider_Pedestal.slider_label = lv_obj_get_child(cont, 3);
    lv_obj_ARM.Motor_Debug.slider_Pedestal.slider->user_data = (void*)Arm_Pedestal_Address;
    return sub_page;
}

static lv_obj_t * lv_menu_page_create_encoder_msg(lv_obj_t *parent)
{
    lv_obj_t * cont;
    lv_obj_t * section;
    lv_obj_t * sub_page = lv_menu_page_create(parent, NULL);
    lv_obj_set_style_pad_hor(sub_page, lv_obj_get_style_pad_left(lv_menu_get_main_header(parent), 0), 0);
    /*第一块*/
    lv_menu_separator_create(sub_page);
    section = lv_menu_section_create(sub_page);
    /*大臂*/
    cont = Encoder_Create(section, "大臂");
    lv_obj_ARM.Encoder_Msg.Rear.label_Encoder = lv_obj_get_child(cont, 3);
    lv_obj_ARM.Encoder_Msg.Rear.label_Angle = lv_obj_get_child(cont, 4);
    lv_obj_ARM.Encoder_Msg.Rear.label_Over_Speed_Flag = lv_obj_get_child(cont, 8);
    lv_obj_ARM.Encoder_Msg.Rear.label_Weak_Magnetic_Flag = lv_obj_get_child(cont, 9);
    lv_obj_ARM.Encoder_Msg.Rear.label_Low_Voltage_Flag = lv_obj_get_child(cont, 10);
    /*第二块*/
    lv_menu_separator_create(sub_page);
    section = lv_menu_section_create(sub_page);    
    /*小臂*/
    cont = Encoder_Create(section, "小臂");
    lv_obj_ARM.Encoder_Msg.Front.label_Encoder = lv_obj_get_child(cont, 3);
    lv_obj_ARM.Encoder_Msg.Front.label_Angle = lv_obj_get_child(cont, 4);
    lv_obj_ARM.Encoder_Msg.Front.label_Over_Speed_Flag = lv_obj_get_child(cont, 8);
    lv_obj_ARM.Encoder_Msg.Front.label_Weak_Magnetic_Flag = lv_obj_get_child(cont, 9);
    lv_obj_ARM.Encoder_Msg.Front.label_Low_Voltage_Flag = lv_obj_get_child(cont, 10);
    /*第三块*/
    lv_menu_separator_create(sub_page);
    section = lv_menu_section_create(sub_page);  
    /*底座*/
    cont = Encoder_Create(section, "底座");
    lv_obj_ARM.Encoder_Msg.Pedestal.label_Encoder = lv_obj_get_child(cont, 3);
    lv_obj_ARM.Encoder_Msg.Pedestal.label_Angle = lv_obj_get_child(cont, 4);
    lv_obj_ARM.Encoder_Msg.Pedestal.label_Over_Speed_Flag = lv_obj_get_child(cont, 8);
    lv_obj_ARM.Encoder_Msg.Pedestal.label_Weak_Magnetic_Flag = lv_obj_get_child(cont, 9);
    lv_obj_ARM.Encoder_Msg.Pedestal.label_Low_Voltage_Flag = lv_obj_get_child(cont, 10);
    
    return sub_page;
}

void ARM_create(lv_obj_t * parent)
{	
    lv_obj_t * cont;
    lv_obj_t * section;
    lv_obj_t * menu = lv_obj_get_parent(lv_obj_get_parent(parent));
    /*第一块*/
    lv_menu_separator_create(parent);
    section = lv_menu_section_create(parent);
    /*调试总开关*/
    cont = create_switch(section, NULL, NULL, "开启调试", &my_style_font_DingTalk_JinBuTi_Chinese_16);
    lv_obj_ARM.sw_debug = lv_obj_get_child(cont, 1);
    lv_obj_set_width(lv_obj_ARM.sw_debug,70);
    lv_obj_add_event_cb(lv_obj_ARM.sw_debug, switch_event_cb_debug, LV_EVENT_VALUE_CHANGED, NULL);
    /*动力开关*/
    cont = create_switch(section, NULL, NULL, "动力控制", &my_style_font_DingTalk_JinBuTi_Chinese_16);
    lv_obj_ARM.sw_power = lv_obj_get_child(cont, 1);
    lv_obj_set_width(lv_obj_ARM.sw_power,70);
    lv_obj_add_event_cb(lv_obj_ARM.sw_power, switch_event_cb_power, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_add_state(lv_obj_ARM.sw_power,  LV_STATE_DISABLED);
    /*第二块*/
    lv_menu_separator_create(parent);
    section = lv_menu_section_create(parent);
    /*末端控制*/
    lv_obj_t * sub_page_end_control = lv_menu_page_create_end_control(menu);
    sub_page_end_control_lable = create_text(section, NULL, &my_style_pic, "末端控制", &my_style_font_DingTalk_JinBuTi_Chinese_16);
    lv_menu_set_load_page_event(menu, sub_page_end_control_lable, sub_page_end_control);
    lv_obj_add_state(sub_page_end_control_lable,  LV_STATE_DISABLED);
    /*电机调试*/
    lv_obj_t * sub_page_motor_debug = lv_menu_page_create_motor_debug(menu);
    sub_page_motor_debug_lable = create_text(section, NULL, &my_style_pic, "电机调试", &my_style_font_DingTalk_JinBuTi_Chinese_16);
    lv_menu_set_load_page_event(menu, sub_page_motor_debug_lable, sub_page_motor_debug);
    lv_obj_add_state(sub_page_motor_debug_lable,  LV_STATE_DISABLED);
    /*第三块*/
    lv_menu_separator_create(parent);
    section = lv_menu_section_create(parent);
    /*编码器信息*/
    lv_obj_t * sub_page_encoder_msg = lv_menu_page_create_encoder_msg(menu);
    lv_obj_t * sub_page_encoder_msg_lable = create_text(section, NULL, &my_style_pic, "编码器信息", &my_style_font_DingTalk_JinBuTi_Chinese_16);
    lv_menu_set_load_page_event(menu, sub_page_encoder_msg_lable, sub_page_encoder_msg);
}

void Arm_Display()
{
    char data[10];

    sprintf(data, "%05.1f", Rear_Encoder);
    lv_label_set_text(lv_obj_ARM.Encoder_Msg.Rear.label_Encoder, data);
    if(Rear_Angle >= 0) sprintf(data, "+%05.1f", Rear_Angle);
    else                sprintf(data, "%05.1f", Rear_Angle);
    lv_label_set_text(lv_obj_ARM.Encoder_Msg.Rear.label_Angle, data);
    lv_label_set_text(lv_obj_ARM.Motor_Debug.label_Angle_Rear_Arm, data);
    if(Sensor_Msg.Encoder_Data[0].Over_Speed_Flag)  lv_label_set_text(lv_obj_ARM.Encoder_Msg.Rear.label_Over_Speed_Flag, "是");
    else                                            lv_label_set_text(lv_obj_ARM.Encoder_Msg.Rear.label_Over_Speed_Flag, "否");
    if(Sensor_Msg.Encoder_Data[0].Weak_Magnetic_Flag)  lv_label_set_text(lv_obj_ARM.Encoder_Msg.Rear.label_Weak_Magnetic_Flag, "是");
    else                                            lv_label_set_text(lv_obj_ARM.Encoder_Msg.Rear.label_Weak_Magnetic_Flag, "否");
    if(Sensor_Msg.Encoder_Data[0].Low_Voltage_Flag)  lv_label_set_text(lv_obj_ARM.Encoder_Msg.Rear.label_Low_Voltage_Flag, "是");
    else                                            lv_label_set_text(lv_obj_ARM.Encoder_Msg.Rear.label_Low_Voltage_Flag, "否");

    sprintf(data, "%05.1f", Front_Encoder);
    lv_label_set_text(lv_obj_ARM.Encoder_Msg.Front.label_Encoder, data);
    if(Front_Angle >= 0) sprintf(data, "+%05.1f", Front_Angle);
    else                sprintf(data, "%05.1f", Front_Angle);
    lv_label_set_text(lv_obj_ARM.Encoder_Msg.Front.label_Angle, data);
    lv_label_set_text(lv_obj_ARM.Motor_Debug.label_Angle_Front_Arm, data);
    if(Sensor_Msg.Encoder_Data[1].Over_Speed_Flag)  lv_label_set_text(lv_obj_ARM.Encoder_Msg.Front.label_Over_Speed_Flag, "是");
    else                                            lv_label_set_text(lv_obj_ARM.Encoder_Msg.Front.label_Over_Speed_Flag, "否");
    if(Sensor_Msg.Encoder_Data[1].Weak_Magnetic_Flag)  lv_label_set_text(lv_obj_ARM.Encoder_Msg.Front.label_Weak_Magnetic_Flag, "是");
    else                                            lv_label_set_text(lv_obj_ARM.Encoder_Msg.Front.label_Weak_Magnetic_Flag, "否");
    if(Sensor_Msg.Encoder_Data[1].Low_Voltage_Flag)  lv_label_set_text(lv_obj_ARM.Encoder_Msg.Front.label_Low_Voltage_Flag, "是");
    else                                            lv_label_set_text(lv_obj_ARM.Encoder_Msg.Front.label_Low_Voltage_Flag, "否");

    sprintf(data, "%05.1f", Pedestal_Encoder);
    lv_label_set_text(lv_obj_ARM.Encoder_Msg.Pedestal.label_Encoder, data);
    if(Pedestal_Angle >= 0) sprintf(data, "+%05.1f", Pedestal_Angle);
    else                sprintf(data, "%05.1f", Pedestal_Angle);
    lv_label_set_text(lv_obj_ARM.Encoder_Msg.Pedestal.label_Angle, data);
    lv_label_set_text(lv_obj_ARM.Motor_Debug.label_Angle_Pedestal, data);
    if(Sensor_Msg.Encoder_Data[2].Over_Speed_Flag)  lv_label_set_text(lv_obj_ARM.Encoder_Msg.Pedestal.label_Over_Speed_Flag, "是");
    else                                            lv_label_set_text(lv_obj_ARM.Encoder_Msg.Pedestal.label_Over_Speed_Flag, "否");
    if(Sensor_Msg.Encoder_Data[2].Weak_Magnetic_Flag)  lv_label_set_text(lv_obj_ARM.Encoder_Msg.Pedestal.label_Weak_Magnetic_Flag, "是");
    else                                            lv_label_set_text(lv_obj_ARM.Encoder_Msg.Pedestal.label_Weak_Magnetic_Flag, "否");
    if(Sensor_Msg.Encoder_Data[2].Low_Voltage_Flag)  lv_label_set_text(lv_obj_ARM.Encoder_Msg.Pedestal.label_Low_Voltage_Flag, "是");
    else                                            lv_label_set_text(lv_obj_ARM.Encoder_Msg.Pedestal.label_Low_Voltage_Flag, "否");
    
}

static void Arm_Symbol_Set(lv_obj_t * pic, bool sign)
{
    void *src;
    if(sign)    src = USR_SYMBOL_Complete;
    else        src = USR_SYMBOL_Underway;
	lv_img_set_src(pic, src);
}

void Arm_Symbol_Refresh()
{
//    Arm_Symbol_Set(lv_obj_ARM.Motor_Debug.Symbol_Rear_Arm, Motro_Control.Angle_Control.Done_sign[Arm_Rear_Address-1]);
//    Arm_Symbol_Set(lv_obj_ARM.Motor_Debug.Symbol_Front_Arm, Motro_Control.Angle_Control.Done_sign[Arm_Front_Address-1]);
//    Arm_Symbol_Set(lv_obj_ARM.Motor_Debug.Symbol_Pedestal, Motro_Control.Angle_Control.Done_sign[Arm_Pedestal_Address-1]);
}