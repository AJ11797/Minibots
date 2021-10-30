void switch() {
    int inPin;
    int reading;
    pinMode(inPin, INPUT);
    while (true) {
        reading = digitalRead(inPin);
        if (reading == HIGH) {
            break;
        }
    }
}
