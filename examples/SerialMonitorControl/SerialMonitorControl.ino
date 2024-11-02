#include <Arduino.h>
#include <string.h>
#include <BT201.h>

/*
 * SerialMonitorControl
 * Reads from Serial1 user input and sends commands to BT201 module from Serial2. Status and responses are printed to Serial1.
 * 
 * Connect Serial2 TX/RX pins to RX/TX pins of BT201 module. Adjust the expected BAUD rate of BT201 module in setup() - currently set at 115200.
*/

#define RXD2 21
#define TXD2 22

BT201 bt201;

String modeStr[4] = 
{
    "Error",
    "Bluetooth",
    "UDisk",
    "TFCard",
};

String statusStr[5] = 
{
    "Pairing",
    "Connected",
    "MusicPlaying",
    "Phone",
    "PhoneTalking",
};

String audioModeToString(AudioMode mode)
{
    return modeStr[(int)mode];
}

String bluetoothStatusToString(BluetoothStatus status)
{
    return statusStr[(int)status];
}

void executeTestCase(String testCase)
{
    Serial.println("=====================================");
    // Export args if any
    int argIndex = testCase.indexOf(' ');
    String cmd;
    String args;
    if(argIndex != -1)
    {
        cmd = testCase.substring(0, argIndex);
        args = testCase.substring(argIndex + 1);
        args.trim();
    }
    else
    {
        cmd = testCase;
    }

    // Generic State Info
    if(cmd == "i")
    {
        AudioMode mode = bt201.getAudioMode();
        BluetoothStatus status = bt201.getBluetoothStatus();
        bool isTFCardInserted = bt201.getTFCardStatus(true);
        bool isUDiskInserted = bt201.getUDiskStatus();
        Serial.printf(
            "=== BT201 ===\n"
            "AudioMode:       %s\n"
            "BluetoothStatus: %s\n"
            "TFCard:          %s\n"
            "UDisk:           %s\n"
            "===========\n", 
            audioModeToString(mode).c_str(),
            bluetoothStatusToString(status).c_str(),
            isTFCardInserted ? "Yes" : "No",
            isUDiskInserted ? "Yes" : "No"
        );
    }

    else if(cmd == "volume")
    {
        // Increase/Decrease/Set volume
        if(!args.isEmpty())
        {
            // Decrease Volume
            if(args.indexOf('-') != -1)
            {
                Serial.printf("Decreasing volume\n");
                bt201.decreaseVolume();
            }
            // Increase Volume
            else if(args.indexOf('+') != -1)
            {
                Serial.printf("Increasing volume\n");
                bt201.increaseVolume();
            }
            // Set volume
            else
            {
                int volLevel = args.toInt();
                Serial.printf("Setting volume to %d\n", volLevel);
                bt201.setVolume(volLevel);
            }
        }
        // Get volume
        else
        {
            int volLevel = bt201.getVolume();
            Serial.printf("Current volume: %d\n", volLevel);
        }
    }

    else if(cmd == "pp")
    {
        Serial.printf("Toggle Play/Pause.\n");
        bt201.togglePlayPause();
    }

    else if(cmd == "previous")
    {
        Serial.printf("Play previous song.\n");
        bt201.playPreviousSong();
    }
    
    else if(cmd == "next")
    {
        Serial.printf("Play next song.\n");
        bt201.playNextSong();
    }

    else if(cmd == "call" && !args.isEmpty())
    {
        if(args == "back")
        {
            Serial.printf("Phone back\n");
            bt201.phoneBack();
        }
        if(args == "accept")
        {
            Serial.printf("Accepting call\n");
            bt201.phonePickUp();
        }
        else if(args == "refuse")
        {
            Serial.printf("Refusing call\n");
            bt201.phoneRefuseAccept();
        }
        else if(args == "hangup")
        {
            Serial.printf("Hanging up call\n");
            bt201.phoneHangUp();
        }
        else
        {
            Serial.printf("Calling phone number");
            bt201.phoneCall(args);
        }
    }

    else if(cmd == "mode")
    {
        if(args.isEmpty())
        {
            Serial.printf("Getting audio mode\n");
            AudioMode mode = bt201.getAudioMode(true);
            Serial.printf("Audio mode: %s\n", audioModeToString(mode).c_str());
        }
        else
        {
            AudioMode mode = (AudioMode)args.toInt();
            Serial.printf("Setting audio mode to %s\n", audioModeToString(mode).c_str());
            bool result = bt201.setAudioMode(mode);
            if(result)
            {
                Serial.printf("Audio mode set successfully\n");
            }
            else
            {
                Serial.printf("Failed to set audio mode\n");
            }
        }
    }

    else if(cmd == "btstat")
    {
        Serial.printf("Getting bluetooth status\n");
        BluetoothStatus status = bt201.getBluetoothStatus(true);
        Serial.printf("Bluetooth status: %s\n", bluetoothStatusToString(status).c_str());
    }

    else
    {
        cmd.toUpperCase();
        args.toUpperCase();
        Serial.printf("Unknown command, forwarding as is: %s %s\n", cmd.c_str(), args.c_str());
        bt201.sendCommand(cmd + " " + args);
    }
}

void setup() {
    Serial.begin(9600);

    Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);

    delay(200);
    bt201.init(&Serial2);
}

void loop() {
    bt201.update();

    if(Serial.available()) {
        String testCase  = Serial.readStringUntil('\n');
        executeTestCase(testCase);
    }
}