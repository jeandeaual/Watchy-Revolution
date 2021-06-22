# Revolution Watchface for [Watchy](https://watchy.sqfmi.com/)

[![Build](https://github.com/jeandeaual/Watchy-Revolution/actions/workflows/build.yml/badge.svg)](https://github.com/jeandeaual/Watchy-Revolution/actions/workflows/build.yml)

## Build

### Using [arduino-cli](https://arduino.github.io/arduino-cli/latest/installation/)

* Install the ESP32 platform:

    ```sh
    arduino-cli core update-index --additional-urls https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
    arduino-cli core install esp32:esp32 --additional-urls https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
    ```

* Install the [Watchy library](https://github.com/sqfmi/Watchy):

    ```sh
    arduino-cli lib install Watchy@1.2.6
    ```

* Compile the sketch:

    ```sh
    arduino-cli compile --fqbn esp32:esp32:esp32 --build-property build.partitions=min_spiffs --build-property upload.maximum_size=1966080 Watchy-Revolution.ino
    ```

* Upload the firmware to the watch:

   ```sh
   arduino-cli upload -p /dev/ttyACM0 --fqbn esp32:esp32:esp32 Watchy-Revolution
   ```

   Replace `/dev/ttyACM0` with the port listed by `arduino-cli board list`.

### Using the [Arduino IDE](https://www.arduino.cc/en/software)

* See <https://watchy.sqfmi.com/docs/getting-started#arduino-setup> and <https://watchy.sqfmi.com/docs/getting-started#upload>.

* Open `Watchy-Revolution.ino`.

* Click on `Upload`.
