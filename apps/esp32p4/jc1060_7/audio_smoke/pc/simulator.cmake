# The shared audio test (apps/shared/audio) with the text lines of the
# network test. The touches play the tone (line 4) and the chime (line 6).
set(audio_shared "${CMAKE_CURRENT_LIST_DIR}/../../../../shared/audio")
set(lines_shared "${CMAKE_CURRENT_LIST_DIR}/../../../../shared/network")
brick_app_simulator(jc1060_audio_smoke PREBUILD font FRAMES 400
  SOURCES "${audio_shared}/AudioTest.cpp" "${lines_shared}/TextLines.cpp"
  SMOKE_ARGS --touch 512,126@3000+100
             --touch 512,182@5000+100)
target_include_directories(jc1060_audio_smoke PRIVATE "${audio_shared}" "${lines_shared}")
