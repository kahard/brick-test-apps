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

1. Starts `WifiManager` with the network saved on the setup page, or else
   the one from the build (below). It scans, joins the network, retries with
   growing pauses and, after 3 failures in a row, opens the hotspot
   `BRICK-<BOARD>` (password `brick-setup`, address 192.168.4.1). Without a
   network the hotspot opens at once.
2. Chooses the link with `NetworkSelector`: Ethernet first when the board has
   it (JC1060), otherwise Wi-Fi.
3. Checks the internet every 30 s with `HttpProbe`:
   `http://connectivitycheck.gstatic.com/generate_204` must answer 204.
4. Serves the Wi-Fi setup page (below) on the hotspot and at the device's
   address.
5. A touch (or the button on the ESP-12F) scans again and repeats the check;
   held for 5 s (a red bar fills the title line) it forgets the saved network
   and opens the hotspot.

## The setup page

Join the hotspot `BRICK-<BOARD>` (password `brick-setup`) with a phone: it
opens the setup page by itself (the hotspot's DNS points every name at the
device), or open `http://192.168.4.1/`. The page lists the networks of the
last scan ("Scan again" refreshes it); choose one (or type a hidden one),
enter its password and press Connect. The device saves it (NVS on the ESP32
boards, the EEPROM sector on the ESP-12F), joins it and closes the hotspot;
the screen shows its new address, where the same page has "Forget". Forget
(or holding the touch screen / button for 5 s) saves "no network" and opens
the hotspot again. A saved network, or a saved "no network", replaces the one
from the build, also after a restart.

In the PC simulator the page is at `http://localhost:8081/` (no captive DNS;
the saved network lasts until the simulator ends).

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
end up in the firmware in plain text. Without either, only the hotspot runs
(until a network is set on the setup page).
The PC simulator always joins its simulated "BRICK-LAB".

## What a pass looks like

Screen, top to bottom:

| Line | Pass |
|---|---|
| `WIFI` | green: SSID, signal in dBm, channel |
| `ETHERNET` (JC1060) | green `up` with a cable, grey `down (no cable?)` without |
| `IP` | the address and the link in use (`via ethernet` / `via wifi`) |
| `INTERNET` | green `OK  HTTP 204  <ms>`; yellow `HTTP <code>` behind a captive portal |
| `HOTSPOT` | `off`, or the SSID / password, the setup page (`http://192.168.4.1`) and number of clients |
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
- [ ] Setup page: the phone opens it by itself after joining the hotspot (or
      at http://192.168.4.1/); a network chosen there is joined, the hotspot
      closes, and after a restart the device joins it again.
- [ ] At the device's address: the page shows "Connected to …"; Forget opens
      the hotspot again, also after a restart.
- [ ] Holding the touch screen or button: a red bar fills the title line;
      letting go early keeps the network, 5 s forget it.
- [ ] JC1060: with a cable `via ethernet`; pulling the cable switches to
      `via wifi` within a second and the check repeats; plugging it back
      switches back.
