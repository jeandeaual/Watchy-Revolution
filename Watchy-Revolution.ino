#include "const.h"
#include "revolution.h"

Revolution watchy(true, 2.0);

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
