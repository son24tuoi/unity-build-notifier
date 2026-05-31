#include <Arduino.h>
#include <pitch.h>

int speakerPin = D3;
int buttonPin = D0;
int greenLedPin = D1;
int redLedPin = D2;

int melody[] = {
    NOTE_C7, NOTE_G6, NOTE_G6, NOTE_A6, NOTE_G6, 0, NOTE_B6, NOTE_C7};

int noteDurations[] = {
    4, 8, 8, 4, 4, 4, 4, 4};

int buttonState = 0;

int mode = 0;

bool isPlaying = false;

// 1. Tăng thời gian cơ sở (từ 1000 lên 1300 hoặc 1500 để chậm hơn)
int tempo = 1300;

// 2. Cấu hình lại độ dài nốt cho giống Mario
// Số càng nhỏ thì nốt càng dài
int melodySuccess[] = {NOTE_C5, NOTE_F5, NOTE_C6, NOTE_A5, NOTE_B5, NOTE_G5};
int durationSuccess[] = {8, 8, 8, 8, 8, 2}; // Nốt cuối (G5) dài gấp 4 lần các nốt khác

int melodyFailed[] = {NOTE_C5, NOTE_G4, NOTE_E4, NOTE_A4, NOTE_B4, NOTE_A4, NOTE_GS4, NOTE_G4};
int durationFailed[] = {6, 6, 6, 6, 6, 6, 6, 2};

void SetLed(bool isOn)
{
  if (mode == 1)
  {
    digitalWrite(greenLedPin, isOn ? HIGH : LOW);
  }
  else if (mode == 2)
  {
    digitalWrite(redLedPin, isOn ? HIGH : LOW);
  }
}

void PlayMarioTheme(bool isSuccess)
{
  if (isPlaying)
    return;

  isPlaying = true;
  SetLed(true);

  int *melody = isSuccess ? melodySuccess : melodyFailed;
  int *durations = isSuccess ? durationSuccess : durationFailed;
  int length = isSuccess ? 6 : 8;

  for (int i = 0; i < length; i++)
  {
    // Thay 1000 bằng biến tempo
    int noteDuration = tempo / durations[i];

    tone(speakerPin, melody[i], noteDuration * 0.9); // Phát 90% thời gian (tạo khoảng nghỉ)

    delay(noteDuration); // Nghỉ 10% còn lại
    noTone(speakerPin);
  }

  SetLed(false);

  isPlaying = false;
  mode = 0;
}

void Play()
{
  if (isPlaying)
    return;

  isPlaying = true;
  SetLed(true);

  for (int thisNote = 0; thisNote < 8; thisNote++)
  {
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(speakerPin, melody[thisNote], noteDuration);

    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);

    noTone(speakerPin);
  }

  SetLed(false);
  isPlaying = false;

  mode = 0;
}

void setup()
{
  Serial.begin(9600);

  pinMode(buttonPin, INPUT);
  pinMode(greenLedPin, OUTPUT);
  pinMode(redLedPin, OUTPUT);

  digitalWrite(greenLedPin, LOW);
  digitalWrite(redLedPin, LOW);

  Play();
}

void loop()
{
  if (Serial.available() > 0)
  {
    String command = Serial.readStringUntil('\n');
    command.trim(); // Loại bỏ khoảng trắng hoặc ký tự xuống dòng thừa

    if (command.equalsIgnoreCase("success"))
    {
      Serial.println("Received success command");
      mode = 1;
      PlayMarioTheme(true);
    }
    else if (command.equalsIgnoreCase("failed"))
    {
      Serial.println("Received failed command");
      mode = 2;
      PlayMarioTheme(false);
    }
  }

  buttonState = digitalRead(buttonPin);

  if (buttonState == HIGH)
  {
    Serial.println("Button is pressed");

    Play();
  }
  else
  {
    Serial.println("Button is not pressed");
  }

  delay(100);
}