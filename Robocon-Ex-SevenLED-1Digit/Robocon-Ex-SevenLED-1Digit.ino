/*!
  @file		Robocon-Ex-SevenLED-1Digit.ino
  @brief	７セグLEDで遊ぼうぜ

  7セグLED(ROHM製LB-502VN)を使って一秒ごとに０から９までカウントアップして、今度は９から０までカウントダウンするのを繰り返す物をつくります。
  ０パディングです。
  おかしかったら直してください

  @author	T.Kawamura
  @version	1.2
  @date		2016-12-24
  @date   2017-01-26

  @copyright	(C) 2016-2017 T.Kawamura

  Written with GNU Emacs.
*/

#define DELAY_TIME 1000
#define MAX_DIGIT 1   //桁数

/*!
  ピンの定義 ピン番号の部分は各々で変えてください
*/
// 一の位
#define SEG_PIN_A1 2
#define SEG_PIN_B1 3
#define SEG_PIN_C1 4
#define SEG_PIN_D1 5
#define SEG_PIN_E1 6
#define SEG_PIN_F1 7
#define SEG_PIN_G1 8
#define SEG_PIN_DP1 9
// 十の位
#define SEG_PIN_A2 10
#define SEG_PIN_B2 11
#define SEG_PIN_C2 12
#define SEG_PIN_D2 13
#define SEG_PIN_E2 14
#define SEG_PIN_F2 15
#define SEG_PIN_G2 16
#define SEG_PIN_DP2 17

/*!
  フォントデータの格納用
  データ構造
  |-----+---+---+---+---+---+---+---+----|
  | bit | 7 | 6 | 5 | 4 | 3 | 2 | 1 |  0 |
  |-----+---+---+---+---+---+---+---+----|
  | SEG | DP | G | F | E | D | C | B | A |
  |-----+---+---+---+---+---+---+---+----|
*/
const uint8_t FontData[] = {
  0b00111111,		// '0'
  0b00000110,		// '1'
  0b01011011,		// '2'
  0b01001111,		// '3'
  0b01100110,		// '4'
  0b01101101,		// '5'
  0b01111101,		// '6'
  0b00100111,		// '7'
  0b01111111,		// '8'
  0b01101111		// '9'
};

/*!
  FontToPinMap[Digit][Segment] = PinNumber
  SegmentはFontDataの0bit目(DP)から

  フォントデータ(1Byte)について、順番に０ビット目(一番右)から７ビット目までがそれぞれどのセグメント(a, b, c, ... ,DP)のピンに対応しているか
*/

/* 2桁表示用
const uint8_t FontToPinMap[MAX_DIGIT][8] = {
  { SEG_PIN_A1, SEG_PIN_B1, SEG_PIN_C1, SEG_PIN_D1, SEG_PIN_E1, SEG_PIN_F1, SEG_PIN_G1, SEG_PIN_DP1 },		// 一の位用
  { SEG_PIN_A2, SEG_PIN_B2, SEG_PIN_C2, SEG_PIN_D2, SEG_PIN_E2, SEG_PIN_F2, SEG_PIN_G2, SEG_PIN_DP2 }		// 十の位用
};*/
const uint8_t FontToPinMap[8] = { SEG_PIN_A2, SEG_PIN_B2, SEG_PIN_C2, SEG_PIN_D2, SEG_PIN_E2, SEG_PIN_F2, SEG_PIN_G2, SEG_PIN_DP2 };   // 十の位用

void DisplayNumber(uint8_t number);

void setup() {
  uint8_t i,j;

  for( i = 0; i < MAX_DIGIT; i++ ){
    for( j = 0; j < sizeof(FontToPinMap[0]); j++ ){
      pinMode(FontToPinMap[j], OUTPUT);
      digitalWrite(FontToPinMap[j], LOW);
    }
  }
}

void loop() {
  uint8_t i;
  for( i = 0; i < 10; i++ ){
    DisplayNumber(i);
    delay(DELAY_TIME);
  }
  for( i = 8; i >= 0; i-- ){
    DisplayNumber(i);
    delay(DELAY_TIME);
  }
}

void DisplayNumber(uint8_t number) {
  uint8_t i, j, font, number_divide = number;
  
  for( i = 0; i < MAX_DIGIT; i++ ){
		/*
			ex.) i = 0（ループ一回目、一の位）, number = 42 :
			font = FontData[42 % 10] (= FontData[2]) つまり"42"の一の位'2'のフォントデータをfontに代入する
			ex.) i = 1（ループ二回目、十の位）, number = 42 :
			font = FontData[4 % 10] (= FontData[4]) つまり"42"の十の位'4'のフォントデータをfontに代入する
		*/
    font = FontData[number_divide % 10];	//1の位から順に、数値に対応するフォントデータを読み込む(上)

		for( j = 0; j < sizeof(FontToPinMap[0]); j++ ){	// fontを右から順(segA,B,C..)に読んで描画する
      digitalWrite(FontToPinMap[j], (font & 0x01));		//fontの一番右端のビットのみ読みこんで点灯させる
      font >> 1;		//fontを1bit右にシフトし、右から2番めのbitを一番右に
    }
    number_divide /= 10;	//［2桁以上用処理］数値を10で割ったものにする。例：12 -> 1, 345 -> 34
  }
}
