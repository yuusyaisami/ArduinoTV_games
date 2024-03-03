//tetris
#include <TVout.h>
#include <fontALL.h>
#include <math.h>
#define PI 3.141592653589793
class Mino{
  public:
  unsigned char x[4] = {0, 0, 0, 0};
  unsigned char y[4] = {0, 0, 0, 0};
};
class JoyStick{
  public:
    class pinData{ //ピンの構造体
      public:
      int x = A0;
      int y = A1;
      pinData(){
        pinMode(x, INPUT);
        pinMode(y, INPUT);
      }
    };
    pinData pin = pinData();
    double x = 0;
    double y = 0;
    JoyStick(){
      
    }
    double normalization(int posi){
      return (double(posi) / 508);
    }
    double noise_reduction(double posi){
      if(posi < 0.05 && posi > -0.05){
        return 0.00;
      }
      if(posi > 0.85){
        return 1.00;
      }
      if(posi < -0.85){
        return -1.00;
      }
      return posi;
    }
    double read_x(){
      x = normalization(analogRead(pin.x));
      x = x - 1.01;
      x = noise_reduction(x);
      return x;
    }
    double read_y(){
      y = normalization(analogRead(pin.y));
      y = y - 1.01;
      y = noise_reduction(y);
      return y;
    }
  };


class Timer{
  public:
  int count = 0;
  bool enable = false;
  bool Do(int goal, bool arg_enable = false){
    if(count > goal){
      count = 0;
      enable = arg_enable;
      return true;
    }
    count++;
    return false;
  }
  void reset(bool arg_enable = false){
    count = 0;
    enable = arg_enable;
  }
};
class Player{
  public:
  int posi_x[4] = {1,1,2,2};
  int posi_y[4] = {3,2,2,1};
  int angle = 0; //弧度法 90 : π / 2
  int x = 3;
  int y = 0;
  void rotate_move(int arg_angle = 0){
    if(arg_angle == 90){
      if(angle == 270){
        angle = 0;
      }
      else{
        angle -= 90;
      }
    }
    else if(arg_angle == -90){
      if(angle == 0){
        angle = 270;
      }
      else{
        angle += 90;
      }
    }
  }
  void posi_center(){
    for(int i = 0; i < 4; i++){
      posi_x[i] = posi_x[i] - 2;
    }
    for(int i = 0; i < 4; i++){
      posi_y[i] = posi_y[i] - 2;
    }
  }
  void posi_return(){
    for(int i = 0; i < 4; i++){
      posi_x[i] = posi_x[i] + 3;
    }
    for(int i = 0; i < 4; i++){
      posi_y[i] = posi_y[i] + 3;
    }
  }


  void rotate(int arg_angle){
    if(arg_angle > 0){
      rotate_move(90);
    }
    if(arg_angle < 0){
      rotate_move(-90);
    }
    posi_center();
    for(int i = 0; i < 4; i++){
      posi_x[i] = posi_x[i] * cos(angle) - posi_y[i] * sin(angle);
      posi_y[i] = posi_y[i] * sin(angle) + posi_x[i] * cos(angle);
    }
    posi_return();
  }
};
class Controller{
  public:
  JoyStick joystick = JoyStick();
  Timer input_timer = Timer();
  bool oneinput_flag = false;
  Controller(){
    
  }
  void FlagRelease(){
    if(joystick.x != 1 && joystick.x != -1 && joystick.y != 1 && joystick.y != -1){
      oneinput_flag = false;
    }
  }
  bool get_right(){
    if(joystick.x == 1 && oneinput_flag == false){
      oneinput_flag = true;
      return true;
    }
    FlagRelease();
    return false;
  }
  bool get_left(){
    if(joystick.x == -1 && oneinput_flag == false){
      oneinput_flag = true;
      return true;
    }
    FlagRelease();
    return false;
  }
  bool get_up(){
    if(joystick.y == -1 && oneinput_flag == false){
      oneinput_flag = true;
      return true;
    }
    FlagRelease();
    return false;
  }
  bool get_down(){
    if(joystick.y == 1 && oneinput_flag == false){
      oneinput_flag = true;
      return true;
    }
    FlagRelease();
    return false;
  }
};
Controller controller = Controller();
Player player = Player();
Timer main_update = Timer();
Mino OMino = Mino();
Mino IMino = Mino();
Mino JMino = Mino();
Mino LMino = Mino();
Mino TMino = Mino();
Mino SMino = Mino();
Mino ZMino = Mino();
bool game_flag = false;
unsigned char world_map[21][12];
void world_map_reset(){
  for(unsigned char r = 0; r < 20; r++){
    for(unsigned char c = 0; c < 10; c++){
      world_map[r][c] = 0;
    }
  }

  for(unsigned char i = 0; i < 21; i++){
    world_map[i][0] = 99;
    world_map[i][11] = 99;
  }
  for(unsigned char i = 0; i < 12; i++){
    world_map[20][i] = 99;
  }
}
void init_mino(){
    OMino.x[0] = OMino.x[2] = 1;
    OMino.x[1] = OMino.x[3] = 2;

    OMino.y[0] = OMino.y[2] = 1;
    OMino.y[1] = OMino.y[3] = 2;

    IMino.x[0] = IMino.x[1] = IMino.x[2] = IMino.x[3] = 2;
    IMino.y[0] = 0;
    IMino.y[1] = 1;
    IMino.y[2] = 2;
    IMino.y[3] = 3;

    JMino.x[0] = JMino.x[1] = JMino.x[2] = 2;
    JMino.x[3] = 1;

    JMino.y[0] = 1;
    JMino.y[1] = 2;
    JMino.y[2] = JMino.y[3] = 3;

    LMino.x[0] = LMino.x[1] = LMino.x[2] = 2;
    LMino.x[3] = 3;

    LMino.y[0] = 1;
    LMino.y[1] = 2;
    LMino.y[2] = LMino.y[3] = 3;

    TMino.x[0] = 1;
    TMino.x[1] = TMino.x[3] = 2;
    TMino.x[2] = 3;

    TMino.y[0] = TMino.y[1] = TMino.y[2] = 2;
    TMino.y[3] = 1;

    SMino.x[0] = SMino.x[1] = 1;
    SMino.x[2] = SMino.x[3] = 2;

    SMino.y[0] = 1;
    SMino.y[1] = SMino.y[2] = 2;
    SMino.y[3] = 3;

    ZMino.x[0] = ZMino.x[1] = 2;
    ZMino.x[2] = ZMino.x[3] = 1;

    ZMino.y[0] = 1;
    ZMino.y[1] = ZMino.y[2] = 2;
    ZMino.y[3] = 3;
}
void GameOver(){
  for(int r = 0; r < 21; r++){
    for(int c = 0; c < 12; c++){
      if(world_map[r][c] != 99){
        world_map[r][c] = 0;
      }
    }
  }
  game_flag = false;
}
bool CheckDownBlock(){
  for(int i = 0; i < 4; i++){
    if(world_map[player.y + player.posi_y[i] + 1][player.x + player.posi_x[i]] != 0){
      return true;
    }
  }
  return false;
}
void MovePlayer(){
  if(controller.get_right() == true){
    for(int i = 0; i < 4; i++){
      if(world_map[player.y + player.posi_y[i]][player.x + player.posi_x[i] + 1] != 0){
        return -1;
      }
    }
    player.x += 1;
  }
  if(controller.get_left() == true){
    for(int i = 0; i < 4; i++){
      if(world_map[player.y + player.posi_y[i]][player.x + player.posi_x[i] - 1] != 0){
        return -1;
      }
    }
    player.x -= 1;
  }
}
int SetBlock(){
  for(int i = 0; i < 4; i++){
    world_map[player.y + player.posi_y[i]][player.x + player.posi_x[i]] = 1;    
  }
  CreatePlayer();
  return -1;
}
int CreatePlayer(){
  player.x = 5;
  player.y = 0;
  int rand = random(0, 7);
  if(rand == 0){
    SetPlayerMino("OMino");
  }
  if(rand == 1){
    SetPlayerMino("IMino");
  }
  if(rand == 2){
    SetPlayerMino("JMino");
  }
  if(rand == 3){
    SetPlayerMino("LMino");
  }
  if(rand == 4){
    SetPlayerMino("SMino");
  }
  if(rand == 5){
    SetPlayerMino("ZMino");
  }
  if(rand == 6){
    SetPlayerMino("TMino");
  }

  for(int i = 0; i < 4; i++){
      if(world_map[player.y + player.posi_y[i]][player.x + player.posi_x[i]] != 0){
        //game over
        GameOver();
      }
    }
    return -1;
}
void SetPlayerMino(String type){
  if(type == "OMino"){
    for(int i = 0; i < 4; i++){
      player.posi_x[i] = OMino.x[i];
      player.posi_y[i] = OMino.y[i];
    }
  }
  else if(type == "IMino"){
    for(int i = 0; i < 4; i++){
      player.posi_x[i] = IMino.x[i];
      player.posi_y[i] = IMino.y[i];
    }
  }
  else if(type == "JMino"){
    for(int i = 0; i < 4; i++){
      player.posi_x[i] = JMino.x[i];
      player.posi_y[i] = JMino.y[i];
    }
  }
  else if(type == "LMino"){
    for(int i = 0; i < 4; i++){
      player.posi_x[i] = LMino.x[i];
      player.posi_y[i] = LMino.y[i];
    }
  }
  else if(type == "SMino"){
    for(int i = 0; i < 4; i++){
      player.posi_x[i] = SMino.x[i];
      player.posi_y[i] = SMino.y[i];
    }
  }
  else if(type == "ZMino"){
    for(int i = 0; i < 4; i++){
      player.posi_x[i] = ZMino.x[i];
      player.posi_y[i] = ZMino.y[i];
    }
  }
  else if(type == "TMino"){
    for(int i = 0; i < 4; i++){
      player.posi_x[i] = TMino.x[i];
      player.posi_y[i] = TMino.y[i];
    }
  }
}
void DownBlock(){
  player.y += 1;
}
TVout TV;
void setup() {
  TV.begin(NTSC,120,96);
  TV.select_font(font6x8);

  Serial.begin(9600);
  world_map_reset();
  init_mino();
}
void loop() {
  TV.draw_line(0,20,60,76,INVERT);
  if(game_flag == true){
    //Serial.println("g");
    if(main_update.Do(10, true) == true){
      if(CheckDownBlock() == true){
        SetBlock();
        CreatePlayer();
      }
      else{
        DownBlock();
      }
    }
  }
  else{
    TV.println(0, 0, "Tetris\nclick to start...");
    //Serial.println("p");
    if(controller.joystick.x == 1 || controller.joystick.x == -1){
      game_flag = true;
    }
  }

  
  TV.delay_frame(1);
  //Serial.println("delay");
  TV.clear_screen();
  //Serial.println("clear");
}