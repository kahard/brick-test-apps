# Network test (shared)

The portable part of the `network_*_internet_smoke` applications. Every board
has its own small application (`src/app/Application.h`, `src/target/main.cpp`,
`pc/`) around this code, so the logic exists once:

| File | Role |
|---|---|
| `NetworkServices.h` | `Services` (what the test needs from a board) and `Settings` |
| `InternetTest.h/.cpp` | Wi-Fi manager, link selection, internet check, log |
| `InternetTestScreen.cpp` | what the screen shows |
| `TextLines.h/.cpp` | the screen as lines of text, redrawn only when they change |

Only BRICK interfaces and core are used, so the same code runs on every board
and in the PC simulator.

## What it does

1. Starts `WifiManager` with the network from the build (below). It scans,
   joins the network, retries with growing pauses and, after 3 failures in a
   row, opens the hotspot `BRICK-<BOARD>` (password `brick-setup`,
   address 192.168.4.1). Without a configured network the hotspot opens at
   once: this is the access point test.
2. Chooses the link with `NetworkSelector`: Ethernet first when the board has
   it (JC1060), otherwise Wi-Fi.
3. Checks the internet every 30 s with `HttpProbe`:
   `http://connectivitycheck.gstatic.com/generate_204` must answer 204.
4. A touch (or the button on the ESP-12F) scans again and repeats the check.

## The network to join

The credentials are compiled in from the environment

```text
BRICK_WIFI_SSID=MyNetwork BRICK_WIFI_PASSWORD=secret make upload
```

or from `wifi-credentials.txt` in the repository root (git-ignored):

```text
SSID=MyNetwork
PASSWORD=secret
```

`tools/wifi_credentials.py` writes them to
`src/target/generated/wifi_credentials.h` (git-ignored) on every build. They
end up in the firmware in plain text. Without either, only the hotspot runs.
The PC simulator always joins its simulated "BRICK-LAB".

## What a pass looks like

Screen, top to bottom:

| Line | Pass |
|---|---|
| `WIFI` | green: SSID, signal in dBm, channel |
| `ETHERNET` (JC1060) | green `up` with a cable, grey `down (no cable?)` without |
| `IP` | the address and the link in use (`via ethernet` / `via wifi`) |
| `INTERNET` | green `OK  HTTP 204  <ms>`; yellow `HTTP <code>` behind a captive portal |
| `HOTSPOT` | `off`, or the SSID / password, address and number of clients |
| `NETWORKS` | the access points of the last scan, strongest first |

Log (115200 baud):

```text
I (brick_wifi_manager) Joining MyNetwork
I (brick_wifi_manager) Connected to MyNetwork
I (network_test) Traffic goes through: wifi (192.168.1.57)
I (network_test) Internet: HTTP 204 from 142.250.186.67 in 85 ms (DNS 20 ms, connect 45 ms)
```

A wrong password logs `Attempt N failed: authentication failed`, a network out
of range `network not found`; after the third failure
`Hotspot BRICK-<BOARD> open`.

## Hardware checklist

- [ ] With credentials: `WIFI` green, `INTERNET OK HTTP 204`.
- [ ] Touch: the network list is refreshed, the check repeats.
- [ ] Router off or out of range: `connection lost`, retries, rejoins when it
      is back.
- [ ] Wrong password: three failures, then the hotspot.
- [ ] Without credentials: the hotspot at once; a phone joins it, gets an
      address from 192.168.4.x and `clients` goes to 1.
- [ ] JC1060: with a cable `via ethernet`; pulling the cable switches to
      `via wifi` within a second and the check repeats; plugging it back
      switches back.
