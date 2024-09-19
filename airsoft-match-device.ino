#include "DFRobotDFPlayerMini.h"
#include "SoftwareSerial.h"

const int speakerRx = 10;
const int speakerTx = 11;
const int speakerVolume = 10;

SoftwareSerial speakerSerial(speakerRx, speakerTx);
DFRobotDFPlayerMini speaker;

const int resetPin = 3;
const int team1Pin = 5;
const int team2Pin = 6;

int matchFlag = 0;

long team1Timer = 0;
long team2Timer = 0;

bool tenMinFlag = true;
bool fiveMinFlag = true;
bool twoMinFlag = true;
bool endGameFlag = true;

String winnerTeam = "";

// 15 min = +-150000
const long endGameTime = 150000;

void setup() {
  pinMode(resetPin, INPUT_PULLUP);
  pinMode(team1Pin, INPUT_PULLUP);
  pinMode(team2Pin, INPUT_PULLUP);

  Serial.begin(9600);
  speakerSerial.begin(9600);

  if (!speaker.begin(speakerSerial)) {
    Serial.println("Speaker setup failed!");
    while (true) ;
  }

  speaker.volume(speakerVolume);
}

void loop() {
  int resetButton = digitalRead(resetPin);
  int button1 = digitalRead(team1Pin);
  int button2 = digitalRead(team2Pin);

  if (resetButton == LOW) {
    matchFlag = 0;

    team1Timer = 0;
    team2Timer = 0;

    tenMinFlag = true;
    fiveMinFlag = true;
    twoMinFlag = true;
    endGameFlag = true;

    String winnerTeam = "";
  }

  if (button1 == LOW) {
    matchFlag = team1Pin;
    speaker.playFolder(1, 5);
    delay(2100);
  }

  if (button2 == LOW) {
    matchFlag = team2Pin;
    speaker.playFolder(1, 4);
    delay(2100);
  }

  if (matchFlag == 0) Serial.println("Waiting for teams to get flag");

  if (matchFlag == team1Pin && winnerTeam.length() == 0) {
    team1Timer = team1Timer + 1;
    Serial.println(team1Timer);
  }

  if (matchFlag == team2Pin && winnerTeam.length() == 0) {
    team2Timer = team2Timer + 1;
    Serial.println(team2Timer);
  }

  if (team1Timer >= 50000 && tenMinFlag || team2Timer >= 50000 && tenMinFlag) {
    speaker.playFolder(1, 3);
    tenMinFlag = false;
  }

  if (team1Timer >= 100000 && fiveMinFlag || team2Timer >= 100000 && fiveMinFlag) {
    speaker.playFolder(1, 2);
    fiveMinFlag = false;
  }

  if (team1Timer >= 130000 && twoMinFlag || team2Timer >= 130000 && twoMinFlag) {
    speaker.playFolder(1, 1);
    twoMinFlag = false;
  }

  if (team1Timer >= endGameTime && endGameFlag) {
    speaker.playFolder(1, 7);
    Serial.println("TEAM 1 WIN");
    winnerTeam = "Team 1";
    endGameFlag = false;
  }

  if (team2Timer >= endGameTime && endGameFlag) {
    speaker.playFolder(1, 6);
    Serial.println("TEAM 2 WIN");
    winnerTeam = "Team 2";
    endGameFlag = false;
  }
}
