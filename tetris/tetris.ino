//tetris
#include <TVout.h>
#include <fontALL.h>
#include <math.h>
class JoyStick{
  public:
    class pinData{ //ピンの構造体
      public:
      int x = A0;
      int y = A1;
      int btn1 = 3;
      int btn2 = 4;
      int btn3 = 2;
      pinData(){
        pinMode(x, INPUT);
        pinMode(y, INPUT);

        pinMode(btn1, INPUT);
        pinMode(btn2, INPUT);
        pinMode(btn3, INPUT);
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
    bool read_btn1(){
      if(digitalRead(pin.btn1) == 1){
        return true;
      }
      return false;
    }
    bool read_btn2(){
      if(digitalRead(pin.btn2) == 1){
        return true;
      }
      return false;
    }
    bool read_btn3(){
      if(digitalRead(pin.btn3) == 1){
        return true;
      }
      return false;
    }
  };


class Timer{
  public:
  unsigned char count = 0;
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
  unsigned char que[7] = {0, 0, 0, 0, 0, 0, 0};
  int type = 0;
  int angle = 0; //弧度法 90 : π / 2
  int x = 3;
  int y = 0;
  Player(){
    for(int i = 0; i < 7; i++){
      que[i] = random(0, 7);
    }
  }
  int get_now_que(){
    type = que[0];
    return que[0];
  }
  void shift_que(){
    for(int i = 0; i < 6; i++){
      que[i] = que[i + 1];
    }
    que[6] = random(0, 7);
  }
};
class Controller{
  public:
  JoyStick joystick = JoyStick();
  Timer input_timer = Timer();
  bool oneinput_flag = false;
  bool btn1_flag = false;
  bool btn2_flag = false;
  bool btn3_flag = false;
  Controller(){
    
  }
  void FlagRelease(){
    if(joystick.read_x() != 1 && joystick.read_x() != -1 && joystick.read_y() != 1 && joystick.read_y() != -1){
      oneinput_flag = false;
    }
    if(joystick.read_btn1() != 1){
      btn1_flag = false;
    }
    if(joystick.read_btn2() != 1){
      btn2_flag = false;
    }
    if(joystick.read_btn3() != 1){
      btn3_flag = false;
    }
  }
  bool get_right(){
    if(joystick.read_x() == 1 && oneinput_flag == false){
      oneinput_flag = true;
      return true;
    }
    FlagRelease();
    return false;
  }
  bool get_left(){
    if(joystick.read_x() == -1 && oneinput_flag == false){
      oneinput_flag = true;
      return true;
    }
    FlagRelease();
    return false;
  }
  bool get_up(){
    if(joystick.read_y() == -1 && oneinput_flag == false){
      oneinput_flag = true;
      return true;
    }
    FlagRelease();
    return false;
  }
  bool get_down(){
    if(joystick.read_y() == 1 && oneinput_flag == false){
      oneinput_flag = true;
      return true;
    }
    FlagRelease();
    return false;
  }
  bool get_btn1(){
    if(joystick.read_btn1() == 1 && btn1_flag == false){
      btn1_flag = true;
      return true;
    }
    FlagRelease();
    return false;
  }
  bool get_btn2(){
    if(joystick.read_btn2() == 1 && btn2_flag == false){
      btn2_flag = true;
      return true;
    }
    FlagRelease();
    return false;
  }
  bool get_btn3(){
    if(joystick.read_btn3() == 1){
      return true;
    }
    return false;
  }

};
TVout TV;
Controller controller = Controller();
Player player = Player();
Timer main_update = Timer();
bool game_flag = false;
unsigned char world_map[21][12];
int game_score = 0;
int game_speed = 8;
int game_level_speed = 8;
void rotate_move(int arg_angle = 0){
  if(arg_angle == 90){
    if(player.angle == 270){
      player.angle = 0;
    }
    else{
      player.angle += 90;
    }
  }
  else if(arg_angle == -90){
    if(player.angle == 0){
      player.angle = 270;
    }
    else{
      player.angle -= 90;
    }
  }
}
int get_angle_position_x(int index, int angle, int type){
  switch(type){
    case 0:
      switch(index){
        case 0: return 1;
        case 1: return 2;
        case 2: return 1;
        case 3: return 2;
      }
    case 1:
      switch(angle){
        case 0:
          switch(index){
            case 0: return 1;
            case 1: return 1;
            case 2: return 1;
            case 3: return 1;
        }
        case 90:
          switch(index){
            case 0: return 0;
            case 1: return 1;
            case 2: return 2;
            case 3: return 3;
          }
        case 180:
          switch(index){
            case 0: return 2;
            case 1: return 2;
            case 2: return 2;
            case 3: return 2;
          }
        case 270:
          switch(index){
            case 0: return 0;
            case 1: return 1;
            case 2: return 2;
            case 3: return 3;
          }
      }
    case 2:
      switch(angle){
        case 0:
          switch(index){
            case 0: return 2;
            case 1: return 2;
            case 2: return 1;
            case 3: return 2;
          }
        case 90:
          switch(index){
            case 0: return 0;
            case 1: return 0;
            case 2: return 1;
            case 3: return 2;
          }
        case 180:
          switch(index){
            case 0: return 1;
            case 1: return 2;
            case 2: return 1;
            case 3: return 1;
          }
        case 270:
          switch(index){
            case 0: return 1;
            case 1: return 2;
            case 2: return 3;
            case 3: return 3;
          }
      }
    case 3:
      switch(angle){
        case 0:
          switch(index){
            case 0: return 1;
            case 1: return 1;
            case 2: return 1;
            case 3: return 2;
          }
        case 90:
          switch(index){
            case 0: return 0;
            case 1: return 1;
            case 2: return 2;
            case 3: return 0;
          }
        case 180:
          switch(index){
            case 0: return 1;
            case 1: return 2;
            case 2: return 2;
            case 3: return 2;
          }
        case 270:
          switch(index){
            case 0: return 3;
            case 1: return 1;
            case 2: return 2;
            case 3: return 3;
          }
      }
    case 4:
      switch(angle){
        case 0:
          switch(index){
            case 0: return 2;
            case 1: return 3;
            case 2: return 1;
            case 3: return 2;
          }
        case 90:
          switch(index){
            case 0: return 1;
            case 1: return 1;
            case 2: return 2;
            case 3: return 2;
          }
        case 180:
          switch(index){
            case 0: return 1;
            case 1: return 2;
            case 2: return 0;
            case 3: return 1;
          }
        case 270:
          switch(index){
            case 0: return 1;
            case 1: return 1;
            case 2: return 2;
            case 3: return 2;
          }
      }
    case 5:
      switch(angle){
        case 0:
          switch(index){
            case 0: return 0;
            case 1: return 1;
            case 2: return 1;
            case 3: return 2;
          }
        case 90:
          switch(index){
            case 0: return 2;
            case 1: return 2;
            case 2: return 1;
            case 3: return 1;
          }
        case 180:
          switch(index){
            case 0: return 1;
            case 1: return 2;
            case 2: return 2;
            case 3: return 3;
          }
        case 270:
          switch(index){
            case 0: return 2;
            case 1: return 2;
            case 2: return 1;
            case 3: return 1;
          }
      }
    case 6:
      switch(angle){
        case 0:
          switch(index){
            case 0: return 1;
            case 1: return 0;
            case 2: return 1;
            case 3: return 2;
          }
        case 90:
          switch(index){
            case 0: return 1;
            case 1: return 1;
            case 2: return 2;
            case 3: return 1;
          }
        case 180:
          switch(index){
            case 0: return 0;
            case 1: return 1;
            case 2: return 2;
            case 3: return 1;
          }
        case 270:
          switch(index){
            case 0: return 1;
            case 1: return 0;
            case 2: return 1;
            case 3: return 1;
          }
      }
  }
  return -1;
}
int get_angle_position_y(int index, int angle, int type){
  switch(type){
    case 0:
      switch(index){
        case 0: return 1;
        case 1: return 1;
        case 2: return 2;
        case 3: return 2;
      }
    case 1:
      switch(angle){
        case 0:
          switch(index){
            case 0: return 0;
            case 1: return 1;
            case 2: return 2;
            case 3: return 3;
        }
        case 90:
          switch(index){
            case 0: return 1;
            case 1: return 1;
            case 2: return 1;
            case 3: return 1;
          }
        case 180:
          switch(index){
            case 0: return 0;
            case 1: return 1;
            case 2: return 2;
            case 3: return 3;
          }
        case 270:
          switch(index){
            case 0: return 2;
            case 1: return 2;
            case 2: return 2;
            case 3: return 2;
          }
      }
    case 2:
      switch(angle){
        case 0:
          switch(index){
            case 0: return 1;
            case 1: return 2;
            case 2: return 3;
            case 3: return 3;
          }
        case 90:
          switch(index){
            case 0: return 1;
            case 1: return 2;
            case 2: return 2;
            case 3: return 2;
          }
        case 180:
          switch(index){
            case 0: return 0;
            case 1: return 0;
            case 2: return 1;
            case 3: return 2;
          }
        case 270:
          switch(index){
            case 0: return 1;
            case 1: return 1;
            case 2: return 1;
            case 3: return 2;
          }
      }
    case 3:
      switch(angle){
        case 0:
          switch(index){
            case 0: return 1;
            case 1: return 2;
            case 2: return 3;
            case 3: return 3;
          }
        case 90:
          switch(index){
            case 0: return 1;
            case 1: return 1;
            case 2: return 1;
            case 3: return 2;
          }
        case 180:
          switch(index){
            case 0: return 0;
            case 1: return 0;
            case 2: return 1;
            case 3: return 2;
          }
        case 270:
          switch(index){
            case 0: return 1;
            case 1: return 2;
            case 2: return 2;
            case 3: return 2;
          }
      }
    case 4:
      switch(angle){
        case 0:
          switch(index){
            case 0: return 1;
            case 1: return 1;
            case 2: return 2;
            case 3: return 2;
          }
        case 90:
          switch(index){
            case 0: return 0;
            case 1: return 1;
            case 2: return 1;
            case 3: return 2;
          }
        case 180:
          switch(index){
            case 0: return 1;
            case 1: return 1;
            case 2: return 2;
            case 3: return 2;
          }
        case 270:
          switch(index){
            case 0: return 0;
            case 1: return 1;
            case 2: return 1;
            case 3: return 2;
          }
      }
    case 5:
      switch(angle){
        case 0:
          switch(index){
            case 0: return 1;
            case 1: return 1;
            case 2: return 2;
            case 3: return 2;
          }
        case 90:
          switch(index){
            case 0: return 0;
            case 1: return 1;
            case 2: return 1;
            case 3: return 2;
          }
        case 180:
          switch(index){
            case 0: return 1;
            case 1: return 1;
            case 2: return 2;
            case 3: return 2;
          }
        case 270:
          switch(index){
            case 0: return 0;
            case 1: return 1;
            case 2: return 1;
            case 3: return 2;
          }
      }
    case 6:
      switch(angle){
        case 0:
          switch(index){
            case 0: return 1;
            case 1: return 2;
            case 2: return 2;
            case 3: return 2;
          }
        case 90:
          switch(index){
            case 0: return 0;
            case 1: return 1;
            case 2: return 1;
            case 3: return 2;
          }
        case 180:
          switch(index){
            case 0: return 1;
            case 1: return 1;
            case 2: return 1;
            case 3: return 2;
          }
        case 270:
          switch(index){
            case 0: return 0;
            case 1: return 1;
            case 2: return 1;
            case 3: return 2;
          }
      }
  }
  return -1;
}
void rotate(int arg_angle){
  if(arg_angle > 0){
    rotate_move(90);
  }
  if(arg_angle < 0){
    rotate_move(-90);
  }
  for(int i = 0; i < 4; i++){
    if(world_map[get_angle_position_y(i, player.angle, player.type) + player.y][get_angle_position_x(i, player.angle, player.type) + player.x] != 0){
      if(arg_angle > 0){
        rotate_move(-90);
      }
      if(arg_angle < 0){
        rotate_move(90);
      }
      return -1;
    }
  }
  for(int i = 0; i < 4; i++){
    player.posi_x[i] = get_angle_position_x(i, player.angle, player.type);
    player.posi_y[i] = get_angle_position_y(i, player.angle, player.type);
  }
}

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

void GameOver(){
  for(unsigned char r = 0; r < 21; r++){
    for(unsigned char c = 0; c < 12; c++){
      if(world_map[r][c] != 99){
        world_map[r][c] = 0;
      }
    }
  }
  game_flag = false;
}
bool CheckDownBlock(){
  for(unsigned char i = 0; i < 4; i++){
    if(world_map[player.y + player.posi_y[i] + 1][player.x + player.posi_x[i]] != 0){
      return true;
    }
  }
  return false;
}
void SetSpeed(int value = -1){
  game_speed = value;
  if(value == -1){
    game_speed = game_level_speed;
  }

}
int RotatePlayer(){
  if(controller.get_btn1() == true){
    rotate(90);
  }
  else if(controller.get_btn2() == true){
    rotate(-90);
  }
  if(controller.get_btn3() == true){
    SetSpeed(0);
  }
  else{
    SetSpeed(-1);
  }
}
int MovePlayer(){
  if(controller.get_right() == true){
    for(unsigned char i = 0; i < 4; i++){
      if(world_map[player.y + player.posi_y[i]][player.x + player.posi_x[i] + 1] != 0){
        return -1;
      }
    }
    player.x += 1;
  }
  if(controller.get_left() == true){
    for(unsigned char i = 0; i < 4; i++){
      if(world_map[player.y + player.posi_y[i]][player.x + player.posi_x[i] - 1] != 0){
        return -1;
      }
    }
    player.x -= 1;
  }
  return -1;
}
void SetBlock(){
  for(unsigned char i = 0; i < 4; i++){
    world_map[player.y + player.posi_y[i]][player.x + player.posi_x[i]] = 1;    
  }
  CreatePlayer();
}
void shiftline(int index){
  for(int r = index - 1; r >= 0; r--){
    for(int c = 0; c < 12; c++){
      world_map[r + 1][c] = world_map[r][c];
    }
  }
}
void clearline(){
  int line_index = 0;
  int line_number = 0;
  for(int r = 0; r < 20; r++){
    int block_num = 0;
    for(int c = 1; c < 11; c++){
      if(world_map[r][c] == 99 || world_map[r][c] == 1) {
        block_num++;
      }
    }
    if(block_num == 10){
      line_index = r;
      line_number++;
    }
  }
  for(int i = 0; i < line_number; i++){
    shiftline(line_index);
    game_score++;
  }

}
void CreatePlayer(){
  player.x = 4;
  player.y = 0;
  player.angle = 0;
  int rand = player.get_now_que();
  player.shift_que();
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
  for(unsigned char i = 0; i < 4; i++){
      if(world_map[player.y + player.posi_y[i]][player.x + player.posi_x[i]] != 0){
        //game over
        GameOver();
      }
    }
}
void SetPlayerMino(String type){
  if(type == "OMino"){
    for(int i = 0; i < 4; i++){
      player.posi_x[i] = get_angle_position_x(i, 0, player.type);
      player.posi_y[i] = get_angle_position_y(i, 0, player.type);
    }
  }
  else if(type == "IMino"){
    for(int i = 0; i < 4; i++){
      player.posi_x[i] = get_angle_position_x(i, 0, player.type);
      player.posi_y[i] = get_angle_position_y(i, 0, player.type);
    }
  }
  else if(type == "JMino"){
    for(int i = 0; i < 4; i++){
      player.posi_x[i] = get_angle_position_x(i, 0, player.type);
      player.posi_y[i] = get_angle_position_y(i, 0, player.type);
    }
  }
  else if(type == "LMino"){
    for(int i = 0; i < 4; i++){
      player.posi_x[i] = get_angle_position_x(i, 0, player.type);
      player.posi_y[i] = get_angle_position_y(i, 0, player.type);
    }
  }
  else if(type == "SMino"){
   for(int i = 0; i < 4; i++){
      player.posi_x[i] = get_angle_position_x(i, 0, player.type);
      player.posi_y[i] = get_angle_position_y(i, 0, player.type);
    }
  }
  else if(type == "ZMino"){
    for(int i = 0; i < 4; i++){
      player.posi_x[i] = get_angle_position_x(i, 0, player.type);
      player.posi_y[i] = get_angle_position_y(i, 0, player.type);
    }
  }
  else if(type == "TMino"){
    for(int i = 0; i < 4; i++){
      player.posi_x[i] = get_angle_position_x(i, 0, player.type);
      player.posi_y[i] = get_angle_position_y(i, 0, player.type);
    }
  }
}
void DownBlock(){
  player.y += 1;
}
void draw(){
  int p_x = 40;
  int p_y = 0;
  int h_que = 4;
  int s_que = 12 * 3 + p_x + 2;
  for(unsigned char r = 0; r < 21; r++){
    for(unsigned char c = 0; c < 12; c++){
      if(world_map[r][c] == 1 || world_map[r][c] == 99){
        TV.draw_line(c * 3 + p_x , r * 2 + p_y  , c * 3 + p_x + 3 , r * 2 + p_y , INVERT);
        TV.draw_line(c * 3 + p_x , r * 2 + 1 + p_y  , c * 3 + p_x + 3 , r * 2 + 1 + p_y , INVERT);
        //TV.draw_line(c * 2 + 20 , r * 2  , c * 2 + 20 , r * 2 , INVERT);
      }
    }
  }
  for(int i = 0; i < 4; i++){
    TV.draw_line((player.x + player.posi_x[i]) * 3 + p_x, (player.y + player.posi_y[i]) * 2 + p_y, (player.x + player.posi_x[i]) * 3 + p_x + 3, (player.y + player.posi_y[i])* 2 + p_y, INVERT);
    TV.draw_line((player.x + player.posi_x[i]) * 3 + p_x, (player.y + player.posi_y[i]) * 2 + p_y + 1, (player.x + player.posi_x[i]) * 3 + p_x + 3, (player.y + player.posi_y[i])* 2 + p_y + 1, INVERT);
    //TV.draw_line((player.x + player.posi_x[i]) * 2 + 20, (player.y + player.posi_y[i]) * 2 + 1, (player.x + player.posi_x[i]) * 2 + 20 + 2, (player.y + player.posi_y[i]) * 2 + 1, INVERT);
  }

  
  for(int i = 0; i < 6; i++){
    if(player.que[i] == 0){//o
      for(int j = 0; j < 4; j++){
        TV.draw_line(s_que + get_angle_position_x(j, 0, player.que[i]), p_y + i * h_que + get_angle_position_y(j, 0, player.que[i]),s_que + get_angle_position_x(j, 0, player.que[i]), p_y + i * h_que + get_angle_position_y(j, 0, player.que[i]), INVERT);
      }
    }
    if(player.que[i] == 1){//I
      for(int j = 0; j < 4; j++){
        TV.draw_line(s_que + get_angle_position_x(j, 0, player.que[i]), p_y + i * h_que + get_angle_position_y(j, 0, player.que[i]),s_que + get_angle_position_x(j, 0, player.que[i]), p_y + i * h_que + get_angle_position_y(j, 0, player.que[i]), INVERT);
      }
    }
    if(player.que[i] == 2){//J
      for(int j = 0; j < 4; j++){
        TV.draw_line(s_que + get_angle_position_x(j, 0, player.que[i]), p_y + i * h_que + get_angle_position_y(j, 0, player.que[i]),s_que + get_angle_position_x(j, 0, player.que[i]), p_y + i * h_que + get_angle_position_y(j, 0, player.que[i]), INVERT);
      }
    }
    if(player.que[i] == 3){//L
      for(int j = 0; j < 4; j++){
        TV.draw_line(s_que + get_angle_position_x(j, 0, player.que[i]), p_y + i * h_que + get_angle_position_y(j, 0, player.que[i]),s_que + get_angle_position_x(j, 0, player.que[i]), p_y + i * h_que + get_angle_position_y(j, 0, player.que[i]), INVERT);
      }
    }
    if(player.que[i] == 4){//S
      for(int j = 0; j < 4; j++){
        TV.draw_line(s_que + get_angle_position_x(j, 0, player.que[i]), p_y + i * h_que + get_angle_position_y(j, 0, player.que[i]),s_que + get_angle_position_x(j, 0, player.que[i]), p_y + i * h_que + get_angle_position_y(j, 0, player.que[i]), INVERT);      
        }
    }
    if(player.que[i] == 5){//Z
      for(int j = 0; j < 4; j++){
        TV.draw_line(s_que + get_angle_position_x(j, 0, player.que[i]), p_y + i * h_que + get_angle_position_y(j, 0, player.que[i]),s_que + get_angle_position_x(j, 0, player.que[i]), p_y + i * h_que + get_angle_position_y(j, 0, player.que[i]), INVERT);      
        }
    }
    if(player.que[i] == 6){//T
      for(int j = 0; j < 4; j++){
        TV.draw_line(s_que + get_angle_position_x(j, 0, player.que[i]), p_y + i * h_que + get_angle_position_y(j, 0, player.que[i]),s_que + get_angle_position_x(j, 0, player.que[i]), p_y + i * h_que + get_angle_position_y(j, 0, player.que[i]), INVERT);      
        }
    }
  }
}
void setup() {
  TV.begin(NTSC,88,44);
  TV.select_font(font6x8);
  randomSeed(analogRead(3));
  world_map_reset();
  CreatePlayer();
}
void loop() {
  if(game_flag == true){
    //Serial.println("g");
    draw();
    MovePlayer();
    RotatePlayer();
    if(main_update.Do(game_speed, true) == true){
      if(CheckDownBlock() == true){
        SetBlock();
        clearline();
      }
      else{
        DownBlock();
      }
    }
  }
  else{
    TV.println(0, 0, "Tetirs\nclick to start...");
    if(controller.joystick.read_x() == 1 || controller.joystick.read_y() == -1){
      game_flag = true;
      game_score = 0;
    }
  }
  
  TV.delay_frame(4);
  //Serial.println("delay");
  TV.clear_screen();
  //Serial.println("clear");
}