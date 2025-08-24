---
sidebar_position: 5
title: 'Wrapping Existing Libraries'
---

# 🎁 Wrapping Existing Libraries into Nextino Modules

One of Nextino's greatest strengths is that you don't have to reinvent the wheel! You can take powerful, popular Arduino libraries and "wrap" them into a Nextino module. This gives you the best of both worlds: the robust, community-tested features of the library and the structure, safety, and scalability of the Nextino framework.

This tutorial will show you how to wrap the famous **`tzapu/WiFiManager`** library to create a plug-and-play `NextinoWifi` module.

## 🎯 The Goal

- Create a `NextinoWifi` module that handles WiFi connection and provisioning.
- Use the `tzapu/WiFiManager` library internally to do the heavy lifting.
- Expose the connection status to the rest of the system via the `EventBus` and `ServiceLocator`.

## Step 1: Create the Module Structure

Start by creating a new library, `lib/NextinoWifi`.

### 1.1. `library.json` - Declaring the Dependency

This is the most important step. We declare a dependency on `tzapu/WiFiManager`. PlatformIO will handle the rest.

```json title="lib/NextinoWifi/library.json"
{
    "name": "NextinoWifi",
    "version": "1.0.0",
    "description": "A Nextino module wrapping the tzapu/WiFiManager library.",
    "keywords": "nextino-module, wifi, wifimanager",
    "dependencies": {
        "tzapu/WiFiManager": "^0.16.0" 
    }
}
```

### 1.2. `config.json` - Configuring the Wrapper

Our config will control the Captive Portal, not the WiFi credentials (which are managed by the library itself).

```json title="lib/NextinoWifi/config.json"
[
  {
    "type": "NextinoWifiModule",
    "instance_name": "wifi_manager",
    "config": {
      "portal_ssid": "Nextino-Device-Setup",
      "portal_password": "", // Leave empty for an open portal network
      "portal_timeout_seconds": 180
    }
  }
]
```

## Step 2: Write the Wrapper Code

The Nextino module acts as a "bridge" or "adapter" to the `WiFiManager` library.

### 2.1. The Header (`NextinoWifiModule.h`)

We include `<WiFiManager.h>` and define our module's interface.

```cpp title="src/NextinoWifiModule.h"
#pragma once
#include <Nextino.h>
#include <WiFiManager.h> // We include the library we are wrapping

class NextinoWifiModule : public BaseModule {
private:
    // Configuration
    const char* _portalSsid;
    const char* _portalPassword;
    int _portalTimeout;
    
    // Internal WiFiManager instance
    WiFiManager _wm;

    // State tracking
    bool _wasConnected;
    
public:
    NextinoWifiModule(const char* instanceName, const JsonObject& config);
    
    static BaseModule* create(const char* instanceName, const JsonObject& config) {
        return new NextinoWifiModule(instanceName, config);
    }

    const char* getName() const override;
    void init() override;
    void loop() override;

    // Public API for other modules
    bool isConnected();
};
```

### 2.2. The Implementation (`NextinoWifiModule.cpp`)

Our module's lifecycle methods will call the `WiFiManager` library's methods.

```cpp title="src/NextinoWifiModule.cpp"
#include "NextinoWifiModule.h"

NextinoWifiModule::NextinoWifiModule(const char* instanceName, const JsonObject& config)
    : BaseModule(instanceName) {

    _portalSsid = config["portal_ssid"] | "Nextino-Setup";
    _portalPassword = config["portal_password"] | nullptr; // nullptr for open network
    _portalTimeout = config["portal_timeout_seconds"] | 180;
    _wasConnected = false;
}

const char* NextinoWifiModule::getName() const {
    return "NextinoWifiModule";
}

void NextinoWifiModule::init() {
    // We use the unique device ID for a unique hostname, which is great practice!
    const char* hostname = NextinoDevice().getUniqueId();
    WiFi.setHostname(hostname);

    // Configure WiFiManager
    _wm.setConfigPortalTimeout(_portalTimeout);
    
    // Register as a service
    NextinoServices().provide(getInstanceName(), this);

    NEXTINO_LOGI(getInstanceName(), "WiFi Manager configured. Hostname: %s", hostname);
    NEXTINO_LOGI(getInstanceName(), "If not configured, portal SSID will be '%s'.", _portalSsid);
    
    // Attempt a non-blocking connection. The portal will start if it fails.
    if(!_wm.autoConnect(_portalSsid, _portalPassword)) {
        NEXTINO_LOGW(getInstanceName(), "Failed to connect to a saved network. Captive portal will start.");
        // Note: autoConnect is blocking, but only on first run or config loss.
        // For a truly non-blocking approach, we would use the process() method in loop().
    }
}

void NextinoWifiModule::loop() {
    // For advanced, non-blocking portal management, you can call _wm.process() here.
    // For now, autoConnect() in init() is sufficient for most use cases.

    // Check connection status and publish events on change
    bool isConn = isConnected();
    if (isConn != _wasConnected) {
        if (isConn) {
            NEXTINO_LOGI(getInstanceName(), "WiFi is connected! IP: %s", WiFi.localIP().toString().c_str());
            NextinoEvent().post("wifi_connected");
        } else {
            NEXTINO_LOGW(getInstanceName(), "WiFi connection lost.");
            NextinoEvent().post("wifi_disconnected");
        }
        _wasConnected = isConn;
    }
}

bool NextinoWifiModule::isConnected() {
    return WiFi.status() == WL_CONNECTED;
}
```

## Step 3: Use the New Module

That's it! Now, to add powerful WiFi provisioning to any Nextino project, you just:

1. Add `lib/NextinoWifi` to your project.
2. Add it to your `platformio.ini` `lib_deps`.

The build script will discover it, and on first boot, your device will create a WiFi hotspot. Any other module (like an `MqttClientModule`) can now simply listen for the `"wifi_connected"` event to know when it can start its own work.

This pattern is incredibly powerful and can be used to integrate almost any existing Arduino library into the Nextino ecosystem.
