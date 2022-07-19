[![Codacy Badge](https://app.codacy.com/project/badge/Grade/98655332b8cf42158d32bdc87a52322d)](https://www.codacy.com/gh/FunIsDangerous/8085_emulator/dashboard?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=FunIsDangerous/8085_emulator&amp;utm_campaign=Badge_Grade)
[![GitHub license](https://img.shields.io/github/license/FunIsDangerous/8085_emulator)](https://github.com/FunIsDangerous/8085_emulator/blob/main/LICENSE.md)
[![Release](https://img.shields.io/github/v/release/FunIsDangerous/8085_emulator?include_prereleases)](https://github.com/FunIsDangerous/8085_emulator/releases)

# Wiki

Full documentation in [Wiki](https://github.com/FunIsDangerous/8085_emulator/wiki)

# Binaries

- Linux: (ProjectPath)/bin/Release-linux-x86_64/GUI/GUI
- Windows: (ProjectPath)/bin/Release-windows-x86_64/GUI/GUI.exe

***OR*** download straight from the latest release.


If you've downloaded an older version before, I recommend deleting "imgui.ini" and "config.ini" in order to reset the UI to the default options.

# Examples

There are lots of examples in the "examples/" folder.

---

# Usage

### Windows
Run *GUI.exe* file, and the rest is pretty self explanatory.

OR if you want to open a file straight away:
- Using commandline: ./GUI.exe [filename]
- OR Drag and drop the file to *GUI.exe*

### Linux
- chmod +x GUI
- ./GUI [filename]


---
# Building

[Build from source](https://github.com/FunIsDangerous/8085_emulator/wiki#build-from-source)
  
---

# Feedback

Feel free to report bugs, ask questions or request a feature in the Issues tab.

# TODO

- [ ] Run-time errors, such as using "POP" on an empty stack.
- [ ] Don't crash when background stuff goes wrong
- [ ] **DAA** instruction should calculate Carry flag.
- [ ] Double-check if interrupts should be disabled while another interrupt is being handled. If so, should they be re-enabled automatically?
- [ ] Greek characters support 

# Performance

## CPU

CPU usage should be very low. You can change the CPU frequency and accuracy, though that will impact how some code (such as DELA,DELB) works.

## GPU

Due to using hardware accelarated UI ([Dear ImGui](https://github.com/ocornut/imgui)), there is **some** GPU usage. In my laptop, this ranges from 5-15%. To lower this, you can lower the UI FPS. 

Normally, this library is used for games, using their existing API, adding almost 0 overhead. In this case, the application doesn't otherwise have any GPU-accelarated code. 

The entire UI is refreshed every frame even when not needed. There are [plans](https://github.com/ocornut/imgui/pull/5116#issuecomment-1161672387) to make optimizations, but with no timeframe. This would make the GPU usage 0% while not running the code, but wouldn't have a less significant impact when running, due to hex editor and registers window. That would, however, give me other ways of optimizing. 

There is currently a [PR](https://github.com/ocornut/imgui/pull/5116) that implements this. But it's for directx12, thus not cross-platform. It's also not for the docking branch. I have plans of ***trying*** to implement this myself, but no promises.

However, in the current state of the application, I don't think this is much of a problem.
