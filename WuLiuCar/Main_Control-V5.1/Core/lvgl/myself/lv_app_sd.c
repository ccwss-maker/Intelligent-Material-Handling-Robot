#include "lv_app_sd.h"
#include "stdio.h"
lv_obj_SD_ lv_obj_SD;

static void btn_event_handler(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED) 
	{
		// FATFS fs;                       /* FatFs 文件系统对象 */
		// FIL file;                       /* 文件对象 */
		// FRESULT f_res;                  /* 文件操作结果 */
		// UINT fnum;                      /* 文件成功读写数量 */
		// BYTE ReadBuffer[1024] = {0};    /* 读缓冲区 */
		BYTE WriteBuffer[] = "This is STM32 working with FatFs\r\n";       /* 写缓冲区 */

		// // f_res = f_mount(&fs, "0:", 1);
		// f_res = f_open(&file, "0:FatFs STM32cube.txt", FA_CREATE_ALWAYS | FA_WRITE);
		// f_res = f_write(&file, WriteBuffer, sizeof(WriteBuffer), &fnum);
		// if(f_res == FR_OK)
		// {
		// 	lv_label_set_text(lv_obj_SD.label, "3333");
		// }
		// f_close(&file);

		lv_fs_file_t lv_file;
		lv_fs_res_t  lv_res;
		
		uint32_t read_num;
		lv_res = lv_fs_open( &lv_file, "0:/FatFs STM32cube.txt", LV_FS_MODE_WR );
		// lv_res = lv_fs_write(&lv_file, WriteBuffer, sizeof(WriteBuffer), &read_num);
			// char buf[8];
			// lv_res = lv_fs_read(&lv_file, buf, 6, &read_num);
		if ( lv_res == LV_FS_RES_OK ) 
		{
			lv_label_set_text(lv_obj_SD.label, "111");
		}
		lv_fs_close(&lv_file);

		// else 
		// {
		// 	lv_label_set_text(lv_obj_SD.label, "2222");
		// }
    }
}
void SD_create(lv_obj_t * parent)
{
///////////////////////////////////////按键//////////////////////////////////////////
	lv_obj_t * btn=lv_btn_create(parent);
	lv_obj_set_size(btn , 100 , 40);
	lv_obj_add_event_cb(btn, btn_event_handler,LV_EVENT_ALL,NULL);
	lv_obj_align_to(btn, parent,LV_ALIGN_CENTER, 0,-110);
	lv_obj_t* btn_label = lv_label_create(btn);	
	lv_label_set_text(btn_label, "RSTET");
	lv_obj_align_to(btn_label, btn, LV_ALIGN_CENTER, 0, 0);	
//////////////////////////////////////字///////////////////////////////////////////
    lv_obj_SD.label = lv_label_create(parent);
	lv_label_set_text(lv_obj_SD.label, "0000");
	lv_obj_align_to(lv_obj_SD.label, parent,LV_ALIGN_CENTER,0,0);
}