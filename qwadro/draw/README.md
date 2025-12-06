![Logo](doc/qwadro-banner-2048x520-bg-t.png)

# Accelerated Video Graphics Extensions (AVX)
Video graphics extensions for Qwadro **Acceleration Framework Experimental** (**AFX**)

## Overview
**AVX**, alphabetism and initalism for **Accelerated Video Graphics Extensions**, is an amendment to the **Acceleration Framework Experimental** (**AFX**) extending the Qwadro Execution Ecosystem with a complete, scalable, hardware-accelerated video graphics foundation.

AVX achieves this by defining and establishing the **Qwadro Video Graphics Infrastructure**, a unified, device-agnostic, low-level video graphics subsystem that standardizes how accelerated video graphics devices are discovered, negotiated, initialized, and operated within the AFX ecosystem.

AVX provides the **infrastructure** and **device-driver** interfaces required to build rendering runtimes on top of a stable, explicit, cross-platform  
foundation.

Alongside AVX exists the **Accelerated MMUX Experimental** (**AUX**) amendment. AUX is another amendment for AFX focusing on the system-wide integration side of the user experience.

AUX provides a shells and desktop-environment intercommunication layers that eliminates the historical issues developers face when trying to interface video graphics runtimes with the host system.

Instead of requiring applications or runtimes to manually deal with platform-specific windowing systems, surface creation APIs, or environment hooks, AUX offers:

- A unified, platform-independent interface for context and surface creation  
- A strict communication channel between the AFX ecosystem and the system shell  
- A mechanism to "hook up" to the GPU without touching OS-specific APIs  
- A clean environment for graphics runtimes built on top of AVX/VGI

Together, AVX handles the GPU, and AUX handles the shell.

## AVX: Goals and Capabilities
AVX provides:

- A unified cross-platform model for GPU device discovery  
- A well-defined DDI (Device Driver Interface)  
- Standardized memory, buffer, and surface allocation  
- Swap-chain and presentation primitives  
- Predictable, low-level behavior independent of OS or window system  
- The foundation for building new rendering frontends  
- Backend flexibility (OpenGL-based, Vulkan-based, ICD-native, software renderer, etc.)

### Capability Source
AVX's expressiveness is inspired by the full set of capabilities historically exposed through OpenGL Core's evolution. 

AVX is by design a Vulkan-flavored OpenGL Core frontend; the never reached OpenGL Core 5. 

Its API, structure, and semantics are fully independent.

AVX runtimes may be backed by:

- Native ICD implementations  
- A VGI-to-Vulkan backend  
- A VGI-to-OpenGL backend  
- A research/experimental backend  
- Software fallback backends  

## MMUX interoperability
The AVX partnership with AUX solves the long-standing pain of "hooking up" a graphics runtime to the system shell.

AUX provides:

- A unified context and surface creation model  
- Strict lifecycle management for surfaces, windows, and GPU contexts  
- A platform-agnostic bridge to desktop environments  
- A guaranteed, consistent path for linking AFX/AVX runtimes with host systems  
- A replacement for legacy platform APIs (WGL, GLX, Wayland protocols, etc.)  
- Full integration with VGI's device-management process

This allows rendering runtimes to rely on AUX instead of:

- Window system APIs
- Platform display servers
- OS-dependent GPU acquisition APIs
- Ad-hoc event loop integration

Together, AVX and AUX provide a complete solution:

- **AUX** handles the system shell; surfaces, windows, contexts  
- **AVX** handles the GPU; devices, memory, presentation  
- **VGI** bridges both; unified infrastructure  
- **AFX** binds it all together; extensible acceleration platform  

The result is a modern, clean, deterministic ecosystem for graphics acceleration.

## Features Summary

| Component | Description |
|----------|-------------|
| **AVX** | Hardware-accelerated graphics subsystem for AFX |
| **AUX** | Shell/desktop intercommunication subsystem |
| **VGI** | Unified low-level infrastructure created by AVX |
| **DDI** | Strict driver interface for GPU/ICD implementations |
| **Backend Flexibility** | Vulkan, GL, ICD-native, or software |
| **Surface Uniformity** | Provided through AUX |

## Contribution

Special thanks to Markus Tavenrath, Christoph Kubisch, Alfonse Reinheart, Evgeny Makarov and NVIDIA guys for contributing with their knowledge in hardware-accelerated 3D design.

You can also contribute or address to any doubt or difficult although by join the SIGMA's gathering point on Discord.

[![SIGMA Discord Server](https://discord.com/api/guilds/349379672351571969/widget.png?style=banner2)](https://sigmaco.org/discord)

The SIGMA thanks [all the contributors][contributors] by their individual and collective involvements in the development of this project.

[![all-contributors](https://contrib.rocks/image?repo=sigmaco/afx&columns=16)][contributors]

## License

This project is published under [Qwadro License][license].

Qwadro is, and its portions are, (c) 2017 SIGMA FEDERATION. All rights reserved; to its elaborators and collaborators.

[license]: https://github.com/sigmaco/afx/blob/master/LICENSE
[contributors]: https://github.com/sigmaco/afx/graphs/contributors
[contribute-guide]: https://github.com/sigmaco/afx/blob/master/docs/CONTRIBUTING.md
