# Audio test (ESP32-S3 4" 480x480)

The shared audio test ([apps/shared/audio](../../../shared/audio/README.md)) on this board:
I2S amplifier without a codec (BCLK 1, LRCK 2, data 40); the volume scales the samples.

```text
make compile
make upload PORT=COMx
make monitor PORT=COMx
```

At the start a C major scale plays. Touch a line to play a 440 Hz tone, the
scale or a chime (a WAV file compiled into the firmware), to change the
volume in steps of 10 % or to stop. The log has one line per sound with the
rate the output took the frames at (about the sample rate).

`make sim` (SDL window) or `make sim-web` (web browser) runs the same test in
the PC simulator, whose speaker only counts the frames (no sound).
