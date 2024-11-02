#ifndef BT201_ARDUINO_H
#define BT201_ARDUINO_H

#include <Arduino.h>
#include <string.h>
#include <Queue.h>

#ifdef BT201_DEBUG
#define BT201_DEBUG_PRINT(format, ...) Serial.printf(format, ##__VA_ARGS__)
#else
#define BT201_DEBUG_PRINT(format, ...)
#endif

enum class BluetoothStatus
{
    Pairing = 0,
    Connected = 1,
    MusicPlaying = 2,
    Phone = 3,
    PhoneTalking = 4,
};

enum class AudioMode
{
    Bluetooth = 1,
    UDisk = 2,
    TFCard = 3,
};

class BT201
{
public:
    BT201()
    {
        this->commandQueue = new Queue<String>(20);
    }

    ~BT201()
    {
        delete this->commandQueue;
    }

    /// @brief Get state from device and initializes all internal variables
    void init(HardwareSerial *serial);

    void update();

    void sendCommand(String cmd);

    bool setVolume(uint8_t volume);

    bool increaseVolume();

    bool decreaseVolume();

    bool togglePlayPause();

    bool playNextSong();

    bool playPreviousSong();

    bool setAudioMode(AudioMode mode);

    // --- Getter Functions ---

    AudioMode getAudioMode(bool forceUpdate = false);

    BluetoothStatus getBluetoothStatus(bool forceUpdate = false);

    /// @brief Gets the volume from the BT201 board.
    /// @return Returns the volume.
    uint8_t getVolume();

    bool getTFCardStatus(bool forceUpdate = false);

    bool getUDiskStatus(bool forceUpdate = false);

    String getCurrentFilePlaying();

    String getCallerPhoneNumber();

    // Phone Call Functions

    /// @brief Calls the specified phone number.
    /// @param phoneNumber The phone number to call.
    /// @return true on successful call.
    bool phoneCall(String phoneNumber);

    /// @brief Call back a phone call.
    /// @return
    bool phoneBack();

    /// @brief Hangs up the incoming phone call.
    /// @return true if successful.
    bool phoneHangUp();

    bool phoneRefuseAccept();

    /// @brief Picks up the incoming phone call.
    /// @return true if successful.
    bool phonePickUp();

private:
    HardwareSerial *serial;
    AudioMode audioMode;
    BluetoothStatus bluetoothStatus;
    uint8_t onlineDevices = 0; // BIT values of online devices ([2]TF-Card,[1]- ,[0]U-Disk)

    // TF-UDisk variables
    String currentFilePlaying = "";

    // Phone call variables
    bool isReceivingPhoneCall = false;
    String phoneNumber = "";

    Queue<String> *commandQueue;

    // Helper functions

    /// @brief Updates the online devices variable.
    void updateOnlineDevices();

    // Serial Communication helping functions

    void readSerialToCommandBuffer();

    void proccessPendingCommands();

    void parseReceivedCommand(String cmd);

    /// @brief The query result from which to extract the value.
    /// @param queryResult The query result.
    /// @return The value from the query result.
    uint8_t extractValueFromQueryResponse(String queryResult);

    /// @brief Waits until data in the serial buffer is available or timeout is reached.
    /// @param timeout The timeout in milliseconds.
    /// @return The data if available, else empty string, if timed out.
    String waitUntilResponseReceived(unsigned long timeout, String expectedResponse);

    /// @brief Waits until "OK" message is received or error message is received.
    /// @param timeout The timeout in milliseconds.
    /// @return true if "OK" message is received, else (if timed out or error message received) false.
    bool waitUntilACKReceived(unsigned long timeout);
};

#endif