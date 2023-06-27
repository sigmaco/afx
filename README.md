# QWADRO; 4D Application Foundation Experiments
QWADRO; 4D Application Foundation Experiments

O nome QWADRO é um acrônimo surgido duma sigla para Quadridimensional World Approximation, que foi um projeto anterior experimentando como seria uma simulação onde espaço-tempo fossem manipulados de forma singular.
Apesar do nome, o prefixo de domínio de código (em C, equivale ao namespace em C++) é "AFX". Existem outros prefixos para outros frameworks internos (GFX, SFX, XFX, MFX, ZFX, EFX, VFX, etc), que conforme items são "promovidos ao núcleo" da implementação (por votação singular da SIGMA), têm seu prefixo substituído por AFX.

O Qwadro não é um "game engine", como popularmente é tratado, mas um conjunto de blocos comuns de construção para edificar sistemas análogos a game engines.
Por que o Qwadro não é uma game engine? Porque o Qwadro foi feito para ser necessariamente customizado, criando sempre produtos diferentes, como eram feitas as game engines in-house do passado, quando tudo não parecia ser distinguível de cara que foi feito numa Unity da vida. Se você não gosta do renderizador da SIGMA, pode fazer o seu próprio com os componentes singulares do AFX.

Apesar de pouca semelhança atual, QWADRO surgiu como uma bifurcação da RenderWare Platform na tentativa de reconstruir as demos num paradigma multithreaded e no OpenGL.
Depois de certas manobras, a RenderWare se mostrou ineficiente para a atualidade, isto é, certas procedimentos poderiam ser melhor optimizados para as capacidades atuais de hardware.
Atualmente o QWADRO constitui uma coleção de middlewares integrados abstraindo sistemas operacionais e plataformas fortemente inspirado no Qt; algo parecido bastante com um Qt implementado em C.
O design de API e código geral foi inspirado em Havok Physics e, principalmente o modelo de objeto, em Qt Framework, além de preservar algumas semânticas remanescentes da RenderWare.

Há diversos nomes estranhos no Qwadro, porque o Qwadro é um experimento que implementa vários paradigmas de uma mesmo conceito com nomes aleatórios, e sua API muda frequentemente conforme optimização de uso.

QWADRO; 4D Math Foundation Experiments

Algumas heranças da RenderWare ainda presentes certamente está no MFX, onde reside a matemática do Qwadro.
afxMatrix, afxQuat, afxV3d, afxV4d, apesar de oferecer opção as vezes, tem sua computação feita no chamado "right-handled coordinate system", tal como é predominantemente nas shaders do OpenGL, e seu arranjo em memória é "row-major" (uma multidimensional array em C), tal como é predominantemente no Direct3D, assim como é na RenderWare.
Esta é uma abordagem complicada porque mescla tanto conceitos, funções e operações do OpenGL quanto do Direct3D ao mesmo tempo. Escrever o MFX (math framework) foi um inferno devido a isto, e nem todas as funções foram testadas ainda.

QWADRO; 4D Graphics Foundation Experiments

AFX Draw System foi implementado inspirado fortemente nos paradigmas do que seria o OpenGL 5. Atualmente parece muito um "Vulkan over OpenGL" wrapper.

Com AFX Draw System, a SIGMA escolheu uma abordagem diferente. Esta introduz suas próprias abstrações e não espelha diretamente quaisquer das APIs nativas da atualidade. Uma das razões é pelo fato de nenhuma única API estar disponível em todos os sistemas, e também porque muitos conceitos (tais como manuseio de memória em nível extremamente baixo) não são idiomático para usuários não acustomado com o calor do metal.
Apesar de certas características herdadas de APIs da 3dfx (tal como Voodoo) haverem sido retrabalhadas para parecerem mais com Vulkan. E também a maioria do dialeto é herdado do lado OpenGL/Vulkan da força.

Um exemplo da abstração pela SIGMA é a inexistência de um objeto conhecido como "framebuffer". O tal do framebuffer é um nome problemático para seu conceito de operação onde não há nenhum buffer de fato e foi um nome herdado do hardware. Devido a isto, no Qwadro, seu equivalente é um afxCanvas, que é um objeto que manuseia uma conjunto de afxSurfaces, de tipo raster ou depth/stencil, onde um desenho é realizado.

O Draw System funciona no modelo "bring your own driver". Ao ver Vulkan, sucessor do OpenGL, sofrer dos mesmos problemas que o OpenGL sofreu no passado em plataformas que não queriam dispor de tal API, a SIGMA optou por replicar um modelo dos tempos ancestrais da computação gráfica acelerada por hardware, onde as pessoas procuravam drivers lógicos e implementações de drivers melhores, feitas sobre APIs comuns já existentes ou APIs específicas da VGA instalada; o installable client driver (ICD) ou, sua versão mais minimalista, o mini client driver (MCD).
SIGMA GL (SGL), distribuído pelo arquivo "e2draw.icd", é um modelo de referência de ICD implementado sobre OpenGL 3.3 core e aproveitando extensões do 4.x, desenvolvido pela SIGMA.

Diferente das APIs de baixo nível, o Qwadro todo segue uma filosofia de rapid application development (RAD), onde entrega várias utilidades para acelerar e automatizar operações, ainda permitindo que o usuário faça a sua própria maneira se assim quiser.

Grande parte do Qwadro ainda está em código fechado devido a tecnologia proprietária. Mas estamos trabalhando para substituí-las e liberar o restante do código.

Junte-se a nós no Discord. https://sigmaco.org/discord
