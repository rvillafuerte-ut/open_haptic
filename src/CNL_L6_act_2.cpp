// ------
//Act 1 ref
/*
qd[0][0] = atan(sin(t)/(cos(t)*(3.0/5.0)+2.0));
qd[1][0] = atan((sqrt(pow(sin(t),2.0)/4.0E+2+pow(cos(t)*(3.0/1.0E+2)+1.0/1.0E+1,2.0))*-8.556623698070271E+67+sqrt(-(pow(sin(t),2.0)*8.22752278660603E+65-sqrt(pow(sin(t),2.0)/4.0E+2+pow(cos(t)*(3.0/1.0E+2)+1.0/1.0E+1,2.0))*1.015641592109322E+52+pow(cos(t)*(3.0/1.0E+2)+1.0/1.0E+1,2.0)*3.291009114642412E+68-1.732222647492033E+67)*(pow(sin(t),2.0)*8.22752278660603E+65-sqrt(pow(sin(t),2.0)/4.0E+2+pow(cos(t)*(3.0/1.0E+2)+1.0/1.0E+1,2.0))*1.015641592109322E+52+pow(cos(t)*(3.0/1.0E+2)+1.0/1.0E+1,2.0)*3.291009114642412E+68+3.898200296293937E+66))+1.320334069742118E+51)/(pow(sin(t),2.0)*8.22752278660603E+65-sqrt(pow(sin(t),2.0)/4.0E+2+pow(cos(t)*(3.0/1.0E+2)+1.0/1.0E+1,2.0))*1.015641592109322E+52+pow(cos(t)*(3.0/1.0E+2)+1.0/1.0E+1,2.0)*3.291009114642412E+68+1.373831754907475E+67))*-2.0-3.7E+1/2.0E+2;
qd[2][0] = 3.141592653589793*(-1.0/2.0)+atan((sqrt(pow(sin(t),2.0)/4.0E+2+pow(cos(t)*(3.0/1.0E+2)+1.0/1.0E+1,2.0))*8.359163151191727E+68+((sqrt(pow(sin(t),2.0)/4.0E+2+pow(cos(t)*(3.0/1.0E+2)+1.0/1.0E+1,2.0))*-8.556623698070271E+67+sqrt(-(pow(sin(t),2.0)*8.22752278660603E+65-sqrt(pow(sin(t),2.0)/4.0E+2+pow(cos(t)*(3.0/1.0E+2)+1.0/1.0E+1,2.0))*1.015641592109322E+52+pow(cos(t)*(3.0/1.0E+2)+1.0/1.0E+1,2.0)*3.291009114642412E+68-1.732222647492033E+67)*(pow(sin(t),2.0)*8.22752278660603E+65-sqrt(pow(sin(t),2.0)/4.0E+2+pow(cos(t)*(3.0/1.0E+2)+1.0/1.0E+1,2.0))*1.015641592109322E+52+pow(cos(t)*(3.0/1.0E+2)+1.0/1.0E+1,2.0)*3.291009114642412E+68+3.898200296293937E+66))+1.320334069742118E+51)*6.869158774537375E+67)/(pow(sin(t),2.0)*8.22752278660603E+65-sqrt(pow(sin(t),2.0)/4.0E+2+pow(cos(t)*(3.0/1.0E+2)+1.0/1.0E+1,2.0))*1.015641592109322E+52+pow(cos(t)*(3.0/1.0E+2)+1.0/1.0E+1,2.0)*3.291009114642412E+68+1.373831754907475E+67)-(sqrt(pow(sin(t),2.0)/4.0E+2+pow(cos(t)*(3.0/1.0E+2)+1.0/1.0E+1,2.0))*(sqrt(pow(sin(t),2.0)/4.0E+2+pow(cos(t)*(3.0/1.0E+2)+1.0/1.0E+1,2.0))*-8.556623698070271E+67+sqrt(-(pow(sin(t),2.0)*8.22752278660603E+65-sqrt(pow(sin(t),2.0)/4.0E+2+pow(cos(t)*(3.0/1.0E+2)+1.0/1.0E+1,2.0))*1.015641592109322E+52+pow(cos(t)*(3.0/1.0E+2)+1.0/1.0E+1,2.0)*3.291009114642412E+68-1.732222647492033E+67)*(pow(sin(t),2.0)*8.22752278660603E+65-sqrt(pow(sin(t),2.0)/4.0E+2+pow(cos(t)*(3.0/1.0E+2)+1.0/1.0E+1,2.0))*1.015641592109322E+52+pow(cos(t)*(3.0/1.0E+2)+1.0/1.0E+1,2.0)*3.291009114642412E+68+3.898200296293937E+66))+1.320334069742118E+51)*5.078207960546609E+52)/(pow(sin(t),2.0)*8.22752278660603E+65-sqrt(pow(sin(t),2.0)/4.0E+2+pow(cos(t)*(3.0/1.0E+2)+1.0/1.0E+1,2.0))*1.015641592109322E+52+pow(cos(t)*(3.0/1.0E+2)+1.0/1.0E+1,2.0)*3.291009114642412E+68+1.373831754907475E+67)+((pow(sin(t),2.0)/4.0E+2+pow(cos(t)*(3.0/1.0E+2)+1.0/1.0E+1,2.0))*(sqrt(pow(sin(t),2.0)/4.0E+2+pow(cos(t)*(3.0/1.0E+2)+1.0/1.0E+1,2.0))*-8.556623698070271E+67+sqrt(-(pow(sin(t),2.0)*8.22752278660603E+65-sqrt(pow(sin(t),2.0)/4.0E+2+pow(cos(t)*(3.0/1.0E+2)+1.0/1.0E+1,2.0))*1.015641592109322E+52+pow(cos(t)*(3.0/1.0E+2)+1.0/1.0E+1,2.0)*3.291009114642412E+68-1.732222647492033E+67)*(pow(sin(t),2.0)*8.22752278660603E+65-sqrt(pow(sin(t),2.0)/4.0E+2+pow(cos(t)*(3.0/1.0E+2)+1.0/1.0E+1,2.0))*1.015641592109322E+52+pow(cos(t)*(3.0/1.0E+2)+1.0/1.0E+1,2.0)*3.291009114642412E+68+3.898200296293937E+66))+1.320334069742118E+51)*1.645504557321206E+69)/(pow(sin(t),2.0)*8.22752278660603E+65-sqrt(pow(sin(t),2.0)/4.0E+2+pow(cos(t)*(3.0/1.0E+2)+1.0/1.0E+1,2.0))*1.015641592109322E+52+pow(cos(t)*(3.0/1.0E+2)+1.0/1.0E+1,2.0)*3.291009114642412E+68+1.373831754907475E+67)-1.289864821978839E+52)/(pow(sin(t),2.0)*4.113761393303015E+66-sqrt(pow(sin(t),2.0)/4.0E+2+pow(cos(t)*(3.0/1.0E+2)+1.0/1.0E+1,2.0))*5.078207960546609E+52+pow(cos(t)*(3.0/1.0E+2)+1.0/1.0E+1,2.0)*1.645504557321206E+69+6.152376989368257E+67))*2.0+atan((sqrt(pow(sin(t),2.0)/4.0E+2+pow(cos(t)*(3.0/1.0E+2)+1.0/1.0E+1,2.0))*-8.556623698070271E+67+sqrt(-(pow(sin(t),2.0)*8.22752278660603E+65-sqrt(pow(sin(t),2.0)/4.0E+2+pow(cos(t)*(3.0/1.0E+2)+1.0/1.0E+1,2.0))*1.015641592109322E+52+pow(cos(t)*(3.0/1.0E+2)+1.0/1.0E+1,2.0)*3.291009114642412E+68-1.732222647492033E+67)*(pow(sin(t),2.0)*8.22752278660603E+65-sqrt(pow(sin(t),2.0)/4.0E+2+pow(cos(t)*(3.0/1.0E+2)+1.0/1.0E+1,2.0))*1.015641592109322E+52+pow(cos(t)*(3.0/1.0E+2)+1.0/1.0E+1,2.0)*3.291009114642412E+68+3.898200296293937E+66))+1.320334069742118E+51)/(pow(sin(t),2.0)*8.22752278660603E+65-sqrt(pow(sin(t),2.0)/4.0E+2+pow(cos(t)*(3.0/1.0E+2)+1.0/1.0E+1,2.0))*1.015641592109322E+52+pow(cos(t)*(3.0/1.0E+2)+1.0/1.0E+1,2.0)*3.291009114642412E+68+1.373831754907475E+67))*2.0+3.7E+1/2.0E+2;
qd[3][0] = 3.141592653589793-atan((sqrt(pow(sin(t),2.0)/4.0E+2+pow(cos(t)*(3.0/1.0E+2)+1.0/1.0E+1,2.0))*8.359163151191727E+68+((sqrt(pow(sin(t),2.0)/4.0E+2+pow(cos(t)*(3.0/1.0E+2)+1.0/1.0E+1,2.0))*-8.556623698070271E+67+sqrt(-(pow(sin(t),2.0)*8.22752278660603E+65-sqrt(pow(sin(t),2.0)/4.0E+2+pow(cos(t)*(3.0/1.0E+2)+1.0/1.0E+1,2.0))*1.015641592109322E+52+pow(cos(t)*(3.0/1.0E+2)+1.0/1.0E+1,2.0)*3.291009114642412E+68-1.732222647492033E+67)*(pow(sin(t),2.0)*8.22752278660603E+65-sqrt(pow(sin(t),2.0)/4.0E+2+pow(cos(t)*(3.0/1.0E+2)+1.0/1.0E+1,2.0))*1.015641592109322E+52+pow(cos(t)*(3.0/1.0E+2)+1.0/1.0E+1,2.0)*3.291009114642412E+68+3.898200296293937E+66))+1.320334069742118E+51)*6.869158774537375E+67)/(pow(sin(t),2.0)*8.22752278660603E+65-sqrt(pow(sin(t),2.0)/4.0E+2+pow(cos(t)*(3.0/1.0E+2)+1.0/1.0E+1,2.0))*1.015641592109322E+52+pow(cos(t)*(3.0/1.0E+2)+1.0/1.0E+1,2.0)*3.291009114642412E+68+1.373831754907475E+67)-(sqrt(pow(sin(t),2.0)/4.0E+2+pow(cos(t)*(3.0/1.0E+2)+1.0/1.0E+1,2.0))*(sqrt(pow(sin(t),2.0)/4.0E+2+pow(cos(t)*(3.0/1.0E+2)+1.0/1.0E+1,2.0))*-8.556623698070271E+67+sqrt(-(pow(sin(t),2.0)*8.22752278660603E+65-sqrt(pow(sin(t),2.0)/4.0E+2+pow(cos(t)*(3.0/1.0E+2)+1.0/1.0E+1,2.0))*1.015641592109322E+52+pow(cos(t)*(3.0/1.0E+2)+1.0/1.0E+1,2.0)*3.291009114642412E+68-1.732222647492033E+67)*(pow(sin(t),2.0)*8.22752278660603E+65-sqrt(pow(sin(t),2.0)/4.0E+2+pow(cos(t)*(3.0/1.0E+2)+1.0/1.0E+1,2.0))*1.015641592109322E+52+pow(cos(t)*(3.0/1.0E+2)+1.0/1.0E+1,2.0)*3.291009114642412E+68+3.898200296293937E+66))+1.320334069742118E+51)*5.078207960546609E+52)/(pow(sin(t),2.0)*8.22752278660603E+65-sqrt(pow(sin(t),2.0)/4.0E+2+pow(cos(t)*(3.0/1.0E+2)+1.0/1.0E+1,2.0))*1.015641592109322E+52+pow(cos(t)*(3.0/1.0E+2)+1.0/1.0E+1,2.0)*3.291009114642412E+68+1.373831754907475E+67)+((pow(sin(t),2.0)/4.0E+2+pow(cos(t)*(3.0/1.0E+2)+1.0/1.0E+1,2.0))*(sqrt(pow(sin(t),2.0)/4.0E+2+pow(cos(t)*(3.0/1.0E+2)+1.0/1.0E+1,2.0))*-8.556623698070271E+67+sqrt(-(pow(sin(t),2.0)*8.22752278660603E+65-sqrt(pow(sin(t),2.0)/4.0E+2+pow(cos(t)*(3.0/1.0E+2)+1.0/1.0E+1,2.0))*1.015641592109322E+52+pow(cos(t)*(3.0/1.0E+2)+1.0/1.0E+1,2.0)*3.291009114642412E+68-1.732222647492033E+67)*(pow(sin(t),2.0)*8.22752278660603E+65-sqrt(pow(sin(t),2.0)/4.0E+2+pow(cos(t)*(3.0/1.0E+2)+1.0/1.0E+1,2.0))*1.015641592109322E+52+pow(cos(t)*(3.0/1.0E+2)+1.0/1.0E+1,2.0)*3.291009114642412E+68+3.898200296293937E+66))+1.320334069742118E+51)*1.645504557321206E+69)/(pow(sin(t),2.0)*8.22752278660603E+65-sqrt(pow(sin(t),2.0)/4.0E+2+pow(cos(t)*(3.0/1.0E+2)+1.0/1.0E+1,2.0))*1.015641592109322E+52+pow(cos(t)*(3.0/1.0E+2)+1.0/1.0E+1,2.0)*3.291009114642412E+68+1.373831754907475E+67)-1.289864821978839E+52)/(pow(sin(t),2.0)*4.113761393303015E+66-sqrt(pow(sin(t),2.0)/4.0E+2+pow(cos(t)*(3.0/1.0E+2)+1.0/1.0E+1,2.0))*5.078207960546609E+52+pow(cos(t)*(3.0/1.0E+2)+1.0/1.0E+1,2.0)*1.645504557321206E+69+6.152376989368257E+67))*2.0;'
*/


// ----------------------

#include <iostream>
#include <chrono>
#include <fcntl.h>
#include <termios.h>
#define STDIN_FILENO 0
#include <stdlib.h>
#include <stdio.h>
#include <csignal>
#include <signal.h>
#include <fstream>
#include <unistd.h>
#include "dynamixel_sdk.h"                                  // Uses Dynamixel SDK library
#include <Eigen/Dense>
#include <atomic>
#include <thread>
#include <cmath>
#include <HD/hd.h>

#define ADDR_OPERATING_MODE         11
#define ADDR_TORQUE_ENABLE          64
#define ADDR_GOAL_CURRENT           102
#define ADDR_CURRENT_LIMIT          38
#define ADDR_PRESENT_CURRENT        126
#define ADDR_PRESENT_VELOCITY       128
#define ADDR_PRESENT_POSITION       132
#define ADDR_RETURN_DELAY           9

#define LEN_GOAL_CURRENT            2
#define LEN_PRESENT_POSITION        4
#define LEN_PRESENT_VELOCITY        4
#define LEN_PRESENT_CURRENT         2
#define TOTAL_LEN_READ              10
#define PROTOCOL_VERSION                2.0                 // See which protocol version is used in the Dynamixel
#define DXL1_ID                         11
#define DXL2_ID                         12
#define DXL3_ID                         13
#define DXL4_ID                         14
//#define DXL5_ID                       15  
#define BAUDRATE                        4500000
#define DEVICENAME                      "/dev/ttyUSB0"
#define TORQUE_ENABLE                   1                   // Value for enabling the torque
#define TORQUE_DISABLE                  0                   // Value for disabling the torque
#define ESC_ASCII_VALUE                 0x1b

#define curr_peak 330
#define curr_max 290

void OMDyn(const double* q, const double* dq, double* M, double* phib);

// --- Haptics bridge (minimal) ------------------------------------------------
struct HapticSample {
  double position[3];
  double transform[16];
  double timestamp;
};

static HHD g_hDevice = HD_INVALID_HANDLE;
static std::atomic<bool> g_hSampleReady{false};
static HapticSample g_hSample;
static std::atomic<int> g_hButtons{0};

static HDCallbackCode HDCALLBACK hapticsServoCallback(void* /*user*/){
  hdBeginFrame(g_hDevice);
  hdGetDoublev(HD_CURRENT_POSITION, g_hSample.position);
  hdGetDoublev(HD_CURRENT_TRANSFORM, g_hSample.transform);
  g_hSample.timestamp = hdGetSchedulerTimeStamp();
  int buttons = 0;
  hdGetIntegerv(HD_CURRENT_BUTTONS, &buttons);
  g_hButtons.store(buttons, std::memory_order_release);
  hdEndFrame(g_hDevice);
  g_hSampleReady.store(true, std::memory_order_release);
  return HD_CALLBACK_CONTINUE;
}
// -----------------------------------------------------------------------------

int getch(){
  struct termios oldt,newt;
  int ch;
  tcgetattr(STDIN_FILENO,&oldt);
  newt=oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO,TCSANOW,&newt);
  ch=getchar();
  tcsetattr(STDIN_FILENO,TCSANOW,&oldt);
  return ch;
}
template<typename T>
void dbg(T a){
  //std::cout<<a<<std::endl;
}
int kbhit(void){
  struct termios oldt,newt;
  int ch;
  int oldf;
  tcgetattr(STDIN_FILENO,&oldt);
  newt=oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO,TCSANOW,&newt);
  oldf=fcntl(STDIN_FILENO,F_GETFL,0);
  fcntl(STDIN_FILENO,F_SETFL,oldf | O_NONBLOCK);
  ch=getchar();
  tcsetattr(STDIN_FILENO,TCSANOW,&oldt);
  fcntl(STDIN_FILENO,F_SETFL,oldf);
  if(ch!=EOF){
    ungetc(ch,stdin);
    return 1;
  }
  return 0;
}
volatile sig_atomic_t shutdown_requested=0;
void signalHandler(int signum){
  shutdown_requested=1;
}

// Global filter state variables for reset on arming
Eigen::Vector4d yd_filtered_state = Eigen::Vector4d::Zero();
Eigen::Vector4d yd_prev_state = Eigen::Vector4d::Zero();

std::vector<Eigen::VectorXd> loadData4(const std::string& filename){
  std::vector<Eigen::VectorXd> data;
  std::ifstream f(filename);std::string line;
  while(getline(f,line)){
    std::istringstream s(line);
    Eigen::VectorXd v(4);
    s>>v(0)>>v(1)>>v(2)>>v(3);
    data.push_back(v);
  }
  return data;
}
std::vector<Eigen::VectorXd> loadData8(const std::string& filename){
  std::vector<Eigen::VectorXd> data;
  std::ifstream f(filename);std::string line;
  while(getline(f,line)){
    std::istringstream s(line);
    Eigen::VectorXd v(8);
    s>>v(0)>>v(1)>>v(2)>>v(3)>>v(4)>>v(5)>>v(6)>>v(7);
    data.push_back(v);
  }
  return data;
}
std::vector<Eigen::MatrixXd> loadDataKTV(const std::string& filename, int NN){
  std::vector<Eigen::MatrixXd> data;
  std::ifstream f(filename);std::string line;
  for(int ii=0;ii<NN;ii++){
    Eigen::MatrixXd v(4,8);
    for(int i=0;i<4;i++){
      getline(f,line);std::istringstream s(line);
      s>>v(i,0)>>v(i,1)>>v(i,2)>>v(i,3)>>v(i,4)>>v(i,5)>>v(i,6)>>v(i,7);
    }
    data.push_back(v);
  }
  return data;
}

Eigen::MatrixXd signF(const Eigen::MatrixXd& x) {
  //Eigen::MatrixXd y=x;
  //for(int i=0;i<x.size();i++) y(i)=(x(i)>0)-(x(i)<0);
  //return y;
  return (x.array() > 0).cast<double>() - (x.array() < 0).cast<double>();
}
Eigen::MatrixXd tanhF(const Eigen::MatrixXd& x) {
  return x.array().tanh().matrix();
}
/*Eigen::MatrixXd sqrtFsignF(const Eigen::MatrixXd& x) {
  return x.array().sqrt().matrix();
}*/

int main(int argc, char* argv[]){
  if(argc!=5&&argc!=10){
    printf("Usage : %s <port>\n",argv[0]);
    return 0;
  }
  float lambda=atof(argv[1]);
  float DZ=atof(argv[2]);
  int bff=atoi(argv[3]);
  float vf=atof(argv[4]);
  float bf=(float)bff/10.f;
  printf("lambda %f DZ %f bf %f vf %f\n",lambda,DZ,bf,vf);
  if(lambda<0 || lambda>90 || DZ<0 || DZ>60|| bff>25){
    printf("Invalid lambda or DZ\n");
    return 0;
  }

  Eigen::VectorXd q=Eigen::VectorXd::Zero(6);
  //Eigen::VectorXd qd=Eigen::VectorXd::Zero(6);//qd(4)=0;qd(5)=0;
  double sf1=.9,sf2=.9,sf3=.9,sf4=.9;
  double ff=1;
  if(argc==10){
    sf1=atof(argv[5]);;
    sf2=atof(argv[6]);
    sf3=atof(argv[7]);
    sf4=atof(argv[8]);
    ff=atof(argv[9]);
  }
  Eigen::VectorXd dq=Eigen::VectorXd::Zero(6);
  //std::cout<<"Mass matrix M(q):\n"<<M<<std::endl;
  //std::cout<<"Nonlinear effects b(q,v):\n"<<b.transpose()<<std::endl;

  std::signal(SIGINT,signalHandler);
  dynamixel::PortHandler *portHandler=dynamixel::PortHandler::getPortHandler(DEVICENAME);
  dynamixel::PacketHandler *packetHandler=dynamixel::PacketHandler::getPacketHandler(PROTOCOL_VERSION);
  dynamixel::GroupSyncWrite groupSyncWriteCurr(portHandler,packetHandler,ADDR_GOAL_CURRENT,LEN_GOAL_CURRENT);
  dynamixel::GroupFastSyncRead groupSFSyncRead_u_dq_q(portHandler,packetHandler,ADDR_PRESENT_CURRENT,TOTAL_LEN_READ);

  int dxl_comm_result=COMM_TX_FAIL;               // Communication result
  bool dxl_addparam_result=false;                 // addParam result
  bool dxl_getdata_result=false;                  // GetParam result
  uint8_t dxl_error=0;                            // Dynamixel error
  uint8_t param_goal_current1[2];
  uint8_t param_goal_current2[2];
  uint8_t param_goal_current3[2];
  uint8_t param_goal_current4[2];
  int32_t dxl1_pos=0,dxl2_pos=0,dxl3_pos=0,dxl4_pos=0;
  int32_t dxl1_vel=0,dxl2_vel=0,dxl3_vel=0,dxl4_vel=0;
  int16_t dxl1_cur=0,dxl2_cur=0,dxl3_cur=0,dxl4_cur=0;
  if(portHandler->openPort()){
    printf("Succeeded to open the port!\n");
  }else{
    printf("Failed to open the port!\n");
    printf("Press any key to terminate...\n");
    getch();
    return 0;
  }
  if(portHandler->setBaudRate(BAUDRATE)){
    printf("Succeeded to change the baudrate!\n");
  }else{
    printf("Failed to change the baudrate!\n");
    printf("Press any key to terminate...\n");
    getch();
    return 0;
  }

  uint8_t mode=0;
  dxl_comm_result=packetHandler->write1ByteTxRx(portHandler,DXL1_ID,ADDR_OPERATING_MODE,mode,&dxl_error);
  if(dxl_comm_result!=COMM_SUCCESS)printf("%s\n",packetHandler->getTxRxResult(dxl_comm_result));
  else if(dxl_error!=0)printf("%s\n",packetHandler->getRxPacketError(dxl_error));
  else printf("Succeeded%d enabling Current Mode.\n",DXL1_ID);
  dxl_comm_result=packetHandler->write1ByteTxRx(portHandler,DXL2_ID,ADDR_OPERATING_MODE,mode,&dxl_error);
  if(dxl_comm_result!=COMM_SUCCESS)printf("%s\n",packetHandler->getTxRxResult(dxl_comm_result));
  else if(dxl_error!=0)printf("%s\n",packetHandler->getRxPacketError(dxl_error));
  else printf("Succeeded%d enabling Current Mode.\n",DXL2_ID);
  dxl_comm_result=packetHandler->write1ByteTxRx(portHandler,DXL3_ID,ADDR_OPERATING_MODE,mode,&dxl_error);
  if(dxl_comm_result!=COMM_SUCCESS)printf("%s\n",packetHandler->getTxRxResult(dxl_comm_result));
  else if(dxl_error!=0)printf("%s\n",packetHandler->getRxPacketError(dxl_error));
  else printf("Succeeded%d enabling Current Mode.\n",DXL3_ID);
  dxl_comm_result=packetHandler->write1ByteTxRx(portHandler,DXL4_ID,ADDR_OPERATING_MODE,mode,&dxl_error);
  if(dxl_comm_result!=COMM_SUCCESS)printf("%s\n",packetHandler->getTxRxResult(dxl_comm_result));
  else if(dxl_error!=0)printf("%s\n",packetHandler->getRxPacketError(dxl_error));
  else printf("Succeeded%d enabling Current Mode.\n",DXL4_ID);

  dxl_comm_result=packetHandler->write1ByteTxRx(portHandler,DXL1_ID,ADDR_RETURN_DELAY,0,&dxl_error);
  if(dxl_comm_result!=COMM_SUCCESS)printf("%s\n",packetHandler->getTxRxResult(dxl_comm_result));
  else if(dxl_error!=0)printf("%s\n",packetHandler->getRxPacketError(dxl_error));
  else printf("Succeeded%d setting delay to zero.\n",DXL1_ID);
  dxl_comm_result=packetHandler->write1ByteTxRx(portHandler,DXL2_ID,ADDR_RETURN_DELAY,0,&dxl_error);
  if(dxl_comm_result!=COMM_SUCCESS)printf("%s\n",packetHandler->getTxRxResult(dxl_comm_result));
  else if(dxl_error!=0)printf("%s\n",packetHandler->getRxPacketError(dxl_error));
  else printf("Succeeded%d setting delay to zero.\n",DXL2_ID);
  dxl_comm_result=packetHandler->write1ByteTxRx(portHandler,DXL3_ID,ADDR_RETURN_DELAY,0,&dxl_error);
  if(dxl_comm_result!=COMM_SUCCESS)printf("%s\n",packetHandler->getTxRxResult(dxl_comm_result));
  else if(dxl_error!=0)printf("%s\n",packetHandler->getRxPacketError(dxl_error));
  else printf("Succeeded%d setting delay to zero.\n",DXL3_ID);
  dxl_comm_result=packetHandler->write1ByteTxRx(portHandler,DXL4_ID,ADDR_RETURN_DELAY,0,&dxl_error);
  if(dxl_comm_result!=COMM_SUCCESS)printf("%s\n",packetHandler->getTxRxResult(dxl_comm_result));
  else if(dxl_error!=0)printf("%s\n",packetHandler->getRxPacketError(dxl_error));
  else printf("Succeeded%d setting delay to zero.\n",DXL4_ID);
  uint16_t c_limit=300;
  dxl_comm_result=packetHandler->write2ByteTxRx(portHandler,DXL1_ID,ADDR_CURRENT_LIMIT,c_limit,&dxl_error);
  if(dxl_comm_result!=COMM_SUCCESS)printf("%s\n",packetHandler->getTxRxResult(dxl_comm_result));
  else if(dxl_error!=0)printf("%s\n",packetHandler->getRxPacketError(dxl_error));
  else printf("Succeeded%d limiting Current Max.\n",DXL1_ID);
  dxl_comm_result=packetHandler->write2ByteTxRx(portHandler,DXL2_ID,ADDR_CURRENT_LIMIT,c_limit,&dxl_error);
  if(dxl_comm_result!=COMM_SUCCESS)printf("%s\n",packetHandler->getTxRxResult(dxl_comm_result));
  else if(dxl_error!=0)printf("%s\n",packetHandler->getRxPacketError(dxl_error));
  else printf("Succeeded%d limiting Current Max.\n",DXL2_ID);
  dxl_comm_result=packetHandler->write2ByteTxRx(portHandler,DXL3_ID,ADDR_CURRENT_LIMIT,c_limit,&dxl_error);
  if(dxl_comm_result!=COMM_SUCCESS)printf("%s\n",packetHandler->getTxRxResult(dxl_comm_result));
  else if(dxl_error!=0)printf("%s\n",packetHandler->getRxPacketError(dxl_error));
  else printf("Succeeded%d limiting Current Max.\n",DXL3_ID);
  dxl_comm_result=packetHandler->write2ByteTxRx(portHandler,DXL4_ID,ADDR_CURRENT_LIMIT,c_limit,&dxl_error);
  if(dxl_comm_result!=COMM_SUCCESS)printf("%s\n",packetHandler->getTxRxResult(dxl_comm_result));
  else if(dxl_error!=0)printf("%s\n",packetHandler->getRxPacketError(dxl_error));
  else printf("Succeeded%d limiting Current Max.\n",DXL4_ID);

  dxl_comm_result=packetHandler->write1ByteTxRx(portHandler,DXL1_ID,ADDR_TORQUE_ENABLE,TORQUE_ENABLE,&dxl_error);
  if(dxl_comm_result!=COMM_SUCCESS)printf("%s\n",packetHandler->getTxRxResult(dxl_comm_result));
  else if(dxl_error!=0)printf("%s\n",packetHandler->getRxPacketError(dxl_error));
  else printf("Dynamixel#%d has been successfully connected \n",DXL1_ID);
  dxl_comm_result=packetHandler->write1ByteTxRx(portHandler,DXL2_ID,ADDR_TORQUE_ENABLE,TORQUE_ENABLE,&dxl_error);
  if(dxl_comm_result!=COMM_SUCCESS)printf("%s\n",packetHandler->getTxRxResult(dxl_comm_result));
  else if(dxl_error!=0)printf("%s\n",packetHandler->getRxPacketError(dxl_error));
  else printf("Dynamixel#%d has been successfully connected \n",DXL2_ID);
  dxl_comm_result=packetHandler->write1ByteTxRx(portHandler,DXL3_ID,ADDR_TORQUE_ENABLE,TORQUE_ENABLE,&dxl_error);
  if(dxl_comm_result!=COMM_SUCCESS)printf("%s\n",packetHandler->getTxRxResult(dxl_comm_result));
  else if(dxl_error!=0)printf("%s\n",packetHandler->getRxPacketError(dxl_error));
  else printf("Dynamixel#%d has been successfully connected \n",DXL3_ID);
  dxl_comm_result=packetHandler->write1ByteTxRx(portHandler,DXL4_ID,ADDR_TORQUE_ENABLE,TORQUE_ENABLE,&dxl_error);
  if(dxl_comm_result!=COMM_SUCCESS)printf("%s\n",packetHandler->getTxRxResult(dxl_comm_result));
  else if(dxl_error!=0)printf("%s\n",packetHandler->getRxPacketError(dxl_error));
  else printf("Dynamixel#%d has been successfully connected \n",DXL4_ID);

  dxl_addparam_result=groupSFSyncRead_u_dq_q.addParam(DXL1_ID);
  if(dxl_addparam_result!=true){fprintf(stderr,"[ID:%03d] groupSFSyncRead_u_dq_q addparam failed",DXL1_ID);return 0;}
  dxl_addparam_result=groupSFSyncRead_u_dq_q.addParam(DXL2_ID);
  if(dxl_addparam_result!=true){fprintf(stderr,"[ID:%03d] groupSFSyncRead_u_dq_q addparam failed",DXL2_ID);return 0;}
  dxl_addparam_result=groupSFSyncRead_u_dq_q.addParam(DXL3_ID);
  if(dxl_addparam_result!=true){fprintf(stderr,"[ID:%03d] groupSFSyncRead_u_dq_q addparam failed",DXL3_ID);return 0;}
  dxl_addparam_result=groupSFSyncRead_u_dq_q.addParam(DXL4_ID);
  if(dxl_addparam_result!=true){fprintf(stderr,"[ID:%03d] groupSFSyncRead_u_dq_q addparam failed",DXL4_ID);return 0;}

  int16_t curr_d1=0,curr_d2=0,curr_d3=0,curr_d4=0;
  printf("Press any key to continue!(or press ESC to quit!)\n");
  if(getch()!=ESC_ASCII_VALUE){
  std::ofstream outfile("data_log.txt", std::ios::out | std::ios::binary);
  if(!outfile.is_open()){
      std::cerr<<"Error opening file!\n";
      return 1;
  }
  int ifl_int=1000;
  int ifl=0;
  /*std::vector<Eigen::VectorXd> dataXref=loadData8("X.txt");
  std::vector<Eigen::VectorXd> dataUref=loadData4("U.txt");
  int ixmax=dataUref.size();
  std::vector<Eigen::MatrixXd> dataKref=loadDataKTV("K_TV.txt",ixmax);
  std::cout<<"All data read"<<std::endl;
  std::cout<<"sizeX"<<dataXref.size()<<std::endl;
  Eigen::VectorXd AAT=dataXref[0];
  std::cout<<"firstLineX"<<AAT(0)<<std::endl;
  std::cout<<"sizeU"<<dataUref.size()<<std::endl;
  std::cout<<"sizeKTV"<<dataKref.size()<<std::endl;
  Eigen::MatrixXd KAAT=dataKref[0];
  std::cout<<"firstLineK: "<<KAAT<<std::endl;
  KAAT=dataKref.back();
  std::cout<<"...lastLineK: "<<KAAT<<std::endl;
  */
  sleep(1);
  auto start=std::chrono::high_resolution_clock::now();
  Eigen::VectorXd xi(4);xi<<0,0,0,0;
  double M_doub[16];
  double phib_doub[4];
  Eigen::VectorXd y(4);
  Eigen::VectorXd dy(4);    
  Eigen::VectorXd yd(4);
  Eigen::VectorXd dyd(4);
  Eigen::VectorXd ddyd(4);
  Eigen::VectorXd dxi,ddxi,dq_(4);
  Eigen::VectorXd s,ds;
  double dJ[4][4];
  double pi_p = 3.141592;
  
  do{
    ifl++;
    //usleep(10);
    dxl_comm_result=groupSFSyncRead_u_dq_q.txRxPacket();// Syncread present position
    dxl_getdata_result=groupSFSyncRead_u_dq_q.isAvailable(DXL1_ID,ADDR_PRESENT_CURRENT,TOTAL_LEN_READ);
    if(dxl_getdata_result!=true){fprintf(stderr,"f[ID:%03d] groupSFSyncRead_u_dq_q getdata failed",DXL1_ID);break;}
    dxl_getdata_result=groupSFSyncRead_u_dq_q.isAvailable(DXL2_ID,ADDR_PRESENT_CURRENT,TOTAL_LEN_READ);
    if(dxl_getdata_result!=true){fprintf(stderr,"[ID:%03d] groupSFSyncRead_u_dq_q getdata failed",DXL2_ID);break;}
    dxl_getdata_result=groupSFSyncRead_u_dq_q.isAvailable(DXL3_ID,ADDR_PRESENT_CURRENT,TOTAL_LEN_READ);
    if(dxl_getdata_result!=true){fprintf(stderr,"[ID:%03d] groupSFSyncRead_u_dq_q getdata failed",DXL3_ID);break;}
    dxl_getdata_result=groupSFSyncRead_u_dq_q.isAvailable(DXL4_ID,ADDR_PRESENT_CURRENT,TOTAL_LEN_READ);
    if(dxl_getdata_result!=true){fprintf(stderr,"[ID:%03d] groupSFSyncRead_u_dq_q getdata failed",DXL4_ID);break;}
    
    
    dxl1_pos=groupSFSyncRead_u_dq_q.getData(DXL1_ID,ADDR_PRESENT_POSITION,LEN_PRESENT_POSITION);
    dxl2_pos=groupSFSyncRead_u_dq_q.getData(DXL2_ID,ADDR_PRESENT_POSITION,LEN_PRESENT_POSITION);
    dxl3_pos=groupSFSyncRead_u_dq_q.getData(DXL3_ID,ADDR_PRESENT_POSITION,LEN_PRESENT_POSITION);
    dxl4_pos=groupSFSyncRead_u_dq_q.getData(DXL4_ID,ADDR_PRESENT_POSITION,LEN_PRESENT_POSITION);

    dxl1_vel=groupSFSyncRead_u_dq_q.getData(DXL1_ID,ADDR_PRESENT_VELOCITY,LEN_PRESENT_VELOCITY);
    dxl2_vel=groupSFSyncRead_u_dq_q.getData(DXL2_ID,ADDR_PRESENT_VELOCITY,LEN_PRESENT_VELOCITY);
    dxl3_vel=groupSFSyncRead_u_dq_q.getData(DXL3_ID,ADDR_PRESENT_VELOCITY,LEN_PRESENT_VELOCITY);
    dxl4_vel=groupSFSyncRead_u_dq_q.getData(DXL4_ID,ADDR_PRESENT_VELOCITY,LEN_PRESENT_VELOCITY);

    dxl1_cur=groupSFSyncRead_u_dq_q.getData(DXL1_ID,ADDR_PRESENT_CURRENT,LEN_PRESENT_CURRENT);
    dxl2_cur=groupSFSyncRead_u_dq_q.getData(DXL2_ID,ADDR_PRESENT_CURRENT,LEN_PRESENT_CURRENT);
    dxl3_cur=groupSFSyncRead_u_dq_q.getData(DXL3_ID,ADDR_PRESENT_CURRENT,LEN_PRESENT_CURRENT);
    dxl4_cur=groupSFSyncRead_u_dq_q.getData(DXL4_ID,ADDR_PRESENT_CURRENT,LEN_PRESENT_CURRENT);

    //GENERAR ley de control u como vector de Eigen. Se obtendra M y b de pinocchio.
    // Use lo siguiente para generar estados q(en rad) como vector Eigen: dxl1_pos,dxl2_pos,dxl3_pos,dxl4_pos tienen 4096 valores por revolución, desfasados 2048 para
    // Use lo siguiente para generar estados dq(en rad/s) como vector Eigen: cada tick de dxl1_vel,dxl2_vel,dxl3_vel,dxl4_vel mide 0.229 rev/min

    q(0)=(dxl1_pos-2048)*3.1416f/2048;
    q(1)=(dxl2_pos-2048)*3.1416f/2048;
    q(2)=(dxl3_pos-2048)*3.1416f/2048;
    q(3)=(dxl4_pos-2048)*3.1416f/2048;
    dq(0)=(dxl1_vel)*0.229f*3.1416f/30.f;
    dq(1)=(dxl2_vel)*0.229f*3.1416f/30.f;
    dq(2)=(dxl3_vel)*0.229f*3.1416f/30.f;
    dq(3)=(dxl4_vel)*0.229f*3.1416f/30.f;

    double q_doub[4]={q(0),q(1),q(2),q(3)};
    double dq_doub[4]={dq(0),dq(1),dq(2),dq(3)};

    OMDyn(q_doub,dq_doub,M_doub,phib_doub);
    Eigen::Map<Eigen::Matrix<double,4,4,Eigen::RowMajor>>M(M_doub);
    Eigen::Map<Eigen::VectorXd>b(phib_doub,4);

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    double t=elapsed.count();
    //qd<<3.1416/4*sin(2*t),-0.5+0.5*sin(2*t),0.3-0.5*sin(2*t),1.3-(-0.5+0.5*sin(2*t))-(0.3-0.5*sin(2*t)),0,0;
    //Eigen::VectorXd dqd=Eigen::VectorXd::Zero(6);;
    //dqd<<3.1416/4*2*cos(2*t),0.5*2*cos(2*t),-0.5*2*cos(2*t),-(0.5*2*cos(2*t))-(-0.5*2*cos(2*t)),0,0;
    //Eigen::VectorXd ddqd=Eigen::VectorXd::Zero(6);;
    //ddqd<<-3.1416*sin(2*t),-4*0.5*sin(2*t),4*0.5*sin(2*t),(0.5*4*sin(2*t))-(0.5*4*sin(2*t)),0,0;
    
    double q1=q(0);
    double q2=q(1);
    double q3=q(2);
    double q4=q(3);

    double dq1=dq(0);
    double dq2=dq(1);
    double dq3=dq(2);
    double dq4=dq(3);

    double Jinv[4][4];
    Jinv[0][0] = (sin(q1)*-5.0E+2)/(cos(q2+q3+q4)*6.3E+1+sin(q2+3.7E+1/2.0E+2)*6.5E+1+cos(q2+q3)*6.2E+1);
    Jinv[0][1] = (cos(q1)*5.0E+2)/(cos(q2+q3+q4)*6.3E+1+sin(q2+3.7E+1/2.0E+2)*6.5E+1+cos(q2+q3)*6.2E+1);
    Jinv[1][0] = (cos(q2+q3)*cos(q1)*(1.0E+2/1.3E+1))/cos(q3-3.7E+1/2.0E+2);
    Jinv[1][1] = (cos(q2+q3)*sin(q1)*(1.0E+2/1.3E+1))/cos(q3-3.7E+1/2.0E+2);
    Jinv[1][2] = (sin(q2+q3)*(-1.0E+2/1.3E+1))/cos(q3-3.7E+1/2.0E+2);
    Jinv[1][3] = (sin(q4)*(6.3E+1/6.5E+1))/cos(q3-3.7E+1/2.0E+2);
    Jinv[2][0] = (cos(q1)*(sin(q2+3.7E+1/2.0E+2)*6.5E+1+cos(q2+q3)*6.2E+1)*(-5.0E+1/4.03E+2))/cos(q3-3.7E+1/2.0E+2);
    Jinv[2][1] = (sin(q1)*(sin(q2+3.7E+1/2.0E+2)*6.5E+1+cos(q2+q3)*6.2E+1)*(-5.0E+1/4.03E+2))/cos(q3-3.7E+1/2.0E+2);
    Jinv[2][2] = ((cos(q2+3.7E+1/2.0E+2)*6.5E+1-sin(q2+q3)*6.2E+1)*(-5.0E+1/4.03E+2))/cos(q3-3.7E+1/2.0E+2);
    Jinv[2][3] = ((cos(q3+q4-3.7E+1/2.0E+2)*6.5E+1+sin(q4)*6.2E+1)*(-1.563275434243176E-2))/cos(q3-3.7E+1/2.0E+2);
    Jinv[3][0] = (sin(q2+3.7E+1/2.0E+2)*cos(q1)*(2.5E+2/3.1E+1))/cos(q3-3.7E+1/2.0E+2);
    Jinv[3][1] = (sin(q2+3.7E+1/2.0E+2)*sin(q1)*(2.5E+2/3.1E+1))/cos(q3-3.7E+1/2.0E+2);
    Jinv[3][2] = (cos(q2+3.7E+1/2.0E+2)*(2.5E+2/3.1E+1))/cos(q3-3.7E+1/2.0E+2);
    Jinv[3][3] = (cos(q3-3.7E+1/2.0E+2)*6.2E+1+cos(q3+q4-3.7E+1/2.0E+2)*6.3E+1)/(cos(q3-3.7E+1/2.0E+2)*6.2E+1);
    Eigen::Matrix4d Jinv_ = Eigen::Map<Eigen::Matrix<double, 4, 4, Eigen::RowMajor>>(&Jinv[0][0]);
      
    dJ[0][0] = dq4*cos(q2+q3+q4+3.141592653589793/2.0)*sin(q1)*(-6.3E+1/5.0E+2)-dq2*sin(q1)*(cos(q2+3.7E+1/2.0E+2)*(1.3E+1/1.0E+2)+cos(q2+q3+3.141592653589793/2.0)*(3.1E+1/2.5E+2)+cos(q2+q3+q4+3.141592653589793/2.0)*(6.3E+1/5.0E+2))-dq1*cos(q1)*(sin(q2+3.7E+1/2.0E+2)*(1.3E+1/1.0E+2)+sin(q2+q3+3.141592653589793/2.0)*(3.1E+1/2.5E+2)+sin(q2+q3+q4+3.141592653589793/2.0)*(6.3E+1/5.0E+2))-dq3*sin(q1)*(cos(q2+q3+3.141592653589793/2.0)*(3.1E+1/2.5E+2)+cos(q2+q3+q4+3.141592653589793/2.0)*(6.3E+1/5.0E+2));
    dJ[0][1] = dq4*cos(q1)*sin(q2+q3+q4+3.141592653589793/2.0)*(-6.3E+1/5.0E+2)-dq1*sin(q1)*(cos(q2+3.7E+1/2.0E+2)*(1.3E+1/1.0E+2)+cos(q2+q3+3.141592653589793/2.0)*(3.1E+1/2.5E+2)+cos(q2+q3+q4+3.141592653589793/2.0)*(6.3E+1/5.0E+2))-dq2*cos(q1)*(sin(q2+3.7E+1/2.0E+2)*(1.3E+1/1.0E+2)+sin(q2+q3+3.141592653589793/2.0)*(3.1E+1/2.5E+2)+sin(q2+q3+q4+3.141592653589793/2.0)*(6.3E+1/5.0E+2))-dq3*cos(q1)*(sin(q2+q3+3.141592653589793/2.0)*(3.1E+1/2.5E+2)+sin(q2+q3+q4+3.141592653589793/2.0)*(6.3E+1/5.0E+2));
    dJ[0][2] = dq4*cos(q1)*sin(q2+q3+q4+3.141592653589793/2.0)*(-6.3E+1/5.0E+2)-dq1*sin(q1)*(cos(q2+q3+3.141592653589793/2.0)*(3.1E+1/2.5E+2)+cos(q2+q3+q4+3.141592653589793/2.0)*(6.3E+1/5.0E+2))-dq2*cos(q1)*(sin(q2+q3+3.141592653589793/2.0)*(3.1E+1/2.5E+2)+sin(q2+q3+q4+3.141592653589793/2.0)*(6.3E+1/5.0E+2))-dq3*cos(q1)*(sin(q2+q3+3.141592653589793/2.0)*(3.1E+1/2.5E+2)+sin(q2+q3+q4+3.141592653589793/2.0)*(6.3E+1/5.0E+2));
    dJ[0][3] = dq1*cos(q2+q3+q4+3.141592653589793/2.0)*sin(q1)*(-6.3E+1/5.0E+2)-dq2*cos(q1)*sin(q2+q3+q4+3.141592653589793/2.0)*(6.3E+1/5.0E+2)-dq3*cos(q1)*sin(q2+q3+q4+3.141592653589793/2.0)*(6.3E+1/5.0E+2)-dq4*cos(q1)*sin(q2+q3+q4+3.141592653589793/2.0)*(6.3E+1/5.0E+2);
    dJ[1][0] = dq4*cos(q2+q3+q4+3.141592653589793/2.0)*cos(q1)*(6.3E+1/5.0E+2)+dq2*cos(q1)*(cos(q2+3.7E+1/2.0E+2)*(1.3E+1/1.0E+2)+cos(q2+q3+3.141592653589793/2.0)*(3.1E+1/2.5E+2)+cos(q2+q3+q4+3.141592653589793/2.0)*(6.3E+1/5.0E+2))-dq1*sin(q1)*(sin(q2+3.7E+1/2.0E+2)*(1.3E+1/1.0E+2)+sin(q2+q3+3.141592653589793/2.0)*(3.1E+1/2.5E+2)+sin(q2+q3+q4+3.141592653589793/2.0)*(6.3E+1/5.0E+2))+dq3*cos(q1)*(cos(q2+q3+3.141592653589793/2.0)*(3.1E+1/2.5E+2)+cos(q2+q3+q4+3.141592653589793/2.0)*(6.3E+1/5.0E+2));
    dJ[1][1] = -dq3*sin(q1)*(sin(q2+q3+3.141592653589793/2.0)*(3.1E+1/2.5E+2)+sin(q2+q3+q4+3.141592653589793/2.0)*(6.3E+1/5.0E+2))+dq1*cos(q1)*(cos(q2+3.7E+1/2.0E+2)*(1.3E+1/1.0E+2)+cos(q2+q3+3.141592653589793/2.0)*(3.1E+1/2.5E+2)+cos(q2+q3+q4+3.141592653589793/2.0)*(6.3E+1/5.0E+2))-dq4*sin(q2+q3+q4+3.141592653589793/2.0)*sin(q1)*(6.3E+1/5.0E+2)-dq2*sin(q1)*(sin(q2+3.7E+1/2.0E+2)*(1.3E+1/1.0E+2)+sin(q2+q3+3.141592653589793/2.0)*(3.1E+1/2.5E+2)+sin(q2+q3+q4+3.141592653589793/2.0)*(6.3E+1/5.0E+2));
    dJ[1][2] = -dq2*sin(q1)*(sin(q2+q3+3.141592653589793/2.0)*(3.1E+1/2.5E+2)+sin(q2+q3+q4+3.141592653589793/2.0)*(6.3E+1/5.0E+2))-dq3*sin(q1)*(sin(q2+q3+3.141592653589793/2.0)*(3.1E+1/2.5E+2)+sin(q2+q3+q4+3.141592653589793/2.0)*(6.3E+1/5.0E+2))-dq4*sin(q2+q3+q4+3.141592653589793/2.0)*sin(q1)*(6.3E+1/5.0E+2)+dq1*cos(q1)*(cos(q2+q3+3.141592653589793/2.0)*(3.1E+1/2.5E+2)+cos(q2+q3+q4+3.141592653589793/2.0)*(6.3E+1/5.0E+2));
    dJ[1][3] = dq1*cos(q2+q3+q4+3.141592653589793/2.0)*cos(q1)*(6.3E+1/5.0E+2)-dq2*sin(q2+q3+q4+3.141592653589793/2.0)*sin(q1)*(6.3E+1/5.0E+2)-dq3*sin(q2+q3+q4+3.141592653589793/2.0)*sin(q1)*(6.3E+1/5.0E+2)-dq4*sin(q2+q3+q4+3.141592653589793/2.0)*sin(q1)*(6.3E+1/5.0E+2);
    dJ[2][1] = dq4*cos(q2+q3+q4+3.141592653589793/2.0)*(-6.3E+1/5.0E+2)-dq2*(cos(q2+3.7E+1/2.0E+2)*(1.3E+1/1.0E+2)+cos(q2+q3+3.141592653589793/2.0)*(3.1E+1/2.5E+2)+cos(q2+q3+q4+3.141592653589793/2.0)*(6.3E+1/5.0E+2))-dq3*(cos(q2+q3+3.141592653589793/2.0)*(3.1E+1/2.5E+2)+cos(q2+q3+q4+3.141592653589793/2.0)*(6.3E+1/5.0E+2));
    dJ[2][2] = dq4*cos(q2+q3+q4+3.141592653589793/2.0)*(-6.3E+1/5.0E+2)-dq2*(cos(q2+q3+3.141592653589793/2.0)*(3.1E+1/2.5E+2)+cos(q2+q3+q4+3.141592653589793/2.0)*(6.3E+1/5.0E+2))-dq3*(cos(q2+q3+3.141592653589793/2.0)*(3.1E+1/2.5E+2)+cos(q2+q3+q4+3.141592653589793/2.0)*(6.3E+1/5.0E+2));
    dJ[2][3] = dq2*cos(q2+q3+q4+3.141592653589793/2.0)*(-6.3E+1/5.0E+2)-dq3*cos(q2+q3+q4+3.141592653589793/2.0)*(6.3E+1/5.0E+2)-dq4*cos(q2+q3+q4+3.141592653589793/2.0)*(6.3E+1/5.0E+2);
    dJ[3][0]=0;
    dJ[3][1]=0;
    dJ[3][2]=0;
    dJ[3][3]=0;
    Eigen::Matrix4d dJ_ = Eigen::Map<Eigen::Matrix<double, 4, 4, Eigen::RowMajor>>(&dJ[0][0]);
      
    y[0] = cos(q1)*(sin(q2+3.7E+1/2.0E+2)*(1.3E+1/1.0E+2)+sin(q2+q3+3.141592653589793/2.0)*(3.1E+1/2.5E+2)+sin(q2+q3+q4+3.141592653589793/2.0)*(6.3E+1/5.0E+2));
    y[1] = sin(q1)*(sin(q2+3.7E+1/2.0E+2)*(1.3E+1/1.0E+2)+sin(q2+q3+3.141592653589793/2.0)*(3.1E+1/2.5E+2)+sin(q2+q3+q4+3.141592653589793/2.0)*(6.3E+1/5.0E+2));
    y[2] = cos(q2+3.7E+1/2.0E+2)*(1.3E+1/1.0E+2)+cos(q2+q3+3.141592653589793/2.0)*(3.1E+1/2.5E+2)+cos(q2+q3+q4+3.141592653589793/2.0)*(6.3E+1/5.0E+2)+7.7E+1/1.0E+3;
    y[3] = q2+q3+q4;
    
    dy[0] = dq4*cos(q2+q3+q4+3.141592653589793/2.0)*cos(q1)*(6.3E+1/5.0E+2)+dq2*cos(q1)*(cos(q2+3.7E+1/2.0E+2)*(1.3E+1/1.0E+2)+cos(q2+q3+3.141592653589793/2.0)*(3.1E+1/2.5E+2)+cos(q2+q3+q4+3.141592653589793/2.0)*(6.3E+1/5.0E+2))-dq1*sin(q1)*(sin(q2+3.7E+1/2.0E+2)*(1.3E+1/1.0E+2)+sin(q2+q3+3.141592653589793/2.0)*(3.1E+1/2.5E+2)+sin(q2+q3+q4+3.141592653589793/2.0)*(6.3E+1/5.0E+2))+dq3*cos(q1)*(cos(q2+q3+3.141592653589793/2.0)*(3.1E+1/2.5E+2)+cos(q2+q3+q4+3.141592653589793/2.0)*(6.3E+1/5.0E+2));
    dy[1] = dq4*cos(q2+q3+q4+3.141592653589793/2.0)*sin(q1)*(6.3E+1/5.0E+2)+dq2*sin(q1)*(cos(q2+3.7E+1/2.0E+2)*(1.3E+1/1.0E+2)+cos(q2+q3+3.141592653589793/2.0)*(3.1E+1/2.5E+2)+cos(q2+q3+q4+3.141592653589793/2.0)*(6.3E+1/5.0E+2))+dq1*cos(q1)*(sin(q2+3.7E+1/2.0E+2)*(1.3E+1/1.0E+2)+sin(q2+q3+3.141592653589793/2.0)*(3.1E+1/2.5E+2)+sin(q2+q3+q4+3.141592653589793/2.0)*(6.3E+1/5.0E+2))+dq3*sin(q1)*(cos(q2+q3+3.141592653589793/2.0)*(3.1E+1/2.5E+2)+cos(q2+q3+q4+3.141592653589793/2.0)*(6.3E+1/5.0E+2));
    dy[2] = -dq3*(sin(q2+q3+3.141592653589793/2.0)*(3.1E+1/2.5E+2)+sin(q2+q3+q4+3.141592653589793/2.0)*(6.3E+1/5.0E+2))-dq4*sin(q2+q3+q4+3.141592653589793/2.0)*(6.3E+1/5.0E+2)-dq2*(sin(q2+3.7E+1/2.0E+2)*(1.3E+1/1.0E+2)+sin(q2+q3+3.141592653589793/2.0)*(3.1E+1/2.5E+2)+sin(q2+q3+q4+3.141592653589793/2.0)*(6.3E+1/5.0E+2));
    dy[3] = dq2+dq3+dq4;
    
    //Completar código ---------------------------------------------
    

    Eigen::VectorXd dq_p=Eigen::VectorXd::Zero(4);

    dq_p(0) = dq(0);
    dq_p(1) = dq(1);
    dq_p(2) = dq(2);
    dq_p(3) = dq(3);


    // Trayectoria deseada (analítica)
      // Default analytic trajectory
      double omega = 0.5;  // frecuencia angular
      Eigen::Vector4d yd_analytic;
      yd_analytic(0) = 0.1 + 0.04*std::cos(omega*t);
      yd_analytic(1) = 0.05*std::sin(omega*t);
      yd_analytic(2) = 0.1;
      yd_analytic(3) = pi_p/2;

      Eigen::Vector4d dyd_analytic;
      dyd_analytic(0) = -0.04*omega*std::sin(omega*t);
      dyd_analytic(1) = 0.05*omega*std::cos(omega*t);
      dyd_analytic(2) = 0.0;
      dyd_analytic(3) = 0.0;

      Eigen::Vector4d ddyd_analytic = Eigen::Vector4d::Zero();
      ddyd_analytic(0) = -0.04*omega*omega*std::cos(omega*t);
      ddyd_analytic(1) = -0.05*omega*omega*std::sin(omega*t);

    // If haptics is available and a fresh sample exists, use it as yd (but only after arming)
    static Eigen::Vector4d h_prev = Eigen::Vector4d::Zero();
    static Eigen::Vector4d dh = Eigen::Vector4d::Zero();
    static Eigen::Vector4d ddh = Eigen::Vector4d::Zero();
    static double h_prev_time = 0.0;
    static bool teleop_active = false; // becomes true when user presses haptic button
    static bool home_captured = false;
    static Eigen::Vector4d h_base = Eigen::Vector4d::Zero();
    static Eigen::Vector4d yd_home = Eigen::Vector4d::Zero();
    static int prev_buttons = 0;
    bool usedHaptics = false;
      if(g_hDevice==HD_INVALID_HANDLE){
        // try init once
        g_hDevice = hdInitDevice(HD_DEFAULT_DEVICE);
        if(!HD_DEVICE_ERROR(hdGetError())){
          hdMakeCurrentDevice(g_hDevice);
          hdEnable(HD_FORCE_OUTPUT);
          hdSetSchedulerRate(1000);
          hdStartScheduler();
          hdScheduleAsynchronous(hapticsServoCallback, nullptr, 0);
        }else{
          g_hDevice = HD_INVALID_HANDLE;
        }
      }

      if(g_hDevice!=HD_INVALID_HANDLE && g_hSampleReady.exchange(false)){
        // map haptic sample to h_cur: x,y,z and roll (from transform)
        double* T = g_hSample.transform;
        double roll  = std::atan2(T[6], T[10]);
        Eigen::Vector4d h_cur;
        h_cur(0) = g_hSample.position[0];
        h_cur(1) = g_hSample.position[1];
        h_cur(2) = g_hSample.position[2];
        h_cur(3) = roll;

        int buttons = g_hButtons.load(std::memory_order_acquire);

        double now = g_hSample.timestamp;
        double dt = (h_prev_time>0) ? std::max(1e-6, now - h_prev_time) : 0.001;

        // finite-difference + simple low-pass for derivative
        Eigen::Vector4d raw_dh = (h_cur - h_prev) / dt;
        const double alpha = 0.2; // low-pass for derivative
        dh = alpha * raw_dh + (1.0 - alpha) * dh;
        // approximate second derivative (placeholder: set to zero or compute with history)
        if(dt > 0.0) {
          // no previous derivative history kept -> keep ddh small (0) for stability
          ddh.setZero();
        } else {
          ddh.setZero();
        }

        // detect rising edge on button 1 (bit 0) to arm, any button press after arming to shutdown
        const int BTN_MASK = 1;
        if(!teleop_active){
          // Not armed yet: detect rising edge to arm
          if((buttons & BTN_MASK) && !(prev_buttons & BTN_MASK)){
            // Capture home at the EXACT moment of arming (zero error guaranteed)
            yd_home = y; // use current robot position as home
            home_captured = true;
            std::cout << "Home captured at arming: [" 
                      << y(0) << ", " << y(1) << ", " << y(2) << ", " << y(3) << "]\n";
            
            // arm teleoperation: capture haptic baseline
            teleop_active = true;
            h_base = h_cur;
            
            // CRITICAL: Reset all filter states to current position (prevents initial jump)
            extern Eigen::Vector4d yd_filtered_state;
            extern Eigen::Vector4d yd_prev_state;
            yd_filtered_state = yd_home;
            yd_prev_state = yd_home;
            
            // reset derivative history
            dh.setZero(); ddh.setZero(); h_prev = h_cur; h_prev_time = now;
            std::cout << "Teleop armed via haptic button. Press any button again to stop safely.\n";
            std::cout << "Haptic base captured: [" 
                      << h_base(0) << ", " << h_base(1) << ", " << h_base(2) << ", " << h_base(3) << "]\n";
            std::cout << "[ARMING] Initial error should be zero: err=[" 
                      << (y(0)-yd_home(0)) << ", " << (y(1)-yd_home(1)) << ", " 
                      << (y(2)-yd_home(2)) << ", " << (y(3)-yd_home(3)) << "]\n";
          }
        } else {
          // Already armed: any button press triggers safe shutdown
          if(buttons != 0 && buttons != prev_buttons){
            std::cout << "Button pressed - initiating safe shutdown...\n";
            shutdown_requested = 1;
          }
        }

        prev_buttons = buttons;

        if(teleop_active){
          // Coordinate system mapping for intuitive control:
          // Haptic X (left/right) → Arm Y (left/right) - CORRECTED
          // Haptic Y (up/down) → Arm Z (up/down) - natural vertical control
          // Haptic Z (forward/back) → Arm X (forward/back) - CORRECTED
          // Haptic Roll → Arm Yaw (rotation)
          
          Eigen::Vector4d delta_haptic = h_cur - h_base;
          
          // Remap haptic coordinates to arm coordinates (CORRECTED X/Y swap)
          Eigen::Vector4d delta;
          delta(0) = delta_haptic(2);   // Haptic Z → Arm X (forward/back)
          delta(1) = delta_haptic(0);   // Haptic X → Arm Y (left/right)
          delta(2) = delta_haptic(1);   // Haptic Y → Arm Z (up/down)
          delta(3) = delta_haptic(3);   // Haptic Roll → Arm Yaw (rotation)
          
          // Scaling: adjust sensitivity for each axis independently
          const double x_scale = 0.0011;    // X axis (forward/back)
          const double y_scale = 0.0011;    // Y axis (left/right) - NOW SEPARATE!
          const double z_scale = 0.001;    // Z axis (up/down)
          const double ang_scale = 1.20;   // Yaw rotation
          Eigen::Vector4d scale;
          scale << x_scale, y_scale, z_scale, ang_scale;

          // Scale haptic delta to robot-space offset (sensitivity)
          Eigen::Vector4d offset = scale.cwiseProduct(delta);

          // Clamp in ROBOT space (so changing scale does NOT change the max allowed displacement)
          Eigen::Vector4d max_offset; // robot-space limits [m, m, m, rad]
          max_offset << 0.30, 0.30, 0.15, 0.5;
          for(int i=0;i<4;i++){
            if(offset(i) > max_offset(i)) offset(i) = max_offset(i);
            if(offset(i) < -max_offset(i)) offset(i) = -max_offset(i);
          }

          Eigen::Vector4d yd_target = yd_home + offset;
          
          // Low-pass filter + rate limiter for smooth yd trajectory
          static Eigen::Vector4d yd_filtered = yd_home;
          static Eigen::Vector4d yd_prev = yd_home;
          
          // Expose filter states for reset on arming
          extern Eigen::Vector4d yd_filtered_state;
          extern Eigen::Vector4d yd_prev_state;
          yd_filtered_state = yd_filtered;
          yd_prev_state = yd_prev;
          
          // Filter tuning (adjust to reduce lag vs smoothness trade-off)
          const double alpha = 0.15;  // INCREASED from 0.05: more responsive, less lag
                                       // Higher = faster response but less smooth
                                       // Lower = smoother but more lag
          
          // Exponential moving average filter: smooth out discrete jumps from haptic
          yd_filtered = alpha * yd_target + (1.0 - alpha) * yd_filtered;
          
          // Rate limiter: prevent sudden changes even after filtering
          const double max_rate_pos = 0.003;  // INCREASED from 0.001mm: 3mm per cycle
          const double max_rate_ang = 0.02;   // INCREASED from 0.01: faster rotation response
          Eigen::Vector4d max_rate; max_rate<<max_rate_pos,max_rate_pos,max_rate_pos,max_rate_ang;
          
          Eigen::Vector4d yd_change = yd_filtered - yd_prev;
          for(int i=0;i<4;i++){
            if(yd_change(i) > max_rate(i)) yd_change(i) = max_rate(i);
            if(yd_change(i) < -max_rate(i)) yd_change(i) = -max_rate(i);
          }
          
          yd = yd_prev + yd_change;
          
          // Compute desired velocity from filtered trajectory (smooth numerical derivative)
          // This provides feedforward to improve tracking and reduce lag
          static Eigen::Vector4d yd_prev_for_vel = yd_prev;
          const double dt_control = 0.001; // 1kHz control loop
          dyd = (yd - yd_prev_for_vel) / dt_control;
          
          // Simple velocity smoothing (optional - reduces noise in dyd)
          static Eigen::Vector4d dyd_prev = Eigen::Vector4d::Zero();
          const double vel_alpha = 0.3; // smoothing for velocity
          dyd = vel_alpha * dyd + (1.0 - vel_alpha) * dyd_prev;
          dyd_prev = dyd;
          
          // Compute desired acceleration (from smoothed velocity)
          static Eigen::Vector4d dyd_for_accel = Eigen::Vector4d::Zero();
          ddyd = (dyd - dyd_for_accel) / dt_control;
          
          // Acceleration smoothing (more aggressive since it's noisier)
          static Eigen::Vector4d ddyd_prev = Eigen::Vector4d::Zero();
          const double accel_alpha = 0.5; // higher smoothing for acceleration
          ddyd = accel_alpha * ddyd + (1.0 - accel_alpha) * ddyd_prev;
          ddyd_prev = ddyd;
          
          // Update states for next iteration
          yd_prev_for_vel = yd;
          dyd_for_accel = dyd;
          yd_prev = yd;
          
          usedHaptics = true;
        }

        // update previous haptic pose/time
        h_prev = h_cur;
        h_prev_time = now;
      }

      // If teleop not active, keep yd equal to current position (zero error, no movement)
      if(!teleop_active){
        if(!home_captured) { 
          yd = y; // Before arming: always follow current position (zero error)
        } else { 
          yd = yd_home; // After capturing home but if teleop got disabled
        }
        dyd.setZero();
        ddyd.setZero();
      }

    // Control
    Eigen::VectorXd u(4);
    Eigen::VectorXd k(4);
    Eigen::VectorXd S(4);
    Eigen::VectorXd lbd(4);
    Eigen::VectorXd v=Eigen::VectorXd::Zero(4);

    // Control gains: lbd (lambda) affects position error weighting
    // High values = aggressive tracking but can cause oscillations
    // If you see oscillations, try reducing these values (e.g., lbd << 40, 20, 40, 12)
    lbd << 55 , 20, 45 ,15 ;   
    lbd << 25 , 10, 25 ,20 ;
    dbg("Actiidad_2");


    k << 3 , 5 , 5 , 5 ;

    Eigen::MatrixXd k3(4,4);
    k3 << 8,0,0,0,
          0,8,0,0,
          0,0,8,0,
          0,0,0,8;

    double k2;
    // bueno k2=9;
    k2=10.5;
    u << 0,0,0,0;
    S = lbd.cwiseProduct(y - yd)+(dy-dyd);

    dbg(S);
    dbg("S");

    //v = -k.cwiseProduct(S);
    v = -k2*S;

    dbg("v");
    dbg(v);

    u = M * Jinv_*(v - lbd.cwiseProduct(dy-dyd) + ddyd - dJ_ * dq_p) + b;

    dbg("u");
    dbg(u);
    dbg("multi");
    dbg(lbd.cwiseProduct(dy-dyd));
    dbg("otro");
    dbg(dJ_ * dq_p);
    dbg("Jinv:");
    dbg(Jinv_);

    //u << 0,0,0,0;
    Eigen::VectorXd curr=u/(1.666*0.00269);

    //(----------------------------------)

    curr_d1=sf1*(curr(0)+bf*dq(0));
    curr_d2=sf2*(curr(1)+bf*dq(1));
    curr_d3=sf3*(curr(2)+bf*dq(2));
    curr_d4=sf4*(curr(3)+bf*dq(3));
    curr_d1+=0.8*DZ*tanh(30*(float)curr_d1*0.00269);
    curr_d2+=0.9*DZ*tanh(30*(float)curr_d2*0.00269);
    curr_d3+=0.9*DZ*tanh(30*(float)curr_d3*0.00269);
    curr_d4+=0.9*DZ*tanh(30*(float)curr_d4*0.00269);//0.18
    
    if(curr_d1>curr_max)curr_d1=curr_max;
    if(curr_d1<-curr_max)curr_d1=-curr_max;
    if(curr_d2>curr_max)curr_d2=curr_max;
    if(curr_d2<-curr_max)curr_d2=-curr_max;
    if(curr_d3>curr_max)curr_d3=curr_max;
    if(curr_d3<-curr_max)curr_d3=-curr_max;
    if(curr_d4>0.8*curr_max)curr_d4=0.8*curr_max;
    if(curr_d4<-0.8*curr_max)curr_d4=-0.8*curr_max;

    param_goal_current1[0]=DXL_LOBYTE(curr_d1);
    param_goal_current1[1]=DXL_HIBYTE(curr_d1);
    param_goal_current2[0]=DXL_LOBYTE(curr_d2);
    param_goal_current2[1]=DXL_HIBYTE(curr_d2);
    param_goal_current3[0]=DXL_LOBYTE(curr_d3);
    param_goal_current3[1]=DXL_HIBYTE(curr_d3);
    param_goal_current4[0]=DXL_LOBYTE(curr_d4);
    param_goal_current4[1]=DXL_HIBYTE(curr_d4);

    dxl_addparam_result=groupSyncWriteCurr.addParam(DXL1_ID,param_goal_current1);
    if(dxl_addparam_result!=true){fprintf(stderr,"[ID:%03d] groupSyncWriteCurr addparam failed",DXL1_ID);return 0;}
    dxl_addparam_result=groupSyncWriteCurr.addParam(DXL2_ID,param_goal_current2);
    if(dxl_addparam_result!=true){fprintf(stderr,"[ID:%03d] groupSyncWriteCurr addparam failed",DXL2_ID);return 0;}
    dxl_addparam_result=groupSyncWriteCurr.addParam(DXL3_ID,param_goal_current3);
    if(dxl_addparam_result!=true){fprintf(stderr,"[ID:%03d] groupSyncWriteCurr addparam failed",DXL3_ID);return 0;}
    dxl_addparam_result=groupSyncWriteCurr.addParam(DXL4_ID,param_goal_current4);
    if(dxl_addparam_result!=true){fprintf(stderr,"[ID:%03d] groupSyncWriteCurr addparam failed",DXL4_ID);return 0;}
    
    dxl_comm_result=groupSyncWriteCurr.txPacket();
    if(dxl_comm_result!=COMM_SUCCESS)printf("%s\n",packetHandler->getTxRxResult(dxl_comm_result));
    

    groupSyncWriteCurr.clearParam();
    
    // Real-time display for calibration (updates every 10 cycles = ~100Hz)
    static int display_counter = 0;
    if(teleop_active && display_counter++ % 10 == 0){
      // Clear line and show current vs desired with visual bars
      printf("\r\033[K");  // Clear line
      printf("X: %+.3f→%+.3f ", (float)y(0), (float)yd(0));
      printf("Y: %+.3f→%+.3f ", (float)y(1), (float)yd(1));
      printf("Z: %+.3f→%+.3f ", (float)y(2), (float)yd(2));
      printf("θ: %+.2f→%+.2f ", (float)y(3), (float)yd(3));
      printf("| Err: X=%+.1fmm Y=%+.1fmm Z=%+.1fmm θ=%+.1f° ", 
             (float)(y(0)-yd(0))*1000, 
             (float)(y(1)-yd(1))*1000, 
             (float)(y(2)-yd(2))*1000, 
             (float)(y(3)-yd(3))*57.3);
      fflush(stdout);
    }
    
    // Diagnostic print when teleop is active (print first 20 cycles after arming for debugging)
    static int print_counter = 0;
    if(teleop_active && print_counter < 20){
      printf("\n[DEBUG] t=%.3f | y=[%.4f %.4f %.4f %.4f] | yd=[%.4f %.4f %.4f %.4f] | err=[%.4f %.4f %.4f %.4f] | cur=[%d %d %d %d]\n",
             (float)t,
             (float)y(0), (float)y(1), (float)y(2), (float)y(3),
             (float)yd(0), (float)yd(1), (float)yd(2), (float)yd(3),
             (float)(y(0)-yd(0)), (float)(y(1)-yd(1)), (float)(y(2)-yd(2)), (float)(y(3)-yd(3)),
             dxl1_cur, dxl2_cur, dxl3_cur, dxl4_cur);
      print_counter++;
    }
    
    if(abs(dxl1_cur)>curr_peak||abs(dxl2_cur)>curr_peak||abs(dxl3_cur)>curr_peak||abs(dxl4_cur)>curr_peak){
      printf("Unsafe %d %d %d %d\n",dxl1_cur,dxl2_cur,dxl3_cur,dxl4_cur);
      printf("[UNSAFE] y=[%.4f %.4f %.4f %.4f] | yd=[%.4f %.4f %.4f %.4f] | err=[%.4f %.4f %.4f %.4f]\n",
             (float)y(0), (float)y(1), (float)y(2), (float)y(3),
             (float)yd(0), (float)yd(1), (float)yd(2), (float)yd(3),
             (float)(y(0)-yd(0)), (float)(y(1)-yd(1)), (float)(y(2)-yd(2)), (float)(y(3)-yd(3)));
      shutdown_requested=1;
    }//else printf("%.3f %.2f %.2f %.2f %.2f %d %d %d %d\n",(float)t,(float)q(0),(float)q(1),(float)q(2),(float)q(3),dxl1_cur,dxl2_cur,dxl3_cur,dxl4_cur);
    else{
      //outfile<<t<<", "<<q(0)<<", "<<q(1)<<", "<<q(2)<<", "<<q(3)<<", "<<dq(0)<<", "<<dq(1)<<", "<<dq(2)<<", "<<dq(3)<<", "<<dxl1_cur<<", "<<dxl2_cur<<", "<<dxl3_cur<<", "<<dxl4_cur<<"\n";
      // Log format: t, y0-3(measured), yd0-3(desired), dq0-3, cur1-4
      outfile<<t<<", "<<y(0)<<", "<<y(1)<<", "<<y(2)<<", "<<y(3)<<", "
             <<yd(0)<<", "<<yd(1)<<", "<<yd(2)<<", "<<yd(3)<<", "
             <<dq(0)<<", "<<dq(1)<<", "<<dq(2)<<", "<<dq(3)<<", "
             <<dxl1_cur<<", "<<dxl2_cur<<", "<<dxl3_cur<<", "<<dxl4_cur<<"\n";
      //outfile<<t<<" Hola\n";
      if(ifl%ifl_int==0){
        outfile.flush();
      }
    }
    //printf("%.3f %.3f %.3f 0 0 0 0 0 0\n",(float)y(0),(float)y(1),(float)y(2));
    //printf("ID%03d: xd:%03d  x:%03d   ID%03d: xd:%03d  x:%03d   ID%03d: xd:%03d  x:%03d   ID%03d: xd:%03d  x:%03d\n",DXL1_ID,dxl_goal_position[index],dxl1_pos,DXL2_ID,dxl_goal_position[index],dxl2_pos,DXL3_ID,dxl_goal_position[index],dxl3_pos,DXL4_ID,dxl_goal_position[index],dxl4_pos);
  }while(!shutdown_requested);
  }
  
  dxl_comm_result=packetHandler->write1ByteTxRx(portHandler,DXL1_ID,ADDR_TORQUE_ENABLE,TORQUE_DISABLE,&dxl_error);
  if(dxl_comm_result!=COMM_SUCCESS)printf("%s\n",packetHandler->getTxRxResult(dxl_comm_result));
  else if(dxl_error!=0)printf("%s\n",packetHandler->getRxPacketError(dxl_error));
  dxl_comm_result=packetHandler->write1ByteTxRx(portHandler,DXL2_ID,ADDR_TORQUE_ENABLE,TORQUE_DISABLE,&dxl_error);
  if(dxl_comm_result!=COMM_SUCCESS)printf("%s\n",packetHandler->getTxRxResult(dxl_comm_result));
  else if(dxl_error!=0)printf("%s\n",packetHandler->getRxPacketError(dxl_error));
  dxl_comm_result=packetHandler->write1ByteTxRx(portHandler,DXL3_ID,ADDR_TORQUE_ENABLE,TORQUE_DISABLE,&dxl_error);
  if(dxl_comm_result!=COMM_SUCCESS)printf("%s\n",packetHandler->getTxRxResult(dxl_comm_result));
  else if(dxl_error!=0)printf("%s\n",packetHandler->getRxPacketError(dxl_error));
  dxl_comm_result=packetHandler->write1ByteTxRx(portHandler,DXL4_ID,ADDR_TORQUE_ENABLE,TORQUE_DISABLE,&dxl_error);
  if(dxl_comm_result!=COMM_SUCCESS)printf("%s\n",packetHandler->getTxRxResult(dxl_comm_result));
  else if(dxl_error!=0)printf("%s\n",packetHandler->getRxPacketError(dxl_error));
  
  // Clean up haptic device if initialized
  if(g_hDevice != HD_INVALID_HANDLE){
    hdStopScheduler();
    hdDisableDevice(g_hDevice);
    g_hDevice = HD_INVALID_HANDLE;
  }
  
  portHandler->closePort();
  return 0;
}


void OMDyn(const double* q, const double* dq, double* M, double* phib){
  double t10;
  double t100;
  double t101;
  double t104;
  double t106;
  double t107;
  double t11;
  double t115;
  double t116_tmp;
  double t12;
  double t15;
  double t16;
  double t17;
  double t18;
  double t19;
  double t2;
  double t20;
  double t21;
  double t22;
  double t23;
  double t24;
  double t25;
  double t26;
  double t27;
  double t28;
  double t29;
  double t3;
  double t30;
  double t31;
  double t32;
  double t33;
  double t34;
  double t35;
  double t36;
  double t37;
  double t38;
  double t4;
  double t46;
  double t48_tmp;
  double t49_tmp;
  double t5;
  double t50;
  double t52;
  double t52_tmp;
  double t53;
  double t53_tmp;
  double t6;
  double t61;
  double t62;
  double t64;
  double t64_tmp;
  double t66;
  double t67;
  double t67_tmp;
  double t68;
  double t68_tmp;
  double t7;
  double t72;
  double t73;
  double t76;
  double t76_tmp;
  double t8;
  double t80;
  double t80_tmp;
  double t82;
  double t82_tmp;
  double t9;
  double t98;
  t2 = cos(q[2]);
  t3 = cos(q[3]);
  t4 = sin(q[2]);
  t5 = sin(q[3]);
  t6 = q[1] + q[2];
  t7 = q[2] + q[3];
  t8 = dq[0] * dq[0];
  t9 = dq[1] * dq[1];
  t10 = dq[2] * dq[2];
  t11 = dq[3] * dq[3];
  t12 = q[1] * 2.0;
  t15 = cos(t12);
  t16 = sin(t12);
  t17 = cos(t6);
  t18 = cos(t7);
  t19 = q[3] + t6;
  t20 = sin(t6);
  t21 = sin(t7);
  t24 = q[1] + t6;
  t28 = t6 * 2.0;
  t46 = t5 * 0.0003578747437347655;
  t48_tmp = dq[1] * dq[3];
  t49_tmp = dq[2] * dq[3];
  t50 = t3 * 0.0035320003205986479;
  t52_tmp = t2 * t3;
  t52 = t52_tmp * 0.00052651602779200609;
  t53_tmp = t4 * t5;
  t53 = t53_tmp * 0.00052651602779200609;
  t67_tmp = t2 * t5;
  t67 = t67_tmp * 0.0017883348676894859;
  t68_tmp = t3 * t4;
  t68 = t68_tmp * 0.0017883348676894859;
  t22 = cos(t19);
  t23 = sin(t19);
  t25 = cos(t24);
  t26 = q[1] + t19;
  t27 = sin(t24);
  t31 = cos(t28);
  t32 = t6 + t19;
  t33 = sin(t28);
  t36 = q[3] * 2.0 + t28;
  t61 = t8 * t21 * 0.0002632580138960031;
  t62 = t9 * t21 * 0.00052651602779200609;
  t66 = -(t5 * t11 * 0.0017660001602993239);
  t72 = t8 * t18 * 0.000894167433844743;
  t73 = t9 * t18 * 0.0017883348676894859;
  t107 = (t5 * 0.0001789373718673827 + t3 * 0.0017660001602993239) +
         0.0010777913814770429;
  t29 = cos(t26);
  t30 = sin(t26);
  t34 = cos(t32);
  t35 = sin(t32);
  t37 = cos(t36);
  t38 = sin(t36);
  t104 = t8 * t33 * 0.0024776578125361871;
  t115 = (((t52 - t53) - t67) - t68) + t107;
  t116_tmp = t46 + t50;
  t67 = ((((((t116_tmp + t52) - t53) - t67) - t68) + t2 * 0.00100240441456248) -
         t4 * 0.0053461223198367682) +
        0.0060492094805849328;
  t64_tmp = t8 * t30;
  t64 = t64_tmp * 7.8548468742575741E-5;
  t80_tmp = t8 * t35;
  t80 = t80_tmp * 0.0017660001602993239;
  t100 = -(t8 * t31 * 2.7331035618357459E-9);
  t101 = t8 * t38 * 0.0004727803062273704;
  t68 = 1.359372084870438E+50 * cos(t19 + 0.017960143170678091) /
        6.1299821634635554E+56;
  t76_tmp = t8 * t34;
  t76 = -(t76_tmp * 0.0001789373718673827);
  t82_tmp = t8 * t29;
  t82 = -(t82_tmp * 0.00092880047356101074);
  t98 = -(t8 * t37 * 6.4649199983927934E-5);
  t19 = -(2.1899226517451251E+18 * sin(t19 + 1.4698174200878422) *
          6.4124981399849019E-20);
  t106 = -(cos(t6 - 1.17085425394026E-6) *
           4.3569993082336428E+39 / 1.0633823966279329E+40);
  t52 = -t68 - cos(t6 + 1.5707931241587512) *
                   5.3080995482529569E+54 / 7.846377169233351E+59;
  t53 = -(cos(q[1] + 0.14643291991477048) *
          3.6912047596486679E+50 / 2.3945242826029511E+55) +
        t52;
  M[0] =
      (((5.7840415964172063E+40 * cos(t24 - 1.3854495476534558) /
             1.0633823966279329E+43 +
         5.7840415964172063E+40 *
             cos(q[2] + 1.3854472059449479) /
             1.0633823966279329E+43) +
        (3.5649357967093821E+19 * cos(t26 - 1.4864272835062562) *
             5.2293562813332533E-23 +
         9.9536064990558271E+62 *
             (cos(t28 - 1.1030996887487746E-6) /
              4.0173451106474762E+65))) +
       (3.5649357967093821E+19 * cos(t7 + 1.2844694700921475) *
            5.2293562813332533E-23 +
        2.1899226517451251E+18 *
            cos(q[3] - 0.10097890670705442) *
            8.1055022360665421E-22)) +
      (((2.1899226517451251E+18 * cos(t32 - 0.10097890670705442) *
             8.1055022360665421E-22 +
         2.231677848771418E+51 * (cos(t36 - 0.13589972325856114) /
                                  4.6768052394588893E+54)) +
        3.017617666849213E+53 * (cos(t12 + 0.32877657078635497) *
                                 -1.3363823550460979E-56)) +
       0.0072605484133183859);
  M[1] = t53;
  M[2] = t52;
  M[3] = -t68;
  M[4] = t53;
  M[5] = (((((t2 * 0.0020048088291249608 - t4 * 0.01069224463967354) + t46) +
            t50) +
           (t52_tmp * 0.001053032055584012 - t67_tmp * 0.0035766697353789719)) +
          (t68_tmp * -0.0035766697353789719 - t53_tmp * 0.001053032055584012)) +
         0.014183673618319671;
  M[6] = t67;
  M[7] = t115;
  M[8] = t52;
  M[9] = t67;
  M[10] = t116_tmp + 0.0060492094805849328;
  M[11] = t107;
  M[12] = -t68;
  M[13] = t115;
  M[14] = t107;
  M[15] = 0.0010777913814770429;
  t52 = dq[0] * dq[2];
  t53 = dq[0] * dq[3];
  t67 = dq[0] * dq[1];
  t68 = dq[1] * dq[2];
  phib[0] =
      (((((((((((((((((((((((((((((((((((((t9 * t17 * 6.7650323630113937E-6 +
                                           t10 * t17 * 6.7650323630113937E-6) +
                                          t9 * t20 * 2.1665937170605689E-11) +
                                         t10 * t20 * 2.1665937170605689E-11) +
                                        t9 * t22 * 3.98258984851377E-9) +
                                       t9 * t23 * 2.2172215357361789E-7) +
                                      t10 * t22 * 3.98258984851377E-9) +
                                     t10 * t23 * 2.2172215357361789E-7) +
                                    t11 * t22 * 3.98258984851377E-9) +
                                   t11 * t23 * 2.2172215357361789E-7) +
                                  t9 * cos(q[1]) *
                                      2.2492329448778688E-6) +
                                 t9 * sin(q[1]) *
                                     1.5250214570970449E-5) +
                                (t52 * t2 * -0.0053461223198367682 -
                                 t52 * t4 * 0.00100240441456248)) +
                               (t53 * t3 * 0.0001789373718673827 -
                                t53 * t5 * 0.0017660001602993239)) +
                              t67 * t15 * 0.0026041938566917981) +
                             (t67 * t16 * 0.0076333846555699039 -
                              t52 * t18 * 0.0017883348676894859)) +
                            (t68 * t17 * 1.3530064726022791E-5 -
                             t53 * t18 * 0.0017883348676894859)) +
                           (t52 * t21 * -0.00052651602779200609 +
                            t68 * t20 * 4.3331874341211379E-11)) +
                          (t53 * t21 * -0.00052651602779200609 +
                           t68 * t22 * 7.96517969702754E-9)) +
                         (t67 * t25 * 0.010692249334322111 +
                          t68 * t23 * 4.4344430714723578E-7)) +
                        (t48_tmp * t22 * 7.96517969702754E-9 +
                         t52 * t25 * 0.0053461246671610563)) +
                       t48_tmp * t23 * 4.4344430714723578E-7) +
                      (t49_tmp * t22 * 7.96517969702754E-9 -
                       t67 * t27 * 0.0020047837909992232)) +
                     (t49_tmp * t23 * 4.4344430714723578E-7 -
                      t52 * t27 * 0.001002391895499612)) +
                    (t67 * t29 * 0.003715201894244043 -
                     t67 * t30 * 0.000314193874970303)) +
                   (t52 * t29 * 0.0018576009471220211 +
                    t67 * t31 * 5.4662071236714919E-9)) +
                  (t52 * t30 * -0.00015709693748515151 +
                   t53 * t29 * 0.0018576009471220211)) +
                 (t52 * t31 * 5.4662071236714919E-9 -
                  t53 * t30 * 0.00015709693748515151)) +
                t67 * t33 * -0.0049553156250723741) +
               t67 * t34 * 0.0003578747437347655) +
              t52 * t33 * -0.0049553156250723741) +
             (t67 * t35 * -0.0035320003205986479 +
              t52 * t34 * 0.0003578747437347655)) +
            (t52 * t35 * -0.0035320003205986479 +
             t53 * t34 * 0.0001789373718673827)) +
           (t67 * t37 * 0.00012929839996785589 -
            t53 * t35 * 0.0017660001602993239)) +
          t67 * t38 * -0.00094556061245474081) +
         t52 * t37 * 0.00012929839996785589) +
        t52 * t38 * -0.00094556061245474081) +
       t53 * t37 * 0.00012929839996785589) +
      t53 * t38 * -0.00094556061245474081;
  t52 = (((t48_tmp * t3 * 0.0003578747437347655 +
           t49_tmp * t3 * 0.0003578747437347655) +
          t3 * t11 * 0.0001789373718673827) -
         t48_tmp * t5 * 0.0035320003205986479) -
        t49_tmp * t5 * 0.0035320003205986479;
  t53 = t8 * t25;
  t67 = t8 * t27;
  phib[1] =
      (((((((((((((((((((((t52 + t66) + t76) + t80) + t98) + t100) + t101) +
                     t19) +
                    t104) +
                   t106) -
                  t2 * t10 * 0.0053461223198367682) -
                 t4 * t10 * 0.00100240441456248) +
                t8 * t15 * -0.0013020969283458991) +
               (t8 * t16 * -0.003816692327784952 -
                t10 * t18 * 0.0017883348676894859)) +
              (t11 * t18 * -0.0017883348676894859 -
               t10 * t21 * 0.00052651602779200609)) +
             (t11 * t21 * -0.00052651602779200609 -
              t53 * 0.0053461246671610563)) +
            (t67 * 0.001002391895499612 - t82_tmp * 0.0018576009471220211)) +
           t64_tmp * 0.00015709693748515151) +
          4.0301533745564482E+34 *
              cos(q[1] - 1.4091303705547358) *
              -1.540743955509789E-35) +
         (t68 * t2 * -0.01069224463967354 - t68 * t4 * 0.0020048088291249608)) +
        (t68 * t18 * -0.0035766697353789719 -
         t48_tmp * t18 * 0.0035766697353789719)) +
       (t49_tmp * t18 * -0.0035766697353789719 -
        t68 * t21 * 0.001053032055584012)) +
      (t48_tmp * t21 * -0.001053032055584012 -
       t49_tmp * t21 * 0.001053032055584012);
  phib[2] =
      ((((((((((((((((((t52 + t61) + t62) + t64) + t66) + t72) + t73) + t76) +
                 t80) +
                t82) +
               t98) +
              t100) +
             t101) +
            t19) +
           t104) +
          t106) +
         t2 * t8 * 0.0026730611599183841) +
        t2 * t9 * 0.0053461223198367682) +
       (t4 * t8 * 0.0005012022072812402 + t4 * t9 * 0.00100240441456248)) +
      (t53 * -0.0026730623335805281 + t67 * 0.00050119594774980579);
  phib[3] =
      (((((((((((((t61 + t62) + t64) + t72) + t73) + t82) + t98) + t101) +
            t19) -
           t3 * t8 * 8.9468685933691362E-5) -
          t3 * t9 * 0.0001789373718673827) +
         (t3 * t10 * -0.0001789373718673827 +
          t5 * t8 * 0.00088300008014966186)) +
        (t5 * t9 * 0.0017660001602993239 + t5 * t10 * 0.0017660001602993239)) +
       (t76_tmp * -8.9468685933691362E-5 + t80_tmp * 0.00088300008014966186)) +
      (t68 * t3 * -0.0003578747437347655 + t68 * t5 * 0.0035320003205986479);
};