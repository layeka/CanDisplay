#ifndef _display_h_
#define _display_h_

extern void lcd_init(void);
extern void lcd_cmd(unsigned char command);
extern void lcd_write(uint8_t data,uint8_t rs);
extern void lcd_clr(void);
extern void lcd_home(void);
extern void lcd_putc(unsigned char c);
extern void lcd_puts(const char *s);
extern void lcd_gotoxy(uint8_t x, uint8_t y);
extern void lcd_set_contrast(uint8_t value);

#endif // _display_h_