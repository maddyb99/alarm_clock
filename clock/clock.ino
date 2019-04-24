#include <Wire.h>
#include <DS3231.h>;
DS3231 Clock;
int sec;
struct Alarm{
  int hour,minute,second;
  bool active;
};
struct Alarm al1,al2;
byte year,month,date,hour,minute,second,DoW;
bool Century=false,h12,PM;
void setup() {
  Wire.begin();
  Serial.begin(9600);
  sec=-1;
  al1.hour=0;
  al1.minute=1;
  al1.second=10;
  al1.active=false;
  al2.hour=0;
  al2.minute=1;
  al2.second=45;
  al2.active=false;
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
  

}

void loop() {
  //Serial.print(Clock.getDate());
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
  if(checkal(&al1)||al1.active)
    alarm(&al1);
  else if(checkal(&al2)||al2.active)
    alarm(&al2);
}

bool checkal(struct Alarm *al)
{
  if((*al).hour!=Clock.getHour(h12,PM))
    return false;
  else if((*al).minute!=Clock.getMinute())
    return false;
  else if((*al).second!=Clock.getSecond())
    return false;
  (*al).active=true;
  return true;
}
void alarm(struct Alarm (*al))
{
  Serial.println("Alarm");
  if((((*al).second+30)>60?(*al).second-30:(*al).second+30)==Clock.getSecond())
     (*al).active=false;
}
