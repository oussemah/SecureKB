char c;

char encrypt(char t)
{
  return t+1;
}

setup()
{
  Serial.begin(9600);
}

loop()
{
  while(Serial.available() == 0){}
  c = Serial.read();
  c = encrypt(c);
  Serial.print(c);
}

