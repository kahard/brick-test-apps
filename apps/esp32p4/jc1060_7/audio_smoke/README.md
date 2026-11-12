# Audio test (JC1060 7" ESP32-P4)

The shared audio test ([apps/shared/audio](../../../shared/audio/README.md)) on this board:
ES8311 codec and NS4150 amplifier (I2S MCLK 13, BCLK 12, LRCK 10, data 9; codec on the touch I2C, GPIO7/8; amplifier enable GPIO11).

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
