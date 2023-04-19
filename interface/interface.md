# Interface

A interação com o braço robótico se dá de maneira física e digital, através da manipulação direta do braço e da interface gráfica, respectivamente.

Na interação física, um botão é responsável por selecionar o modo de operação, manual ou automático, indicados respectivamente pelos LEDs verde e vermelho. O modo manual desativa os motores, possibilitando que o braço seja livremente manipulado, e rejeita os comandos de movimento, de maneira a garantir a segurança da manipulação do braço. Por outro lado, no modo automático, os motores exercem torque para manter sua posição, opondo-se à força aplicada pela manipulação direta do braço.

Na interação digital, a visualização da posição do braço é obtida a partir da informação da geometria do braço e da medição de posição de cada um dos eixos, atualizada em tempo real, a uma frequência de 30 Hz. Quando em modo automático, é possível comandar o movimento do braço especificando-se a coordenada desejada.
 