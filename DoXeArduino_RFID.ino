
#include <Arduino.h>
#include <AT24Cxx.h>
#include <LiquidCrystal.h>
#define i2c_address 0x50
#include "BasicStepperDriver.h"
#include <SPI.h>
#include <MFRC522.h> // thu vien "RFID".
AT24Cxx eep(i2c_address, 512);  // thay 512 bằng xx sau 24Cxx
String content= "";
/*
Ket noi voi Arduino Uno hoac Mega
 ----------------------------------------------------- Nicola Coppola
 * Pin layout should be as follows:
 * Signal     Pin              Pin               Pin
 *            Arduino Uno      Arduino Mega      MFRC522 board
 * ------------------------------------------------------------
 * Reset      9                5                 RST
 * SPI SS     10               53                SDA
 * SPI MOSI   11               51                MOSI
 * SPI MISO   12               50                MISO
 * SPI SCK    13               52                SCK
 
 */
#define SS_PIN 10
#define RST_PIN 9

#define MOTOR_STEPS 200
#define RPM 50

#define MICROSTEPS 1

#define DIR 2
#define STEP 5
#define ENABLE 8 // optional (just delete ENABLE from everywhere if not used)

#define ty_le 10 // tỷ số truyền

BasicStepperDriver stepper(MOTOR_STEPS, DIR, STEP);
MFRC522 mfrc522(SS_PIN, RST_PIN); 
LiquidCrystal lcd(A0,A1,A2,A3,3,4);

byte data[4];


String tag1 = "6e-b-79-25"; // các ID của thẻ bạn dùng example rc522 của arduino đọc ra rồi ghi lại vào đây
String tag2 = "4-4c-79-25";
String tag3 = "5-56-79-25";
String tag4 = "3d-56-78-25";
String tag5 = "ac-24-79-25";
String tag6 = "e3-b2-78-25";
String tag7 = "db-d9-77-25";
String tag8 = "cb-4d-79-25";

boolean slot1 = 0 ,slot2 = 0 ,slot3 = 0 ,slot4 = 0 ,slot5 = 0 ,slot6 = 0 ,slot7 = 0 ,slot8 = 0 ;



int32_t vi_tri_hien_tai, vi_tri=0;
int32_t goc_phai_xoay=0;
int32_t tag_value;
int32_t num_tag;

void write_slot() // lưu vị trạng thái các ô để xe
{
  eep.write(5,slot1);
  eep.write(6,slot2);
  eep.write(7,slot3);
  eep.write(8,slot4);
  eep.write(9,slot5);
  eep.write(10,slot6);
  eep.write(11,slot7);
  eep.write(12,slot8);
}

void read_slot() // đọc 
{
  slot1 = eep.read(5);
  if(slot1>1) slot1=1;
  slot2 = eep.read(6);
  if(slot2>1) slot2=1;
  slot3 = eep.read(7);
  if(slot3>1) slot3=1;
  slot4 = eep.read(8);
  if(slot4>1) slot4=1;
  slot5 = eep.read(9);
  if(slot5>1) slot5=1;
  slot6 = eep.read(10);
  if(slot6>1) slot6=1;
  slot7 = eep.read(11);
  if(slot7>1) slot7=1;
  slot8 = eep.read(12);
  if(slot8>1) slot8=1;
  
}

void write_eeprom_pos() // lưu góc hiện tại
{
  eep.write(1,vi_tri_hien_tai/100);
  eep.write(2,vi_tri_hien_tai%100/10);
  eep.write(3,vi_tri_hien_tai%10);
}
void read_eeprom_pos()
{
  int a,b,c;
  a = eep.read(1);
  b = eep.read(2);
  c = eep.read(3);
  vi_tri_hien_tai = a*100 + b*10 + c;
}

void slot_car_stt() /// hiển thị trạng thái các ô để xe
{
  if(slot1!=0)
  {
   lcd.setCursor(7, 1);
   lcd.print("x"); 
  }
  else if(slot1==0)
  {
   lcd.setCursor(7, 1);
   lcd.print("_"); 
  }
  
  if(slot2==true)
  {
   lcd.print("x"); 
  }
  else if(slot2==false)
  {
   lcd.print("_"); 
  }
  
  if(slot3==true)
  {
   lcd.print("x"); 
  }
  else if(slot3==false)
  {
   lcd.print("_"); 
  }
  
  if(slot4==true)
  {
   lcd.print("x"); 
  }
  else if(slot4==false)
  {
   lcd.print("_"); 
  }
  
  if(slot5==true)
  {
   lcd.print("x"); 
  }
  else if(slot5==false)
  {
   lcd.print("_"); 
  }
  
  if(slot6==true)
  {
   lcd.print("x"); 
  }
  else if(slot6==false)
  {
   lcd.print("_"); 
  }
  
  if(slot7==true)
  {
   lcd.print("x"); 
  }
  else if (slot7==false)
  {
   lcd.print("_"); 
  }
  
  if(slot8==true)
  {
   lcd.print("x"); 
  }
  else if(slot8==false)
  {
   lcd.print("_"); 
  }
}


void dieu_khien_step_theo_the(int num_tag) // điều khiển step quay theo thẻ 
{
 read_eeprom_pos();
 if(num_tag==1)
 {
  slot1 = not slot1;
  vi_tri = 0-vi_tri_hien_tai;
  stepper.rotate(vi_tri); // vị trí được tính bằng độ (degrees), đặt bao nhiêu độ thì nó quay bấy nhiêu
  vi_tri_hien_tai = 0;
 }
 else if (num_tag==2)
 {
  slot2 = not slot2;
  vi_tri = 45*ty_le-vi_tri_hien_tai;
  stepper.rotate(vi_tri);
  vi_tri_hien_tai = 45*ty_le;
 }
 else if (num_tag==3)
 {
  slot3 = not slot3;
  vi_tri = 90*ty_le-vi_tri_hien_tai;
  stepper.rotate(vi_tri);
  vi_tri_hien_tai = 90*ty_le;
 }
 else if (num_tag==4)
 {
  slot4 = not slot4;
  vi_tri = 135*ty_le-vi_tri_hien_tai;
  stepper.rotate(vi_tri);
  vi_tri_hien_tai = 135*ty_le;
 }
 else if (num_tag==5)
 {
  slot5 = not slot5;
  vi_tri = 180*ty_le-vi_tri_hien_tai;
  stepper.rotate(vi_tri);
  vi_tri_hien_tai = 180*ty_le;
 }
 else if (num_tag==6)
 {
  slot6 = not slot6;
  vi_tri = 225*ty_le-vi_tri_hien_tai;
  stepper.rotate(vi_tri);
  vi_tri_hien_tai = 225*ty_le;
 }
 else if (num_tag==7)
 {
  slot7 = not slot7;
  vi_tri = 270*ty_le-vi_tri_hien_tai;
  stepper.rotate(vi_tri);
  vi_tri_hien_tai = 270*ty_le;
 }
 else if (num_tag==8)
 {
  slot8 = not slot8;
  vi_tri = 315*ty_le-vi_tri_hien_tai;
  stepper.rotate(vi_tri);
  vi_tri_hien_tai = 315*ty_le;
 }
 write_eeprom_pos();
 write_slot();
}


void check_tag() // đọc thẻ
{
  if( content == tag1 )
  {
    Serial.println("tag1");
    num_tag=1;
  }
  else if( content == tag2 )
  {
    Serial.println("tag2");
    num_tag=2;
  }
  else if( content == tag3 )
  {
    Serial.println("tag3");
    num_tag=3;
  }
  else if( content == tag4 )
  {
    Serial.println("tag4");
    num_tag=4;
  }
  else if( content == tag5 )
  {
    Serial.println("tag5");
    num_tag=5;
  }
  else if( content == tag6 )
  {
    Serial.println("tag6");
    num_tag=6;
  }
  else if( content == tag7 )
  {
    Serial.println("tag7");
    num_tag=7;
  }
  else if( content == tag8 )
  {
    Serial.println("tag8");
    num_tag=8;
  }
  else
  {
    Serial.println("null");
    num_tag=0;
  }
  content= "";
}



void setup() {
  
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("Do Xe");
  lcd.setCursor(7, 0);
  lcd.print("12345678");
  read_eeprom_pos();
  read_slot();
  stepper.begin(RPM, MICROSTEPS);
  SPI.begin();            
  mfrc522.PCD_Init();
  Serial.begin(9600);   
  Serial.println(vi_tri_hien_tai);
}

void loop() {
   //stepper.rotate(-360);
   //int value = eep.read(1);
   lcd.setCursor(0, 1);
   lcd.print("XE:");
   lcd.setCursor(4, 1);
   lcd.print(num_tag);
   slot_car_stt();
   /// tìm thẻ mới
   if ( ! mfrc522.PICC_IsNewCardPresent()) 
   {
    return;
   }
   // Đọc thẻ
   if ( ! mfrc522.PICC_ReadCardSerial()) 
   {
    return;
   }
   //

   byte letter;
   for( byte i = 0; i < mfrc522.uid.size; i++ )
   {
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
     if( i < mfrc522.uid.size-1 ) content+="-";
   }
   
   Serial.println(content);
   check_tag();
   dieu_khien_step_theo_the(num_tag);
   ///lcd.setCursor(7,1);
   //Serial.println(vi_tri_hien_tai);
   Serial.println(slot1);
   mfrc522.PICC_HaltA();

   // Stop encryption on PCD
   mfrc522.PCD_StopCrypto1();
 

}
