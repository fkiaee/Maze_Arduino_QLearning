#include<LiquidCrystal.h>
LiquidCrystal lcd(13,12,11,10,9,8);
/* shift regiter ds_col and ds_row inputs are the data shown at output. 
If 3 clk-pulses are given to sh_col, then output data is shifted by 3 to right but the output is appeared only if a clk-pulse is also given to st_col   
*/
#define ds_col 15
#define sh_col 16
#define st_col 14
#define ds_row 17

#define MATRIX_ROW 8
#define MATRIX_COLUMN 8
// state num must be equal to action num
#define STATE_NUM 64
#define ACTION_NUM 4  //0:up  1:down   2: right  3:left
#define DES_STATE 7
#define INIT_STATE 56
#define MAX_EPISODE 100
#define Max_STEPS_PER_EPISODE 70
#define SHOW_EPISODE 3
#define PATHS 1
#define WALLS -1
#define alpha 0.9
#define gamma 0.9
//arrays Col and Row show the position of walls. 
//Maze 1:
//const char Row[] = {0,0,1,2,3,5,6,7,0,1,2,3,5,6,7,1,2,3,5,6,7,7,1,2,3,4,5,7,4,5,7,1,2,3,4,5,6,7};
//const char Col[] = {0,1,1,1,1,1,1,1,2,2,2,2,2,2,2,3,3,3,3,3,3,4,5,5,5,5,5,5,6,6,6,7,7,7,7,7,7,7};
//const int R[MATRIX_ROW][MATRIX_COLUMN] = {{-1,  -1,  -1,  1,  1, 1, 1, 100},
//                     {1,  -1,  -1,  -1,  1, -1, 1, 1},
//                     {1,  -1, -1, -1, 1, -1, 1, -1},
//                     {1, -1, -1, -1, 1,  -1, 1, -1},
//                     {1,  1,  1,  1, 1, -1, -1, -1},
//                     {1, -1, -1, -1, 1,  -1, -1, -1},
//                     {1, -1, -1, -1, 1,  1, 1, -1},
//                     {1, -1, -1, -1, -1, -1, -1, -1}};
//Maze 2:
//const char Row[] = {0,1,2,1,2,3,5,7,1,2,3,5,7,1,2,3,5,7,5,6,7,1,2,3,5,6,7,1,2,3,7,1,2,3,4,5,6,7};
//const char Col[] = {0,0,0,1,1,1,1,1,2,2,2,2,2,3,3,3,3,3,4,4,4,5,5,5,5,5,5,6,6,6,6,7,7,7,7,7,7,7};
//const int R[MATRIX_ROW][MATRIX_COLUMN] = {{-1, 1, 1, 1, 1, 1, 1, 100},
//                    {-1, -1, -1, -1, 1, -1, -1, -1},
//                    {-1, -1, -1, -1, 1, -1, -1, -1},
//                    {1, -1, -1, -1, 1, -1, -1, -1},
//                    {1, 1, 1, 1, 1, 1, 1, -1},
//                    {1, -1, -1, -1, -1, -1, 1, -1},
//                    {1, 1, 1, 1, -1, -1, 1, -1},
//                    {1, -1, -1, -1, -1, -1, -1, -1}};
//Maze 3:
//const char Row[] = {5,1,2,5,7,5,7,0,1,3,5,7,1,3,5,7,1,3,4,5,7,7,2,3,4,5,6,7};
//const char Col[] = {0,1,1,1,1,2,2,3,3,3,3,3,4,4,4,4,5,5,5,5,5,6,7,7,7,7,7,7};
//const int R[MATRIX_ROW][MATRIX_COLUMN] = {{1,1,1,-1,1,1,1,100},
//                        {1,-1,1,-1,-1,-1,1,1},
//                        {1,-1,1,1,1,1,1,-1},
//                        {1,1,1,-1,-1,-1,1,-1},
//                        {1,1,1,1,1,-1,1,-1},
//                        {-1,-1,-1,-1,-1,-1,1,-1},
//                        {1,1,1,1,1,1,1,-1},
//                        {1,-1,-1,-1,-1,-1,-1,-1}};
//Maze 4:
const char Row[] = {3,1,3,4,5,1,7,3,4,5,7,5,7,0,1,2,3,4,5,7,7,1,2,3,4,5,6,7};
const char Col[] = {0,1,1,1,1,2,2,3,3,3,3,4,4,5,5,5,5,5,5,5,6,7,7,7,7,7,7,7};
const int R[MATRIX_ROW][MATRIX_COLUMN] = {{1,1,1,1,1,-1,1,100},
                          {1,-1,-1,1,1,-1,1,-1},
                          {1,1,1,1,1,-1,1,-1},
                          {-1,-1,1,-1,1,-1,1,-1},
                          {1,-1,1,-1,1,-1,1,-1},
                          {1,-1,1,-1,-1,-1,1,-1},
                          {1,1,1,1,1,1,1,-1},
                          {1,1,-1,-1,-1,-1,-1,-1}};
//Maze 5:
//const char Row[] = {3,1,2,3,5,6,7,3,7,0,1,3,5,7,1,3,5,7,1,3,4,5,7,7,1,2,3,4,5,6,7};
//const char Col[] = {0,1,1,1,1,1,1,2,2,3,3,3,3,3,4,4,4,4,5,5,5,5,5,6,7,7,7,7,7,7,7};
//const int R[MATRIX_ROW][MATRIX_COLUMN] ={{1,1,1,-1,1,1,1,100},
//                      {1,-1,1,-1,-1,-1,1,-1},
//                      {1,-1,1,1,1,1,1,-1},
//                      {-1,-1,-1,-1,-1,-1,1,-1},
//                      {1,1,1,1,1,-1,1,-1},
//                      {1,-1,1,-1,-1,-1,1,-1},
//                      {1,-1,1,1,1,1,1,-1},
//                      {1,-1,-1,-1,-1,-1,-1,-1}}; 
//Maze 6:
//const char Row[] = {3,1,3,4,5,7,1,7,0,1,2,3,4,5,7,5,7,1,3,4,5,7,1,7,1,2,3,4,5};
//const char Col[] = {0,1,1,1,1,1,2,2,3,3,3,3,3,3,3,4,4,5,5,5,5,5,6,6,7,7,7,7,7};
//const int R[MATRIX_ROW][MATRIX_COLUMN] = {{1,1,1,-1,1,1,1,100},
//                      {1,-1,-1,-1,1,-1,-1,-1},
//                      {1,1,1,-1,1,1,1,-1},
//                      {-1,-1,1,-1,1,-1,1,-1},
//                      {1,-1,1,-1,1,-1,1,-1},
//                      {1,-1,1,-1,-1,-1,1,-1},
//                      {1,1,1,1,1,1,1,-1},
//                      {1,-1,-1,-1,-1,-1,-1,-1}};
const int move_c[] = {0,0,1,-1};
const int move_r[] = {1,-1,0,0};
///%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
double epsilon = 1.0;
// define the most important Q-matrix
double Q[STATE_NUM][ACTION_NUM];
int possible_action[ACTION_NUM];
int possible_action_num;
int position1;
int loop_iter = 1;
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
int colum_data(int temp)
{
  switch(temp)
  {
   case 1: return 1;break;
   case 2: return 2; break;
   case 3: return 4; break;
   case 4: return 8; break;
   case 5: return 16; break;
   case 6: return 32; break;
   case 7: return 64; break;
   case 8: return 128; break;
   default: return 0; break;
  }
}

int row_data(int temp)
{
  switch(temp)
  {
   case 1: return 1;break;
   case 2: return 2; break;
   case 3: return 4; break;
   case 4: return 8; break;
   case 5: return 16; break;
   case 6: return 32; break;
   case 7: return 64; break;
   case 8: return 128; break;
   default: return 0; break;
  }
}

void show_maze(int state_r, int state_c)
{
  for (int kk=0;kk<20;kk++)
  {
    int r,c, walls_size;
    walls_size = sizeof(Col)/sizeof(char);
   //this loop shows the walls of the maze map  
    for(int k=0;k<walls_size;k++)
    {
     int temp1=Col[k];
     c=colum_data(temp1+1);
     int temp2=Row[k];
     r=0xff-row_data(temp2+1);
      //this loop loads bit by bit the 8-bit c and r values correctly at output of shift register by giving 8 clock to sh_col
     for(int i=0;i<8;i++)
     {   
           int ds=(c & 0x01);
           digitalWrite(ds_col, ds);
           ds=(r & 0x01);
           digitalWrite(ds_row, ds);
           digitalWrite(sh_col, HIGH);
           c>>=1; //shifts c to the right c = c>>1
           r>>=1;
           digitalWrite(sh_col, LOW);
      }
      digitalWrite(st_col, HIGH);
      digitalWrite(st_col, LOW);
    }
   //this prart shows the agent  
   r = 0xff-row_data(state_r+1); c = colum_data(state_c+1);
   for(int i=0;i<8;i++)
   {   
         int ds=(c & 0x01);
         digitalWrite(ds_col, ds);
         ds=(r & 0x01);
         digitalWrite(ds_row, ds);
         digitalWrite(sh_col, HIGH);
         c>>=1; //shifts c to the right c = c>>1
         r>>=1;
         digitalWrite(sh_col, LOW);
    }
    digitalWrite(st_col, HIGH);
    digitalWrite(st_col, LOW);
  }
}

void print_matrix(double m[STATE_NUM][ACTION_NUM], int rows, int columns){
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            Serial.print(m[i][j]); Serial.print(" ");
        }
        Serial.println(";");
    }
}

int get_random_init_state(){
    loop_iter++;
    srand(loop_iter);
    int init_state, state_r1, state_c1;
    do{
      init_state = rand()%STATE_NUM;
      state_r1 = init_state/MATRIX_ROW;
      state_c1 = init_state%MATRIX_COLUMN; 
      }while(R[state_r1][state_c1]<= WALLS || init_state==DES_STATE);
    return init_state;
 }
// get in state s, possible actions
void get_possible_action(int nowstate, int possible_action[ACTION_NUM]){
    // find R[i][j] value > WALLS is action we can do later
    int state_r, state_c;
    state_r = nowstate/MATRIX_ROW;
    state_c = nowstate%MATRIX_COLUMN;
    possible_action_num = 0;
    for(int i = 0; i < ACTION_NUM; i++){
      if (state_r+move_r[i]>=0 & state_r+move_r[i]<=7 & state_c+move_c[i]>=0 & state_c+move_c[i]<=7) 
        if (R[state_r+move_r[i]][state_c+move_c[i]] > WALLS){
            possible_action[possible_action_num] = i;
            possible_action_num++;
        }
    }
}
double inference_best_action(int state, double Q[STATE_NUM][ACTION_NUM]){
    int arg_max = 0;
    int ps_action; 
    double temp_max = -100; 
    for (int i = 0; i < possible_action_num; ++i) {
        ps_action = possible_action[i];
        if (Q[state][ps_action] >= temp_max){
            temp_max = Q[state][ps_action];
            arg_max = ps_action;
        }
    }
    return arg_max;
}

 int inference_next_state(int now_state, int current_action){
    int next_state,state_c1,state_r1;
    state_r1 = now_state/MATRIX_ROW;
    state_c1 = now_state%MATRIX_COLUMN;  
    state_c1 = state_c1 + move_c[current_action];
    state_r1 = state_r1 + move_r[current_action];
    next_state = state_r1*MATRIX_COLUMN + state_c1;
    return next_state;
 }

int episode_iterator(int init_state, double Q[STATE_NUM][ACTION_NUM], int show_flag){
    int current_action, next_state, argmax_q, state_r, state_c;
    double max_q;
    int step1=0;
    // memset possible_action array
    memset(possible_action, 0, ACTION_NUM*sizeof(int));
    get_possible_action(init_state, possible_action);
    argmax_q = inference_best_action(init_state,Q);
    state_r = init_state/MATRIX_ROW; state_c = init_state%MATRIX_COLUMN;
    while (step1<Max_STEPS_PER_EPISODE){
        if (show_flag ==1) show_maze(state_r,state_c);
        // get next action
        double r_uniform = double(rand())/RAND_MAX;
        if (r_uniform > epsilon)
          current_action = argmax_q; //Exploitation
        else{
          current_action = possible_action[rand()%possible_action_num];
        }
        // next episode rules
        next_state = inference_next_state(init_state, current_action);
        state_r = next_state/MATRIX_ROW;
        state_c = next_state%MATRIX_COLUMN;          
        // memset possible_action array
        memset(possible_action, 0, ACTION_NUM*sizeof(int));
        get_possible_action(next_state, possible_action);
        argmax_q = inference_best_action(next_state, Q);
        max_q = Q[next_state][argmax_q];
        if (epsilon != 0.0){
          Q[init_state][current_action] = Q[init_state][current_action] + alpha*(R[state_r][state_c] + gamma* max_q - Q[init_state][current_action]);
        }
        // if next_state is destination state, then go next episode
        // if not destination state, then next state becomes initial state, then go back to this loop
        if (next_state == DES_STATE){
            break;
        }
        init_state = next_state;       
        //epsilon = max(epsilon - 0.01,0.2);
        step1++;
    }
    return next_state;
}


void run_training(){
    int init_state, DEST;
    int show_flag = 1; 
    for (int i = 0; i < MAX_EPISODE; ++i) {
        srand(MAX_EPISODE+i);    // start random
        init_state = get_random_init_state(); 
        if (show_flag ==1) { Serial.print("Episode iterator: ");Serial.println(i); delay(500);}
        if (i%(SHOW_EPISODE+1)==SHOW_EPISODE){
          epsilon = 0.0;  //exploitation
          show_flag = 1; 
          Serial.print("Test Episode (Exploration) ");Serial.println(i);  
          init_state = INIT_STATE; 
          DEST = episode_iterator(init_state, Q, show_flag);
          show_flag = 0; 
          if (DEST == DES_STATE){
            break;
          }
        }else{
          epsilon = 1.0;  //exploration 
          init_state = get_random_init_state(); 
          episode_iterator(init_state, Q, show_flag);
        }
        episode_iterator(init_state, Q, show_flag);
    }
}


void setup() {
  lcd.begin(16,2);
  pinMode(ds_col, OUTPUT);
  pinMode(sh_col, OUTPUT);
  pinMode(st_col, OUTPUT);
  pinMode(ds_row, OUTPUT);
  lcd.setCursor(0,0);
  lcd.print("  Maze Game    ");
  lcd.setCursor(0,1);
  lcd.print("Welcome ece_society");
  delay(1000);
  lcd.setCursor(0,0);
  lcd.print(" ece_society ");
  lcd.setCursor(0,1);
  lcd.print(" Maze Training ");
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Training Phase:");
  run_training();
  Serial.println("updated Q matrix:");
  print_matrix(Q, STATE_NUM, ACTION_NUM);
  lcd.setCursor(0,0);
  lcd.print(" ece_society ");
  lcd.setCursor(0,1);
  lcd.print(" Maze Test ");
  Serial.println("Test Phase:");
  position1 = INIT_STATE;
  Serial.print("Initial state is: "); 
  }

void loop() {
  // put your main code here, to run repeatedly:
  show_maze(position1/MATRIX_ROW,position1%MATRIX_COLUMN);
  memset(possible_action, 0, ACTION_NUM*sizeof(int));
  get_possible_action(position1, possible_action);
  int best_action = inference_best_action(position1, Q);
  int next_state = inference_next_state(position1, best_action);  
  Serial.print(" (action taken: ");
  Serial.print(best_action);
  Serial.print(") ");
  Serial.println("->");
  delay(100);
      if (next_state == DES_STATE){
          Serial.println("-> out");
          position1 = INIT_STATE; 
      }else{
          position1 = next_state;
      }
}
