# Audio test (shared)

The portable part of the `audio_smoke` applications (ESP32-P4 JC1060 7″ and
JC8012 10″, ESP32-S3 4″ 480x480, CYD). Every board has its own small
application (`src/app/Application.h`, `src/target/main.cpp`, `pc/`) around
this code, so the logic exists once:

| File | Role |
|---|---|
| `AudioServices.h` | `Services` (display, touch, `IPcmOutput`, time, logger) and `Settings` |
| `AudioTest.h/.cpp` | the sounds, the touch lines and the log |
| `chime_wav.h` | a 1.1 s chime, 22.05 kHz mono 16-bit WAV, compiled into the firmware |
| `make_chime.py` | writes `chime_wav.h` |

The screen lines of the network test (`apps/shared/network/TextLines`) are
reused. Only BRICK interfaces and core are used (see the framework's
`docs/audio.md`), so the same code runs on every board and in the PC
simulator.

## What it does

At the start a C major scale (C5..C6, 220 ms per note) plays. Touching a line:

| Line | Action |
|---|---|
| TONE | 440 Hz sine, 1.5 s (`ToneSource`) |
| SCALE | the scale again |
| CHIME | the WAV chime (`WavSource` over `MemoryBytes`) |
| VOLUME- / VOLUME+ | volume in steps of 10 % |
| STOP | stops the sound |

A sound touched while another plays replaces it. A second after a sound ends
the output is stopped (amplifier off). The screen shows the output, the state,
the volume and the measured rate. The log has one line per sound:

```text
Playing the chime (WAV) (22050 Hz, volume 80%)
Played the chime (WAV): 24255 frames in 1099 ms (about 22070 Hz)
```

The rate is the frames the output took over the time of the sound. Its
buffers fill at the start, so a short sound shows a few percent too much.

## Checklist on a board

- the scale is heard at the start, then the tone, the scale and the chime on
  a touch, at the right pitch, without clicks;
- one touch on VOLUME changes the volume by 10 % (the log shows one
  `Volume` line per touch), and 0 % is silent;
- no distortion at 100 % (the P4 panels cap the ES8311 at -10 dB because
  their NS4150 amplifier clips above that);
- after a sound the speaker is quiet: no hum or click when the output stops.

Results: the JC1060 and JC8012 are clean over the whole range, the S3 panel
too. The CYD is clean with rare faint clicks from its 8-bit DAC and amplifier.
