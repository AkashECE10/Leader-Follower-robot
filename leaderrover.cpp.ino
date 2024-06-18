#include <M5StickCPlus2.h>
#include <M5_RoverC.h>
#include <BluetoothSerial.h>
#include <WiFi.h>

M5_RoverC roverc;
BluetoothSerial SerialBT;
WiFiServer wifiServer(80); 
void setup() {
    M5.begin();
    Serial.begin(115200); // Serial communication for debugging

    // Initialize Bluetooth
    if (!SerialBT.begin("M5rover")) {
        Serial.println("An error occurred initializing Bluetooth");
    } else {
        Serial.println("Bluetooth initialized");
    }

    // Initialize WiFi in AP mode
    const char *ssid = "M5AP";
    const char *password = "77777777";
    WiFi.softAP(ssid, password);
    IPAddress myIP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(myIP);

    // Start the WiFi server
    wifiServer.begin();
    Serial.println("WiFi server started");

    // Display instructions on the screen
    M5.Lcd.setRotation(1);
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.setTextSize(2);
    M5.Lcd.setCursor(0, 0);
    M5.Lcd.println("Send commands via BT");

    // Initialize RoverC
    roverc.begin();
    roverc.setSpeed(0, 0, 0); // Start with rover stopped
}

void loop() {
    M5.update();

    // Handle Bluetooth commands
    if (SerialBT.available()) {
        char command = SerialBT.read();

        // Send the command over WiFi to the follower
        WiFiClient client = wifiServer.available();
        if (client) {
            client.print(command);
            client.stop();
        }

        // Execute command locally
        executeCommand(command);

        // Display the received command on M5StickC Plus2's screen
        M5.Lcd.fillScreen(BLACK);
        M5.Lcd.setCursor(0, 0);
        M5.Lcd.println("Received: " + String(command));
    }

    delay(3000); // Adjust delay as needed for responsiveness
}

void executeCommand(char command) {
    switch (command) {
        case 'l':
           
            roverc.setSpeed(100, 0, 0);
            delay(3000); 
            roverc.setSpeed(0, 0, 0);
            break;
        case 'r':
            // Move rover backward
            roverc.setSpeed(-100, 0, 0);
            delay(3000); // Move for 1 second
            roverc.setSpeed(0, 0, 0);
            break;
        case 'f':
            // Move rover left
            roverc.setSpeed(0, 100, 0);
            delay(3000); // Move for 1 second
            roverc.setSpeed(0, 0, 0);
            break;
        case 'b':
            // Move rover right
            roverc.setSpeed(0, -100, 0);
            delay(3000); // Move for 1 second
            roverc.setSpeed(0, 0, 0);
            break;
        case 'a':
            // Rotate rover clockwise
            roverc.setSpeed(0, 0, 100);
            delay(3000); // Rotate for 1 second
            roverc.setSpeed(0, 0, 0);
            break;
        case 'c':
            // Rotate rover counterclockwise
            roverc.setSpeed(0, 0, -100);
            delay(3000); // Rotate for 1 second
            roverc.setSpeed(0, 0, 0);
            break;
        default:
            // Stop the rover if an unrecognized command is received
            roverc.setSpeed(0, 0, 0);
            break;
    }
}
