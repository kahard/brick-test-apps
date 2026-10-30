"""Writes the serial input of the PC smoke test.

The frames are the ones tools/color_calibration_gui.py sends: brightness, a
320x240 RGB565 test image in 16-line stripes and a darkening RGB565 profile.
"""

import sys
from pathlib import Path

FRAME_START = 0xA5
COMMAND_BRIGHTNESS = 1
COMMAND_PROFILE = 2
COMMAND_IMAGE_STRIPE = 4
WIDTH, HEIGHT, STRIPE_HEIGHT = 320, 240, 16


def frame(command: int, payload: bytes) -> bytes:
    return bytes([FRAME_START, command]) + payload


def test_image() -> bytes:
    # Red rises left to right, green top to bottom, blue is constant.
    raw = bytearray()
    for y in range(HEIGHT):
        for x in range(WIDTH):
            pixel = ((x * 31 // (WIDTH - 1)) << 11) | ((y * 63 // (HEIGHT - 1)) << 5) | 12
            raw += pixel.to_bytes(2, "little")
    return bytes(raw)


def channel(maximum: int, gamma: float) -> bytes:
    return bytes(round(maximum * (index / maximum) ** gamma) for index in range(maximum + 1))


def main() -> None:
    output = Path(sys.argv[1])
    output.parent.mkdir(parents=True, exist_ok=True)
    image = test_image()
    stripe_bytes = WIDTH * STRIPE_HEIGHT * 2
    data = frame(COMMAND_BRIGHTNESS, bytes([80]))
    for stripe in range(HEIGHT // STRIPE_HEIGHT):
        start = stripe * stripe_bytes
        data += frame(COMMAND_IMAGE_STRIPE, bytes([stripe]) + image[start:start + stripe_bytes])
    data += frame(COMMAND_PROFILE, channel(31, 2.0) + channel(63, 2.0) + channel(31, 2.0))
    output.write_bytes(data)


if __name__ == "__main__":
    main()
