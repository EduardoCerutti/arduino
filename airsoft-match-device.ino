#include <SD.h>
#include <TMRpcm.h>

const int sdCardPort = 10;
const int speakerPort = 9;

TMRpcm speaker;

int matchFlag = 0;

long team1Timer = 0;
long team2Timer = 0;

int resetPin = 3;
int team1Pin = 5;
int team2Pin = 6;

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

  if (!SD.begin(sdCardPort)) {
    Serial.println("SD card error");
    while(true);
  }

  speaker.speakerPin = speakerPort;
  speaker.setVolume(2);
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
    speaker.play("capturaAzul.wav");
  }

  if (button2 == LOW) {
    matchFlag = team2Pin;
    speaker.play("capturaAmarelo.wav");
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
    speaker.play("10min.wav");
    tenMinFlag = false;
  }

  if (team1Timer >= 100000 && fiveMinFlag || team2Timer >= 100000 && fiveMinFlag) {
    speaker.play("5min.wav");
    fiveMinFlag = false;
  }

  if (team1Timer >= 130000 && twoMinFlag || team2Timer >= 130000 && twoMinFlag) {
    speaker.play("2min.wav");
    twoMinFlag = false;
  }

  if (team1Timer >= endGameTime && endGameFlag) {
    speaker.play("endGameAzul.wav");
    Serial.println("TEAM 1 WIN");
    winnerTeam = "Team 1";
    endGameFlag = false;
  }

  if (team2Timer >= endGameTime && endGameFlag) {
    speaker.play("endGameAmarelo.wav");
    Serial.println("TEAM 2 WIN");
    winnerTeam = "Team 2";
    endGameFlag = false;
  }
}
