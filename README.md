# CS:GO Crash Fix

> Fixes the **sticker item crash exploit** affecting older CS:GO versions from 2023 and earlier.

## Features

- 🛠️ Fixes `CS_UM_SendPlayerItemFound` crash
- 🪝 Hooks `DispatchUserMessage` with MinHook
- 💉 Compiles as a `.DLL` for injection into `csgo.exe`
- 🎮 Designed for legacy CS:GO builds

## Usage

1. Build the project as an **x86 `.DLL`**.
2. Launch a compatible CS:GO build.
3. Inject the DLL into `csgo.exe`.

## Requirements

- Visual Studio / MSVC
- x86 build
- [MinHook](https://github.com/TsudaKageyu/minhook)
