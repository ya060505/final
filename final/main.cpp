#include "mbed.h"

#include "bbcar.h"


Ticker servo_ticker;

Ticker encoder_ticker;

Thread thread2;

PwmOut pin8(D8), pin9(D9);

DigitalIn pin3(D3);

DigitalOut led1(LED1);

DigitalInOut pin10(D10);

Serial pc(USBTX,USBRX); //tx,rx

Serial uart(D1,D0); //tx,rx

RawSerial xbee(D12, D11);

//DigitalIn button(SW2);

EventQueue queue(32 * EVENTS_EVENT_SIZE);

Thread t;

parallax_encoder encoder0(pin3, encoder_ticker);

parallax_ping  ping1(pin10);

volatile int speed = 0;

/*
void log(void);

void xbee_rx_interrupt(void);

void xbee_rx(void);

void reply_messange(char *xbee_reply, char *messange);

void check_addr(char *xbee_reply, char *messenger);
*/

BBCar car(pin8, pin9, servo_ticker);

void recieve_thread(){
   while(1) {
      if(uart.readable()){
            char recv = uart.getc();

            pc.putc(recv);

            pc.printf("\r\n");
      }
   }
}


void send_thread(){
     char s[21];

     sprintf(s,"image_classification");

     uart.puts(s);

     pc.printf("send\r\n");

     wait(0.5);
}


int main() {

    uart.baud(9600);

    // please contruct you own calibration table with each servo

    double pwm_table0[] = {-150, -120, -90, -60, -30, 0, 30, 60, 90, 120, 150};

    double speed_table0[] = {-16.987, -16.508, -15.312, -11.564, -5.263, 0.000, 7.098, 12.760, 15.631, 16.588, 17.067};

    double pwm_table1[] = {-150, -120, -90, -60, -30, 0, 30, 60, 90, 120, 150};

    double speed_table1[] = {-17.385, -16.987, -15.790, -12.361, -6.141, 0.000, 6.460, 12.760, 16.189, 17.146, 17.545};


    // first and fourth argument : length of table

    car.setCalibTable(11, pwm_table0, speed_table0, 11, pwm_table1, speed_table1);
    //car.goStraightCalib(5);
    //car.turn(100, 0.3);//left

//    parallax_encoder encoder0(pin3, encoder_ticker);

//    parallax_ping  ping1(pin10);
/*
    char xbee_reply[4];

    // XBee setting

  xbee.baud(9600);

  xbee.printf("+++");

  xbee_reply[0] = xbee.getc();

  xbee_reply[1] = xbee.getc();

  if(xbee_reply[0] == 'O' && xbee_reply[1] == 'K'){

    pc.printf("enter AT mode.\r\n");

    xbee_reply[0] = '\0';

    xbee_reply[1] = '\0';

  }

  xbee.printf("ATMY 0x240\r\n");

  reply_messange(xbee_reply, "setting MY : 0x240");


  xbee.printf("ATDL 0x140\r\n");

  reply_messange(xbee_reply, "setting DL : 0x140");


  xbee.printf("ATID 0x1\r\n");

  reply_messange(xbee_reply, "setting PAN ID : 0x1");


  xbee.printf("ATWR\r\n");

  reply_messange(xbee_reply, "write config");


  xbee.printf("ATMY\r\n");

  check_addr(xbee_reply, "MY");


  xbee.printf("ATDL\r\n");

  check_addr(xbee_reply, "DL");


  xbee.printf("ATCN\r\n");

  reply_messange(xbee_reply, "exit AT mode");

  xbee.getc();

  // start

  pc.printf("start\r\n");

  t.start(callback(&queue, &EventQueue::dispatch_forever));


  // Setup a serial interrupt function of receiving data from xbee

  xbee.attach(xbee_rx_interrupt, Serial::RxIrq);

  queue.call_every(1000, log);

*/

    for (int i=0; i<4; i++) {
        encoder0.reset();
        car.goStraightCalib(100);
        speed = 100;
        while(encoder0.get_cm()<20) wait_ms(50);
        car.stop();
        speed = 0;
        wait(4);
    }

    car.goStraightCalib(50);
    speed = 50;
    while(1){
         if((float)ping1>20) led1 = 1;
         else{
             led1 = 0;
             car.stop();
             speed = 0;
             break;
         }
         wait(.01);
    }
    wait(1);
    led1 = 1;
    car.goStraightCalib(-50);
    speed = -50;
    wait(1);
    car.turn(100, 0.3);//left
    speed = 100;
    wait(2.5);
    car.stop();
    speed = 0;
    wait(1);

    for (int i=0; i<2; i++) {
        encoder0.reset();
        car.goStraightCalib(100);
        speed = 100;
        while(encoder0.get_cm()<15) wait_ms(50);
        car.stop();
        speed = 0;
        wait(3);
    }

    car.goStraightCalib(50);
    while(1){
         if((float)ping1>20) led1 = 1;
         else{
             led1 = 0;
             car.stop();
             break;
         }
         wait(.01);
    }
    wait(1);
    led1 = 1;

    car.turn(-100, 0.3);//l back
    wait(1.5);
    car.stop();
    wait(1);

    car.goStraightCalib(-50);
    while(1){
         if((float)ping1<40) led1 = 1;
         else{
             led1 = 0;
             car.stop();
             break;
         }
         wait(.01);
    }

    wait(1);
    led1 = 1;/////

    car.goStraightCalib(50);
    while(1){
         if((float)ping1>30) led1 = 1;
         else{
             led1 = 0;
             car.stop();
             break;
         }
         wait(.01);
    }
    wait(1);
    led1 = 1;/////

    car.turn(100, -0.3);//r
    wait(2);
    car.stop();
    wait(2);

    encoder0.reset();
    car.goStraightCalib(100);
    while(encoder0.get_cm()<15) wait_ms(50);
    car.stop();
    wait(2);

    car.turn(-100, -0.3);//r back
    wait(2);
    car.stop();
    
    char s[6];
    sprintf(s,"photo");
    uart.puts(s);
    //xbee.printf("take photo\n");
    wait(2.2);

    send_thread();
    //xbee.printf("image_classification\n");
    thread2.start(recieve_thread);

    car.turn(100, -0.3);//r
    wait(1.5);
    car.stop();
    wait(1);

    encoder0.reset();
    car.goStraightCalib(100);
    while(encoder0.get_cm()<25) wait_ms(50);
    car.stop();
    wait(3);

    car.turn(100, -0.3);//r
    wait(1.5);
    car.stop();
    wait(1);

    for (int i=0; i<4; i++) {
        encoder0.reset();
        car.goStraightCalib(100);
        speed = 100;
        while(encoder0.get_cm()<20) wait_ms(50);
        car.stop();
        speed = 0;
        wait(4);
    }

    car.goStraightCalib(50);
    speed = 50;
    while(1){
         if((float)ping1>20) led1 = 1;
         else{
             led1 = 0;
             car.stop();
             speed = 0;
             break;
         }
         wait(.01);
    }

    wait(1);
    led1 = 1;
    car.goStraightCalib(-50);
    speed = -50;
    wait(1);
    car.turn(100, -0.3);//right
    speed = 100;
    wait(2.5);
    car.stop();
    speed = 0;
    wait(1);
}

/*
void xbee_rx_interrupt(void)

{

  xbee.attach(NULL, Serial::RxIrq); // detach interrupt

  queue.call(&xbee_rx);

}


void xbee_rx(void)

{

  static int i = 0;

  static char buf[100] = {0};

  while(xbee.readable()){

    char c = xbee.getc();

    if(c!='\r' && c!='\n'){

      buf[i] = c;

      i++;

      buf[i] = '\0';

    }else{

      i = 0;

      pc.printf("Get: %s\r\n", buf);

      xbee.printf("%s", buf);

    }

  }

  wait(0.1);

  xbee.attach(xbee_rx_interrupt, Serial::RxIrq); // reattach interrupt

}


void reply_messange(char *xbee_reply, char *messange){

  xbee_reply[0] = xbee.getc();

  xbee_reply[1] = xbee.getc();

  xbee_reply[2] = xbee.getc();

  if(xbee_reply[1] == 'O' && xbee_reply[2] == 'K'){

    pc.printf("%s\r\n", messange);

    xbee_reply[0] = '\0';

    xbee_reply[1] = '\0';

    xbee_reply[2] = '\0';

  }

}


void check_addr(char *xbee_reply, char *messenger){

  xbee_reply[0] = xbee.getc();

  xbee_reply[1] = xbee.getc();

  xbee_reply[2] = xbee.getc();

  xbee_reply[3] = xbee.getc();

  pc.printf("%s = %c%c%c\r\n", messenger, xbee_reply[1], xbee_reply[2], xbee_reply[3]);

  xbee_reply[0] = '\0';

  xbee_reply[1] = '\0';

  xbee_reply[2] = '\0';

  xbee_reply[3] = '\0';

}

void log(void) {
    //xbee.printf("Ping=%1.2f, Speed=%d\n", (float)ping1, speed);
}
*/