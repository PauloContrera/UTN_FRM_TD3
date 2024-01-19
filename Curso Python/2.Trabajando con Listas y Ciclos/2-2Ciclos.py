# Ciclo for para iterar sobre una lista
print("\nCiclo con un FOR")
mi_lista = [1, 2, 3, 4, 5]
for elemento in mi_lista:
    print(elemento)

# Ciclo for con range
print("\nCiclo con un FOR con range")
for i in range(5):
    print(i)

# Ciclo while
contador = 0
print("\nCiclo con un While")
while contador < 5:
    print(contador)
    contador += 1


#Practica Crea un programa que utilice un ciclo for para imprimir 
# los cuadrados de los números en una lista.
print("\nLa lista al cuadado y los elementos")
mi_lista = [1, 2, 3, 4, 5]
mi_lista2=[]
for elemento in mi_lista:
    print("El elemento al cuadrado",elemento**2)
    mi_lista2.append(elemento**2)
print(mi_lista2)