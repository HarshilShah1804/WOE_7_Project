#include <BLEAdvertisedDevice.h>
#include <BLEDevice.h>
#include <BLEScan.h>
#include <BLEAddress.h>
#include <string>

using namespace std;
const int PIN2 = 23;
const int PIN = 2;
const int COMM = 22;
const int CUTOFF = -55;
const int BUZZ = -65;

string want = "unset";

void setup() {
  pinMode(PIN, OUTPUT);
  pinMode(PIN2, OUTPUT);
  pinMode(COMM, OUTPUT);
  
  BLEDevice::init("");
  Serial.begin(115200);
  
  BLEScan *scan = BLEDevice::getScan();
  scan->setActiveScan(true);
  BLEScanResults results = scan->start(1);
  int best = -1e5;

  for (int i = 0; i < results.getCount(); i++) {
    BLEAdvertisedDevice device = results.getDevice(i);
    int rssi = device.getRSSI();

    if (rssi > best) {
      best = rssi;
      want = device.getAddress().toString();
    }
  }
}

void loop() {
  BLEScan *scan = BLEDevice::getScan();
  scan->setActiveScan(true);
  BLEScanResults results = scan->start(1);
  int best = -1e5;

  for (int i = 0; i < results.getCount(); i++) {
    BLEAdvertisedDevice device = results.getDevice(i);
    BLEAddress address = device.getAddress();

    int rssi = device.getRSSI();
    if (want == address.toString()) {
      best = rssi;
    }
  }
  
  digitalWrite(PIN, best > CUTOFF ? HIGH : LOW);
  if(best > CUTOFF){
    digitalWrite(COMM, HIGH);
  } else{
    digitalWrite(COMM, LOW);
  }
  digitalWrite(PIN2, ((CUTOFF>best) && (best > BUZZ)) ? HIGH : LOW);
  Serial.println(best);
}
