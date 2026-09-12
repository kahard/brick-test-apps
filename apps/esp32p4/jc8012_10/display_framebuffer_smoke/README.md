# display_framebuffer_smoke

See [the JC8012 demo guide](../README.md) for architecture, build instructions
and the difference between P4 render buffers and S3 scan-out buffers.

Alternates full-screen red/blue every 500 ms without assets or touch.
Logs fill time separately from submit-and-refresh time. Two application render
buffers are allocated in PSRAM; the MIPI driver retains ownership of scan-out.

Classes: Application -> FramebufferTest -> FrameBuffers + Screen + Timer.
