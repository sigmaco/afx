```
/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                  Q W A D R O   E X E C U T I O N   E C O S Y S T E M
 *
 *                                   Public Test Build
 *                       (c) 2017 SIGMA, Engineering In Technology
 *                             <https://sigmaco.org/qwadro/>
 */
```

# The Qwadro Execution Ecosystem
The SIGMA autonomous execution platform.

Qwadro is an autonomous execution ecosystem, a level-3 system infrastructure, a data-oriented virtual platform, that offers a set of common building blocks to build systems analogous to game engines. It is a coalition of middlewares and engines.

Qwadro is not a game engine. Qwadro is an initiative of the SIGMA Federation to foster the genuine development of hardware-accelerated 2D, 3D and 4D drawing applications for the Latin American audience. This is a top secret project with top secret objectives as expected. Yeah, now you know about it we have to led you to some basement from El Dorado.

Why is Qwadro not a game engine? Because the Qwadro ecosystem was initially made to run old demoscenes, mainly from RenderWare, but not limited to them, and then to be necessarily customized, always creating different products, as in-house game engines were made in the past, when everything seemed to be distinguishable and was made using a specific technology.

Despite little current similarity, Qwadro AFX emerged as a fork of the RenderWare Platform in an attempt to rebuild the demos in a multithreaded paradigm and OpenGL.
After certain maneuvers, RenderWare proved to be very inefficient on new hardware, that is, certain procedures could be better optimized for current hardware capabilities at the same or lower computational cost.

Currently, Qwadro constitutes a collection of middlewares integrating systems, subsystems and drivers, abstracting operating systems and platforms.

Despite the name, the code domain prefix (in C, equivalent to the namespace in C++) is AFX. There are other prefixes for other partitions (AVX, AEX, AAX, AMX, AUX, ATX, etc). The SIGMA reserves for itself and its experiments every possible prefix A\*X for official expansions of the Qwadro.

The API and general code design was inspired by Havok Physics and, mainly the object model, by Qt Framework, in addition to preserving some semantics remaining from RenderWare.

There are several strange names in Qwadro, because Qwadro is an experiment that implements several paradigms of the same concept with random names to test both in parallel, and its API changes frequently depending on usage optimization. And... it is governed by methodist laws of Go Horse.
Some RenderWare heritage still present is certainly in the AMX, where Qwadro's mathematics resides. For example, we have afxMatrix, afxQuat, afxV3d, afxV4d.

Qwadro was firstly designed for POSIX/Unix/Linux systems. But due complexities handling multiple OpenGL contexts in Windows, we was forced to move to Windows and stay there still now.

About code generation, Qwadro code base was written for compilation in LLVM/Clang. No other toolset was tested except MSVC, wich is not supported due it had never been a real C compiler but a C++ compiler. Qwadro also doesn't use CMake because the main developer had problems with Visual Studio 2017, the IDE used to code Qwadro.

Game-related works featured in Qwadro are strongely inspired in notorious gaming works on Bang! Engine by Ensemble Studios and MaxFX by Remedy Entertainment.

## The Unified Qwadro Simulation Infrastructure
The quadridimensional world approximation.

Before Qwadro become an whole execution ecosystem, it was an old project experimenting with what a simulation would be like where time-space (see, time over space) were manipulated in unconventional ways. From there became its name, which is an acronym for Tetradimensional/Quadridimensional World Approximation.
The old project was a experiment to test the possibility of our real world be in fact some type of simulation. We discovered interesting things about the "behavior and fabric of the reality", but it is too complex concept to explain.

Such as RenderWare, Qwadro forces the use of the so-called "right-handed coordination system", where positive X advances to right, positive Y advances to up and positive Z advances to front.

In Qwadro, a matrix is neither row-major nor column-major. Their matrices are hybrids. The upper 3x3 submatrix (where linear operations occur) is column-major, while the translation set is row-major. Yes, that madness there. Different from what you will expect in the war of row-major (Direct3D) vs. column-major (OpenGL). Qwadro is the definitive supremacy.

This is a complicated approach because it mixes both OpenGL and Direct3D concepts, functions, and operations at the same time. Writing AMX was hell because of this, and not all functions have been tested yet.

Several parts of simulators in Qwadro is inspired in Bang! Engine (Age Of Empires III engineering) and MaxFX 2.0 (Max Payne 2 engineering) in how to handle assets.

A Qwadro Simulation works on the "bring your own driver" model. Driver? Yes. You can install a custom device driver to run your simulation. 
It allow you to change physics engines and sample animations in GPU pipelines with OpenGL, Vulkan, OpenCL, CUDA, DSPs, SPUs or other coprocessors.
This is why we don't have SIMD methods for math (at least yet); you can install a hardware-accelerated physics engine.

## The Unified Qwadro Draw I/O System Infrastructure
The purple side of the force.

SIGMA GL/2 (aka SGL, SIGGL) is an data-oriented API specification where the common things are (relatively) easy, and the super powerful low-level optimizations are optional.

The idea behind its proposal was the development overhead of Vulkan API being absolutely insane when we tried to move from OpenGL to Vulkan in our recent works.

There were several helper functions to produce a minimalist usable library that made Vulkan a lot productive, which became effectily an other middleware.

After several deployments in Vulkan, the SIGMA Technology Group, the technology arm of Federação SIGMA, pushed by graphics programming community, started development a new API: the purple side of the force.

This new API was firstly idealized to be easy as OpenGL but still trying to be powerful close to Vulkan, Direct3D e Metal. Another Glide?

But don't think we are just creating a wrapper around them.

Draw I/O System is a modulation of Qwadro Execution Ecosystem, also developed by SIGMA, effectively a try to implement the SIGMA GL/2 API specification. You can install custom device drivers in Qwadro, like in early days of computer graphics where you could download and install device drivers directly in root directory of your application.

These custom device drivers obviously will be wrappers. But you can write your own so no relying on the only driver to X platform or Y API. Could have even drivers using combined physical devices at once.

Why custom device drivers? Well. You see. Vulkan started suffering of same issues than OpenGL suffered in past. Vulkan runs over wrappers in Metal, for example. For presentation, Vulkan relies on Direct3D 12 swapchains on Windows.

Old APIs such as Direct3D 9 and OpenGL also runs over wrappers in some recent versions of some IHV drivers. WebGL (almost unnecessarily) runs over wrappers. WebGPU runs over wrappers. These wrappers seems several bugs and issues due to IHVs being negligent. In the end, almost everything will run in wrappers.

In SIGMA GL/2, devices are virtual. You probably will not seen the name of your GPU there, but a custom name. Wrappers broken the concept of handling a real device, so we adopted it as a fact.

The first (standard) implementation of SIGMA GL/2 is strongely inspired on paradigms of that was supposed to be OpenGL 5.
This is an effort by SIGMA to offer a high-performance real-time 3D drawing API. SIGMA chose a different approach with SIGMA GL/2: introducing its own abstractions and not directly mirroring any of today's native APIs. One reason is because no single API is available on all systems, and also because many concepts (such as extremely low-level memory handling) are not idiomatic to users not accustomed to the heat of metal.
Although certain features inherited from 3dfx Voodoo APIs, they have been reworked to look more like Vulkan. And also most of the dialect of SIGMA GL/2 is inherited from the OpenGL/Vulkan side of the force, becoming the purple side (blue + red).

An example of abstraction in SIGMA GL/2 is the lack of an object known as a "framebuffer". The framebuffer is a problematic name for its concept of operation where there is no actual buffer and it was a name inherited from the hardware. Due to this, in Qwadro, its pseudo-equivalent is a "canvas", which is an object that handles an arrangement of surfaces, capable of being bound with rasters (de facto, the buffer), where a drawing is made.

As aforementioned, SIGMA GL/2 works on the "bring your own driver" model. Seeing Vulkan, OpenGL's successor, suffer from the same problems that OpenGL suffered in the past when platform holders did not want to have such an API, SIGMA chose to replicate a model from the ancient times of hardware-accelerated computer graphics, where people looked for better driver implementations, including those based on existing common APIs or APIs specific to the installed VGA; the installable client driver (ICD) or, its more minimalist version, the mini client driver (MCD).

SIGMA GL/2 is distributed with the "e2draw.icd" module, which is a reference model for ICD implemented on OpenGL 4 core and taking advantage of 4.x extensions, developed by SIGMA.

Due to several problems in OpenGL architecture, SIGMA GL/2 on Qwadro works in differently of any other API. For every CPU core available in the host platform, SIGMA GL/2 will run a Drawing Processing Unit (DPU). 

DPUs are autonomous services, designed as a system on a threading unit, provided by a drawing device for process the lines of execution in Qwadro Draw I/O System. DPUs calls draw input services to perform transhipment of draw workload, process the drawing queues and flushes draw output services.

In Qwadro, threads are virtualized effectively working as fibers. Real threads in Qwadro are abstracted as "thread execution units". This allow us to better manage host platform resources and consistence across synchronization primitives avoiding several manual sync/lock operations.

Unlike other low-level APIs, SIGMA G/2 follows a rapid application development (RAD) philosophy, where it delivers several utilities to speed up and automate operations, still allowing the user to do it their own way if they want.

SIGMA GL/2 tried and will keep trying to match the OpenGL conventions or, when not existing, the Vulkan conventions.

The default pixel format will probably be ARGB (as known in LE as, BGRA) why it is faster.
And the image data origin will continue to be left-bottom-near (even it being weird for work in video ops) why it makes sense going up in positive Y and right in positive X and far in positive Z, preserving consistence to the right-handed coordinate system.
Also, in SIGMA GL/2, the triangle front facing is counter-clockwise (CCW).

Why happened to "GL/1"? Flopped. There were several problems inherited from RenderWare way of doing drawing.

SIGMA GL/2 will include a fixed-function video decoder.

To establish the mandatory features in SIGMA GL/2, we grab a ordinary office desktop PC released in 2013, four years before the begining of Qwadro Execution Ecosystem development and we are working with the whole set of features available in OpenGL driver (updated by Intel until 2021) for this machine. By default, this machine garantees full support up to OpenGL 4.0 but it have extensions up to 4.6 (the last one). Things like buffer storage and texture storage are required, for example, when using SIGMA GL/2 over Advanced OpenGL.

Despite driver implementation, Qwadro uses extended OpenGL Shading Language (GLSL) as native shading language.

Special thanks to Markus Tavenrath, Christoph Kubisch, Alfonse Reinheart, Evgeny Makarov and NVIDIA, for contributing their techniques and knowledge in hardware-accelerated 3D design.

## The Unified Qwadro Sound I/O System Infrastructure
The autonomous accoustic system.

SIGMA A4D is an API for rendering accoustic simulations inspired on Miles Sound System way of handling several applicable service interfaces.

Instead of just offer another API, the SIGMA proposal is to allow you to run other middlewares using same interface. But it is in phase of decisions for its basic foundation, mainly due to Microsoft have killed hardware acceleration for audio since Windows Vista.

(c) 2017 Federação SIGMA. All rights reserved; to its elaborators and collaborators.
