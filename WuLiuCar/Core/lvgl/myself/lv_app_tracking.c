#include "lv_app_tracking.h"
#include "Tracking.h"
lv_obj_Tracking_ lv_obj_Tracking;

static lv_obj_t * create_Title(lv_obj_t * parent, const char * txt, lv_style_t* style_font_p)
{
    lv_obj_t * obj;
    lv_obj_t * label;
    static lv_coord_t col_dsc[] = {30, 12, 12, 12, 12, 12, 12, 12, 12, LV_GRID_TEMPLATE_LAST};
	static lv_coord_t row_dsc[] = {17, LV_GRID_TEMPLATE_LAST}; /*3 100 px tall rows*/
    
    obj = lv_menu_cont_create(parent);
    lv_obj_set_grid_dsc_array(obj, col_dsc, row_dsc);
    /*文字1*/
    label = lv_label_create(obj);
    if(style_font_p)
    {
        lv_obj_add_style(label,style_font_p,LV_STATE_DEFAULT);
    }
    lv_label_set_text(label, txt);
	lv_obj_set_grid_cell(label, LV_GRID_ALIGN_CENTER, 0, 1, LV_GRID_ALIGN_START, 0, 1);
    /*文字1*/
    char data[10]={0};
    for(uint8_t i = 0; i<8; i++)
    {
        label = lv_label_create(obj);
        if(style_font_p)
        {
            lv_obj_add_style(label,style_font_p,LV_STATE_DEFAULT);
        }
        lv_snprintf(data, sizeof(data), "%d", i+1);
        lv_label_set_text(label, data);
        lv_obj_set_grid_cell(label, LV_GRID_ALIGN_CENTER, i+1, 1, LV_GRID_ALIGN_START, 0, 1);
    }

    return obj;
}

static lv_obj_t * create_Circle(lv_obj_t * parent, const char * txt, lv_style_t* style_font_p, const char * icon, lv_style_t* style_pic_p)
{
    lv_obj_t * obj;
    lv_obj_t * label;
    lv_obj_t * img;
    static lv_coord_t col_dsc[] = {30, 12, 12, 12, 12, 12, 12, 12, 12, LV_GRID_TEMPLATE_LAST};
	static lv_coord_t row_dsc[] = {17, LV_GRID_TEMPLATE_LAST}; /*3 100 px tall rows*/
    
    obj = lv_menu_cont_create(parent);
    lv_obj_set_grid_dsc_array(obj, col_dsc, row_dsc);
    /*文字*/
    label = lv_label_create(obj);
    if(style_font_p)
    {
        lv_obj_add_style(label,style_font_p,LV_STATE_DEFAULT);
    }
    lv_label_set_text(label, txt);
	lv_obj_set_grid_cell(label, LV_GRID_ALIGN_CENTER, 0, 1, LV_GRID_ALIGN_START, 0, 1);
    /*图片*/
    for(uint8_t i = 0; i<8; i++)
    {
        img = lv_img_create(obj);
        if(style_pic_p)
        {
            lv_obj_add_style(img,style_pic_p,LV_STATE_DEFAULT);
        }
        lv_img_set_src(img, icon);
        lv_obj_set_grid_cell(img, LV_GRID_ALIGN_CENTER, i+1, 1, LV_GRID_ALIGN_START, 0, 1);
    }

    return obj;
}

void Circle_Set(lv_obj_t* Circle, uint8_t sign)
{
    if(sign)
    {
        lv_img_set_src(Circle, USR_SYMBOL_Solid_Circle);
    }
    else
    {
        lv_img_set_src(Circle, USR_SYMBOL_Hollow_Circle);
    }
}

void Tracking_create(lv_obj_t * parent)
{
    lv_obj_t * cont;
    lv_obj_t * section;

    /*第一块*/
    lv_menu_separator_create(parent);
    section = lv_menu_section_create(parent);
    /*各组信息*/
    cont = create_text(section, NULL, NULL, "各组信息", &my_style_font_DingTalk_JinBuTi_Chinese_16);
    /*第二块*/
    lv_menu_separator_create(parent);
    section = lv_menu_section_create(parent);
    /*组号*/
    create_Title(section, "组号", &my_style_font_DingTalk_JinBuTi_Chinese_16);
    /*前*/  
    cont =create_Circle(section, "前", &my_style_font_DingTalk_JinBuTi_Chinese_16, USR_SYMBOL_Hollow_Circle, &my_style_pic);
    for(uint8_t i = 0; i<8; i++)
    {
        lv_obj_Tracking.Tracking_Kid[0].Circle[i] = lv_obj_get_child(cont, i+1);
    }
    /*后*/
    cont =create_Circle(section, "后", &my_style_font_DingTalk_JinBuTi_Chinese_16, USR_SYMBOL_Hollow_Circle, &my_style_pic);
    for(uint8_t i = 0; i<8; i++)
    {
        lv_obj_Tracking.Tracking_Kid[1].Circle[i] = lv_obj_get_child(cont, i+1);
    }
    /*左*/
    cont =create_Circle(section, "左", &my_style_font_DingTalk_JinBuTi_Chinese_16, USR_SYMBOL_Hollow_Circle, &my_style_pic);
    for(uint8_t i = 0; i<8; i++)
    {
        lv_obj_Tracking.Tracking_Kid[2].Circle[i] = lv_obj_get_child(cont, i+1);
    }
    /*右*/
    cont =create_Circle(section, "右", &my_style_font_DingTalk_JinBuTi_Chinese_16, USR_SYMBOL_Hollow_Circle, &my_style_pic);
    for(uint8_t i = 0; i<8; i++)
    {
        lv_obj_Tracking.Tracking_Kid[3].Circle[i] = lv_obj_get_child(cont, i+1);
    }
}

void Tracking_Draw()
{
    uint8_t data;
    for(uint8_t i=0; i<4; i++)
    {
        data = Tracking.data[i];
        for(uint8_t j=0; j<8; j++)
        {
            Circle_Set(lv_obj_Tracking.Tracking_Kid[i].Circle[j], (data>>j)&0x01);
        }
    }
}