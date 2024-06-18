#include <M5StickCPlus2.h>
#include <M5_RoverC.h>
#include <WiFi.h>

M5_RoverC roverc;

const char *ssid = "M5AP";
const char *password = "77777777";
const char *leaderIP = "192.168.4.1"; // IP address of the leader M5StickC
WiFiClient client;

void setup() {
    M5.begin();
    Serial.begin(115200);

    // Connect to the AP
    WiFi.begin(ssid, password);

    // Wait until the device is connected to the AP
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("Connected to Wi-Fi");

    // Print the IP address assigned to the device
    IPAddress myIP = WiFi.localIP();
    Serial.print("Station IP address: ");
    Serial.println(myIP);

    // Display connection information on screen
    M5.Lcd.setRotation(1);
    M5.Lcd.setTextSize(2);
    M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);
    M5.Lcd.fillScreen(TFT_BLACK);
    M5.Lcd.setCursor(0, 10);
    M5.Lcd.printf("Connected to %s", ssid);
    M5.Lcd.setCursor(0, 40);
    M5.Lcd.printf("IP: %s", myIP.toString().c_str());

    // Initialize RoverC
    roverc.begin();
    roverc.setSpeed(0, 0, 0); // Start with rover stopped

    // Connect to the leader
    connectToLeader();
}

void loop() {
    // Check if client is connected
    if (!client.connected()) {
        connectToLeader();
    }

    // Check if data is available from the leader
    if (client.available() > 0) {
        char command = client.read();
        unsigned long currentMillis = millis();
        delay(3000) ;
        // Execute the received command
        executeCommand(command);

        // Display the received command on M5StickC Plus2's screen
        M5.Lcd.fillScreen(TFT_BLACK);
        M5.Lcd.setCursor(0, 0);
        M5.Lcd.println("Received command: " + String(command));
    }

    delay(1000); // Adjust delay as needed for responsiveness
}

void connectToLeader() {
    // Attempt to connect to the leader
    client.stop();
    Serial.print("Connecting to leader at ");
    Serial.print(leaderIP);
    Serial.print("...");
    
    if (client.connect(leaderIP, 80)) {
        Serial.println("connected");
    } else {
        Serial.println("connection failed");
    }
}

void executeCommand(char command) {
    switch (command) {
        case 'l':
            // Move rover forward
            roverc.setSpeed(100, 0, 0);
            delay(5000); // Move for 1 second
            roverc.setSpeed(0, 0, 0);
            break;
        case 'r':
            // Move rover backward
            roverc.setSpeed(-100, 0, 0);
            delay(5000); // Move for 1 second
            roverc.setSpeed(0, 0, 0);
            break;
        case 'f':
            // Move rover left
            roverc.setSpeed(0, 100, 0);
            delay(5000); // Move for 1 second
            roverc.setSpeed(0, 0, 0);
            break;
        case 'b':
            // Move rover right
            roverc.setSpeed(0, -100, 0);
            delay(5000); // Move for 1 second
            roverc.setSpeed(0, 0, 0);
            break;
        case 'a':
            // Rotate rover clockwise
            roverc.setSpeed(0, 0, 100);
            delay(5000); // Rotate for 1 second
            roverc.setSpeed(0, 0, 0);
            break;
        case 'c':
            // Rotate rover counterclockwise
            roverc.setSpeed(0, 0, -100);
            delay(5000); // Rotate for 3 second
            roverc.setSpeed(0, 0, 0);
            break;
        default:
            // Stop the rover if an unrecognized command is received
            roverc.setSpeed(0, 0, 0);
            break;
    }
}
