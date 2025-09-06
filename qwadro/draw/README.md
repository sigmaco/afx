```

          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
          ###### ###  ###   ###   ###     ### #########  ###    ###  ########

                  Q W A D R O   E X E C U T I O N   E C O S Y S T E M

                                   Public Test Build
                               (c) 2017 SIGMA FEDERATION
                             <https://sigmaco.org/qwadro/>

```

## Qwadro Video Graphics Infrastructure
The **Qwadro Video Graphics Infrastructure** (**VGI**) is the foundation of the Draw I/O System that was developed to encapsulates some of the low-level tasks that encompasses most of the code for device discovery, solicitation and enumeration, swap-chain creation, and presentation that are needed by device driver interfaces (DDI) and implementations to Draw I/O System. VGI provides a common framework for future graphics components. VGI defines a DDI that manages low-level shared tasks independently from the drawing mechanism runtime. The tasks implemented with VGI will be handled by the VGI DDI.

## The AVX Specification .
The **Acceleration for Video Experimentals** and the **Advanced Video Extensions for Qwadro** comprehends an data-oriented API specification where the common things are (relatively) easy, and the super powerful low-level optimizations are optional. This new API was firstly idealized to be easy as OpenGL but still trying to be powerful close to Vulkan, Direct3D e Metal. Unlike other low-level APIs, Draw I/O System follows a rapid application development (RAD) philosophy, where it delivers utilities to speed up and automate operations, still allowing the users to do it their own way if they want.

The first reference implementation of the Qwadro Draw I/O System, SIGMA GL/2, was strongely inspired on paradigms of that was supposed to be OpenGL 5 based on expectations of developers toward Direct3D 12 and its facilities. This was an effort by SIGMA to offer a high-performance real-time intiutive 3D drawing API. The Video Graphics Infrastructure was later developed to merge the SIGMA GL/2 into the Qwadro Execution Ecosystem as the Draw I/O System.

Because SIGMA GL/2 was first designed around the creation of a decent API based on OpenGL capabilities, unlikely in Vulkan, certain features are mutually exclusive, such as super/multisamples and mip levels, coming to be integrated under the generic denomination "level of detail (LOD)". Also rasters (images or textures) can only be 3D or layered, not both at same time. This makes the API straightforward to handle rasters.

The SIGMA Technology Group introduced its own abstractions, not directly mirroring any of today's native APIs. One reason is because no single API is available on all systems, and also because many concepts (such as extremely low-level memory handling) are not idiomatic to users not accustomed to the heat of metal. Although certain features inherited from several APIs (up to Glide, really), they have been reworked to look and feel more like Vulkan, while most of dialect used in Draw I/O System is inherited from the OpenGL/Vulkan (blue/red) side of the force, becoming the purple side of the force.

An example of abstraction in Draw I/O System is the lack of an entity known as "framebuffer". This (problematic) name was inherited from a no longer existent hardware component of the emergent days of computer graphics. This term causes confusion still now because actually this entity is not a buffer. Due to this fact, in Qwadro, its equivalent is a "canvas", which is an object that arranges surfaces, capable of being bound with rasters (de facto, the buffer), where a drawing is made on it.

## Draw I/O Device
In Draw I/O System, **draw devices** (**DDEV**) are logical. You probably will not seen the name of your GPU there, but a custom name. Wrappers broken the concept of handling a real device, so we adopted it as a fact. You can install custom device drivers in Qwadro, like in early days of computer graphics where you could download and install device drivers directly in root directory of your application.
These custom device drivers obviously will be wrappers. But you can write your own so no relying on the only driver to X platform or Y API. Could have even drivers using combined physical devices at once.

As aforementioned, Draw I/O System works on the "bring your own driver" model. We are living in a new war of APIs after Metal, Direct3D 12 and Vulkan emerged. Platform holders does not want to have such another API. SIGMA chose to replicate a model from the ancient times of hardware-accelerated computer graphics, where people looked for better driver implementations, including those based on existing common APIs or APIs specific to the installed VGA; the installable client driver (ICD) or, its more minimalist version, the mini client driver (MCD).

Despite the device driver implementation, Draw I/O System is planned to use only a extended version of the OpenGL Shading Language (GLSL) as native shading language to foster knowledge propagation.

## Contribution

Special thanks to Markus Tavenrath, Christoph Kubisch, Alfonse Reinheart, Evgeny Makarov and NVIDIA guys for contributing with their techniques and knowledge in hardware-accelerated 3D design.

Contributions are what make the open-source community such an amazing place to learn, inspire, and create. Any contributions you make are greatly appreciated. For details, see the "[Contributing Guidelines][contribute-guide]".

You can also contribute or address to any doubt or difficult although by join the SIGMA FEDERATION's gathering point on Discord.

[![SIGMA Discord Server](https://discord.com/api/guilds/349379672351571969/widget.png?style=banner2)](https://sigmaco.org/discord)

The SIGMA FEDERATION thanks [all the contributors][contributors] by their individual and collective involvements in the development of this project.

[![all-contributors](https://contrib.rocks/image?repo=sigmaco/afx&columns=16)][contributors]

## License

This project is published under [Qwadro License][license].

Qwadro is, and its portions are, (c) 2017 SIGMA FEDERATION. All rights reserved; to its elaborators and collaborators.

[license]: https://github.com/sigmaco/qwadro/blob/master/LICENSE.txt
[contributors]: https://github.com/sigmaco/afx/graphs/contributors
[contribute-guide]: https://github.com/sigmaco/afx/blob/master/docs/CONTRIBUTING.md
