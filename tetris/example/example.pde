//
#include <TVout.h>
#include <fontALL.h>
TVout TV;

void setup() {
  TV.begin(NTSC,120,96);
  TV.select_font(font6x8);
}
int i = 0;
void loop() {
  TV.draw_line(i,20,60,76,INVERT);
  TV.draw_line(30,30,32,30,INVERT);
  i++;
  if(i > 119){
    i = 0;
  }
  TV.delay_frame(8);
  TV.clear_screen();
}