#include "lv_app_function.h"

lv_style_t my_style_pic;
lv_style_t my_style_font_DingTalk_JinBuTi_Chinese_16;  
lv_style_t my_style_font_DingTalk_JinBuTi_English_14;  

void lv_app_function_init()
{
	lv_style_init(&my_style_pic);
	lv_style_set_opa(&my_style_pic,LV_OPA_100);
	lv_style_set_text_font(&my_style_pic,&My_Symbol_15);

	lv_style_init(&my_style_font_DingTalk_JinBuTi_Chinese_16);
	lv_style_set_opa(&my_style_font_DingTalk_JinBuTi_Chinese_16,LV_OPA_100);
	lv_style_set_text_font(&my_style_font_DingTalk_JinBuTi_Chinese_16,&DingTalk_JinBuTi_Chinese_16);
	
	lv_style_init(&my_style_font_DingTalk_JinBuTi_English_14);
	lv_style_set_opa(&my_style_font_DingTalk_JinBuTi_English_14,LV_OPA_100);
	lv_style_set_text_font(&my_style_font_DingTalk_JinBuTi_English_14,&DingTalk_JinBuTi_English_14);
}
lv_obj_t * create_text(lv_obj_t * parent, const char * icon, lv_style_t* style_pic_p, const char * txt, lv_style_t* style_font_p)
{
    lv_obj_t * obj = lv_menu_cont_create(parent);

    lv_obj_t * img = NULL;
    lv_obj_t * label = NULL;

    if(icon) 
    {
        img = lv_img_create(obj);
        if(style_pic_p)
            lv_obj_add_style(img,style_pic_p,LV_STATE_DEFAULT);
        lv_img_set_src(img, icon);
    }

    if(txt) 
    {
        label = lv_label_create(obj);
        if(style_font_p)
        {
            lv_obj_add_style(label,style_font_p,LV_STATE_DEFAULT);
        }
        lv_label_set_text(label, txt);
        lv_label_set_long_mode(label, LV_LABEL_LONG_SCROLL_CIRCULAR);
        lv_obj_set_flex_grow(label, 1);
    }

    return obj;
}

lv_obj_t *  create_text_two(lv_obj_t * parent, const char * title, lv_style_t* style_font_title_p, char * txt, lv_style_t* style_font_txt_p)
{
    lv_obj_t * label = NULL;
    /*标题*/
    label = lv_label_create(parent);
    lv_obj_add_flag(label, LV_OBJ_FLAG_FLEX_IN_NEW_TRACK);
    if(style_font_title_p)
    {
        lv_obj_add_style(label,style_font_title_p,LV_STATE_DEFAULT);
    }
    lv_label_set_text(label, title);
    lv_obj_set_flex_grow(label, 1);
    /*内容*/
    label = lv_label_create(parent);
    if(style_font_txt_p)
    {
        lv_obj_add_style(label,style_font_txt_p,LV_STATE_DEFAULT);
    }
    lv_label_set_text(label, txt);

    return label;
}

lv_obj_t * create_switch(lv_obj_t * parent, const char * icon, lv_style_t* style_pic_p, const char * txt, lv_style_t* style_font_p)
{
    lv_obj_t * obj = create_text(parent, icon, style_pic_p, txt, style_font_p);
    lv_obj_t * sw = lv_switch_create(obj);

    return obj;
}

lv_obj_t * create_btn(lv_obj_t * parent, const char * icon, lv_style_t* style_pic_p, const char * txt, lv_style_t* style_font_p)
{
    lv_obj_t * obj = create_text(parent, icon, style_pic_p, txt, style_font_p);
    lv_obj_t * btn = lv_btn_create(obj);

    return obj;
}

lv_obj_t * create_slider(lv_obj_t * parent, const char * icon, lv_style_t* style_pic_p, const char * txt, lv_style_t* style_font_p,
                         int32_t min, int32_t max,int32_t val)
{
    lv_obj_t * obj = create_text(parent, icon, style_pic_p, txt, style_font_p);
    char string[10];
    sprintf(string,"%d",val);
    lv_obj_t * label = lv_label_create(obj);
    lv_obj_add_style(label,&my_style_font_DingTalk_JinBuTi_English_14,LV_STATE_DEFAULT);
    lv_label_set_text(label, string);

    lv_obj_t * slider = lv_slider_create(obj);
    lv_obj_set_flex_grow(slider, 1);
    lv_slider_set_range(slider, min, max);
    lv_slider_set_value(slider, val, LV_ANIM_ON);
    lv_obj_add_flag(slider, LV_OBJ_FLAG_FLEX_IN_NEW_TRACK);

    return obj;
}

lv_obj_t * create_slider_two(lv_obj_t * parent, const char * icon, lv_style_t* style_pic_p, const char * txt, lv_style_t* style_font_p,
                         int32_t min, int32_t max, float val)
{
    static lv_coord_t col_dsc[] = {28, 35, 20, 70, 6, LV_GRID_TEMPLATE_LAST};
	static lv_coord_t row_dsc[] = {18, 20, LV_GRID_TEMPLATE_LAST}; /*3 100 px tall rows*/
    
    lv_obj_t * obj = lv_menu_cont_create(parent);
    lv_obj_set_grid_dsc_array(obj, col_dsc, row_dsc);

    lv_obj_t * label;
    label = lv_label_create(obj);
    lv_obj_add_style(label,style_font_p,LV_STATE_DEFAULT);
    lv_label_set_text(label, txt);
    lv_obj_set_grid_cell(label, LV_GRID_ALIGN_START, 0, 1, LV_GRID_ALIGN_START, 0, 1);

    lv_obj_t * img = lv_img_create(obj);
    if(icon)
    {
        if(style_pic_p)
            lv_obj_add_style(img,style_pic_p,LV_STATE_DEFAULT);
        lv_img_set_src(img, icon);
        lv_obj_set_grid_cell(img, LV_GRID_ALIGN_START, 1, 1, LV_GRID_ALIGN_START, 0, 1);
    }

    char string[10];
    sprintf(string,"%5.1f",val);
    label = lv_label_create(obj);
    lv_obj_add_style(label,&my_style_font_DingTalk_JinBuTi_English_14,LV_STATE_DEFAULT);
    lv_label_set_text(label, string);
    lv_obj_set_grid_cell(label, LV_GRID_ALIGN_START, 2, 1, LV_GRID_ALIGN_START, 0, 1);
    
    label = lv_label_create(obj);
    lv_obj_add_style(label,&my_style_font_DingTalk_JinBuTi_English_14,LV_STATE_DEFAULT);
    lv_label_set_text(label, string);
    lv_obj_set_grid_cell(label, LV_GRID_ALIGN_END, 3, 1, LV_GRID_ALIGN_START, 0, 1);

    lv_obj_t * slider = lv_slider_create(obj);
    lv_obj_set_width(slider, 185);
    lv_slider_set_range(slider, min, max);
    lv_slider_set_value(slider, val, LV_ANIM_ON);
    lv_obj_set_grid_cell(slider, LV_GRID_ALIGN_CENTER, 0, 5, LV_GRID_ALIGN_START, 1, 1);

    return obj;
}

lv_obj_t * create_bar(lv_obj_t * parent, const char * icon, lv_style_t* style_pic_p, const char * txt, lv_style_t* style_font_p,
                         int32_t min, int32_t max,int32_t val)
{
    /*标题*/
    lv_obj_t * obj = create_text(parent, icon, style_pic_p, txt, style_font_p);
    /*标题右侧文字*/
    lv_obj_t * label = lv_label_create(obj);
    lv_obj_add_style(label, &my_style_font_DingTalk_JinBuTi_English_14,LV_STATE_DEFAULT);
	/*进度条文字*/
	lv_obj_t * label_bar = lv_label_create(obj);
    lv_obj_add_style(label_bar,&my_style_font_DingTalk_JinBuTi_English_14,LV_STATE_DEFAULT);
    lv_obj_add_flag(label_bar, LV_OBJ_FLAG_FLEX_IN_NEW_TRACK);
	/*进度条*/
	lv_obj_t * bar = lv_bar_create(obj);
	lv_bar_set_range(bar, min, max);
	lv_bar_set_value(bar, val, LV_ANIM_ON);
	lv_obj_set_flex_grow(bar, 1);

    return obj;
}

lv_obj_t * create_spinbox(lv_obj_t * parent, const char * icon, lv_style_t* style_pic_p, const char * txt, lv_style_t* style_font_p,
                          uint8_t digit_count, uint8_t separator_position, int32_t i)
{
    lv_obj_t * button;
    lv_obj_t * spinbox;
    /*标题*/
    lv_obj_t * obj = create_text(parent, icon, style_pic_p, txt, style_font_p);
    /*减按钮*/
    button = lv_btn_create(obj);
    lv_obj_set_style_bg_img_src(button, LV_SYMBOL_MINUS, 0);
    lv_obj_set_size(button,28,32);
    /*微调器*/
    spinbox = lv_spinbox_create(obj);
    lv_obj_add_style(spinbox,&my_style_font_DingTalk_JinBuTi_English_14,LV_STATE_DEFAULT);
    lv_spinbox_set_digit_format(spinbox, digit_count, separator_position);
    lv_spinbox_set_range(spinbox,-100000,100000);
    lv_spinbox_set_value(spinbox, i);
    lv_obj_set_size(spinbox,80,36);
    /*加按钮*/
    button = lv_btn_create(obj);
    lv_obj_set_style_bg_img_src(button, LV_SYMBOL_PLUS, 0);
    lv_obj_set_size(button,28,32);

    return obj;
}