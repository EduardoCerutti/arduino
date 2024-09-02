#include <SD.h>
#include <TMRpcm.h>

const int sdCardPort = 10;
const int speakerPort = 9;

TMRpcm speaker;

int matchFlag = 0;

int team2Timer = 0;
int team3Timer = 0;

String winnerTeam = "";

// 15 min = +-150000
const int endGameTime = 2000;

void setup() {
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  Serial.begin(9600);

  if (!SD.begin(sdCardPort)) {
    Serial.println("SD card error");
    while(true);
  }

  speaker.speakerPin = speakerPort;
  speaker.setVolume(5);
}

void loop() {
  int button2 = digitalRead(2);
  int button3 = digitalRead(3);

  if (button2 == LOW) matchFlag = 2;

  if (button3 == LOW) matchFlag = 3;

  if (matchFlag == 0) Serial.println("Waiting for teams to get flag");

  if (matchFlag == 2 && winnerTeam.length() == 0) {
    team2Timer = team2Timer + 1;
    Serial.println(team2Timer);
  }

  if (matchFlag == 3 && winnerTeam.length() == 0) {
    team3Timer = team3Timer + 1;
    Serial.println(team3Timer);
  }

  if (team2Timer >= endGameTime) winnerTeam = "Team 2";

  if (team3Timer >= endGameTime) winnerTeam = "Team 3";

  if (winnerTeam.length() > 0) Serial.println(winnerTeam);
}
