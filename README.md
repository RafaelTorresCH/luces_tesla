# luces_tesla
Descripción General
El sistema cuenta con tres botones: uno para el giro a la izquierda, otro para el giro a la derecha, y un tercero para la señal de estacionamiento. Cada uno de estos botones controla su respectivo LED: uno para la señal de giro a la izquierda y otro para la señal de giro a la derecha. Además, un LED separado indica el "Heartbeat" del sistema, que parpadea a una frecuencia de 1 Hz para mostrar que el sistema está funcionando correctamente.

Cuando se presiona un botón de giro, el LED correspondiente parpadea tres veces. Si el botón se presiona dos veces en menos de 300 ms, el LED parpadea indefinidamente hasta que se vuelva a presionar el botón. Si se activa una señal de giro mientras la luz del lado opuesto está activa, esta última se apaga automáticamente. La señal de estacionamiento funciona de manera similar a la de un coche real, activando ambos LEDs de giro de manera intermitente en sincronía. La frecuencia de parpadeo de las luces de giro cumple con las normativas establecidas en "El Reglamento General de Circulación".

El sistema utiliza el puerto USART2 para la comunicación con un PC, permitiendo que los eventos principales sean monitoreados en tiempo real a través de la consola serial. Cada vez que se presiona un botón, el sistema envía un mensaje a través de USART2 indicando cuál fue el botón presionado (izquierda, derecha, o estacionamiento). Esto facilita la identificación de errores y el análisis del comportamiento del sistema.
