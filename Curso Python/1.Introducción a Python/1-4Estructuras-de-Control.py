# Estructura if
print("\nSolo if:")

edad = 18

if edad >= 18:
    print("Eres mayor de edad.")

# Estructura if...else
print("\nif y else:")

edad = 15

if edad >= 18:
    print("Eres mayor de edad.")
else:
    print("Eres menor de edad.")

# Estructura if...elif...else
print("\nPuntacion if-else-else:")

puntuacion = 80

if puntuacion >= 90:
    print("Excelente")
elif puntuacion >= 70:
    print("Buen trabajo")
else:
    print("Puedes mejorar")

# Estructura while
print("\nWhile:")

contador = 0

while contador < 5:
    print("Iteración", contador)
    contador += 1

# Estructura for
print("\nFor:")

nombres = ["Juan", "Ana", "Carlos"]

for nombre in nombres:
    print("Hola,", nombre)
