#include "const.h"
#include "revolution.h"

Revolution watchy;

void setup()
{
#ifdef DEBUG
    Serial.begin(115200);
#endif
    watchy.init();
}

void loop()
{
}
