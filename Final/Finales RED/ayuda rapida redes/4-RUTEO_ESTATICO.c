//RUTEO ESTATICO

La capa de red se encarga de llevar los paquetes desde el origen hasta el destino. Llegar al destino puede requerir muchos saltos por enrutadores intermedios. Para lograr su cometido, la capa de red debe conocer la topología de la subred de comunicación y elegir las rutas adecuadas a través de ella.

Una puerta de enlace predeterminada es un dispositivo que sirve como enlace entre dos redes informáticas, es decir, es el dispositivo que conecta y dirige el tráfico de datos entre dos redes o más. Es basicamente la direccion IP de la interfaz del router conectada a la red.

El ruteo estático es la forma más sencilla y que menos conocimientos exige para configurar las tablas de ruteo en un dispositivo. Es un método manual en el que se indica explícitamente en cada equipo las redes que puede alcanzar y por qué camino hacerlo.

ANTES DE PODER ROUTEAR SE RECOMIENDA CONFIGURAR TODAS LAS DIRECCIONES IP


Para configurar ruteo estatico:

Router > Enable
Router # configure terminal
Router # ip route <DIRECCION_IP_RED_DESTINO> <MASCARA_RED_DESTINO> <INTERFAZ_DE_SALIDA>	// ip route 192.168.1.0 255.255.255.0 serial2/0 - FastEthernet0/0
Router # exit											// 192.168.1.0 es la direccion IP de la red destino

Router > Enable
Router # configure terminal
Router # ip route <DIRECCION_IP_RED_DESTINO> <MASCARA_RED_DESTINO> <DIRECCION_IP_SIGUIENTE_SALTO>	// ip route 192.168.1.0 255.255.255.0 192.168.2.2
Router # exit												// 192.168.2.2 es la ip del router del siguiente salto

Rutas por defecto:

Router > Enable
Router # configure terminal
Router # ip route 0.0.0.0 0.0.0.0 <INTERFAZ_DE_SALIDA>			// ip route 0.0.0.0 0.0.0.0 serial2/0
Router # exit									

Router > Enable
Router # configure terminal
Router # ip route 0.0.0.0 0.0.0.0 <DIRECCION_IP_SIGUIENTE_SALTO>	// ip route 0.0.0.0 0.0.0.0 192.168.2.2
Router # exit								// 192.168.2.2 es la ip del router del siguiente salto

PARA VER LAS RUTAS CREADAS:

	show ip route


//EXTRA EXTRA
Para hacer enrutamiento dinamico: (Primero borra el estatico)
Protocolo RIP:

Router > Enable
Router # route rip
Router # network <DIRECCION_DE_RED_CONECTADA> 	//network 192.168.0.0	Se debe hacer con todas las redes que se deseen rutear

Protocolo OSPF:
Router > Enable
Router # route ospf 1
Router # network <DIRECCION_DE_RED_CONECTADA> area <Num>	//network 192.168.0.0 0.0.0.255 area 0	Se debe hacer con todas las redes que se deseen rutear
						


