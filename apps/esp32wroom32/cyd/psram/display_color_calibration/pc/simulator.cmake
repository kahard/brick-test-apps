# `make pc-serial-input` writes the GUI frames (brightness, test image, profile)
# replayed through --serial-in. Reading 512 bytes per update, the image is in
# after about 300 updates; the scripted touch then drags the divider to x=240.
set(serial_input "${CMAKE_CURRENT_LIST_DIR}/generated/serial_in.bin")
brick_app_simulator(cyd_display_color_calibration
  FRAMES 400
  PREBUILD pc-serial-input
  SMOKE_ARGS --serial-in "${serial_input}"
             --touch 160,120@5200+100 --touch 200,120@5300+100 --touch 240,120@5400+100)
