#include <Wire.h>
#include <DS3231.h>;
DS3231 Clock;
#define Down A1
#define Up A2
#define Mode A3
int sec;
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
  Serial.begin(9600);
  sec=-1;
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

}

void loop() {
  //Serial.print(Clock.getDate());
  int i;
  if (mode==0)
  {
    if(sec!=Clock.getSecond())
    {
      switch(Clock.getDoW())
      {
        case 1:Serial.print("Sunday");
                break;
        case 2:Serial.print("Monday");
                break;
        case 3:Serial.print("Tuesday");
                break;
        case 4:Serial.print("Wednesday");
                break;
        case 5:Serial.print("Thursday");
                break;
        case 6:Serial.print("Friday");
                break;
        case 7:Serial.print("Saturday");
                break;
      }
      Serial.print(" ");
      Serial.print(Clock.getHour(h12,PM));
      Serial.print(":");
      Serial.print(Clock.getMinute());
      Serial.print(":");
      sec=Clock.getSecond();
      Serial.print(sec);
      Serial.print(" ");
      Serial.print(Clock.getDate());
      Serial.print("/");
      Serial.print(Clock.getMonth(Century));
      Serial.print("/");
      Serial.println(Clock.getYear()+2000);
    }
    for(i=0;i<2;i++)
    {
      if(al[i].set&&(checkal(i)||al[i].active))
      alarm(i);
    }
    
  }
  else if(mode==1)
  {
    set();
  }
  else if(mode==2)
    alset();
  if(mode==0&&analogRead(A3)>=900)
  {
    delay(1000);
    mode=1;
    Serial.print("Mode:");
    Serial.println(mode);
  }
  if(mode==0&&analogRead(A0)>=800)
  {
    delay(1000);
    mode=2;
    Serial.print("Mode:");
    Serial.println(mode);
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
  Serial.println("Alarm");
  if(analogRead(A0)>=800)
  {
    delay(1000);
    analogRead(A0);
    al[i].active=false;
    al[i].minute+=5;
    if(al[i].minute>=60)
    {
      al[i].minute-=60;
      al[i].hour++;
    }
  }
  if((analogRead(A3)>=900)||(((al[i].minute+1)==60)?0:al[i].minute+1==Clock.getMinute()))
  {   
      delay(1000);
      analogRead(A3);
      al[i].active=false;
      al[i].set=false;
  }
}
void set()
{
  static int mode=0,h=Clock.getHour(h12,PM),m=Clock.getMinute();
  if(mode==0)
  {
    Serial.print("Hour: ");
    Serial.println(h);
    if(analogRead(A2)>=800)
    {
      delay(1000);
      h++;
      if(h==24)
        h=0;
    }
    else if(analogRead(A1)>=800)
    {
      delay(1000);
      h--;
      if(h==-1)
        h=23;
    }
  }
  else if(mode==1)
  {
    Serial.print("Minute: ");
    Serial.println(m);
    if(analogRead(A2)>=800)
    {
      delay(1000);
      m++;
      if(m==60)
        m=0;
    }
    else if(analogRead(A1)>=800)
    {
      delay(1000);
      m--;
      if(m==-1)
        m=59;
    }
  }
  if(analogRead(A3)>=900)
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
      Serial.println(::mode);
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
    Serial.print(a);
    Serial.print("AlHour: ");
    Serial.println(h);
    if(analogRead(A2)>=800)
    {
      delay(1000);
      h++;
      if(h==24)
        h=0;
    }
    else if(analogRead(A1)>=800)
    {
      delay(1000);
      h--;
      if(h==-1)
        h=23;
    }
  }
  else if(mode==1)
  {
    Serial.print(a);
    Serial.print("AlMinute: ");
    Serial.println(m);
    if(analogRead(A2)>=800)
    {
      delay(1000);
      m++;
      if(m==60)
        m=0;
    }
    else if(analogRead(A1)>=800)
    {
      delay(1000);
      m--;
      if(m==-1)
        m=59;
    }
  }
  if(analogRead(A3)>=800)
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
      Serial.println(::mode);
      delay(1000);
      return; 
    }
  }
  if(analogRead(A0)>=800)
  {
      delay(1000);
      mode=0;
      a++;
      if(a==2)
        a=0;
  }
}
