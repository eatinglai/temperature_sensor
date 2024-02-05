void Getting_Sensor_Data(float &th_tem, float &env_tem, int &env_hum) {
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  env_hum = dht.readHumidity();
  // Read temperature as Celsius (the default)
  env_tem = dht.readTemperature();
  th_tem = thermocouple.readCelsius();
  
  // Check if any reads failed and exit early (to try again).
  if (isnan(env_hum) || isnan(env_tem) || isnan(th_tem)) {
    Serial.println("Failed to read from sensor !");
    delay(5000);
    return;
  }

  Serial.println("ENV_Temperature : " + String(env_tem) + " °C");
  Serial.println("ENV_Humidity : " + String(env_hum) + " %");
  Serial.println("TH_Temperature : " + String(th_tem) + " °C");
  delay(100);
  
  //OLED_DISPLAY
  display.clearDisplay();//clear display
  //
  display.setTextSize(1);
  display.setCursor(0,10);
  display.println("Temp: ");
  display.setTextSize(2);
  display.setCursor(38,10);
  display.print(th_tem);
  display.print(" ");
  display.setTextSize(1);
  display.print("C"); 

  display.setTextSize(1);
  display.setCursor(0,20);
  display.println("ENV_");
  display.setCursor(0,30);
  display.println("Temp: ");
  display.setTextSize(2);
  display.setCursor(38,30);
  display.print(env_tem);
  display.print(" ");
  display.setTextSize(1);
  display.print("C");

  display.setTextSize(1);
  display.setCursor(0,40);
  display.println("ENV_");
  display.setCursor(0,50);
  display.println("Humi: ");
  display.setTextSize(2);
  display.setCursor(38,50);
  display.print(env_hum);
  display.print(" ");
  display.setTextSize(1);
  display.print("%");
  display.display(); 
}