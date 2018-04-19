# Identidade Expandida - Gráficos

Esse repositório guarda o projeto de código relacionado a parte de renderização da obra "Identidade Expandida", feita em colaboração com Anaísa Franco. Essa parte da obra é feita em C++11, usando a biblioteca [openframeworks.cc](OpenFrameworks) (que serve, dentre ouras coisas, como uma camada de abstração para a OpenGL; mesmo que não muito eficiente, ajuda demais a não se preocupar em cuidar da máquina de estado enorme que é a GL).

## Funcionamento
O aplicativo essencialmente monitora um diretório, procurando por um arquivo que define as curvas de uma digital (cujo nome é uma constante definida em código) gerada pelo componente / projeto *Identidade Expandida - Curvas*, que por sua vez lê um arquivo de imagem gerado pelo *Identidade Expandida - Captura* e o transforma em um arquivo de texto definindo os contornos da digital presente na imagem.

Tal arquivo é lido e transformado em uma mesh 3D pela classe `FingerMesh`, que poligoniza os contornos dos dedos, os extruda, e gera um vetor de `ofMesh`es, cada uma representando uma curva do dedo - permitindo ainda que a altura de cada curva seja individualmente ajustada, para fins de animação.

Por fim, nosso `ofApp` renderiza nossa `FingerMesh` enquanto o arquivo existir - isto é, até outros componentes apagarem o arquivo, informando que o usuário tirou seu dedo do leitor de digitais. Ele então reverte ao estado inicial, em que a tela oscila suavemente entre tons de cinza.

## Componentes
### - `ofApp`
Classe principal do aplicativo, responsável por coordenar eventos e manter os objetos das classes subalternas.

### - `FingerMesh`
Classe herdada de `std::vector<ofMesh>`, que cria e gerencia as meshes de cada curva das digitais.

### - `FileDaemon`
Classe que monitora o sistema de arquivos assincronamente, garantindo que eventos do sistema de arquivos (criação e deleção de arquivos) sejam repassados à `ofApp`.
