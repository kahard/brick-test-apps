#pragma once

#include <cstddef>
#include <cstdint>

#include "brick/Status.h"
#include "brick/interfaces/display/IDisplayDevice.h"
#include "brick/interfaces/display/ITouchscreen.h"
#include "brick/interfaces/input/IButton.h"
#include "brick/interfaces/logging/ILogger.h"
#include "brick/interfaces/network/IDnsResolver.h"
#include "brick/interfaces/network/INetworkInterface.h"
#include "brick/interfaces/network/ITcpSocket.h"
#include "brick/interfaces/network/IWifiAccessPoint.h"
#include "brick/interfaces/network/IWifiStation.h"
#include "brick/interfaces/time/ITimeProvider.h"

namespace network_test
{

using Status = brick::Status;

/// What the network test needs from a board, filled by the target entry
/// point (hardware board or PC simulator).
struct Services
{
    brick::interfaces::display::IDisplayDevice&    display;
    brick::interfaces::display::ITouchscreen*      touch;   ///< nullptr on a board without touch
    brick::interfaces::input::IButton*             button;  ///< nullptr on a board without a button
    brick::interfaces::network::IWifiStation&      wifi;
    brick::interfaces::network::IWifiAccessPoint&  access_point;
    brick::interfaces::network::INetworkInterface* ethernet;  ///< nullptr on a board without Ethernet
    brick::interfaces::network::IDnsResolver&      dns;
    brick::interfaces::network::ITcpSocket&        socket;
    brick::interfaces::time::ITimeProvider&        time;
    brick::interfaces::logging::ILogger&           logger;
};

/// The network to join (from the build, see tools/wifi_credentials.py) and
/// the hotspot opened when that fails or no network is configured.
struct Settings
{
    brick::interfaces::network::WifiCredentials network;  ///< empty SSID: hotspot only
    const char*                                 hotspot_ssid     = "BRICK-SETUP";
    const char*                                 hotspot_password = "brick-setup";
    /// One text line: line_height (about 1.4 x the font size) and its pixels,
    /// screen width x line_height.
    std::int32_t   line_height   = 28;
    std::uint16_t* line_pixels   = nullptr;
    std::size_t    line_capacity = 0;
};

}  // namespace network_test
