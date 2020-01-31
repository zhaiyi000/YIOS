#include "bootpack.h"

extern struct FIFO8 keyfifo;
extern struct FIFO8 mousefifo;

void init_keyboard(void);
void enable_mouse(void);

void yiPrintf(){
    struct BootInfo *bInfo = (struct BootInfo *)ADR_BOOTINFO;
    boxfill8(bInfo->VRAM, bInfo->SCRNX, COL8_RED, 0, 20, 310, 36);
    putfont8_asc(bInfo->VRAM, bInfo->SCRNX, 0, 20, COL8_YELLOW, "wo lai~~~~~");
}

void HariMain(){

    char s[100];
	struct BootInfo *bInfo = (struct BootInfo *)ADR_BOOTINFO;
	unsigned char *vram = bInfo->VRAM;
	int xsize = bInfo->SCRNX;
	int ysize = bInfo->SCRNY;
    char keyBuf__[32];
    char mouseBuf__[128];
    fifo8_init(&keyfifo, 32, keyBuf__);
    fifo8_init(&mousefifo, 128, mouseBuf__);

    init_gdtidt();
    init_pic();
    io_sti();   //这个地方产生了一个bug,调试了好久.....
	init_palette();
	init_screen(vram, xsize, ysize);

    init_keyboard();
    enable_mouse();
    
	//logo
    boxfill8(bInfo->VRAM, bInfo->SCRNX, COL8_RED, 0, 0, 310, 18);
	putfont8_asc(vram, xsize,1,1,COL8_YELLOW,"HELLO YIOS");
	putfont8_asc(vram, xsize,0,0,COL8_YELLOW,"HELLO YIOS");


	//鼠标
	char mouse[256];
	init_mouse_cursor8(mouse,COL8_RED);
	putblock8_8(vram,xsize,16,16,160,100,mouse,16);

    unsigned char data;
    int mouse_phase = 0;
    char mouseBuf[5] = {};
	for(;;){
        io_cli();
        if (fifo8_status(&keyfifo) + fifo8_status(&mousefifo)==0) {
            io_stihlt();
        }else{
            if (fifo8_status(&keyfifo)!=0) {
                data = fifo8_get(&keyfifo);
                io_sti();
                sprintf(s, "%02X",data);
                
                struct BootInfo *bInfo = (struct BootInfo *)ADR_BOOTINFO;
                boxfill8(bInfo->VRAM, bInfo->SCRNX, COL8_RED, 0, 40, 310, 56);
                putfont8_asc(bInfo->VRAM, bInfo->SCRNX, 0, 40, COL8_YELLOW, s);
            }else if(fifo8_status(&mousefifo)!=0){
                data = fifo8_get(&mousefifo);
                io_sti();
                
                if (mouse_phase==0 && data == 0xfa) {
                    mouse_phase = 1;
                }else if(mouse_phase == 1){
                    mouse_phase = 2;
                    mouseBuf[0] = data;
                }else if(mouse_phase == 2){
                    mouse_phase = 3;
                    mouseBuf[1] = data;
                }else if(mouse_phase == 3){
                    mouse_phase = 1;
                    mouseBuf[2] = data;
                    
                    sprintf(s, "%08X %08X %08X",mouseBuf[0],mouseBuf[1],mouseBuf[2]);
                    
                    struct BootInfo *bInfo = (struct BootInfo *)ADR_BOOTINFO;
                    boxfill8(bInfo->VRAM, bInfo->SCRNX, COL8_RED, 0, 60, 310, 76);
                    putfont8_asc(bInfo->VRAM, bInfo->SCRNX, 0, 60, COL8_YELLOW, s);
                }
                
                
            }
        }
	}
    
    
}


void wait_KBC_sendready(void){
    for (; ; ) {
        if ((io_in8(0x64)&0x2)==0) {
            break;
        }
    }
}

        
void init_keyboard(void){
    wait_KBC_sendready();
    io_out8(0x64, 0x60);
    wait_KBC_sendready();
    io_out8(0x60, 0x47);
}

void enable_mouse(void) {
    wait_KBC_sendready();
    io_out8(0x64, 0xd4);
    wait_KBC_sendready();
    io_out8(0x60, 0xf4);  // 0x64是控制和状态端口,0x60是数据端口
}

