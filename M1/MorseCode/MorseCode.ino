//#define myLED 2
#define myLED LED_BUILTIN

int timeunit = 150;

int h[] = { 1, 1, 1, 1};
int e[] = { 1};
int l[] = {1, 0, 1, 1};
int o[] = {0, 0, 0};
int i[] = {1, 1};
int m[] = {0, 0};
int u[] = {1,1, 0};

void setup() {
  // put your setup code here, to run once:
  pinMode(myLED, OUTPUT);
}

void writechar(int letter[], int length, bool wordend = false){
  for ( byte i = 0; i < length; i ++){
    int dotOrDash = letter[i];
    //On time
    digitalWrite(myLED, HIGH);
    //1 is dot = 1 unit of time
    if (dotOrDash){ 
      delay(timeunit);
    }
    //2 is Dash = 3 unit of time
    else{
      delay(timeunit * 3);
    }

    //Off Time
    digitalWrite(myLED, LOW);
    if (wordend && i == (length - 1)){
      delay(7 * timeunit);
    }
    else if (i == (length - 1)){  
      delay(3 * timeunit);
    }
    else{
      delay(timeunit);
    }
  }
}
void loop() {
  // put your main code here, to run repeatedly:
  writechar(h, 4);
  writechar(e, 1);
  writechar(l, 4);
  writechar(l, 3);
  writechar(o, 3, true);
  writechar(i, 2);
  writechar(m, 2);
  writechar(u,3, true);
}
