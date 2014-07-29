/*
 * log.c
 *
 * Created: 6/30/2014 3:24:58 PM
 *  Author: sailbot
 */ 
#include <log.h>

#include <string.h>
#include <stdio.h>

#include <sd_mmc.h>
#include <sd_mmc_mem.h>
#include <ff.h>
#include <ctrl_access.h>

#include <conf_custom.h>

extern FIL log_file_object;

// writes log files to SD card
// the log files are numbered 1,2,3,4....
int write_log(char* data, char* filename)
{
	char log_file_name[20] = "0:";
	
	Ctrl_status status;
	FRESULT res;
	FATFS fs;
	
	/* Wait card present and ready */
	do {
		status = sd_mmc_test_unit_ready(0);
		
		if(status == CTRL_NO_PRESENT)
		{
			delay_ms(500); // give it some more time
			
			if(sd_mmc_test_unit_ready(0) == CTRL_NO_PRESENT)
				return 1;
		}
		
	} while (CTRL_GOOD != status);
	
	memset(&fs, 0, sizeof(FATFS));
	res = f_mount(LUN_ID_SD_MMC_0_MEM, &fs);
	if (FR_INVALID_DRIVE == res) {
		
		return 1; // fail
	}
	
	log_file_name[0] = LUN_ID_SD_MMC_0_MEM + '0';
	
	uint8_t filenumber = 0;
	
	do
	{
		sprintf(log_file_name, "%s%d.txt\0", filename, filenumber++);
		
		res = f_open(&log_file_object,
				(char const *)log_file_name,
				FA_WRITE | FA_CREATE_NEW);
	}while(res == FR_EXIST);
	
	
	if (res != FR_OK) {
		return 1; // fail
	}
	 
	if (0 == f_puts(data, &log_file_object)) {
			printf("fail");
  	}
  
	f_close(&log_file_object);
	
	return 0;
}

void close_log_file(void)
{
	f_close(&log_file_object);
}