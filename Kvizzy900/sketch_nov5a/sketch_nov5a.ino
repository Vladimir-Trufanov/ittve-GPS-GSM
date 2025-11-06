

uint32_t glat=52518611, glon=13376111, gcik=67;     // Передаваемые на сайт значения координат 

//ATcom("AT+HTTPPARA=\"URL\",\"http://probatv.ru/State/?cycle=7&num=5&ctrl=204&sjson={%22trkpt%22:{%22lat%22:52518611,%22lon%22:13376111,%22color%22:%22yellow%22}}\"","OK",2500);

/*
char* str1 = R"(AT+HTTPPARA=\"URL\",\"http://probatv.ru/State/?cycle=)"; 
char* str2 = R"(&num=5&ctrl=204&sjson={"trkpt":{"lat":52518611)"; 
char* str3 = R"(,"lon":13376111)";
char* str4 = R"(,%22color%22:%22yellow%22}})"; 
*/

String str1 = R"(AT+HTTPPARA=\"URL\",\"http://probatv.ru/State/?cycle=)"; 
String str2 = R"(&num=5&ctrl=204&sjson={"trkpt":{"lat":)"; 
String str3 = R"(,"lon":)";
String str4 = R"(,"color":"blue"}})"; 

void setup() 
{
  Serial.begin(115200);

// 2025-11-04 вид запроса, введенный вручную 
// http://probatv.ru/State/?cycle=7&num=5&ctrl=204&sjson={"trkpt":{"lat":52518611,"lon":13376111,"color":"yellow"}}
 
  //const char* json = R"({"str":"text","value":123})"; // == {"str":"text","value":123}
  const char* json = R"({"str":"text","value":glat})"; // == {"str":"text","value":123}
  Serial.print("json="); Serial.println(json);

  //char* s = ""; 
  String s = ""; 
  s+=str1;
  s+=gcik;
  s+=str2;
  s+=glat;
  s+=str3;
  s+=glon;
  s+=str4;
  //Serial.print("s="); Serial.println(s);

  // Готовим строку с URL-адресом 
  char url[256];
  memset(url,'\0',256);  
  
  Serial.print("s.length()="); Serial.println(s.length());
  Serial.println(s);
  s.toCharArray(url,s.length()+1);
  Serial.println(url);
}

void loop() {
  // put your main code here, to run repeatedly:

}
