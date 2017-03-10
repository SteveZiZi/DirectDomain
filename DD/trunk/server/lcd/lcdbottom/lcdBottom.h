#ifndef LCD_BOTTOM_H
#define LCD_BOTTOM_H
#define CONS_LCD_WIDTH 160
#define CONS_LCD_HEIGHT 160

extern char * fbptr;

extern int FrameBuffer_fd;
extern int  Button_fd;
extern int  LCDLight_fd;

extern int xres, yres,fbpp, line_length;
extern char mold[32];
extern void ScreenDirection_PixelBitblt_1(unsigned char Pixel, unsigned char *pScreen, unsigned char BitMask);
extern void ScreenDirection_PixelBitblt_3(unsigned char Pixel, unsigned char *pScreen, unsigned char BitMask);
extern void ScreenDirection_0(unsigned char *pScreenBuffer, const unsigned char *pUserBuffer);
extern void ScreenDirection_1(unsigned char *pScreenBuffer, const unsigned char *pUserBuffer);
extern void ScreenDirection_2(unsigned char *pScreenBuffer, const unsigned char *pUserBuffer);
extern void ScreenDirection_3(unsigned char *pScreenBuffer, const unsigned char *pUserBuffer);
extern void fb_fresh();

extern int fb_init(void);
extern void fb_clear(unsigned colidx);
extern void LCD_SetContrast(int contrast);//设屏幕对比度 zhgx
extern void LCD_SetGrayness(int grayness);//设屏幕灰度 zhgx
extern void fb_blacklight(int on); //on =1 开
extern int Get_Key(unsigned char *keyc);
extern void pixel (int x, int y, unsigned colidx);
extern void put_char_58(int x, int y, int c);
extern void put_char_88(int x, int y, int c);
extern  void put_char_w_16(int x, int y, int c);
extern  void put_char_w_12(int x, int y, int c);
extern int get_chi_index(char *s);
extern void GetChar24(int j,char *mold);
extern void put_string(int x, int y, char *s);
extern void line (int x1, int y1, int x2, int y2, unsigned colidx);
extern void rect (int x1, int y1, int x2, int y2, unsigned colidx);
extern void box (int x1, int y1, int x2, int y2, unsigned colidx);
#endif
