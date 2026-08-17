# BRICK test applications

Test applications are grouped by MCU family and board variant:

```text
brick_test_apps/
├── libs/
│   └── brick/         BRICK Framework submodule under test
├── apps/
│   ├── esp32p4/
│   │   └── jc1060_7/
│   ├── esp32s3/
│   │   └── panel_480/
│   └── esp32wroom32/
│       └── cyd/
└── README.md
```

Use names in the form:

```text
<board>_<scope>_<purpose>
```

Examples: `display_touch_smoke`, `jpeg_decode_benchmark`,
`audio_output_smoke`.

Each application should contain its own `README.md` describing the hardware,
expected wiring, build/upload commands, and the result of the test.

## Checkout and update

Clone the repository together with BRICK:

```text
git clone --recurse-submodules <brick-test-apps-url>
```

For an existing checkout:

```text
git submodule update --init --recursive
```

The submodule is pinned to a specific BRICK commit so test results remain
reproducible. Update it deliberately when a new framework version is ready.
