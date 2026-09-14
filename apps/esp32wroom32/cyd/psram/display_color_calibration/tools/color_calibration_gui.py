"""Interactive UART client for the CYD RGB565 color calibration application."""

from __future__ import annotations

import json
import math
from pathlib import Path
import tkinter as tk
import time
from tkinter import filedialog, messagebox, ttk

from PIL import Image, ImageOps, ImageTk

try:
    import serial
    from serial.tools import list_ports
except ImportError:
    serial = None
    list_ports = None


FRAME_START = 0xA5
COMMAND_BRIGHTNESS = 1
COMMAND_PROFILE = 2
COMMAND_PATTERN = 3
COMMAND_IMAGE_STRIPE = 4


class CalibrationGui(tk.Tk):
    def __init__(self) -> None:
        super().__init__()
        self.title("CYD RGB565 Color Calibration")
        self.minsize(900, 600)
        self.serial_port: serial.Serial | None = None
        self.preview_source: Image.Image | None = None
        self.preview_photo: ImageTk.PhotoImage | None = None
        self.pending_update: str | None = None
        self.console_window: tk.Toplevel | None = None
        self.device_output: tk.Text | None = None
        self.device_log = ""

        self.port_name = tk.StringVar()
        self.status = tk.StringVar(value="Nie połączono")
        self.brightness = tk.IntVar(value=100)
        self.red_gain = tk.IntVar(value=100)
        self.green_gain = tk.IntVar(value=100)
        self.blue_gain = tk.IntVar(value=100)
        self.red_gamma = tk.IntVar(value=100)
        self.green_gamma = tk.IntVar(value=100)
        self.blue_gamma = tk.IntVar(value=100)

        self._build_ui()
        self.refresh_ports()
        self.after(50, self.poll_device_output)

    def _build_ui(self) -> None:
        controls = ttk.Frame(self, padding=12)
        controls.grid(row=0, column=0, sticky="nsew")
        preview = ttk.Frame(self, padding=12)
        preview.grid(row=0, column=1, sticky="nsew")
        self.columnconfigure(1, weight=1)
        self.rowconfigure(0, weight=1)

        connection = ttk.LabelFrame(controls, text="UART", padding=8)
        connection.grid(row=0, column=0, sticky="ew")
        connection.columnconfigure(0, weight=1)
        ttk.Combobox(connection, textvariable=self.port_name, state="readonly", width=16).grid(
            row=0, column=0, sticky="ew"
        )
        self.port_selector = connection.winfo_children()[0]
        ttk.Button(connection, text="Odśwież", command=self.refresh_ports).grid(row=0, column=1, padx=(6, 0))
        ttk.Button(connection, text="Połącz / rozłącz", command=self.toggle_connection).grid(
            row=1, column=0, columnspan=2, pady=(6, 0), sticky="ew"
        )
        ttk.Label(connection, textvariable=self.status, wraplength=260).grid(
            row=2, column=0, columnspan=2, pady=(6, 0), sticky="w"
        )
        ttk.Button(connection, text="Pokaż / ukryj konsolę CYD", command=self.toggle_console).grid(
            row=3, column=0, columnspan=2, pady=(6, 0), sticky="ew"
        )

        backlight = ttk.LabelFrame(controls, text="Podświetlenie", padding=8)
        backlight.grid(row=1, column=0, pady=(10, 0), sticky="ew")
        self._slider(backlight, "Jasność PWM", self.brightness, 0, 100, 1, 0)

        color = ttk.LabelFrame(controls, text="Korekcja kanałów", padding=8)
        color.grid(row=2, column=0, pady=(10, 0), sticky="ew")
        self._slider(color, "R gain (%)", self.red_gain, 0, 200, 1, 0)
        self._slider(color, "G gain (%)", self.green_gain, 0, 200, 1, 1)
        self._slider(color, "B gain (%)", self.blue_gain, 0, 200, 1, 2)
        self._slider(color, "R gamma", self.red_gamma, 30, 300, 1, 3, scale=100)
        self._slider(color, "G gamma", self.green_gamma, 30, 300, 1, 4, scale=100)
        self._slider(color, "B gamma", self.blue_gamma, 30, 300, 1, 5, scale=100)

        patterns = ttk.LabelFrame(controls, text="Wzorzec na CYD", padding=8)
        patterns.grid(row=3, column=0, pady=(10, 0), sticky="ew")
        for index, name in enumerate(("Szarości", "Pasy kolorów", "Gradienty RGB")):
            ttk.Button(patterns, text=name, command=lambda value=index: self.send_pattern(value)).grid(
                row=0, column=index, padx=2, sticky="ew"
            )
            patterns.columnconfigure(index, weight=1)

        ttk.Button(controls, text="Wczytaj obraz do podglądu", command=self.open_image).grid(
            row=4, column=0, pady=(10, 0), sticky="ew"
        )
        ttk.Button(controls, text="Wyślij obraz do CYD", command=self.send_image).grid(
            row=5, column=0, pady=(6, 0), sticky="ew"
        )
        ttk.Button(controls, text="Zapisz konfigurację i profil C++", command=self.save_profile).grid(
            row=6, column=0, pady=(6, 0), sticky="ew"
        )
        ttk.Button(controls, text="Wczytaj konfigurację", command=self.load_profile).grid(
            row=7, column=0, pady=(6, 0), sticky="ew"
        )
        ttk.Button(controls, text="Przywróć domyślny profil", command=self.reset_profile).grid(
            row=8, column=0, pady=(6, 0), sticky="ew"
        )

        ttk.Label(preview, text="Wyśrodkowany kadr 320×240 wysyłany do CYD — bez korekcji").pack(anchor="w")
        self.preview_label = ttk.Label(preview, text="Wczytaj PNG/JPG, aby zobaczyć podgląd.")
        self.preview_label.pack(fill="both", expand=True, pady=(8, 0))

    def _slider(
        self, parent: ttk.LabelFrame, label: str, value: tk.IntVar, minimum: int, maximum: int, step: int, row: int,
        scale: int = 1,
    ) -> None:
        text = tk.StringVar()
        default = value.get()

        def changed(*_args: object) -> None:
            suffix = "%" if "gain" in label or "Jasność" in label else ""
            text.set(f"{value.get() / scale:.2f}{suffix}" if scale != 1 else f"{value.get()}{suffix}")
            self.schedule_update()

        value.trace_add("write", changed)
        changed()
        ttk.Label(parent, text=label).grid(row=row, column=0, sticky="w")
        slider = ttk.Scale(parent, from_=minimum, to=maximum, variable=value, command=lambda _v: None)
        slider.grid(
            row=row, column=1, padx=8, sticky="ew"
        )

        def scrolled(event: tk.Event[tk.Misc]) -> str:
            direction = 1 if event.delta > 0 else -1
            value.set(max(minimum, min(maximum, value.get() + direction * step)))
            return "break"

        slider.bind("<MouseWheel>", scrolled)
        ttk.Label(parent, textvariable=text, width=7).grid(row=row, column=2, sticky="e")
        ttk.Button(parent, text="Reset", command=lambda: value.set(default), width=6).grid(
            row=row, column=3, padx=(6, 0), sticky="e"
        )
        parent.columnconfigure(1, weight=1)

    def refresh_ports(self) -> None:
        if list_ports is None:
            self.status.set("Brak pyserial: uruchom pip install pyserial")
            return
        ports = [port.device for port in list_ports.comports()]
        self.port_selector["values"] = ports
        if ports and self.port_name.get() not in ports:
            self.port_name.set(ports[0])

    def toggle_connection(self) -> None:
        if self.serial_port is not None:
            self.serial_port.close()
            self.serial_port = None
            self.status.set("Rozłączono")
            return
        if serial is None or not self.port_name.get():
            self.status.set("Wybierz port COM i zainstaluj pyserial")
            return
        try:
            self.serial_port = serial.Serial(self.port_name.get(), 115200, timeout=0.1)
            # Opening the USB-UART port commonly resets ESP32 through DTR/RTS.
            # Give the calibration firmware time to boot before its first frame.
            self.status.set(f"Połączono: {self.port_name.get()} @ 115200; oczekiwanie na start CYD…")
            self.after(1800, self._send_initial_settings)
        except serial.SerialException as error:
            self.serial_port = None
            self.status.set(f"Błąd połączenia: {error}")

    def _send_initial_settings(self) -> None:
        if self.serial_port is None:
            return
        self.send_current_settings()
        self.status.set("CYD gotowy: wysłano jasność i profil RGB565")

    def schedule_update(self) -> None:
        if self.pending_update is not None:
            self.after_cancel(self.pending_update)
        self.pending_update = self.after(50, self.send_current_settings)

    def send_current_settings(self) -> None:
        self.pending_update = None
        self._write_frame(COMMAND_BRIGHTNESS, bytes([self.brightness.get()]))
        self._write_frame(COMMAND_PROFILE, self.build_lut())
        self.update_preview()

    def send_pattern(self, pattern: int) -> None:
        self._write_frame(COMMAND_PATTERN, bytes([pattern]))

    def reset_profile(self) -> None:
        self.brightness.set(100)
        self.red_gain.set(100)
        self.green_gain.set(100)
        self.blue_gain.set(100)
        self.red_gamma.set(100)
        self.green_gamma.set(100)
        self.blue_gamma.set(100)

    def _write_frame(self, command: int, payload: bytes) -> None:
        if self.serial_port is None:
            return
        try:
            self.serial_port.write(bytes([FRAME_START, command]) + payload)
        except serial.SerialException as error:
            self.status.set(f"Utracono połączenie: {error}")
            self.serial_port = None

    def toggle_console(self) -> None:
        if self.console_window is None:
            self.console_window = tk.Toplevel(self)
            self.console_window.title("CYD UART console")
            self.console_window.geometry("720x360")
            self.console_window.protocol("WM_DELETE_WINDOW", self.console_window.withdraw)
            self.device_output = tk.Text(self.console_window, state="disabled", wrap="word")
            self.device_output.pack(fill="both", expand=True, padx=8, pady=8)
            self._show_device_log()
            return
        if self.console_window.winfo_viewable():
            self.console_window.withdraw()
        else:
            self.console_window.deiconify()
            self.console_window.lift()

    def _show_device_log(self) -> None:
        if self.device_output is None:
            return
        self.device_output.configure(state="normal")
        self.device_output.delete("1.0", "end")
        self.device_output.insert("end", self.device_log)
        self.device_output.see("end")
        self.device_output.configure(state="disabled")

    def poll_device_output(self) -> None:
        if self.serial_port is not None:
            try:
                available = self.serial_port.in_waiting
                received = self.serial_port.read(available) if available else b""
                if received:
                    self.device_log = (self.device_log + received.decode("utf-8", errors="replace"))[-16000:]
                    if self.console_window is not None and self.console_window.winfo_viewable():
                        self._show_device_log()
            except serial.SerialException as error:
                self.status.set(f"Utracono połączenie: {error}")
                self.serial_port = None
        self.after(50, self.poll_device_output)

    def build_lut(self) -> bytes:
        return b"".join((
            self._channel_lut(31, self.red_gain.get(), self.red_gamma.get() / 100.0),
            self._channel_lut(63, self.green_gain.get(), self.green_gamma.get() / 100.0),
            self._channel_lut(31, self.blue_gain.get(), self.blue_gamma.get() / 100.0),
        ))

    @staticmethod
    def _channel_lut(maximum: int, gain_percent: int, gamma: float) -> bytes:
        values = []
        for level in range(maximum + 1):
            normalized = level / maximum
            corrected = math.pow(normalized, gamma) * gain_percent / 100.0
            values.append(max(0, min(maximum, round(corrected * maximum))))
        return bytes(values)

    def open_image(self) -> None:
        filename = filedialog.askopenfilename(filetypes=[("Images", "*.png *.jpg *.jpeg *.bmp"), ("All files", "*.*")])
        if not filename:
            return
        try:
            self.preview_source = Image.open(filename).convert("RGB")
            self.update_preview()
        except OSError as error:
            messagebox.showerror("Nie można otworzyć obrazu", str(error))

    def send_image(self) -> None:
        if self.serial_port is None or self.preview_source is None:
            self.status.set("Połącz CYD i wczytaj obraz")
            return
        image = self.reference_image()
        raw = bytearray()
        for red, green, blue in image.getdata():
            pixel = ((red >> 3) << 11) | ((green >> 2) << 5) | (blue >> 3)
            raw.extend((pixel & 0xFF, pixel >> 8))
        self.status.set("Wysyłanie obrazu do CYD…")
        self.update_idletasks()
        for stripe in range(15):
            start = stripe * 320 * 16 * 2
            self._write_frame(COMMAND_IMAGE_STRIPE, bytes([stripe]) + raw[start:start + 320 * 16 * 2])
            # Do not fill the host serial queue with the whole image. CYD has
            # time to cache each frame before the next one begins.
            self.serial_port.flush()
            time.sleep(0.02)
        if self.serial_port is not None:
            self.status.set("Obraz przesłany do CYD; suwaki przeliczają prawą część obrazu lokalnie")

    def reference_image(self) -> Image.Image:
        assert self.preview_source is not None
        # Scale proportionally until the image covers the whole screen; crop
        # the excess symmetrically around its centre. The result is exactly
        # one RGB565 pixel for each physical CYD pixel.
        return ImageOps.fit(
            self.preview_source.convert("RGB"), (320, 240), method=Image.Resampling.LANCZOS, centering=(0.5, 0.5)
        )

    def update_preview(self) -> None:
        if self.preview_source is None:
            return
        image = self.reference_image()
        image.thumbnail((600, 500))
        self.preview_photo = ImageTk.PhotoImage(image)
        self.preview_label.configure(image=self.preview_photo, text="")

    def save_profile(self) -> None:
        filename = filedialog.asksaveasfilename(
            defaultextension=".json", initialfile="CydColorCalibration.json", filetypes=[("Calibration config", "*.json")]
        )
        if not filename:
            return
        config_path = Path(filename)
        config = {
            "version": 1,
            "brightness": self.brightness.get(),
            "red_gain": self.red_gain.get(),
            "green_gain": self.green_gain.get(),
            "blue_gain": self.blue_gain.get(),
            "red_gamma": self.red_gamma.get(),
            "green_gamma": self.green_gamma.get(),
            "blue_gamma": self.blue_gamma.get(),
        }
        config_path.write_text(json.dumps(config, indent=2) + "\n", encoding="utf-8")

        red, green, blue = self.build_lut()[:32], self.build_lut()[32:96], self.build_lut()[96:]
        content = "#pragma once\n\n#include \"brick/core/display/Rgb565ColorProfile.h\"\n\n"
        content += "namespace cyd_color_profile\n{\n\ninline brick::core::display::Rgb565ColorProfile profile()\n{\n"
        content += "    brick::core::display::Rgb565ColorProfile value{};\n"
        for name, values in (("red", red), ("green", green), ("blue", blue)):
            content += f"    value.{name} = {{ " + ", ".join(f"{value}U" for value in values) + " };\n"
        content += "    return value;\n}\n\n}  // namespace cyd_color_profile\n"
        header_path = config_path.with_suffix(".h")
        header_path.write_text(content, encoding="utf-8")
        self.status.set(f"Zapisano: {config_path.name} oraz {header_path.name}")

    def load_profile(self) -> None:
        filename = filedialog.askopenfilename(filetypes=[("Calibration config", "*.json")])
        if not filename:
            return
        try:
            config = json.loads(Path(filename).read_text(encoding="utf-8"))
            names = ("brightness", "red_gain", "green_gain", "blue_gain", "red_gamma", "green_gamma", "blue_gamma")
            if config.get("version") != 1 or any(name not in config for name in names):
                raise ValueError("nieprawidłowy format konfiguracji")
            for variable, name in (
                (self.brightness, "brightness"),
                (self.red_gain, "red_gain"),
                (self.green_gain, "green_gain"),
                (self.blue_gain, "blue_gain"),
                (self.red_gamma, "red_gamma"),
                (self.green_gamma, "green_gamma"),
                (self.blue_gamma, "blue_gamma"),
            ):
                variable.set(int(config[name]))
            self.status.set(f"Wczytano konfigurację: {Path(filename).name}")
        except (OSError, ValueError, json.JSONDecodeError) as error:
            messagebox.showerror("Nie można wczytać konfiguracji", str(error))


if __name__ == "__main__":
    CalibrationGui().mainloop()
