void sendData(float th_tem, float env_tem, int env_hum) {
  Serial.println("==========");
  Serial.print("connecting to ");
  Serial.println(host);

    //----------------------------------------Connect to Google host
    int r=0;  // 尝试连接服务器并等待
    while((!client.connect(host, httpsPort)) && (r < 30)){
        delay(100);
        Serial.print(".");
        r++;
    }
    // 连接超时后输出"连接失败"信息并返回
    if(r==30) { 
      Serial.println("Connection failed");
      return;
    } 
    else { // 连接成功则输出“连接成功”信息
      Serial.println("Connected...");
    }
    //----------------------------------------Processing data and sending data
    String url = "/macros/s/" + GOOGLE_SCRIPT_ID +
                "/exec?temperature=" + String(th_tem) +
                "&env_temperature=" + String(env_tem) +
                "&env_humidity=" + String(env_hum);
    String request = String("GET ") + url + " HTTP/1.1\r\n" +
          "Host: " + host + "\r\n" +
          "Connection: close\r\n\r\n";
    Serial.println("requesting info: ");
    Serial.println(request);
    // 发送 GET 请求
    client.print(request);
    Serial.println("request sent");
    
    //----------------------------------------Checking whether the data was sent successfully or not

    // 等待服务器响应
    while (client.connected()) {
      String line = client.readStringUntil('\n');
      if (line == "\r") {
        Serial.println("Headers received");
        break;
      }
    }
    // 读取和处理服务器响应的其余部分
    while (client.available()) {
      char c = client.read();
      // 处理服务器响应
      Serial.print(c);
    }
    String line = client.readStringUntil('\n');
    Serial.println(line);
    if (line.startsWith("{\"state\":\"success\"")) {
      Serial.println("esp8266/Arduino CI successfull!");
    } else {
      Serial.println("esp8266/Arduino CI has failed");
    }

    Serial.println("closing connection");
    Serial.println("==========");
    Serial.println();
    client.stop();

} 