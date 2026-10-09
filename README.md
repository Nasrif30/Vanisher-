# VANISHER | Epson Counter Reset

**Created by ALNASRIF HALIDDIN**

An English-language, unofficial counter-reset utility for Epson L3250 and L4260 printers connected by USB. Version 1.0.0, for Windows 10/11 **64-bit**. Menus, prompts, errors, launchers, and build instructions are in English.

## Download and run: no compilation needed

1. [Download the Windows app ZIP](https://github.com/Nasrif30/Vanisher-/raw/refs/heads/main/downloads/VANISHER-Windows-x64-English.zip).
2. Right-click the downloaded ZIP, choose **Extract All**, and open the extracted `VANISHER` folder. Do not run it inside the ZIP.
3. Double-click **Validate L4260.cmd** for a local check. For an L3250, use **Validate L3250.cmd**. This check does **not** access the printer.
4. Double-click **Open VANISHER.cmd** to open the menu. Choose **1** for L3250, **2** for L4260, or **0** to exit.

Keep all extracted files together. `database.json` must stay beside `VANISHER.exe`. You do not need Visual Studio, CMake, Python, or a separate Visual C++ runtime to run this package. For the Unicode artwork, use Windows Terminal and a font with Braille-character support if the old console displays boxes. The EXE is unsigned; keep Windows security protections enabled. If Windows blocks it, review the source or obtain help from a trusted technician.

**Opening the menu does not reset the printer.** Selecting a model displays the planned changes and warnings. The program requires you to type the exact model-specific confirmation before it accesses the printer. Any other response cancels.

## Before using the reset

**Have the waste-ink pads serviced or replaced first.** Clearing the electronic counter does not remove collected ink. Saturated pads can leak even if the printer starts printing again.

The project owner reported an L4260 reset with all **14 EEPROM writes acknowledged** and printing restored afterward; see the hardware result below. This documents a reported result on one printer, not compatibility with every L4260 firmware version. If compatibility for your printer cannot be established, use Epson service instead of experimenting with EEPROM writes.

Only after pad service, compatibility verification, and the owner's explicit approval:

1. Connect the intended printer directly by USB and turn it on. Close other printer applications, including Epson Status Monitor.
2. Open **Open VANISHER.cmd** and select the correct model.
3. Read the warnings. To approve the EEPROM writes, type **RESET L4260** or **RESET L3250**, matching the selected model. Enter anything else to cancel.
4. Keep USB and power connected throughout the operation.
5. If the app reports **RESET ACKNOWLEDGED**, turn the printer off with its power button, wait 10 seconds, and turn it on. Verify that the error has cleared and print a test page.

If it reports **RESET NOT CONFIRMED**, stop and have `ewr_trace.log` reviewed. Do not keep retrying. If you get an access error, close printer applications first; an Administrator terminal may be required for device access. The app does not automatically elevate or bypass identity checks.

### What the reset changes

The inherited profile schedules **14 persistent EEPROM byte writes** across the main and platen counter groups:

| EEPROM address | Decimal value written |
| --- | --- |
| 28, 52, 53 | 0 |
| 54, 55, 255 | 94 |
| 47, 48, 49, 50, 51, 252, 253, 254 | 0 |

There is no firmware installation or flashing operation. The app checks the printer's USB identity and the expected profile before writing. The platen-group mapping remains uncertain in the upstream data. Incorrect mappings or an interrupted reset can leave the printer in an unintended or partially changed state.

**Acknowledged is not read back:** the program checks write acknowledgements, but does not read the final counter values, save the original bytes, or roll back a partial failure. It cannot display the current waste-ink counter level.

## Hardware result: Epson L4260

**ALNASRIF HALIDDIN reported a successful USB reset on an Epson L4260 under Windows and confirmed that the printer could print afterward.** The supplied output shows a 45-packet sequence with all 14 EEPROM writes receiving `:42:OK;` acknowledgements, followed by release of the hardware lock.

The following is the owner's supplied output, with the final message translated into English. The current English app uses **RESET ACKNOWLEDGED** to distinguish write acknowledgements from reading the counter values back.

<details>
<summary>Show the supplied 45-packet hardware output</summary>

```text
Executing universal Windows hardware state machine...
[i] Saving hardware trace to ewr_trace.log for diagnostics.
-> Packet 1 / 45 | Triggered ACK: Cleared 8 bytes.
-> Packet 2 / 45 | Triggered ACK: Cleared 9 bytes.
-> Packet 3 / 45 | Triggered ACK: Cleared 16 bytes.
-> Packet 4 / 45 | Triggered ACK: Cleared 10 bytes.
-> Packet 5 / 45 | Triggered ACK: Cleared 12 bytes.
-> Packet 6 / 45 | EEPROM write verified (||:42:OK;).
-> Packet 7 / 45 | Triggered ACK: Cleared 10 bytes.
-> Packet 8 / 45 | Triggered ACK: Cleared 12 bytes.
-> Packet 9 / 45 | EEPROM write verified (||:42:OK;).
-> Packet 10 / 45 | Triggered ACK: Cleared 10 bytes.
-> Packet 11 / 45 | Triggered ACK: Cleared 12 bytes.
-> Packet 12 / 45 | EEPROM write verified (||:42:OK;).
-> Packet 13 / 45 | Triggered ACK: Cleared 10 bytes.
-> Packet 14 / 45 | Triggered ACK: Cleared 12 bytes.
-> Packet 15 / 45 | EEPROM write verified (||:42:OK;).
-> Packet 16 / 45 | Triggered ACK: Cleared 10 bytes.
-> Packet 17 / 45 | Triggered ACK: Cleared 12 bytes.
-> Packet 18 / 45 | EEPROM write verified (||:42:OK;).
-> Packet 19 / 45 | Triggered ACK: Cleared 10 bytes.
-> Packet 20 / 45 | Triggered ACK: Cleared 12 bytes.
-> Packet 21 / 45 | EEPROM write verified (||:42:OK;).
-> Packet 22 / 45 | Triggered ACK: Cleared 10 bytes.
-> Packet 23 / 45 | Triggered ACK: Cleared 12 bytes.
-> Packet 24 / 45 | EEPROM write verified (||:42:OK;).
-> Packet 25 / 45 | Triggered ACK: Cleared 10 bytes.
-> Packet 26 / 45 | Triggered ACK: Cleared 12 bytes.
-> Packet 27 / 45 | EEPROM write verified (||:42:OK;).
-> Packet 28 / 45 | Triggered ACK: Cleared 10 bytes.
-> Packet 29 / 45 | Triggered ACK: Cleared 12 bytes.
-> Packet 30 / 45 | EEPROM write verified (||:42:OK;).
-> Packet 31 / 45 | Triggered ACK: Cleared 10 bytes.
-> Packet 32 / 45 | Triggered ACK: Cleared 12 bytes.
-> Packet 33 / 45 | EEPROM write verified (||:42:OK;).
-> Packet 34 / 45 | Triggered ACK: Cleared 10 bytes.
-> Packet 35 / 45 | Triggered ACK: Cleared 12 bytes.
-> Packet 36 / 45 | EEPROM write verified (||:42:OK;).
-> Packet 37 / 45 | Triggered ACK: Cleared 10 bytes.
-> Packet 38 / 45 | Triggered ACK: Cleared 12 bytes.
-> Packet 39 / 45 | EEPROM write verified (||:42:OK;).
-> Packet 40 / 45 | Triggered ACK: Cleared 10 bytes.
-> Packet 41 / 45 | Triggered ACK: Cleared 12 bytes.
-> Packet 42 / 45 | EEPROM write verified (||:42:OK;).
-> Packet 43 / 45 | Triggered ACK: Cleared 10 bytes.
-> Packet 44 / 45 | Triggered ACK: Cleared 12 bytes.
-> Packet 45 / 45 | EEPROM write verified (||:42:OK;).
Hardware lock released.

RESET CONFIRMED: all 14 writes received acknowledgements from the printer.
```

</details>

This supplied trace records write acknowledgements; it does not include counter readback, the firmware version, or confirmation that the waste-ink pads were serviced. The reported printing result does not remove the need for physical pad service. No additional hardware reset was run while preparing this documentation.

## Build your own EXE: double-click after one-time setup

Building means turning the included source code into `VANISHER.exe`. Skip this section if you downloaded the ready-to-run app ZIP above.

### One-time setup

1. [Download Microsoft Visual Studio 2022 Build Tools](https://aka.ms/vs/17/release/vs_BuildTools.exe). An existing Visual Studio 2022 Community, Professional, or Enterprise installation also works.
2. In the installer, select **Desktop development with C++**. Include **MSVC v143 C++ x64/x86 build tools**, a **Windows 10 or Windows 11 SDK**, and **C++ CMake tools for Windows**. Finish installing. [Microsoft's C++ setup instructions](https://learn.microsoft.com/en-us/cpp/build/vscpp-step-0-installation?view=msvc-170).
3. On [this repository](https://github.com/Nasrif30/Vanisher-), choose **Code → Download ZIP**. Extract the entire source ZIP. This source download is different from the ready-to-run app ZIP.

### Each time you build

1. Open the extracted source folder containing `CMakeLists.txt`.
2. Double-click **Build VANISHER.cmd**.
3. Wait for **BUILD COMPLETE**. The new `VANISHER.exe` appears in that folder.
4. Double-click **Validate L4260.cmd** to check the local profile, then **Open VANISHER.cmd** to open the app.

The build button detects Visual Studio 2022 and CMake, compiles locally, and copies the EXE beside the database. **The first build needs internet access** to fetch the pinned JSON source header from its official repository over HTTPS. CMake verifies its SHA256 hash and caches it in `build/dependencies` for offline rebuilds. If the direct secure download fails, an installed [Git for Windows](https://git-scm.com/install/windows) provides a fallback with certificate checks enabled. Git is optional when the direct download works; Python is not required. The helper does not install tools, connect to a printer, or perform a reset. The launcher applies PowerShell's execution-policy setting only to its own build process; it does not change the computer's policy. Organization-managed restrictions may still prevent scripts from running.

### Optional terminal commands

Open PowerShell **in the extracted folder**. The local checks below do not access USB:

```powershell
.\VANISHER.exe --help
.\VANISHER.exe --dry-run L4260
.\VANISHER.exe --dry-run L3250
```

A successful dry run reports: `DRY-RUN OK: 45 packets, 14 planned EEPROM writes. USB was not accessed.` This validates local packet generation, not printer compatibility.

To build manually with CMake available in your terminal:

```powershell
cmake -S . -B build -G "Visual Studio 17 2022" -A x64 -DEWR_BUILD_TESTS=OFF -DCMAKE_MSVC_RUNTIME_LIBRARY=MultiThreaded
cmake --build build --config Release
Copy-Item .\Release\VANISHER.exe .\VANISHER.exe -Force
```

## Help with common problems

| Message or problem | What to do |
| --- | --- |
| `VANISHER.exe was not found` | Extract the complete app ZIP, or run **Build VANISHER.cmd** in the source folder. |
| `Could not load database.json` | Keep the EXE and database together and open the app through its launcher. |
| `Visual Studio 2022 C++ tools were not found` | Complete the one-time setup above. Visual Studio Code alone is not the C++ compiler. |
| `CMake was not found` | Add **C++ CMake tools for Windows** in Visual Studio Installer. |
| JSON header download or hash check failed | Check internet access to `raw.githubusercontent.com` and `github.com`. For a TLS failure, install Git for Windows to enable the secure fallback. Do not disable TLS or the hash check. |
| `No Epson ... with a confirmed USB identity was found` | Check USB, printer power, and the official Epson driver. Do not bypass the identity check. |
| `RESET NOT CONFIRMED` | Stop. Have a technician review the trace and any partial EEPROM changes before retrying. |

## Future printer support

VANISHER is maintained as a separate project. This release enables only **L3250 and L4260**. More printer models can be added in future releases after their identity checks, reset mappings, and hardware compatibility have been reviewed. An entry in the database alone does not enable a model or prove it works.

## Credits and license

VANISHER branding, English interface, documentation, and launchers: **ALNASRIF HALIDDIN**.

Project code is provided under **Apache License 2.0**. Included components retain their applicable licenses. Required license text and acknowledgements are collected in [LICENSE](LICENSE). This software is unofficial and is not endorsed by Epson. The owner's reported L4260 hardware reset and supplied output are documented above.
