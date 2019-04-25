#include <Wire.h>
#include <DS3231.h>;
DS3231 Clock;
#define Down A1
#define Up A2
#define Mode A3
#define AlMode A0
#define pinA 2
#define pinB 3
#define pinC 4
#define pinD 5
#define pinE 6
#define pinF 7
#define pinG 8
#define C1 9
#define C2 10
#define C3 11
#define C4 12
int sec,hr;
struct Alarm{
  int hour,minute;
  bool active,set;
};
struct Alarm al[2];
byte year,month,date,hour,minute,second,DoW;
bool Century=false,h12,PM;
int mode;
void setup() {
  Wire.begin();
  //Serial.begin(9600);
  mode=0;
  al[1].hour=0;
  al[1].minute=1;
  al[1].active=false;
  al[0].hour=0;
  al[0].minute=2;
  al[0].active=false;
  al[0].set=false;
  al[1].set=false;
  year=(byte)19;
  month=(byte)1;
  date=(byte)1;
  hour=(byte)0;
  minute=(byte)0;
  second=(byte)0;
  DoW=(byte)3;
 Clock.setYear(year);
 Clock.setMonth(month);
 Clock.setHour(hour);
 Clock.setMinute(minute);
 Clock.setSecond(second);
 Clock.setDate(date);
 Clock.setDoW(DoW);
  pinMode(mode,INPUT);
  pinMode(Up,INPUT);
  pinMode(Down,INPUT);
  pinMode(pinA,OUTPUT);
  pinMode(pinB,OUTPUT);
  pinMode(pinC,OUTPUT);
  pinMode(pinD,OUTPUT);
  pinMode(pinE,OUTPUT);
  pinMode(pinF,OUTPUT);
  pinMode(pinG,OUTPUT);
  pinMode(C4,OUTPUT);
  pinMode(C3,OUTPUT);
  pinMode(C2,OUTPUT);
  pinMode(C1,OUTPUT);
  pinMode(13,OUTPUT);

}

void loop() {
  //Serial.print(Clock.getDate());
  bool dot=true;
  int i,sec=-1,hr=-1,m=-1;
  if (mode==0)
  {
    if(sec!=Clock.getSecond())
    {
      /*Serial.print(Clock.getHour(h12,PM));
      Serial.print(":");
      Serial.print(Clock.getMinute());
      Serial.print(":");*/
      sec=Clock.getSecond();
      /*Serial.print(sec);
      Serial.print(" ");
      Serial.print(Clock.getDate());
      Serial.print("/");
      Serial.print(Clock.getMonth(Century));
      Serial.print("/");
      Serial.println(Clock.getYear()+2000);*/
    }
    if(hr!=Clock.getHour(h12,PM))
      hr=Clock.getHour(h12,PM);
    if(m!=Clock.getMinute())
      m=Clock.getMinute();
    for(i=0;i<2;i++)
    {
      if(al[i].set&&(checkal(i)||al[i].active))
      alarm(i);
    }
    if(sec%2)
    {
      digitalWrite(13,HIGH);
    }
    else
      digitalWrite(13,LOW);
    disp(hr,1);
    disp(m,3);
  }
  else if(mode==1)
  {
    set();
  }
  else if(mode==2)
    alset();
  if(mode==0&&analogRead(Mode)>=900)
  {
    delay(1000);
    mode=1;
    //Serial.print("Mode:");
    //Serial.println(mode);
  }
  if(mode==0&&analogRead(AlMode)>=800)
  {
    delay(1000);
    mode=2;
    //Serial.print("Mode:");
    //Serial.println(mode);
  }
}

bool checkal(int i)
{
  if(al[i].hour!=Clock.getHour(h12,PM))
    return false;
  else if(al[i].minute!=Clock.getMinute())
    return false;
  al[i].active=true;
  return true;
}
void alarm(int i)
{
  //Serial.println("Alarm");
    if(Clock.getSecond()%2){
  dispal();
  disp(i,3);
    }
    else
    disp(0,4);
  if(analogRead(AlMode)>=800)
  {
    delay(1000);
    analogRead(AlMode);
    al[i].active=false;
    al[i].minute+=5;
    if(al[i].minute>=60)
    {
      al[i].minute-=60;
      al[i].hour++;
    }
  }
  if((analogRead(Mode)>=900)||(((al[i].minute+1)==60)?0:al[i].minute+1==Clock.getMinute()))
  {   
      delay(1000);
      analogRead(Mode);
      al[i].active=false;
      al[i].set=false;
  }
}
void set()
{
  static int mode=0,h=Clock.getHour(h12,PM),m=Clock.getMinute();
  if(mode==0)
  {
    //Serial.print("Hour: ");
    disp(m,3);
    if(Clock.getSecond()%2)
    disp(h,1);
    else
    disp(0,4);
    //Serial.println(h);
    if(analogRead(Up)>=800)
    {
      delay(1000);
      h++;
      if(h==24)
        h=0;
    }
    else if(analogRead(Down)>=800)
    {
      delay(1000);
      h--;
      if(h==-1)
        h=23;
    }
  }
  else if(mode==1)
  {
    //Serial.print("Minute: ");
    //Serial.println(m);
    disp(h,1);
    if(Clock.getSecond()%2)
    disp(m,3);
    else
    disp(0,4);
    if(analogRead(Up)>=800)
    {
      delay(1000);
      m++;
      if(m==60)
        m=0;
    }
    else if(analogRead(Down)>=800)
    {
      delay(1000);
      m--;
      if(m==-1)
        m=59;
    }
  }
  if(analogRead(Mode)>=900)
  {
    delay(1000);
    if(mode==0)
    {
      hour=(byte)h;
      Clock.setHour(hour);
      mode++;
    }
    else if(mode==1)
    {
      minute=(byte)m;
      second=(byte)0;
      Clock.setMinute(minute);
      Clock.setSecond(second);
      mode--;
      ::mode=0;
      //Serial.println(::mode);
      delay(1000);
      return;
    }
  }
}
void alset()
{
  static int mode=0,a=0,h=al[a].hour,m=al[a].minute;
  if(mode==2)
    mode=0;
  if(mode==0)
  {
    //Serial.print(a);
    //Serial.print("AlHour: ");
    //Serial.println(h);
    disp(m,3);
    if(Clock.getSecond()%2)
    disp(h,1);
    else
    disp(0,4);
    if(analogRead(Up)>=800)
    {
      delay(1000);
      h++;
      if(h==24)
        h=0;
    }
    else if(analogRead(Down)>=800)
    {
      delay(1000);
      h--;
      if(h==-1)
        h=23;
    }
  }
  else if(mode==1)
  {
    //Serial.print(a);
    //Serial.print("AlMinute: ");
    //Serial.println(m);
    disp(h,1);
    if(Clock.getSecond()%2)
    disp(m,3);
    else
    disp(0,4);
    if(analogRead(Up)>=800)
    {
      delay(1000);
      m++;
      if(m==60)
        m=0;
    }
    else if(analogRead(Down)>=800)
    {
      delay(1000);
      m--;
      if(m==-1)
        m=59;
    }
  }
  if(analogRead(Mode)>=800)
  {
    delay(1000);
    if(mode==0)
    {
      al[a].hour=h;
      mode++;
    }
    else if(mode==1)
    {
      al[a].minute=m;
      al[a].set=true;
      mode++;
      ::mode=0;
      //Serial.println(::mode);
      delay(1000);
      return; 
    }
  }
  if(analogRead(AlMode)>=800)
  {
      delay(1000);
      mode=0;
      a++;
      if(a==2)
        a=0;
  }
}

void disp(int num,int dig)
{
  if(num>9)
  {
  disp(num/10,dig-1);
  num=num%10;
  }
  else if(dig%2)
    disp(0,dig-1);
  digitalWrite(C1, LOW);
  digitalWrite(C2, LOW);
  digitalWrite(C3, LOW);
  digitalWrite(C4, LOW);
  if(num==0)
  {
    digitalWrite(pinA, LOW);   
    digitalWrite(pinB, LOW);   
    digitalWrite(pinC, LOW);   
    digitalWrite(pinD, LOW);   
    digitalWrite(pinE, LOW);   
    digitalWrite(pinF, LOW);   
    digitalWrite(pinG, HIGH);  
  }
  else if(num==1)
  {
    digitalWrite(pinA, HIGH);   
    digitalWrite(pinB, LOW);   
    digitalWrite(pinC, LOW);   
    digitalWrite(pinD, HIGH);   
    digitalWrite(pinE, HIGH);   
    digitalWrite(pinF, HIGH);   
    digitalWrite(pinG, HIGH); 
  }
  else if(num==2)
  {
    digitalWrite(pinC, HIGH);   
  digitalWrite(pinE, LOW);   
  digitalWrite(pinA, LOW);   
  digitalWrite(pinD, LOW);   
  digitalWrite(pinB, LOW);   
  digitalWrite(pinF, HIGH);   
  digitalWrite(pinG, LOW);  
  }
  else if(num==3)
  {
      digitalWrite(pinA, LOW);   
  digitalWrite(pinB, LOW);   
  digitalWrite(pinC, LOW);   
  digitalWrite(pinD, LOW);   
  digitalWrite(pinE, HIGH);   
  digitalWrite(pinF, HIGH);   
  digitalWrite(pinG, LOW);
  }
  else if(num==4)
  {
    digitalWrite(pinA, HIGH);   
  digitalWrite(pinB, LOW);   
  digitalWrite(pinC, LOW);   
  digitalWrite(pinD, HIGH);   
  digitalWrite(pinE, HIGH);   
  digitalWrite(pinF, LOW);   
  digitalWrite(pinG, LOW);
  }
  else if(num==5)
  {
      digitalWrite(pinA, LOW);   
  digitalWrite(pinB, HIGH);   
  digitalWrite(pinC, LOW);   
  digitalWrite(pinD, LOW);   
  digitalWrite(pinE, HIGH);   
  digitalWrite(pinF, LOW);   
  digitalWrite(pinG, LOW);  
  }
  else if(num==6)
  {
      digitalWrite(pinA, LOW);   
  digitalWrite(pinB, HIGH);   
  digitalWrite(pinC, LOW);   
  digitalWrite(pinD, LOW);   
  digitalWrite(pinE, LOW);   
  digitalWrite(pinF, LOW);   
  digitalWrite(pinG, LOW);  
  }
  else if(num==7)
  {
      digitalWrite(pinA, LOW);   
  digitalWrite(pinB, LOW);   
  digitalWrite(pinC, LOW);   
  digitalWrite(pinD, HIGH);   
  digitalWrite(pinE, HIGH);   
  digitalWrite(pinF, HIGH);   
  digitalWrite(pinG, HIGH);  
  }
  else if(num==8)
  {
      digitalWrite(pinA, LOW);   
  digitalWrite(pinB, LOW);   
  digitalWrite(pinC, LOW);   
  digitalWrite(pinD, LOW);   
  digitalWrite(pinE, LOW);   
  digitalWrite(pinF, LOW);   
  digitalWrite(pinG, LOW); 
  }
  else if(num==9)
  {
      digitalWrite(pinA, LOW);   
  digitalWrite(pinB, LOW);   
  digitalWrite(pinC, LOW);   
  digitalWrite(pinD, HIGH);   
  digitalWrite(pinE, HIGH);   
  digitalWrite(pinF, LOW);   
  digitalWrite(pinG, LOW);
  }
  if(dig==0)
      digitalWrite(C1, HIGH);
  else if(dig==1)
      digitalWrite(C2, HIGH);
  else if(dig==2)
      digitalWrite(C3, HIGH);
  else if(dig==3)
      digitalWrite(C4, HIGH);
  else
  {
  digitalWrite(C1, LOW);
  digitalWrite(C2, LOW);
  digitalWrite(C3, LOW);
  digitalWrite(C4, LOW);
    
  }
}

void dispal()
{
  digitalWrite(C2, LOW);
  digitalWrite(C3, LOW);
  digitalWrite(C4, LOW);
  digitalWrite(C1, HIGH);
  digitalWrite(pinA, LOW);   
  digitalWrite(pinB, LOW);   
  digitalWrite(pinC, LOW);   
  digitalWrite(pinD, HIGH);   
  digitalWrite(pinE, LOW);   
  digitalWrite(pinF, LOW);   
  digitalWrite(pinG, LOW);
  digitalWrite(C1, LOW);
  digitalWrite(C2, HIGH);
  digitalWrite(pinA, HIGH);   
  digitalWrite(pinB, HIGH);   
  digitalWrite(pinC, HIGH);   
  digitalWrite(pinD, LOW);   
  digitalWrite(pinE, LOW);   
  digitalWrite(pinF, LOW);   
  digitalWrite(pinG, HIGH);
}
