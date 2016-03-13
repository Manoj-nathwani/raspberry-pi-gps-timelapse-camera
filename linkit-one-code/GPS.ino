// taken from https://www.hackster.io/Momy93/gps-datalogger-with-linkit-one-fe6541
#include <LGPS.h>

gpsSentenceInfoStruct info;

double latitude = 0.00;
double longitude = 0.00;
int sat_num = 0; //number of visible satellites
String lat_format = "0.00000", lon_format = "0.00000";

void setup()
{
  Serial.begin(115200);
  LGPS.powerOn();
}

void loop()
{
  if (getData(&info) > 3)
  {
    Serial.println(lat_format + "," + lon_format);
  }  
  delay(3000);
}

/**
*Converts degrees from (d)ddmm.mmmm to (d)dd.mmmmmm
*@param str the string rappresentation of the angle in (d)ddmm.mmmm format
*@param dir if true the direction is south, and the angle is negative.
*@return the given angle in dd.mmmmmm format.
*/
float convert(String str, boolean dir)
{
  double mm, dd;
  int point = str.indexOf('.');
  dd = str.substring(0, (point - 2)).toFloat();
  mm = str.substring(point - 2).toFloat() / 60.00;
  return (dir ? -1 : 1) * (dd + mm);
}

/**
*Gets gps informations
*@param info gpsSentenceInfoStruct is a struct containing NMEA sentence infomation
*@return the number of hooked satellites, or 0 if there was an error getting informations
*/
int getData(gpsSentenceInfoStruct* info)
{  
  LGPS.getData(info);  
  if (info->GPGGA[0] == '$')
  {
    String str = (char*)(info->GPGGA);
    str = str.substring(str.indexOf(',') + 1);    
    str = str.substring(str.indexOf(',') + 1);
    latitude = convert(str.substring(0, str.indexOf(',')), str.charAt(str.indexOf(',') + 1) == 'S');
    int val = latitude * 1000000;
    String s = String(val);
    lat_format = s.substring(0, (abs(latitude) < 100) ? 2 : 3);
    lat_format += '.';
    lat_format += s.substring((abs(latitude) < 100) ? 2 : 3);
    str = str.substring(str.indexOf(',') + 3);
    longitude = convert(str.substring(0, str.indexOf(',')), str.charAt(str.indexOf(',') + 1) == 'W');
    val = longitude * 1000000;
    s = String(val);
    lon_format = s.substring(0, (abs(longitude) < 100) ? 2 : 3);
    lon_format += '.';
    lon_format += s.substring((abs(longitude) < 100) ? 2 : 3);
    str = str.substring(str.indexOf(',') + 3);
    str = str.substring(2);
    sat_num = str.substring(0, 2).toInt();  
    return sat_num;    
  }
  return 0;
}
