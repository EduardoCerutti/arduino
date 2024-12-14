#include "DFRobotDFPlayerMini.h"
#include "SoftwareSerial.h"

const int speakerRx = 10;
const int speakerTx = 11;
const int speakerVolume = 30;

SoftwareSerial speakerSerial(speakerRx, speakerTx);
DFRobotDFPlayerMini speaker;

const int resetPin = 3;
//white
const int team1Pin = 5;
//yellow
const int team2Pin = 6;

int matchFlag = 0;

long team1Timer = 0;
long team2Timer = 0;

bool T1FiveMinFlag = true;
bool T2FiveMinFlag = true;
bool T1TwoMinFlag = true;
bool T2TwoMinFlag = true;
bool endGameFlag = true;

String winnerTeam = "";

const long endGameTime = 50000;

int soundTime = 2000;

unsigned long previousMs = 0;

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
  speaker.playFolder(1, 8);
  delay(2600);
}

void loop() {
  unsigned long currentMs = millis();

  int resetButton = digitalRead(resetPin);
  int button1 = digitalRead(team1Pin);
  int button2 = digitalRead(team2Pin);

  if (resetButton == LOW) {
    resetGame();
  }

  checkButtons(button1, button2);

  if (matchFlag == 0) Serial.println("Waiting for teams to get flag");

  if (endGameFlag) {
    countTimers();

    checkWinner();

    playWarnings();
  
    playSounds(currentMs);

    checkSoundTiming();
  }
}

void resetGame() {
  matchFlag = 0;

  team1Timer = 0;
  team2Timer = 0;

  T1FiveMinFlag = true;
  T2FiveMinFlag = true;
  T1TwoMinFlag = true;
  T2TwoMinFlag = true;
  endGameFlag = true;
  soundTime = 2000;

  String winnerTeam = "";
  speaker.playFolder(1, 8);
  delay(2600);
}

void checkButtons(int button1, int button2) {
  if (button1 == LOW) {
    matchFlag = team1Pin;
    speaker.playFolder(1, 5);
    delay(2600);
  }

  if (button2 == LOW) {
    matchFlag = team2Pin;
    speaker.playFolder(1, 4);
    delay(2600);
  }
}

void countTimers() {
  if (matchFlag == team1Pin && winnerTeam.length() == 0) {
    team1Timer = team1Timer + 1;
    Serial.println(team1Timer);
  }

  if (matchFlag == team2Pin && winnerTeam.length() == 0) {
    team2Timer = team2Timer + 1;
    Serial.println(team2Timer);
  }
}

void playWarnings() {
  if (team1Timer >= 9000 && T1FiveMinFlag) {
    speaker.playFolder(1, 2);
    T1FiveMinFlag = false;
    delay(2600);
  }

  if (team2Timer >= 9000 && T2FiveMinFlag) {
    speaker.playFolder(1, 2);
    T2FiveMinFlag = false;
    delay(2600);
  }

  if (team1Timer >= 34000 && T1TwoMinFlag) {
    speaker.playFolder(1, 1);
    T1TwoMinFlag = false;
    delay(3800);
  }

  if (team2Timer >= 34000 && T2TwoMinFlag) {
    speaker.playFolder(1, 1);
    T2TwoMinFlag = false;
    delay(3800);
  }
}

void checkWinner() {
  if (team1Timer >= endGameTime) {
    speaker.playFolder(1, 7);
    Serial.println("TEAM 1 WIN");
    winnerTeam = "Team 1";
    endGameFlag = false;
  }

  if (team2Timer >= endGameTime) {
    speaker.playFolder(1, 6);
    Serial.println("TEAM 2 WIN");
    winnerTeam = "Team 2";
    endGameFlag = false;
  }
}

void playSounds(unsigned long currentMs) {
  bool playSound = currentMs - previousMs >= soundTime;

  if(matchFlag != 0 && playSound) {
    speaker.playFolder(1, 10);
    previousMs = currentMs;
  }

  if(matchFlag == 0 && playSound) {
    speaker.playFolder(1, 9);
    previousMs = currentMs;
  }
}

void checkSoundTiming() {
  if (team1Timer >= 9000 || team2Timer >= 9000) soundTime = 1000;
  if (team1Timer >= 34000 || team2Timer >= 34000) soundTime = 500;
}
