hello zy:
	org 0x7c00
	int 0x10,显示字符  p46
		al 要显示的字符
		ah = 0x0e
		bh = 0
		bl = 颜色代码
		返回值 无
	hlt 暂停cpu
	0x0a 换行
	resb 0x7bfe 填充0
	扇区结束标志 0x55,0xaa

读第二个扇区:
	读到0x0820
	读盘 int 0x13:  p54
		ah=2读;3写盘;4校验;0x0c寻道
		al=扇区数
		ch = 柱面号 0
		cl = 扇区号 2
		dh = 磁头号 0
		dl = 驱动器号 0
		es:bx = 缓冲区地址
		返回值 cf=0没有错误,ah=0
			cf=1有错误,错误号码在ah内


读第二个扇区,失败重复5次:
	int 13,重置驱动器
		ah=0
		dl = 0

读到18扇区:
	读取成功则跳至next

读取10个柱面:
	cl = 扇区号 置为1
	dh+1, 磁头号
		如果dh小于2,则跳转读	
		大于2,则置为0,ch柱面号+1
	CYLS	EQU		10

从启动区执行操作系统,并确认操作系统的执行情况:
	jmp oxc200
	int 0x10,设置显卡模式
		ah=0
		al = 0x3 0x12 0x13:320*200*8位色彩模式,调色板模式 0x61
		返回值无
	将CYLS的值写到内存0x0ff0

8. 32位模式前期准备:
		CYLS=0xff0;读取的柱面数
		LEDS=0xff1;
		VMODE=0xff2;颜色的位数 byte ,8
		SCRNX=0xff4;分辨率的x word 320
		SCRNY=0xff6;分辨率的y word 200
		VRAM=0xff8;图像缓冲区的开始地址 dw 0xa0000
		int 0x16;键盘上各种LED指示灯的状态
			ah=2
			[LEDS] = al


9. 开始导入C语言
	新建bootpack.c文件
	增加HariMain函数,无参无返回


10. 实现HLT
	增加naskfunc.nas文件 p70
	[FORMAT "WCOFF"];制作目标文件的模式
	[BITS 32];制作32位模式用的机械语言

	[FILE "naskfunc.nas"] ;源文件名信息
	GLOBAL _io_hlt  ;

	[SECTION .text];目标文件写了这些之后再写程序
	ret


day04
1. 用c语言实现内存写入
	_write_mem8
	ecx保存addr
	al保存data
	0xa0000-0xb0000,写入15
	[INSTRSET "i486p"]

2. 条纹图案
	i&0xf

3. 挑战指针

6. 色号设定
	init_palette
	set_palette(int start, int end, unsigned char *rgb)
		load_eflags
		cli
		io_out8(0x03c8,start)
		从start到end,io_out8(0x03c9,rbg[0]/4) 为什幺要除4,不懂!
		rgb+=3
		store_eflags
	_io_cli
	_io_sti
	_io_stihlt
	_io_in8
		mov edx,[esp+4]
		mov eax,0
		in al,dx
		ret
	_io_in16
	_io_in32
	_io_out8
	_io_load_eflags
		pushfd
		pop
		ret
	_io_store_eflags

7. 绘制矩形
	void boxfill8(vram,xsize,c,x0,y0,x1,y1)


day05
1. 接受启动信息
	vram
	xsize
	ysize
	init_screen函数

4. 显示字符
	putfont8(vram,xsize,x,y,c,font)

5. 增加字体
	extern char hankaku[4096];

6. 显示字符串
	putfont8_asc(vram,xsize,x,y,c,s)

7. 显示变量值
	sprintf()

8. 显示鼠标指针
	init_mouse_cursor8(mouse,bc)
	putblock8_8(vram,vxsize,pxsize,pysize,px0,py0,buf,bxsize)

9. GDT和IDT的初始化

day06
2. 整理Makefile
	第一行用%,第二行用$*
3. 整理头文件










